#include "gtest/gtest.h"

#include "miraclegrue_jsontoolpath_dialect.h"

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
