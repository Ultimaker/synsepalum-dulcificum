#ifndef INCLUDE_DULCIFICUM_PROTO_PATH_AST_AST_H
#define INCLUDE_DULCIFICUM_PROTO_PATH_AST_AST_H

#include "dulcificum/proto_path/ast/translate.h"

#include <variant>
#include <vector>

namespace dulcificum::proto_path::ast
{
using node_t = std::variant<Translate>;
using ast_t = std::vector<node_t>;

node_t factory(const auto& ast)
{
    return Translate{};
}
} // namespace dulcificum::proto_path::ast

#endif // INCLUDE_DULCIFICUM_PROTO_PATH_AST_AST_H
