#ifndef INCLUDE_DULCIFICUM_GCODE_TO_COMMAND_H
#define INCLUDE_DULCIFICUM_GCODE_TO_COMMAND_H

#include "dulcificum/gcode/ast/ast.h"
#include "dulcificum/command_types.h"

namespace dulcificum::gcode
{

botcmd::CommandList toCommand(gcode::ast::ast_t& gcode);

} // namespace dulcificum::gcode

#endif // INCLUDE_DULCIFICUM_GCODE_TO_COMMAND_H
