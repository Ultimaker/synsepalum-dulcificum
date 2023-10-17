#include "dulcificum/proto_path/translate.h"

#include <spdlog/spdlog.h>

namespace dulcificum::proto_path
{

ast::ast_t translate(gcode::ast::ast_t gcode)
{
    spdlog::info("Translating GCode to Proto Path");
    ast::ast_t ast;

    return ast;
}

} // namespace dulcificum::proto_path