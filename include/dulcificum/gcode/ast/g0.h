#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_G0_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_G0_H

#include "dulcificum/gcode/ast/entry.h"

namespace dulcificum::gcode::ast
{
class G0 : public Entry<R"(G0((?:\sX(?<X>\d+(?:\.\d+)?))|(?:\sY(?<Y>\d+(?:\.\d+)?))|(?:\sZ(?<Z>\d+(?:\.\d+)?))|(?:\sE(?<E>\d+(?:\.\d+)?))|(?:\sF(?<F>\d+(?:\.\d+)?)))*$)">
{
public:
    G0() = delete;
    G0(size_t index, std::string line)
        : Entry{ index, std::move(line) } {};
};
} // namespace dulcificum::gcode::ast

#endif // DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_G0_H
