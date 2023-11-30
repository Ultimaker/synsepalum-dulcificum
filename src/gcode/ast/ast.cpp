#include "dulcificum/gcode/ast/ast.h"

#include <cstddef>
#include <functional>
#include <map>
#include <string>

namespace dulcificum::gcode::ast
{
    using node_likely_t = std::variant<
        G0_G1,
        M204,
        M205,
        M104,
        M106,
        M107,
        M109,
        M140,
        M190>;
    using node_occasional_t = std::variant<
        G4,
        T,
        G90,
        G91,
        G92,
        G280,
        M82,
        M83>;
    using node_rare_t = std::variant<
        Layer,
        Mesh,
        FeatureType,
        InitialTemperatureBuildPlate,
        BuildVolumeTemperature,
        InitialTemperatureExtruder,
        Comment>;

    template<typename N, size_t I = 0>
    void factory_helper(size_t line_index, const std::string& raw_line, std::optional<node_t>& result)
    {
        if (result)
        {
            return;
        }
        if constexpr (I < std::variant_size_v<N>)
        {
            using Command = std::variant_alternative_t<I, N>;
            if (auto re_match = Command::match(raw_line))
            {
                result = Command(line_index, raw_line, re_match);
            }
        }
    }

    node_t factory(size_t line_index, const std::string& raw_line)
    {
        static const auto split_helpers = { factory_helper<node_likely_t>, factory_helper<node_occasional_t>, factory_helper<node_rare_t>, };

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
