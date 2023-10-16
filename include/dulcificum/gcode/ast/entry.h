#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_ENTRY_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_ENTRY_H

#include "dulcificum/utils/char_range_literal.h"

#include <spdlog/spdlog.h>

#include <cstddef>
#include <ctre.hpp>
#include <string>

namespace dulcificum::gcode::ast
{

class BaseEntry
{
public:
    //    BaseEntry() = delete;
    BaseEntry(size_t index, std::string line)
        : index{ index }
        , line{ std::move(line) } {};

    size_t index{};
    std::string line{};

    virtual constexpr void operator()() = 0;
};

template<dulcificum::utils::CharRangeLiteral Pattern>
class Entry : public BaseEntry
{
public:
    Entry() = delete;
    Entry(size_t index, std::string line)
        : BaseEntry{ index, std::move(line) } {};

    constexpr auto get()
    {
        return ctre::match<Pattern.value>(line);
    }

    virtual constexpr void operator()() override
    {
        spdlog::info("lino: {} -> {}", index, line);
    }
};

} // namespace dulcificum::gcode::ast

#endif // DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_ENTRY_H
