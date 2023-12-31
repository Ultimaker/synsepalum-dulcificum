#ifndef INCLUDE_DULCIFICUM_GCODE_AST_BED_TEMPERATURE_H
#define INCLUDE_DULCIFICUM_GCODE_AST_BED_TEMPERATURE_H

#include "dulcificum/gcode/ast/entry.h"

#include <optional>

namespace dulcificum::gcode::ast
{
/*!
 * /brief Set a new target bed temperature (non-blocking).
 * S = Target temperature
 */
class M140 : public Entry<R"(^M140(?:(?:\sS(?<S>\d+(?:\.\d+)?)))*$)", ctre::captured_content<1, ctre::id<'S'>>>
{
public:
    M140() = delete;
    M140(size_t line_index, const std::string& raw_line, regex_result_t captured);
    std::optional<double> S;
};

/*!
 * /brief Wait for the bed to reach target temperature (blocking).
 * R = Target temperature (wait for cooling or heating).
 * S = Target temperature (wait only when heating)
 */
class M190 : public Entry<R"(^M190(?:(?:\sS(?<S>\d+(?:\.\d+)?))|(?:\sR(?<R>\d+(?:\.\d+)?)))*$)", ctre::captured_content<1, ctre::id<'S'>>, ctre::captured_content<2, ctre::id<'R'>>>
{
public:
    M190() = delete;
    M190(size_t line_index, const std::string& raw_line, regex_result_t captured);
    std::optional<double> S;
    std::optional<double> R;
};
} // namespace dulcificum::gcode::ast

#endif // INCLUDE_DULCIFICUM_GCODE_AST_BED_TEMPERATURE_H
