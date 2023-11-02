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

template<utils::CharRangeLiteral Pattern, class... Captures>
class Entry
{
public:
    using regex_result_t = ctre::regex_results<std::string::const_iterator, Captures...>;

    Entry(size_t index, const std::string& line)
        : index{ index }
        , line(std::move(line))
    {
    }
    Entry() = delete;
    Entry(size_t idx, std::string raw_line)
        : index{ idx }
        , line{ std::move(raw_line) }
    {
    }
    Entry(const Entry&) = default;
    Entry(Entry&&) = default;
    Entry& operator=(const Entry&) = default;
    Entry& operator=(Entry&&) = default;
    virtual ~Entry() = default;

    static inline constexpr ctll::fixed_string pattern{ Pattern.value };

    static auto match(const std::string& line)
    {
        return ctre::match<pattern>(line);
    }

    size_t index;
    std::string line;
};

} // namespace dulcificum::gcode::ast

#endif // INCLUDE_DULCIFICUM_GCODE_AST_ENTRY_H
