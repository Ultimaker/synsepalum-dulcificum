#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_PARSE_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_PARSE_H

#include "dulcificum/gcode/ast/ast.h"

#include <spdlog/spdlog.h>

#include <cstddef>
#include <sstream>
#include <string>
#include <string_view>

namespace dulcificum::gcode
{

ast::ast_t parse(std::string_view content)
{
    std::istringstream stream(content.data());
    std::string line;
    ast::ast_t ast;

    size_t index{ 0 };
    while (std::getline(stream, line))
    {
        ast.emplace_back(ast::factory(index++, line));
    }

    return ast;
}


} // namespace dulcificum::gcode

#endif // DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_PARSE_H
