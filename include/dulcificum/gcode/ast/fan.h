#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_FAN_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_FAN_H

#include "dulcificum/gcode/ast/entry.h"

namespace dulcificum::gcode::ast
{
/*!
 * /brief with no speed sets the fan to full speed.
 * S = Speed, from 0 to 255
 */
class M106 : public Entry<R"(M106((?:\sS(?<S>\d+(?:\.\d+)?)))*$)">
{
public:
    M106() = delete;
    M106(size_t index, std::string line)
        : Entry{ index, std::move(line) } {};
};

/*!
 * /brief Turn off a fan
 */
class M107 : public Entry<R"(M107)">
{
public:
    M107() = delete;
    M107(size_t index, std::string line)
        : Entry{ index, std::move(line) } {};
};
} // namespace dulcificum::gcode::ast

#endif // DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_FAN_H
