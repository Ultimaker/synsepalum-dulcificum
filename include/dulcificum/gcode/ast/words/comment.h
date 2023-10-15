#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_WORDS_COMMENT_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_WORDS_COMMENT_H

#include "dulcificum/gcode/ast/words/entry.h"

namespace dulcificum::gcode::ast
{
struct Comment : public Entry
{
    static constexpr std::string_view word{ ";" };
    std::string msg; ///< Message contained in the comment
};
} // namespace dulcificum::gcode::details

#endif // DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_WORDS_COMMENT_H
