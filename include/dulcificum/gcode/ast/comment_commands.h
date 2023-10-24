#ifndef INCLUDE_DULCIFICUM_GCODE_AST_COMMENT_COMMANDS_H
#define INCLUDE_DULCIFICUM_GCODE_AST_COMMENT_COMMANDS_H

#include "dulcificum/gcode/ast/entry.h"

#include <optional>

namespace dulcificum::gcode::ast
{
/*!
 * /brief The layer index
 * L = index
 */
class Layer : public Entry<R"(;LAYER:(?<L>-?\d+))">
{
public:
    Layer() = delete;
    Layer(size_t index, std::string line);
    std::optional<size_t> L;
};

/*!
 * /brief The mesh name
 * M = mesh name
 */
class Mesh : public Entry<R"(;MESH:(?<M>.*))">
{
public:
    Mesh() = delete;
    Mesh(size_t index, std::string line);
    std::optional<std::string_view> M;
};

/*!
 * /brief Feature type
 * T = feature type (WALL-OUTER, WALL_INNER, SKIN, SUPPORT, SKIRT, FILL, SUPPORT-INTERFACE, PRIME_TOWER)
 */
class FeatureType : public Entry<R"(;TYPE:(?<T>.*))">
{
public:
    FeatureType() = delete;
    FeatureType(size_t index, std::string line);
    std::optional<std::string_view> T;
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
    InitialTemperatureExtruder(size_t index, std::string line);
    std::optional<size_t> T;
    std::optional<double> S;
};

/*!
 * /brief Initial build plate temperature
 * S = Temperature
 */
class InitialTemperatureBuildPlate : public Entry<R"(;BUILD_PLATE.INITIAL_TEMPERATURE:(?<S>([\d|\.]*)))">
{
public:
    InitialTemperatureBuildPlate() = delete;
    InitialTemperatureBuildPlate(size_t index, std::string line);
    std::optional<double> S;
};

/*!
 * /brief Initial build volume temperature
 * S = Temperature
 */
class BuildVolumeTemperature : public Entry<R"(;BUILD_VOLUME.TEMPERATURE:(?<S>([\d|\.]*)))">
{
public:
    BuildVolumeTemperature() = delete;
    BuildVolumeTemperature(size_t index, std::string line);
    std::optional<double> S;
};

/*!
 * /brief A generic comment
 * comment = The text of the comment
 */
class Comment : public Entry<R"(;(?<C>\.*))">
{
public:
    Comment() = delete;
    Comment(size_t index, std::string line);
    std::string C;
};


} // namespace dulcificum::gcode::ast

#endif // INCLUDE_DULCIFICUM_GCODE_AST_COMMENT_COMMANDS_H
