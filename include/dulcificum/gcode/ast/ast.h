#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_AST_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_AST_H

#include "dulcificum/gcode/ast/acceleration.h"
#include "dulcificum/gcode/ast/bed_temperature.h"
#include "dulcificum/gcode/ast/comment_commands.h"
#include "dulcificum/gcode/ast/entry.h"
#include "dulcificum/gcode/ast/extruder_temperature.h"
#include "dulcificum/gcode/ast/fan.h"
#include "dulcificum/gcode/ast/position.h"
#include "dulcificum/gcode/ast/positioning_mode.h"
#include "dulcificum/gcode/ast/purge.h"
#include "dulcificum/gcode/ast/toolchange.h"
#include "dulcificum/gcode/ast/translate.h"

#include <cstddef>
#include <memory>
#include <string>
#include <map>
#include <vector>

namespace dulcificum::gcode::ast
{

using Unknown = Entry<R"((.*))">;

// using node_t = std::variant<
//     G0,
//     G1,
//     G90,
//     G91,
//     G92,
//     G280,
//     M82,
//     M83,
//     M104,
//     M106,
//     M107,
//     M109,
//     M140,
//     M190,
//     M204,
//     M205,
//     Layer,
//     Mesh,
//     FeatureType,
//     InitialTemperatureExtruder,
//     InitialTemperatureBuildPlate,
//     BuildVolumeTemperature,
//     T,
//     Unknown>;
using ast_t = std::multimap<std::shared_ptr<BaseEntry>, std::shared_ptr<BaseEntry>>;

using Creator = std::function<std::shared_ptr<BaseEntry>(size_t, const std::string&)>;

// clang-format off
std::map<std::string, Creator> creators
{
    { "G1", [](size_t index, const std::string& line) { return std::make_shared<G1>( index, line ); } },
    { "G0", [](size_t index, const std::string& line) { return std::make_shared<G0>( index, line ); } },
    { "T", [](size_t index, const std::string& line) { return std::make_shared<T>( index, line ); } },
    { "G90", [](size_t index, const std::string& line) { return std::make_shared<G90>( index, line ); } },
    { "G91", [](size_t index, const std::string& line) { return std::make_shared<G91>( index, line ); } },
    { "G92", [](size_t index, const std::string& line) { return std::make_shared<G92>( index, line ); } },
    { "G280", [](size_t index, const std::string& line) { return std::make_shared<G280>( index, line ); } },
    { "M82", [](size_t index, const std::string& line) { return std::make_shared<M82>( index, line ); } },
    { "M83", [](size_t index, const std::string& line) { return std::make_shared<M83>( index, line ); } },
    { "M104", [](size_t index, const std::string& line) { return std::make_shared<M104>( index, line ); } },
    { "M106", [](size_t index, const std::string& line) { return std::make_shared<M106>( index, line ); } },
    { "M107", [](size_t index, const std::string& line) { return std::make_shared<M107>( index, line ); } },
    { "M109", [](size_t index, const std::string& line) { return std::make_shared<M109>( index, line ); } },
    { "M140", [](size_t index, const std::string& line) { return std::make_shared<M140>( index, line ); } },
    { "M190", [](size_t index, const std::string& line) { return std::make_shared<M190>( index, line ); } },
    { "M204", [](size_t index, const std::string& line) { return std::make_shared<M204>( index, line ); } },
    { "M205", [](size_t index, const std::string& line) { return std::make_shared<M205>( index, line ); } },
    { ";LAYER:", [](size_t index, const std::string& line) { return std::make_shared<Layer>( index, line ); } },
    { ";MESH:", [](size_t index, const std::string& line) { return std::make_shared<Mesh>( index, line ); } },
    { ";TYPE:", [](size_t index, const std::string& line) { return std::make_shared<FeatureType>( index, line ); } },
    { ";BUILD_PLATE.INITIAL_TEMPERATURE:", [](size_t index, const std::string& line) { return std::make_shared<InitialTemperatureBuildPlate>( index, line ); } },
    { ";BUILD_VOLUME.TEMPERATURE:", [](size_t index, const std::string& line) { return std::make_shared<BuildVolumeTemperature>( index, line ); } },
    { ";EXTRUDER_TRAIN.0.INITIAL_TEMPERATURE:", [](size_t index, const std::string& line) { return std::make_shared<InitialTemperatureExtruder>( index, line ); } },
    { ";EXTRUDER_TRAIN.1.INITIAL_TEMPERATURE:", [](size_t index, const std::string& line) { return std::make_shared<InitialTemperatureExtruder>( index, line ); } },
};
// clang-format on

std::shared_ptr<BaseEntry> factory(size_t index, const std::string& line)
{
    for (const auto& pair : creators)
    {
        if (line.starts_with(pair.first))
        {
            return pair.second(index, line);
        }
    }
    return std::make_shared<Unknown>(index, line);
}

} // namespace dulcificum::gcode::ast

#endif // DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_AST_H
