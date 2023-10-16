#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_TRANSLATE_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_TRANSLATE_H

#include "dulcificum/gcode/ast/entry.h"


namespace dulcificum::gcode::ast
{
/*!
 * /brief Add a straight line movement to the planner (by convention non-extrusion)
 * X = An absolute or relative coordinate on the X axis (in current units).
 * Y = An absolute or relative coordinate on the Y axis (in current units).
 * Z = An absolute or relative coordinate on the Z axis (in current units).
 * E = An absolute or relative coordinate on the E axis (in current units).
 * F = The maximum movement rate of the move between the start and end point.
 */
class G0 : public Entry<R"(G0((?:\sX(?<X>\d+(?:\.\d+)?))|(?:\sY(?<Y>\d+(?:\.\d+)?))|(?:\sZ(?<Z>\d+(?:\.\d+)?))|(?:\sE(?<E>\d+(?:\.\d+)?))|(?:\sF(?<F>\d+(?:\.\d+)?)))*$)">
{
public:
    G0() = delete;
    G0(size_t index, std::string line)
        : Entry{ index, std::move(line) } {};

    void operator()() override final
    {
        if (auto x = get().get<"X">())
        {
            spdlog::debug("x: {}", x);
        }
        spdlog::info("lino: {} -> {}", index, line);
    }
};

/*!
 * /brief Add a straight line movement to the planner (by convention extrusion)
 * X = An absolute or relative coordinate on the X axis (in current units).
 * Y = An absolute or relative coordinate on the Y axis (in current units).
 * Z = An absolute or relative coordinate on the Z axis (in current units).
 * E = An absolute or relative coordinate on the E axis (in current units).
 * F = The maximum movement rate of the move between the start and end point.
 */
class G1 : public Entry<R"(G1((?:\sX(?<X>\d+(?:\.\d+)?))|(?:\sY(?<Y>\d+(?:\.\d+)?))|(?:\sZ(?<Z>\d+(?:\.\d+)?))|(?:\sE(?<E>\d+(?:\.\d+)?))|(?:\sF(?<F>\d+(?:\.\d+)?)))*$)">
{
public:
    G1() = delete;
    G1(size_t index, std::string line)
        : Entry{ index, std::move(line) } {};
};

} // namespace dulcificum::gcode::ast

#endif // DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_TRANSLATE_H
