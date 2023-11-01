#ifndef INCLUDE_DULCIFICUM_GCODE_AST_TRANSLATE_H
#define INCLUDE_DULCIFICUM_GCODE_AST_TRANSLATE_H

#include "dulcificum/gcode/ast/entry.h"
#include "dulcificum/state.h"

#include <cstddef>
#include <optional>
#include <string>


namespace dulcificum::gcode::ast
{
/*!
 * /brief Add a straight line movement to the planner
 * X = An absolute or relative coordinate on the X axis (in current units).
 * Y = An absolute or relative coordinate on the Y axis (in current units).
 * Z = An absolute or relative coordinate on the Z axis (in current units).
 * E = An absolute or relative coordinate on the E axis (in current units).
 * F = The maximum movement rate of the move between the start and end point.
 */
class G0_G1 : public Entry<
                  R"(G[0|1](?:(?:\sX(?<X>-?\d*(?:\.\d*)?))|(?:\sY(?<Y>-?\d*(?:\.\d*)?))|(?:\sZ(?<Z>-?\d*(?:\.\d*)?))|(?:\sE(?<E>-?\d*(?:\.\d*)?))|(?:\sF(?<F>-?\d*(?:\.\d*)?)))*$)",
                  ctre::captured_content<1, ctre::id<'X'>>,
                  ctre::captured_content<2, ctre::id<'Y'>>,
                  ctre::captured_content<3, ctre::id<'Z'>>,
                  ctre::captured_content<4, ctre::id<'E'>>,
                  ctre::captured_content<5, ctre::id<'F'>>>
{
public:
    std::optional<double> X;
    std::optional<double> Y;
    std::optional<double> Z;
    std::optional<double> E;
    std::optional<double> F;

    G0_G1() = delete;
    G0_G1(size_t idx, std::string line, regex_result_t captured);
};

} // namespace dulcificum::gcode::ast

#endif // INCLUDE_DULCIFICUM_GCODE_AST_TRANSLATE_H
