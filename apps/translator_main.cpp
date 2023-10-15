#include "cmdline.h"

#include <fmt/ranges.h>
#include <spdlog/spdlog.h>

#include <docopt/docopt.h>
#include <dulcificum/gcode/ast/ast.h>
#include <dulcificum/utils/io.h>
#include <iostream>
#include <map>


int main(int argc, const char** argv)
{
    constexpr bool show_help = true;
    const std::map<std::string, docopt::value> args
        = docopt::docopt(fmt::format(apps::cmdline::USAGE, apps::cmdline::NAME), { argv + 1, argv + argc }, show_help, apps::cmdline::VERSION_ID);

    if (args.at("--quiet").asBool())
    {
        spdlog::set_level(spdlog::level::err);
    }
    else if (args.at("--verbose").asBool())
    {
        spdlog::set_level(spdlog::level::debug);
    }
    else
    {
        spdlog::set_level(spdlog::level::info);
    }
    spdlog::info("Tasting the menu");

    auto input{ dulcificum::utils::readFile(args.at("INPUT").asString()).value() };
    std::vector<dulcificum::gcode::ast::command_t> parsedCommand;
    bool r = boost::spirit::x3::phrase_parse(input.begin(), input.end(), *dulcificum::gcode::ast::all_rule, boost::spirit::x3::space, parsedCommand);

    return 0;
}