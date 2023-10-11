#include "gtest/gtest.h"

#include "dulcificum/miracle_jtp/mgjtp_json_to_command.h"
#include "dulcificum/miracle_jtp/mgjtp_command_to_json.h"

#include <fstream>
#include "test_data_dir.h"

using namespace dulcificum;

TEST(miracle_jtp_tests, move) {
    botcmd::Move move;
    move.feedrate = 0.4242;
    move.point = {-4.2, 42.0, 0.42, 0.0, 0.12};
    move.tags = {botcmd::Tag::Roof};
    auto jmove = miracle_jtp::toJson(move);
    auto cmove = miracle_jtp::toCommand(jmove);
    auto jmove1 = miracle_jtp::toJson(*cmove);
    EXPECT_EQ(jmove, jmove1);
}

TEST(miracle_jtp_tests, change_toolhead) {
    botcmd::ChangeTool cmd;
    cmd.position[0] = -4.2;
    cmd.position[1] = 42.0;
    cmd.tags = {botcmd::Tag::TravelMove};
    auto jcmd = miracle_jtp::toJson(cmd);
    auto cmd1 = miracle_jtp::toCommand(jcmd);
    auto jcmd1 = miracle_jtp::toJson(*cmd1);
    EXPECT_EQ(jcmd, jcmd1);
}

TEST(miracle_jtp_tests, rwrw) {
    std::filesystem::path example_path = kTestDataDir / "cmd_example.json";
    ASSERT_TRUE(std::filesystem::exists(example_path));
    std::ifstream fin(example_path);
    nlohmann::json jin = nlohmann::json::parse(fin);
    for (const auto& jcmd : jin) {
        const auto cmd0 = miracle_jtp::toCommand(jcmd);
        const auto jcmd1 = miracle_jtp::toJson(*cmd0);
        EXPECT_EQ(jcmd, jcmd1);
    }
}