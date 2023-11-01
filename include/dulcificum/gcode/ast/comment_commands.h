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
class Layer : public Entry<R"(;LAYER:(?<L>-?\d+))", ctre::captured_content<1, ctre::id<'L'>>>
{
public:
    Layer() = delete;
    Layer(size_t idx, std::string line, regex_result_t captured);
    std::optional<int64_t> L;
};

/*!
 * /brief The mesh name
 * M = mesh name
 */
class Mesh : public Entry<R"(;MESH:(?<M>.*))", ctre::captured_content<1, ctre::id<'M'>>>
{
public:
    Mesh() = delete;
    Mesh(size_t idx, std::string raw_line, regex_result_t captured);
    std::optional<std::string> M;
};

/*!
 * /brief Feature type
 * T = feature type (WALL-OUTER, WALL_INNER, SKIN, SUPPORT, SKIRT, FILL, SUPPORT-INTERFACE, PRIME_TOWER)
 */
class FeatureType : public Entry<R"(;TYPE:(?<T>.*))", ctre::captured_content<1, ctre::id<'T'>>>
{
public:
    FeatureType() = delete;
    FeatureType(size_t idx, std::string line, regex_result_t captured);
    std::optional<std::string> T;
};

/*!
 * /brief Initial extruder temperature
 * T = tool index
 * S = Temperature
 */
class InitialTemperatureExtruder
    : public Entry<R"(;EXTRUDER_TRAIN\.(?<T>\d)\.INITIAL_TEMPERATURE:(?<S>(?:[\d|\.]*)))", ctre::captured_content<1, ctre::id<'T'>>, ctre::captured_content<2, ctre::id<'S'>>>
{
public:
    InitialTemperatureExtruder() = delete;
    InitialTemperatureExtruder(size_t idx, std::string line, regex_result_t captured);
    std::optional<size_t> T;
    std::optional<double> S;
};

/*!
 * /brief Initial build plate temperature
 * S = Temperature
 */
class InitialTemperatureBuildPlate : public Entry<R"(;BUILD_PLATE.INITIAL_TEMPERATURE:(?<S>(?:[\d|\.]*)))", ctre::captured_content<1, ctre::id<'S'>>>
{
public:
    InitialTemperatureBuildPlate() = delete;
    InitialTemperatureBuildPlate(size_t idx, std::string raw_line, regex_result_t captured);
    std::optional<double> S;
};

/*!
 * /brief Initial build volume temperature
 * S = Temperature
 */
class BuildVolumeTemperature : public Entry<R"(;BUILD_VOLUME.TEMPERATURE:(?<S>(?:[\d|\.]*)))", ctre::captured_content<1, ctre::id<'S'>>>
{
public:
    BuildVolumeTemperature() = delete;
    BuildVolumeTemperature(size_t idx, std::string line, regex_result_t captured);
    std::optional<double> S;
};

/*!
 * /brief A generic comment
 * comment = The text of the comment
 */
class Comment : public Entry<R"(;(?<C>\.*))", ctre::captured_content<1, ctre::id<'C'>>>
{
public:
    Comment() = delete;
    Comment(size_t idx, std::string raw_line, regex_result_t captured);
    std::string C;
};

} // namespace dulcificum::gcode::ast

#endif // INCLUDE_DULCIFICUM_GCODE_AST_COMMENT_COMMANDS_H
