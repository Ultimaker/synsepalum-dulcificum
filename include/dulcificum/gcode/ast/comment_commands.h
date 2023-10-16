#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_COMMENT_COMMANDS_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_COMMENT_COMMANDS_H

#include "dulcificum/gcode/ast/entry.h"

namespace dulcificum::gcode::ast
{
/*!
 * /brief The layer index
 * L = index
 */
class Layer : public Entry<R"(;LAYER:(?<L>\d*))">
{
public:
    Layer() = delete;
    Layer(size_t index, std::string line)
        : Entry{ index, std::move(line) } {};
};

/*!
 * /brief The mesh name
 * M = mesh name
 */
class Mesh : public Entry<R"(;MESH:(?<M>.*))">
{
public:
    Mesh() = delete;
    Mesh(size_t index, std::string line)
        : Entry{ index, std::move(line) } {};
};

/*!
 * /brief Feature type
 * T = feature type (WALL-OUTER, WALL_INNER, SKIN, SUPPORT, SKIRT, FILL, SUPPORT-INTERFACE, PRIME_TOWER)
 */
class FeatureType : public Entry<R"(;TYPE:(?<T>.*))">
{
public:
    FeatureType() = delete;
    FeatureType(size_t index, std::string line)
        : Entry{ index, std::move(line) } {};
};

/*!
 * /brief Initial extruder temperature
 * T = tool index
 * S = Temperature
 */
class InitialTemperatureExtruder : public Entry<R"(;EXTRUDER_TRAIN\.(?<T>\d)\.INITIAL_TEMPERATURE:(?<S>([\d|\.]*)))">
{
public:
    InitialTemperatureExtruder() = delete;
    InitialTemperatureExtruder(size_t index, std::string line)
        : Entry{ index, std::move(line) } {};
};

/*!
 * /brief Initial build plate temperature
 * S = Temperature
 */
class InitialTemperatureBuildPlate : public Entry<R"(;BUILD_PLATE.INITIAL_TEMPERATURE:(?<S>([\d|\.]*)))">
{
public:
    InitialTemperatureBuildPlate() = delete;
    InitialTemperatureBuildPlate(size_t index, std::string line)
        : Entry{ index, std::move(line) } {};
};

/*!
 * /brief Initial build volume temperature
 * S = Temperature
 */
class BuildVolumeTemperature : public Entry<R"(;BUILD_VOLUME.TEMPERATURE:(?<S>([\d|\.]*)))">
{
public:
    BuildVolumeTemperature() = delete;
    BuildVolumeTemperature(size_t index, std::string line)
        : Entry{ index, std::move(line) } {};
};

} // namespace dulcificum::gcode::ast

#endif // DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_COMMENT_COMMANDS_H
