#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_BED_TEMPERATURE_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_BED_TEMPERATURE_H

#include "dulcificum/gcode/ast/entry.h"

namespace dulcificum::gcode::ast
{
/*!
 * /brief Set a new target bed temperature (non-blocking).
 * S = Target temperature
 */
class M140 : public Entry<R"(M140((?:\sS(?<S>\d+(?:\.\d+)?)))*$)">
{
public:
    M140() = delete;
    M140(size_t index, std::string line)
        : Entry{ index, std::move(line) } {};
};

/*!
 * /brief Wait for the bed to reach target temperature (blocking).
 * R = Target temperature (wait for cooling or heating).
 * S = Target temperature (wait only when heating)
 */
class M190 : public Entry<R"(M190((?:\sS(?<S>\d+(?:\.\d+)?))|(?:\sR(?<R>\d+(?:\.\d+)?)))*$)">
{
public:
    M190() = delete;
    M190(size_t index, std::string line)
        : Entry{ index, std::move(line) } {};
};
} // namespace dulcificum::gcode::ast

#endif // DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_BED_TEMPERATURE_H
