#include "dulcificum/miracle_jtp/mgjtp_command_file_stream.h"
#include "dulcificum/miracle_jtp/mgjtp_command_to_json.h"
#include "dulcificum/miracle_jtp/mgjtp_json_to_command.h"
<<<<<<< HEAD
#include "dulcificum/miracle_jtp/mgjtp_command_stream.h"

=======
>>>>>>> 02de50b5febf71709526393981a1651516bc337a
#include "test_data_dir.h"

#include "gtest/gtest.h"
#include <fstream>


using namespace dulcificum;

TEST(miracle_jtp_tests, move)
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

TEST(miracle_jtp_tests, change_toolhead)
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

TEST(miracle_jtp_tests, rwrw)
{
    const std::filesystem::path example_path = kTestDataDir / "cmd_example.json";
    ASSERT_TRUE(std::filesystem::exists(example_path));
    std::ifstream fin(example_path);
    const nlohmann::json jin = nlohmann::json::parse(fin);
    fin.close();
    for (const auto& jcmd : jin)
    {
        const auto cmd0 = miracle_jtp::toCommand(jcmd);
        const auto jcmd1 = miracle_jtp::toJson(*cmd0);
        EXPECT_EQ(jcmd, jcmd1);
    }
}

TEST(miracle_jtp_tests, command_fstream) {
    const std::filesystem::path example_path = kTestDataDir / "cmd_example.json";
    ASSERT_TRUE(std::filesystem::exists(example_path));
    miracle_jtp::CommandFileStream fstream;
    fstream.open(example_path);
    std::vector<std::string> command_lines;
    while (! fstream.eof())
    {
        const auto& line = fstream.getCommandLine();
        if (! line.empty())
        {
            command_lines.push_back(line);
        }
    }
    fstream.close();
    EXPECT_EQ(command_lines.size(), 7);
}

TEST(miracle_jtp_tests, command_stream) {
    const std::filesystem::path example_path = kTestDataDir / "cmd_example.json";
    ASSERT_TRUE(std::filesystem::exists(example_path));
    miracle_jtp::CommandStream stream;
    stream.open(example_path);
    const auto& cmd_list = stream.getCommandList();
    stream.close();
    EXPECT_EQ(cmd_list.size(), 7);
}