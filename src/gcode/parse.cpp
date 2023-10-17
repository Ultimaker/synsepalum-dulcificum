#include "dulcificum/gcode/parse.h"
#include "dulcificum/gcode/ast/ast.h"

#include <spdlog/spdlog.h>

#include <cstddef>
#include <sstream>
#include <string>
#include <string_view>

namespace dulcificum::gcode
{

gcode::ast::ast_t parse(std::string_view content)
{
    spdlog::info("Parsing GCode as AST");
    std::istringstream stream(content.data());
    std::string line;
    gcode::ast::ast_t ast;

    size_t index{ 0 };
    while (std::getline(stream, line))
    {
        ast.emplace_back(gcode::ast::factory(index++, line));
    }

    return ast;
}

} // namespace dulcificum::gcode