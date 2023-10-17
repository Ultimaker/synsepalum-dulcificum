#ifndef INCLUDE_DULCIFICUM_GCODE_AST_TOOLCHANGE_H
#define INCLUDE_DULCIFICUM_GCODE_AST_TOOLCHANGE_H

#include "dulcificum/gcode/ast/entry.h"

namespace dulcificum::gcode::ast
{
/*!
 * /brief Switch to the specified tool
 */
class T : public Entry<R"(T(?<S>\d))">
{
public:
    T() = delete;
    T(size_t index, std::string line);
    std::optional<size_t> S;
};
} // namespace dulcificum::gcode::ast

#endif // INCLUDE_DULCIFICUM_GCODE_AST_TOOLCHANGE_H
