#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_TOOLCHANGE_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_TOOLCHANGE_H

#include "dulcificum/gcode/ast/entry.h"

namespace dulcificum::gcode::ast
{
/*!
 * /brief Switch to the specified tool
 */
class T : public Entry<R"(T(?<T>\d))">
{
public:
    T() = delete;
    T(size_t index, std::string line)
        : Entry{ index, std::move(line) } {};
};
} // namespace dulcificum::gcode::ast

#endif // DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_TOOLCHANGE_H
