#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_WORDS_COMMAND_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_WORDS_COMMAND_H

#include "dulcificum/gcode/ast/words/comment.h"
#include "dulcificum/gcode/ast/words/entry.h"
#include "dulcificum/utils/char_range_literal.h"

#include <optional>

namespace dulcificum::gcode::ast
{
template<utils::CharRangeLiteral Word>
struct Command : public Entry
{
    static constexpr std::string_view word{ Word.value }; ///< The specific word
    std::optional<Comment> comment; ///< Optional comment related to the command
};

} // namespace dulcificum::gcode::details

#endif // DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_WORDS_COMMAND_H
