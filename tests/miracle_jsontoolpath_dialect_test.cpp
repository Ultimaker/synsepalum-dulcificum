#include "dulcificum/miracle_jtp/mgjtp_command_to_json.h"
#include "dulcificum/miracle_jtp/mgjtp_json_to_command.h"
#include "test_data_dir.h"

#include "gtest/gtest.h"
#include <fstream>

using namespace dulcificum;

class MiracleJTP : public testing::Test
{
protected:
    void SetUp() override
    {
        miracle_jtp::k_key_str::init(2);
    }

    void TearDown() override { }
};

TEST_F(MiracleJTP, move)
{
    botcmd::Move move;
    move.feedrate = 0.4242;
    move.point = { -4.2, 42.0, 0.42, 0.0, 0.12 };
    move.tags = { botcmd::Tag::Roof };
    auto jmove = miracle_jtp::toJson(move);
    auto cmove = miracle_jtp::toCommand(jmove);
    auto jmove1 = miracle_jtp::toJson(*cmove);
    EXPECT_EQ(jmove, jmove1);
}

TEST_F(MiracleJTP, change_toolhead)
{
    botcmd::ChangeTool cmd;
    cmd.position[0] = -4.2;
    cmd.position[1] = 42.0;
    cmd.tags = { botcmd::Tag::TravelMove };
    auto jcmd = miracle_jtp::toJson(cmd);
    auto cmd1 = miracle_jtp::toCommand(jcmd);
    auto jcmd1 = miracle_jtp::toJson(*cmd1);
    EXPECT_EQ(jcmd, jcmd1);
}

TEST_F(MiracleJTP, fan_duty)
{
    botcmd::FanDuty cmd;
    cmd.index = 0;
    cmd.duty = 0.12;
    cmd.tags = { botcmd::Tag::TopSurface_1};
    auto jcmd = miracle_jtp::toJson(cmd);
    auto cmd1 = miracle_jtp::toCommand(jcmd);
    auto jcmd1 = miracle_jtp::toJson(*cmd1);
    EXPECT_EQ(jcmd, jcmd1);

    EXPECT_EQ(nlohmann::to_string(jcmd), "{\"command\":{\"function\":\"fan_duty\",\"metadata\":{},\"parameters\":{\"index\":0,\"value\":0.12},\"tags\":[\"TOP_SURFACE_1\"]}}");
}

TEST_F(MiracleJTP, rwrw)
{
    std::filesystem::path example_path = kTestDataDir / "cmd_example.json";
    ASSERT_TRUE(std::filesystem::exists(example_path));
    std::ifstream fin(example_path);
    nlohmann::json jin = nlohmann::json::parse(fin);
    for (const auto& jcmd : jin)
    {
        const auto cmd0 = miracle_jtp::toCommand(jcmd);
        const auto jcmd1 = miracle_jtp::toJson(*cmd0);
        EXPECT_EQ(jcmd, jcmd1);
    }
}
