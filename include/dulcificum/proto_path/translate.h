#ifndef INCLUDE_DULCIFICUM_PROTO_PATH_TRANSLATE_H
#define INCLUDE_DULCIFICUM_PROTO_PATH_TRANSLATE_H

#include "dulcificum/gcode/ast/ast.h"
#include "dulcificum/proto_path/ast/ast.h"

namespace dulcificum::proto_path
{

ast::ast_t translate(gcode::ast::ast_t gcode);

} // namespace dulcificum::proto_path

#endif // INCLUDE_DULCIFICUM_PROTO_PATH_TRANSLATE_H
