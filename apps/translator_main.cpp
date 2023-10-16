#include "cmdline.h"

#include <spdlog/spdlog.h>

#include <docopt/docopt.h>
#include <dulcificum/gcode/parse.h>
#include <dulcificum/utils/io.h>
#include <map>

template<typename... Ts>
struct Overload : Ts...
{
    using Ts::operator()...;
};
template<class... Ts>
Overload(Ts...) -> Overload<Ts...>;

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
    auto ast = dulcificum::gcode::parse(input);

    auto TypeOfNode = Overload{ [](auto& node)
                                {
                                    node();
                                }
    };

    for (auto& node : ast)
    {
        std::visit(TypeOfNode, node);
    }

    auto x = 1;
}