#ifndef INCLUDE_DULCIFICUM_GCODE_AST_TOOLCHANGE_H
#define INCLUDE_DULCIFICUM_GCODE_AST_TOOLCHANGE_H

#include "dulcificum/gcode/ast/entry.h"
#include "optional"

namespace dulcificum::gcode::ast
{
/*!
 * /brief Switch to the specified tool
 */
class T : public Entry<R"(T(?<S>\d))", ctre::captured_content<1, ctre::id<'S'>>>
{
public:
    T() = delete;
    T(size_t line_index, const std::string& raw_line, regex_result_t captured);
    std::optional<size_t> S;
};
} // namespace dulcificum::gcode::ast

#endif // INCLUDE_DULCIFICUM_GCODE_AST_TOOLCHANGE_H
