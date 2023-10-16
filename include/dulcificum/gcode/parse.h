#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_PARSE_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_PARSE_H

#include "dulcificum/gcode/ast/ast.h"

#include <spdlog/spdlog.h>

#include <cstddef>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>

namespace dulcificum::gcode
{

auto parse(std::string_view content)
{
    std::istringstream stream(content.data());
    std::string line;
    ast::ast_t ast;

    size_t index{ 0 };
    auto root = std::make_shared<ast::Entry<"">>(0, "griffin");
    while (std::getline(stream, line))
    {
        ast.emplace(root, ast::factory(index++, line));
    }

    return std::make_tuple(root, ast);
}


} // namespace dulcificum::gcode

#endif // DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_PARSE_H
