#ifndef INCLUDE_DULCIFICUM_GCODE_AST_FAN_H
#define INCLUDE_DULCIFICUM_GCODE_AST_FAN_H

#include "dulcificum/gcode/ast/entry.h"

#include <optional>

namespace dulcificum::gcode::ast
{
/*!
 * /brief with no speed sets the fan to full speed.
 * S = Speed, from 0 to 255
 * P = Fan index
 */
class M106 : public Entry<R"(M106((?:\sS(?<S>\d+(?:\.\d+)?))|(?:\sP(?<P>\d+(?:\.\d+)?)))*$)">
{
public:
    M106() = delete;
    M106(size_t idx, std::string raw_line);
    std::optional<double> S;
    std::optional<size_t> P;
};

/*!
 * /brief Turn off a fan
 */
class M107 : public Entry<R"(M107)">
{
public:
    M107() = delete;
    M107(size_t idx, std::string raw_line)
        : Entry{ idx, std::move(raw_line) } {};
};
} // namespace dulcificum::gcode::ast

#endif // INCLUDE_DULCIFICUM_GCODE_AST_FAN_H
