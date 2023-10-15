#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_WORDS_ENTRY_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_WORDS_ENTRY_H

#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>

namespace dulcificum::gcode::ast
{
struct Entry : public boost::spirit::x3::position_tagged
{
    std::size_t index{ 0 }; ///< Represents the index in the source
    std::string raw_value; ///< Raw value as represented in the source
};

} // namespace dulcificum::gcode::ast


#endif // DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_WORDS_ENTRY_H
