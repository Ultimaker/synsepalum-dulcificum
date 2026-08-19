#include "dulcificum.h"
#include "dulcificum/gcode/ast/comment_commands.h"
#include "dulcificum/gcode/ast/translate.h"
#include "dulcificum/gcode/gcode_to_command.h"
#include "dulcificum/gcode/parse.h"
#include "dulcificum/miracle_jtp/mgjtp_command_to_json.h"

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

using namespace dulcificum;

TEST(GCodeTranslation, CommentArbitraryText)
{
    const std::string gcode = "; This is a test comment with special chars: 123!@#$%^&*()_+\n;TYPE:WALL-OUTER\n";
    const auto ast = gcode::parse(gcode);
    ASSERT_EQ(ast.size(), 2);
}

TEST(GCodeTranslation, CRLFLineEndings)
{
    const std::string gcode = ";START_OF_HEADER\r\n;HEADER_VERSION:0.1\r\nG0 X10 Y20\r\n";
    const auto ast = gcode::parse(gcode);
    ASSERT_GE(ast.size(), 2);
}

TEST(GCodeTranslation, G4DwellConversion)
{
    const std::string gcode = "G4 P500\nG4 S2\n";
    const auto ast = gcode::parse(gcode);
    const auto commands = gcode::toCommand(ast);

    ASSERT_EQ(commands.size(), 2);
    const auto delay_p = std::dynamic_pointer_cast<const botcmd::Delay>(commands[0]);
    ASSERT_NE(delay_p, nullptr);
    EXPECT_NEAR(delay_p->seconds, 0.5, 0.001);

    const auto delay_s = std::dynamic_pointer_cast<const botcmd::Delay>(commands[1]);
    ASSERT_NE(delay_s, nullptr);
    EXPECT_NEAR(delay_s->seconds, 2.0, 0.001);
}

TEST(GCodeTranslation, FanDutyScaling)
{
    const std::string gcode = "M106 S128\nM106 S255\nM106 S0.5\n";
    const auto ast = gcode::parse(gcode);
    const auto commands = gcode::toCommand(ast);

    ASSERT_EQ(commands.size(), 3);
    const auto fan0 = std::dynamic_pointer_cast<const botcmd::FanDuty>(commands[0]);
    ASSERT_NE(fan0, nullptr);
    EXPECT_NEAR(fan0->duty, 128.0 / 255.0, 0.001);

    const auto fan1 = std::dynamic_pointer_cast<const botcmd::FanDuty>(commands[1]);
    ASSERT_NE(fan1, nullptr);
    EXPECT_NEAR(fan1->duty, 1.0, 0.001);

    const auto fan2 = std::dynamic_pointer_cast<const botcmd::FanDuty>(commands[2]);
    ASSERT_NE(fan2, nullptr);
    EXPECT_NEAR(fan2->duty, 0.5, 0.001);
}

TEST(GCodeTranslation, TemperatureFallbackToR)
{
    const std::string gcode = "M109 R210\nM190 R60\n";
    const auto ast = gcode::parse(gcode);
    const auto commands = gcode::toCommand(ast);

    ASSERT_EQ(commands.size(), 2);
    const auto temp_ext = std::dynamic_pointer_cast<const botcmd::SetUserExtruderTemperature>(commands[0]);
    ASSERT_NE(temp_ext, nullptr);
    EXPECT_NEAR(temp_ext->temperature, 210.0, 0.001);

    const auto temp_bed = std::dynamic_pointer_cast<const botcmd::SetBedTemperature>(commands[1]);
    ASSERT_NE(temp_bed, nullptr);
    EXPECT_NEAR(temp_bed->temperature, 60.0, 0.001);
}

TEST(GCodeTranslation, FullPipelineGCode2MiracleJTP)
{
    const std::string gcode = ";START_OF_HEADER\n"
                              ";HEADER_VERSION:0.1\n"
                              ";EXTRUDER_TRAIN.0.INITIAL_TEMPERATURE:205\n"
                              ";BUILD_PLATE.INITIAL_TEMPERATURE:60\n"
                              ";TYPE:WALL-OUTER\n"
                              "G1 X10.0 Y10.0 E1.0 F3000\n"
                              "G1 X20.0 Y10.0 E2.0 F3000\n";

    const auto json_str = GCode2Miracle_JTP(gcode, 1);
    const auto parsed = nlohmann::json::parse(json_str);
    EXPECT_TRUE(parsed.is_array());
    EXPECT_FALSE(parsed.empty());
}
