#ifndef INCLUDE_DULCIFICUM_GCODE_AST_POSITION_H
#define INCLUDE_DULCIFICUM_GCODE_AST_POSITION_H

#include "dulcificum/gcode/ast/entry.h"

#include <optional>

namespace dulcificum::gcode::ast
{
/*!
 * /brief Set the current position of one or more axes
 * X = New X axis position
 * Y = New Y axis position
 * Z = New Z axis position
 * E New extruder position
 */
class G92 : public Entry<
                R"(^G92(?:(?:\sX(?<X>-?\d+(?:\.\d+)?))|(?:\sY(?<Y>-?\d+(?:\.\d+)?))|(?:\sZ(?<Z>-?\d+(?:\.\d+)?))|(?:\sE(?<E>-?\d+(?:\.\d+)?)))*$)",
                ctre::captured_content<1, ctre::id<'X'>>,
                ctre::captured_content<2, ctre::id<'Y'>>,
                ctre::captured_content<3, ctre::id<'Z'>>,
                ctre::captured_content<4, ctre::id<'E'>>>
{
public:
    G92() = delete;
    G92(size_t line_index, const std::string& raw_line, regex_result_t captured);
    std::optional<double> X;
    std::optional<double> Y;
    std::optional<double> Z;
    std::optional<double> E;
};
} // namespace dulcificum::gcode::ast

#endif // INCLUDE_DULCIFICUM_GCODE_AST_POSITION_H
