#include "dulcificum/gcode/ast/ast.h"

#include <cstddef>
#include <functional>
#include <map>
#include <string>

namespace dulcificum::gcode::ast
{

template<class Command>
void singleMatch(size_t index, const std::string& line, std::optional<node_t>& result)
{
    if (! result)
    {
        if (auto re_match = Command::match(line))
        {
            result = std::make_optional(Command(index, line, re_match));
        }
    }
}

template<class... Commands>
std::optional<node_t> match(size_t index, const std::string& line)
{
    std::optional<node_t> result;
    (singleMatch<Commands>(index, line, result), ...);
    return result;
}

node_t factory(size_t index, const std::string& line)
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
               Comment>(index, line)
        .value_or(Unknown{ index, line });
}

} // namespace dulcificum::gcode::ast
