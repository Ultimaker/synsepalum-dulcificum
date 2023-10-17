#ifndef INCLUDE_DULCIFICUM_GCODE_AST_EXTRUDER_TEMPERATURE_H
#define INCLUDE_DULCIFICUM_GCODE_AST_EXTRUDER_TEMPERATURE_H

#include "dulcificum/gcode/ast/entry.h"

namespace dulcificum::gcode::ast
{
/*!
 * /brief Set a new target hot end temperature (non-blocking).
 * S = Target temperature
 */
class M104 : public Entry<R"(M104((?:\sS(?<S>\d+(?:\.\d+)?)))*$)">
{
public:
    M104() = delete;
    M104(size_t index, std::string line)
        : Entry{ index, std::move(line) } {};
};

/*!
 * /brief Wait for the hot end to reach its target (blocking).
 * R = Target temperature (wait for cooling or heating).
 * S = Target temperature (wait only when heating)
 */
class M109 : public Entry<R"(M109((?:\sS(?<S>\d+(?:\.\d+)?))|(?:\sR(?<R>\d+(?:\.\d+)?)))*$)">
{
public:
    M109() = delete;
    M109(size_t index, std::string line)
        : Entry{ index, std::move(line) } {};
};
} // namespace dulcificum::gcode::ast

#endif // INCLUDE_DULCIFICUM_GCODE_AST_EXTRUDER_TEMPERATURE_H
