#include "cmdline.h"

#include <spdlog/spdlog.h>

#include <docopt/docopt.h>
#include <dulcificum/gcode/parse.h>
#include <dulcificum/utils/io.h>
#include <map>
#include <unordered_set>

template<typename... Ts>
struct Overload : Ts...
{
    using Ts::operator()...;
};
template<class... Ts>
Overload(Ts...) -> Overload<Ts...>;

void DFS(dulcificum::gcode::ast::ast_t& m, std::shared_ptr<dulcificum::gcode::ast::BaseEntry> root, std::unordered_set<std::shared_ptr<dulcificum::gcode::ast::BaseEntry>>& visited)
{
    auto range = m.equal_range(root);
    for(auto i = range.first; i != range.second; ++i)
    {
        if(visited.find(i->second) == visited.end())
        {
            i->second->operator()();
            visited.insert(i->second);
            DFS(m, i->second, visited);
        }
    }
}

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
    auto [root, ast] = dulcificum::gcode::parse(input);

    auto TypeOfNode = Overload{ [](auto& node)
                                {
                                    node();
                                }
    };

    std::unordered_set<std::shared_ptr<dulcificum::gcode::ast::BaseEntry>> visited;
    DFS(ast, root, visited);
    auto x = 1;
}