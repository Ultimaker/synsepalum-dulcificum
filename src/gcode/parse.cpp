#include "dulcificum/gcode/parse.h"

#include "dulcificum/gcode/ast/ast.h"

#include <spdlog/spdlog.h>

#include <cstddef>
#include <sstream>
#include <string>
#include <string_view>

namespace dulcificum::gcode
{

gcode::ast::ast_t parse(std::string_view content)
{
    spdlog::info("Parsing GCode as AST test");
    try {
        spdlog::info("Creating string stream from content");
        std::istringstream stream(content.data());
        std::string line;
        gcode::ast::ast_t ast;
        size_t index{ 1 };
        spdlog::info("Starting line parsing loop");
        while (std::getline(stream, line))
        {
            spdlog::info("Parsing line {}", index);
            std::optional<ast::node_t> node;
            try {
                node = gcode::ast::factory(index, line);
                spdlog::info("Factory returned node: {}", node.has_value());
            } catch (const std::exception& e) {
                spdlog::info("Exception in factory for index {}: {}", index, e.what());
                throw;
            }
            ++index;
            if (node)
            {
                spdlog::info("Emplacing node at AST index: {}", index - 1);
                ast.emplace_back(*node);
            }
        }
        spdlog::info("Finished parsing, returning AST");
        return ast;
    } catch (const std::exception& e) {
        spdlog::info("Exception in parse(): {}", e.what());
        throw;
    }
}

} // namespace dulcificum::gcode
