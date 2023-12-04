#ifndef INCLUDE_DULCIFICUM_GCODE_AST_PURGE_H
#define INCLUDE_DULCIFICUM_GCODE_AST_PURGE_H

#include "dulcificum/gcode/ast/entry.h"

#include <optional>

namespace dulcificum::gcode::ast
{
/*!
 * /brief Purge the tool
 */
class G280 : public Entry<R"(^G280(?:(?:\sS(?<S>\d+(?:\.\d+)?)))*$)", ctre::captured_content<1, ctre::id<'S'>>>
{
public:
    G280() = delete;
    G280(size_t line_index, const std::string& raw_line, regex_result_t captured);
    std::optional<size_t> S;
};
} // namespace dulcificum::gcode::ast

#endif // INCLUDE_DULCIFICUM_GCODE_AST_PURGE_H
