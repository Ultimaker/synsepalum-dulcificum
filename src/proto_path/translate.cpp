#include "dulcificum/proto_path/translate.h"

#include "dulcificum/gcode/ast/ast.h"
#include "dulcificum/proto_path/ast/ast.h"

#include <spdlog/spdlog.h>

#include <variant>

namespace dulcificum::proto_path
{

proto_path::ast::ast_t translate(gcode::ast::ast_t gcode)
{
    spdlog::info("Translating GCode to Proto Path");
    proto_path::ast::ast_t ast;

    for (auto& node : gcode)
    {
        std::visit(
            [](auto& n)
            {
                auto args = n();
                if (args.contains("X"))
                {
                    spdlog::info("X: {}", std::get<double>(args["X"]));
                }
            },
            node);
    }

    return ast;
}

} // namespace dulcificum::proto_path