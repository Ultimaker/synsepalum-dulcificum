#ifndef INCLUDE_DULCIFICUM_GCODE_AST_EXTRUDER_TEMPERATURE_H
#define INCLUDE_DULCIFICUM_GCODE_AST_EXTRUDER_TEMPERATURE_H

#include "dulcificum/gcode/ast/entry.h"

#include <optional>

namespace dulcificum::gcode::ast
{
/*!
 * /brief Set a new target hot end temperature (non-blocking).
 * S = Target temperature
 */
class M104 : public Entry<R"(M104((?:\sS(?<S>\d+(?:\.\d+)?))|(?:\sT(?<T>\d+(?:\.\d+)?)))*$)">
{
public:
    M104() = delete;
    M104(size_t idx, std::string raw_line);
    std::optional<double> S;
    std::optional<size_t> T;
};

/*!
 * /brief Wait for the hot end to reach its target (blocking).
 * R = Target temperature (wait for cooling or heating).
 * S = Target temperature (wait only when heating)
 */
class M109 : public Entry<R"(M109((?:\sS(?<S>\d+(?:\.\d+)?))|(?:\sR(?<R>\d+(?:\.\d+)?))|(?:\sT(?<T>\d+(?:\.\d+)?)))*$)">
{
public:
    M109() = delete;
    M109(size_t idx, std::string raw_line);
    std::optional<double> S;
    std::optional<double> R;
    std::optional<size_t> T;
};
} // namespace dulcificum::gcode::ast

#endif // INCLUDE_DULCIFICUM_GCODE_AST_EXTRUDER_TEMPERATURE_H
