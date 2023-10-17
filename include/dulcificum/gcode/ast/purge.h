#ifndef INCLUDE_DULCIFICUM_GCODE_AST_PURGE_H
#define INCLUDE_DULCIFICUM_GCODE_AST_PURGE_H

#include "dulcificum/gcode/ast/entry.h"

namespace dulcificum::gcode::ast
{
/*!
 * /brief Purge the tool
 */
class G280 : public Entry<R"(G280((?:\sS(?<S>\d+(?:\.\d+)?)))*$)">
{
public:
    G280() = delete;
    G280(size_t index, std::string line)
        : Entry{ index, std::move(line) } {};
};
} // namespace dulcificum::gcode::ast

#endif // INCLUDE_DULCIFICUM_GCODE_AST_PURGE_H
