#ifndef INCLUDE_DULCIFICUM_GCODE_PARSE_H
#define INCLUDE_DULCIFICUM_GCODE_PARSE_H

#include "dulcificum/gcode/ast/ast.h"

#include <string_view>

namespace dulcificum::gcode
{

ast::ast_t parse(std::string_view content);

} // namespace dulcificum::gcode

#endif // INCLUDE_DULCIFICUM_GCODE_PARSE_H
