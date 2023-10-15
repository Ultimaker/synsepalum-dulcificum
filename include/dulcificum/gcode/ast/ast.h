#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_AST_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_AST_H

#include "dulcificum/gcode/ast/words/words.h"

namespace dulcificum::gcode::ast
{
auto const all_rule = boost::spirit::x3::rule<struct class_all, command_t>{} = g0_rule | g1_rule; // NOLINT

}

#endif // DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_AST_H
