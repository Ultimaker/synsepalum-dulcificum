#include "cmdline.h"

#include <spdlog/spdlog.h>

#include <docopt/docopt.h>
#include <map>

int main(int argc, const char** argv)
{
    constexpr bool show_help = true;
    const std::map<std::string, docopt::value> args
        = docopt::docopt(fmt::format(apps::cmdline::USAGE, apps::cmdline::NAME), { argv + 1, argv + argc }, show_help, apps::cmdline::VERSION_ID);

    if (args.contains("--quiet"))
    {
        spdlog::set_level(spdlog::level::err);
    }
    else if (args.contains("--verbose"))
    {
        spdlog::set_level(spdlog::level::debug);
    }
    else
    {
        spdlog::set_level(spdlog::level::info);
    }
    spdlog::info("Tasting the menu");
}