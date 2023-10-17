#ifndef INCLUDE_DULCIFICUM_GCODE_AST_POSITION_H
#define INCLUDE_DULCIFICUM_GCODE_AST_POSITION_H

#include "dulcificum/gcode/ast/entry.h"

namespace dulcificum::gcode::ast
{
/*!
 * /brief Set the current position of one or more axes
 * X = New X axis position
 * Y = New Y axis position
 * Z = New Z axis position
 * E New extruder position
 */
class G92 : public Entry<R"(G92((?:\sX(?<X>\d+(?:\.\d+)?))|(?:\sY(?<Y>\d+(?:\.\d+)?))|(?:\sZ(?<Z>\d+(?:\.\d+)?))|(?:\sE(?<E>\d+(?:\.\d+)?)))*$)">
{
public:
    G92() = delete;
    G92(size_t index, std::string line)
        : Entry{ index, std::move(line) } {};
};
} // namespace dulcificum::gcode::ast

#endif // INCLUDE_DULCIFICUM_GCODE_AST_POSITION_H
