#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_PROTO_PATH_AST_TRANSLATE_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_PROTO_PATH_AST_TRANSLATE_H

#include "dulcificum/utils/char_range_literal.h"

#include <memory>
#include <optional>
#include <string_view>
#include <vector>

namespace dulcificum::proto_path::ast
{

struct Magnitude
{
    std::optional<double> value;
};

template<utils::CharRangeLiteral Axis>
struct Coordinate : Magnitude
{
    static constexpr std::string_view axis{ Axis.value };
};

template<bool Relative = true>
struct X : Coordinate<"X">
{
};

template<bool Relative = true>
struct Y : Coordinate<"Y">
{
};

template<bool Relative = true>
struct Z : Coordinate<"Z">
{
};

template<bool Relative = true>
struct E : Coordinate<"E">
{
};

struct Translate
{
    std::shared_ptr<Translate> prev;
    X<> x{};
    Y<> y{};
    Z<> z{};
    std::vector<E<>> T{ E<>{} };
};

} // namespace dulcificum::proto_path::ast

#endif // DULCIFICUM_INCLUDE_DULCIFICUM_PROTO_PATH_AST_TRANSLATE_H
