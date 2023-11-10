#include "gtest/gtest.h"

#include "dulcificum/miracle_jtp/mgjtp_command_file_stream.h"
#include "dulcificum/miracle_jtp/mgjtp_command_to_json.h"
#include "dulcificum/miracle_jtp/mgjtp_json_to_command.h"
#include "dulcificum/miracle_jtp/mgjtp_command_stream.h"

#include "test_data_dir.h"

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

TEST(miracle_jtp_tests, command_fstream)
{
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

TEST(miracle_jtp_tests, command_stream)
{
    const std::filesystem::path example_path = kTestDataDir / "cmd_example.json";
    ASSERT_TRUE(std::filesystem::exists(example_path));
    miracle_jtp::CommandStream stream;
    stream.open(example_path);
    const auto& cmd_list = stream.getCommandList();
    stream.close();
    EXPECT_EQ(cmd_list.size(), 7);
}


TEST(miracle_jtp_tests, command_stream_vs_oneshot)
{
    const std::filesystem::path example_path = kTestDataDir / "cal.jsontoolpath";
    ASSERT_TRUE(std::filesystem::exists(example_path));

    // stream
    miracle_jtp::CommandStream stream;
    stream.open(example_path);
    const auto cmd_list_from_stream = stream.getCommandList();

    // oneshot
    std::ifstream fin(example_path);
    const nlohmann::json jin = nlohmann::json::parse(fin);
    fin.close();
    botcmd::CommandList cmd_list_from_oneshot;
    for (const auto& jcmd : jin)
    {
        const auto& cmd0 = miracle_jtp::toCommand(jcmd);
        cmd_list_from_oneshot.push_back(cmd0);
    }

    // check types
    EXPECT_EQ(cmd_list_from_stream.size(), cmd_list_from_oneshot.size());
    for (size_t cmd_ii=0; cmd_ii< cmd_list_from_stream.size(); cmd_ii++) {
        EXPECT_EQ(
            cmd_list_from_stream[cmd_ii]->type,
            cmd_list_from_oneshot[cmd_ii]->type
        );
    }
}


