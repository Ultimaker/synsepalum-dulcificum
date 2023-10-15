#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_WORDS_G1_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_WORDS_G1_H

#include "dulcificum/gcode/ast/rules.h"
#include "dulcificum/gcode/ast/words/command.h"

namespace dulcificum::gcode::ast
{
struct G1 : public Command<"G1">
{
    std::optional<double> X{}; ///< X coordinate
    std::optional<double> Y{}; ///< Y coordinate
    std::optional<double> Z{}; ///< Z coordinate
    std::optional<double> E{}; ///< E position
    std::optional<double> F{}; ///< Feedrate
};


// Construct the parser rules

details::OptionalValues G1_VALUES;

auto G1_ASSIGN_OPTIONAL_VALUES = [](auto& ctx)
{
    _val(ctx).X = G0_VALUES.val_map["x"];
    _val(ctx).Y = G0_VALUES.val_map["y"];
    _val(ctx).Z = G0_VALUES.val_map["z"];
    _val(ctx).E = G0_VALUES.val_map["e"];
    _val(ctx).F = G0_VALUES.val_map["f"];
};

// clang-format off
auto const g1_rule = boost::spirit::x3::rule<struct g1, G1>{ "g1" } = G1::word.data()
                                                                   >> *(x_rule[G1_VALUES.assign("x", G1_VALUES.val_map)]
                                                                        | y_rule[G1_VALUES.assign("y", G1_VALUES.val_map)]
                                                                        | z_rule[G1_VALUES.assign("z", G1_VALUES.val_map)]
                                                                        | e_rule[G1_VALUES.assign("e", G1_VALUES.val_map)]
                                                                        | f_rule[G1_VALUES.assign("f", G1_VALUES.val_map)])
                                                                   >> boost::spirit::x3::eps[G1_ASSIGN_OPTIONAL_VALUES];
// clang-format on

} // namespace dulcificum::gcode::ast

#endif // DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_WORDS_G1_H
