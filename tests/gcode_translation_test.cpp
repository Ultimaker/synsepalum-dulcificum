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
    auto ast = gcode::parse(gcode);
    const auto commands = gcode::toCommand(ast);

    ASSERT_EQ(commands.size(), 2);
    ASSERT_EQ(commands[0]->type, botcmd::CommandType::Delay);
    const auto delay_p = std::static_pointer_cast<const botcmd::Delay>(commands[0]);
    EXPECT_NEAR(delay_p->seconds, 0.5, 0.001);

    ASSERT_EQ(commands[1]->type, botcmd::CommandType::Delay);
    const auto delay_s = std::static_pointer_cast<const botcmd::Delay>(commands[1]);
    EXPECT_NEAR(delay_s->seconds, 2.0, 0.001);
}

TEST(GCodeTranslation, FanDutyScaling)
{
    const std::string gcode = "M106 S128\nM106 S255\nM106 S0.5\n";
    auto ast = gcode::parse(gcode);
    const auto commands = gcode::toCommand(ast);

    ASSERT_EQ(commands.size(), 3);
    ASSERT_EQ(commands[0]->type, botcmd::CommandType::ActiveFanDuty);
    const auto fan0 = std::static_pointer_cast<const botcmd::FanDuty>(commands[0]);
    EXPECT_NEAR(fan0->duty, 128.0 / 255.0, 0.001);

    ASSERT_EQ(commands[1]->type, botcmd::CommandType::ActiveFanDuty);
    const auto fan1 = std::static_pointer_cast<const botcmd::FanDuty>(commands[1]);
    EXPECT_NEAR(fan1->duty, 1.0, 0.001);

    ASSERT_EQ(commands[2]->type, botcmd::CommandType::ActiveFanDuty);
    const auto fan2 = std::static_pointer_cast<const botcmd::FanDuty>(commands[2]);
    EXPECT_NEAR(fan2->duty, 0.5, 0.001);
}

TEST(GCodeTranslation, TemperatureFallbackToR)
{
    const std::string gcode = "M109 R210\n";
    auto ast = gcode::parse(gcode);
    const auto commands = gcode::toCommand(ast);

    ASSERT_EQ(commands.size(), 2);
    ASSERT_EQ(commands[0]->type, botcmd::CommandType::SetTemperature);
    const auto temp_ext = std::static_pointer_cast<const botcmd::SetTemperature>(commands[0]);
    EXPECT_NEAR(temp_ext->temperature, 210.0, 0.001);

    ASSERT_EQ(commands[1]->type, botcmd::CommandType::WaitForTemperature);
    const auto wait_ext = std::static_pointer_cast<const botcmd::WaitForTemperature>(commands[1]);
    EXPECT_EQ(wait_ext->index, 0);
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
