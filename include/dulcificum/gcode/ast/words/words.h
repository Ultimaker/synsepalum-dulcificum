#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_WORDS_WORDS_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_WORDS_WORDS_H

#include "dulcificum/gcode/ast/words/G0.h"
#include "dulcificum/gcode/ast/words/G1.h"

#include <variant>

namespace dulcificum::gcode::ast
{
using command_t = std::variant<G0, G1>;
}


#endif // DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_WORDS_WORDS_H
