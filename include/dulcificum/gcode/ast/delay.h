#ifndef DULCIFICUM_DELAY_H
#define DULCIFICUM_DELAY_H

#include "dulcificum/gcode/ast/entry.h"

namespace dulcificum::gcode::ast
{
/*!
 * /brief Dwell, or delay, for the specified time
 * S = Time to delay in seconds
 * P = Time to delay in milliseconds
 * when both S and P are specified, S takes precedence
 */
class G4 : public Entry<R"(^G4(?:(?:\sS(?<S>\d+(?:\.\d+)?))|(?:\sP(?<P>\d+(?:\.\d+)?)))*$)", ctre::captured_content<1, ctre::id<'S'>>, ctre::captured_content<2, ctre::id<'P'>>>
{
public:
    G4() = delete;
    G4(size_t line_index, const std::string& raw_line, regex_result_t captured);
    std::optional<double> S;
    std::optional<double> P;
};
} // namespace dulcificum::gcode::ast


#endif // DULCIFICUM_DELAY_H
