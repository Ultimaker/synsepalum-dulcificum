#ifndef INCLUDE_DULCIFICUM_GCODE_AST_POSITIONING_MODE_H
#define INCLUDE_DULCIFICUM_GCODE_AST_POSITIONING_MODE_H

#include "dulcificum/gcode/ast/entry.h"

namespace dulcificum::gcode::ast
{

/*!
 * /brief Set the interpreter to absolute positions
 */
class G90 : public Entry<R"(G90)">
{
public:
    G90() = delete;
    G90(size_t idx, std::string raw_line, regex_result_t captured)
        : Entry{ idx, raw_line } {};
};

/*!
 * /brief Set the interpreter to relative positions
 */
class G91 : public Entry<R"(G91)">
{
public:
    G91() = delete;
    G91(size_t idx, std::string raw_line, regex_result_t captured)
        : Entry{ idx, raw_line } {};
};

/*!
 * /brief Set E to absolute positioning.
 */
class M82 : public Entry<R"(M82)">
{
public:
    M82() = delete;
    M82(size_t idx, std::string raw_line, regex_result_t captured)
        : Entry{ idx, raw_line } {};
};

/*!
 * /brief Set E to relative positioning.
 */
class M83 : public Entry<R"(M83)">
{
public:
    M83() = delete;
    M83(size_t idx, std::string raw_line, regex_result_t captured)
        : Entry{ idx, raw_line } {};
};

} // namespace dulcificum::gcode::ast

#endif // INCLUDE_DULCIFICUM_GCODE_AST_POSITIONING_MODE_H
