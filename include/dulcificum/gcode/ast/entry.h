#ifndef INCLUDE_DULCIFICUM_GCODE_AST_ENTRY_H
#define INCLUDE_DULCIFICUM_GCODE_AST_ENTRY_H

#include "dulcificum/utils/char_range_literal.h"

#include <array>
#include <cstddef>
#include <ctll/fixed_string.hpp>
#include <ctre.hpp>
#include <map>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <variant>

namespace dulcificum::gcode::ast
{

template<utils::CharRangeLiteral Pattern>
class Entry
{
public:
    using value_type = std::map<std::string_view, std::variant<double, int, std::string_view>>;

    Entry() = delete;
    Entry(size_t idx, std::string raw_line)
        : index{ idx }
        , line{ std::move(raw_line) } {};
    Entry(const Entry&) = default;
    Entry(Entry&&) = default;
    Entry& operator=(const Entry&) = default;
    Entry& operator=(Entry&&) = default;
    virtual ~Entry() = default;

    size_t index;
    std::string line;
    static inline constexpr ctll::fixed_string pattern{ Pattern.value };

    constexpr auto get()
    {
        return ctre::match<pattern>(line);
    };

    virtual value_type operator()()
    {
        return {};
    };
};

} // namespace dulcificum::gcode::ast

#endif // INCLUDE_DULCIFICUM_GCODE_AST_ENTRY_H
