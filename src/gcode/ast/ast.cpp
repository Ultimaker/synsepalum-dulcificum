#include "dulcificum/gcode/ast/ast.h"

#include <cstddef>
#include <functional>
#include <map>
#include <string>

namespace dulcificum::gcode::ast
{

template<class Command>
void singleMatch(size_t line_index, const std::string& raw_line, std::optional<node_t>& result)
{
    if (! result)
    {
        if (auto re_match = Command::match(raw_line))
        {
            result = std::make_optional(Command(line_index, raw_line, re_match));
        }
    }
}

template<class... Commands>
std::optional<node_t> match(size_t line_index, const std::string& raw_line)
{
    std::optional<node_t> result;
    (singleMatch<Commands>(line_index, raw_line, result), ...);
    return result;
}

node_t factory(size_t line_index, const std::string& raw_line)
{
    // Make sure to list the commands classes in order of most probable appearance in the gcode file for better performance
    return match<
               G0_G1,
               M204,
               M205,
               M104,
               M106,
               M107,
               M109,
               M140,
               M190,
               G4,
               T,
               G90,
               G91,
               G92,
               G280,
               M82,
               M83,
               Layer,
               Mesh,
               FeatureType,
               InitialTemperatureBuildPlate,
               BuildVolumeTemperature,
               InitialTemperatureExtruder,
               Comment>(line_index, raw_line)
        .value_or(Unknown{ line_index, raw_line });
}

} // namespace dulcificum::gcode::ast
