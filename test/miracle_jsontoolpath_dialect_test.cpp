#include "gtest/gtest.h"

#include "miraclegrue_jsontoolpath_dialect.h"

using namespace dulcificum;

TEST(miracgle_jtp, move) {
    botcmd::Move move;
    move.point = {-4.2, 42.0, 0.42, 0.0, 0.12};
    move.tags = {botcmd::Tag::Roof};
    auto jmove = miracle_jtp::toJson(move);
    std::cout << jmove << std::endl;
    auto cmove = miracle_jtp::toCommand(jmove);
}
