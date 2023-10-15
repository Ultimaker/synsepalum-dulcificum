#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_WORDS_G0_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_WORDS_G0_H
#include "dulcificum/gcode/ast/rules.h"
#include "dulcificum/gcode/ast/words/command.h"
#include "dulcificum/gcode/ast/words/optional_values.h"
namespace dulcificum::gcode::ast
{
struct G0 : public Command<"G0">
{
    std::optional<double> X{}; ///< X coordinate
    std::optional<double> Y{}; ///< Y coordinate
    std::optional<double> Z{}; ///< Z coordinate
    std::optional<double> F{}; ///< Feedrate
};


// Construct the parser rules

details::OptionalValues G0_VALUES;

auto G0_ASSIGN_OPTIONAL_VALUES = [](auto& ctx)
{
    _val(ctx).X = G0_VALUES.val_map["x"];
    _val(ctx).Y = G0_VALUES.val_map["y"];
    _val(ctx).Z = G0_VALUES.val_map["z"];
    _val(ctx).F = G0_VALUES.val_map["f"];
};

// clang-format off
auto const g0_rule = boost::spirit::x3::rule<struct g0, G0>{ "g0" } = G0::word.data()
                                                                   >> *(x_rule[G0_VALUES.assign("x", G0_VALUES.val_map)]
                                                                        | y_rule[G0_VALUES.assign("y", G0_VALUES.val_map)]
                                                                        | z_rule[G0_VALUES.assign("z", G0_VALUES.val_map)]
                                                                        | f_rule[G0_VALUES.assign("f", G0_VALUES.val_map)])
                                                                   >> boost::spirit::x3::eps[G0_ASSIGN_OPTIONAL_VALUES];
// clang-format on

} // namespace dulcificum::gcode::ast
#endif // DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_WORDS_G0_H