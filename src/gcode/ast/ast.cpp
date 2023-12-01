#include "dulcificum/gcode/ast/ast.h"

#include <cstddef>
#include <functional>
#include <map>
#include <string>

namespace dulcificum::gcode::ast
{
// Split-up templates. 'Difficult' node-types have templates with 'large' CharRangeLiterals. See below as well.
using node_normal_t = std::variant<G0_G1, M204, M205, M104, M106, M107, M109, M140, M190, G4, T, G90, G91, G92, G280, M82, M83, Layer, Mesh, FeatureType, Comment>;
using node_difficult_t = std::variant<InitialTemperatureBuildPlate, BuildVolumeTemperature, InitialTemperatureExtruder>;

template<typename N, size_t I = 0>
void factory_helper(size_t line_index, const std::string& raw_line, std::optional<node_t>& result)
{
    if constexpr (I < std::variant_size_v<N>)
    {
        using Command = std::variant_alternative_t<I, N>;
        if (auto re_match = Command::match(raw_line))
        {
            result = Command(line_index, raw_line, re_match);
        }
        else
        {
            factory_helper<N, I + 1>(line_index, raw_line, result);
        }
    }
}

node_t factory(size_t line_index, const std::string& raw_line)
{
    // Split-up all templates across multiple calls, in order to;
    //  1. fix an issue where in at least one compiler (MSVC) it'd not work, and,
    //  2. reduce compilation-time just a bit (still long though).
    const static auto split_helpers = {
        factory_helper<node_normal_t>,
        factory_helper<node_difficult_t>,
    };

    std::optional<node_t> result;
    for (const auto& helper_func : split_helpers)
    {
        helper_func(line_index, raw_line, result);
        if (result.has_value())
        {
            return result.value();
        }
    }
    return Unknown{ line_index, raw_line };
}

} // namespace dulcificum::gcode::ast
