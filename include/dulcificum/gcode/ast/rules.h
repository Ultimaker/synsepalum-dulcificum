#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_RULES_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_RULES_H

#include <boost/spirit/home/x3.hpp>

namespace dulcificum::gcode::ast
{

// TODO: Figure out if the I should maybe use this pattern
//struct rules {
//    const boost::spirit::x3::rule<struct x, double> x_rule;
//
//    rules() : x_rule{ "x" }
//    {
//        x_rule = 'X' >> boost::spirit::x3::double_;
//    }
//};
// NOLINTBEGIN
auto const x_rule = boost::spirit::x3::rule<struct x, double>{ "x" } = 'X' >> boost::spirit::x3::double_;
auto const y_rule = boost::spirit::x3::rule<struct y, double>{ "y" } = 'Y' >> boost::spirit::x3::double_;
auto const z_rule = boost::spirit::x3::rule<struct z, double>{ "z" } = 'Z' >> boost::spirit::x3::double_;
auto const e_rule = boost::spirit::x3::rule<struct e, double>{ "e" } = 'E' >> boost::spirit::x3::double_;
auto const f_rule = boost::spirit::x3::rule<struct f, double>{ "f" } = 'F' >> boost::spirit::x3::double_;
// NOLINTEND

} // namespace dulcificum::gcode::ast

#endif // DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_RULES_H
