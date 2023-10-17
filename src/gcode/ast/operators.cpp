#include "dulcificum/gcode/ast/translate.h"

#include <spdlog/spdlog.h>


namespace dulcificum::gcode::ast
{

void G0::operator()()
{
    if (auto x = get().get<"X">())
    {
        spdlog::debug("x: {}", x);
    }
    spdlog::info("lino: {} -> {}", index, line);
}

void G1::operator()()
{
    if (auto x = get().get<"Y">())
    {
        spdlog::debug("Y: {}", x);
    }
    spdlog::info("lino: {} -> {}", index, line);
}

} // namespace dulcificum::gcode::ast