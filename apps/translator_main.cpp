#include "cmdline.h"

#include <spdlog/spdlog.h>

#include <docopt/docopt.h>
#include <dulcificum/gcode/parse.h>
#include <dulcificum/miracle_jtp/mgjtp_command_to_json.h>
#include <dulcificum/gcode/gcode_to_command.h>
#include <dulcificum/utils/io.h>
#include <map>

#include <nlohmann/json.hpp>

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
    auto gcode_ast = dulcificum::gcode::parse(input);
    auto command_list = dulcificum::gcode::toCommand(gcode_ast);

    auto json_commands = nlohmann::json::array();
    for (const auto& command: command_list)
    {
        json_commands.emplace_back(dulcificum::miracle_jtp::toJson(*command));
    }

    auto x = 1;
}