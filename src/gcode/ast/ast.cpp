#include "dulcificum/gcode/ast/ast.h"

#include <cstddef>
#include <functional>
#include <map>
#include <string>

namespace dulcificum::gcode::ast
{

node_t factory(size_t index, const std::string& line)
{
    using Creator = std::function<node_t(size_t, const std::string&)>;
    // clang-format off
    std::map<std::string, Creator> const creators
    {
        { "G1", [](size_t index, const std::string& line) { return G0_G1{ index, line }; } },
        { "G0", [](size_t index, const std::string& line) { return G0_G1{ index, line }; } },
        { "T", [](size_t index, const std::string& line) { return T{ index, line }; } },
        { "G90", [](size_t index, const std::string& line) { return G90{ index, line }; } },
        { "G91", [](size_t index, const std::string& line) { return G91{ index, line }; } },
        { "G92", [](size_t index, const std::string& line) { return G92{ index, line }; } },
        { "G280", [](size_t index, const std::string& line) { return G280{ index, line }; } },
        { "M82", [](size_t index, const std::string& line) { return M82{ index, line }; } },
        { "M83", [](size_t index, const std::string& line) { return M83{ index, line }; } },
        { "M104", [](size_t index, const std::string& line) { return M104{ index, line }; } },
        { "M106", [](size_t index, const std::string& line) { return M106{ index, line }; } },
        { "M107", [](size_t index, const std::string& line) { return M107{ index, line }; } },
        { "M109", [](size_t index, const std::string& line) { return M109{ index, line }; } },
        { "M140", [](size_t index, const std::string& line) { return M140{ index, line }; } },
        { "M190", [](size_t index, const std::string& line) { return M190{ index, line }; } },
        { "M204", [](size_t index, const std::string& line) { return M204{ index, line }; } },
        { "M205", [](size_t index, const std::string& line) { return M205{ index, line }; } },
        { ";LAYER:", [](size_t index, const std::string& line) { return Layer{ index, line }; } },
        { ";MESH:", [](size_t index, const std::string& line) { return Mesh{ index, line }; } },
        { ";TYPE:", [](size_t index, const std::string& line) { return FeatureType{ index, line }; } },
        { ";BUILD_PLATE.INITIAL_TEMPERATURE:", [](size_t index, const std::string& line) { return InitialTemperatureBuildPlate{ index, line }; } },
        { ";BUILD_VOLUME.TEMPERATURE:", [](size_t index, const std::string& line) { return BuildVolumeTemperature{ index, line }; } },
        { ";EXTRUDER_TRAIN.0.INITIAL_TEMPERATURE:", [](size_t index, const std::string& line) { return InitialTemperatureExtruder{ index, line }; } },
        { ";EXTRUDER_TRAIN.1.INITIAL_TEMPERATURE:", [](size_t index, const std::string& line) { return InitialTemperatureExtruder{ index, line }; } },
    };
    // clang-format on
    for (const auto& [id, creator] : creators)
    {
        if (line.starts_with(id))
        {
            return creator(index, line);
        }
    }
    return Unknown{ index, line };
}

} // namespace dulcificum::gcode::ast