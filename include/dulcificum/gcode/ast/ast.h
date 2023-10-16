#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_AST_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_AST_H

#include "dulcificum/gcode/ast/g0.h"
#include "dulcificum/gcode/ast/g1.h"

#include <variant>
#include <vector>
#include <string_view>

namespace dulcificum::gcode::ast
{

using Unknown = Entry<R"((.*))">;
using Comment = Entry<R"(;(.*))">;
using node_t = std::variant<Unknown, Comment, G0, G1>;
using ast_t = std::vector<node_t>;

node_t factory(size_t index, std::string line)
{
    if (line.starts_with(';'))
    {
        return Comment { index, line };
    }
    if (line.starts_with("G0"))
    {
        return G0 { index, line };
    }
    if (line.starts_with("G1"))
    {
        return G1 { index, line };
    }
    return Unknown { index, line };
}

} // namespace dulcificum::gcode::ast

#endif // DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_AST_H
