#include "cmdline.h"

#include <dulcificum.h>
#include <dulcificum/utils/io.h>

#include <spdlog/spdlog.h>
#include <docopt/docopt.h>

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

    size_t nb_extruders = 2;
    auto nb_extruders_arg = args.at("--nb_extruders");
    if(nb_extruders_arg)
    {
        nb_extruders = nb_extruders_arg.asLong();
    }

    auto input{ dulcificum::utils::readFile(args.at("INPUT").asString()).value() };
    auto translated = dulcificum::GCode2Miracle_JTP(input, nb_extruders);
    dulcificum::utils::writeFile(args.at("OUTPUT").asString(), translated);
}
