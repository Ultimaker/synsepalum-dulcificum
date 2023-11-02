#include "dulcificum/gcode/ast/acceleration.h"
#include "dulcificum/gcode/ast/bed_temperature.h"
#include "dulcificum/gcode/ast/comment_commands.h"
#include "dulcificum/gcode/ast/delay.h"
#include "dulcificum/gcode/ast/extruder_temperature.h"
#include "dulcificum/gcode/ast/fan.h"
#include "dulcificum/gcode/ast/position.h"
#include "dulcificum/gcode/ast/purge.h"
#include "dulcificum/gcode/ast/toolchange.h"
#include "dulcificum/gcode/ast/translate.h"
#include "dulcificum/utils/string_convert.h"

#include <fmt/format.h>
#include <spdlog/spdlog.h>


namespace dulcificum::gcode::ast
{

BuildVolumeTemperature::BuildVolumeTemperature(size_t line_index, const std::string& raw_line, regex_result_t captured)
    : Entry{ line_index, raw_line }
{
    if (const auto& value = captured.get<"S">())
    {
        S = utils::stringConvert<double>(value.to_view());
    }
    else
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", line_index, raw_line));
    }
}

FeatureType::FeatureType(size_t line_index, const std::string& raw_line, regex_result_t captured)
    : Entry{ line_index, raw_line }
{
    if (const auto& value = captured.get<"T">())
    {
        T = value.to_view();
    }
    else
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", line_index, raw_line));
    }
}

G0_G1::G0_G1(size_t line_index, const std::string& raw_line, regex_result_t captured)
    : Entry{ line_index, raw_line }
{
    // clang-format off
    if (const auto& value = captured.get<"X">()) { X = utils::stringConvert<double>(value.to_view()); }
    if (const auto& value = captured.get<"Y">()) { Y = utils::stringConvert<double>(value.to_view()); }
    if (const auto& value = captured.get<"Z">()) { Z = utils::stringConvert<double>(value.to_view()); }
    if (const auto& value = captured.get<"E">()) { E = utils::stringConvert<double>(value.to_view()); }
    if (const auto& value = captured.get<"F">()) { F = utils::stringConvert<double>(value.to_view()); }
    // clang-format on
    if (X == std::nullopt && Y == std::nullopt && Z == std::nullopt && E == std::nullopt && F == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", line_index, raw_line));
    }
}

G4::G4(size_t line_index, const std::string& raw_line, regex_result_t captured)
    : Entry{ line_index, raw_line }
{
    if (const auto& value = captured.get<"P">())
    {
        P = utils::stringConvert<double>(value.to_view());
    }
    if (const auto& value = captured.get<"S">())
    {
        S = utils::stringConvert<double>(value.to_view());
    }

    if (P == std::nullopt && S == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", line_index, raw_line));
    }
}

G280::G280(size_t line_index, const std::string& raw_line, regex_result_t captured)
    : Entry{ line_index, raw_line }
{
    if (const auto& value = captured.get<"S">())
    {
        S = utils::stringConvert<size_t>(value.to_view());
    }
    else
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", line_index, raw_line));
    }
}

G92::G92(size_t line_index, const std::string& raw_line, regex_result_t captured)
    : Entry{ line_index, raw_line }
{
    // clang-format off
    if (const auto& value = captured.get<"X">()) { X = utils::stringConvert<double>(value.to_view()); }
    if (const auto& value = captured.get<"Y">()) { Y = utils::stringConvert<double>(value.to_view()); }
    if (const auto& value = captured.get<"Z">()) { Z = utils::stringConvert<double>(value.to_view()); }
    if (const auto& value = captured.get<"E">()) { E = utils::stringConvert<double>(value.to_view()); }
    // clang-format on
    if (X == std::nullopt && Y == std::nullopt && Z == std::nullopt && E == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", line_index, raw_line));
    }
}

InitialTemperatureBuildPlate::InitialTemperatureBuildPlate(size_t line_index, const std::string& raw_line, regex_result_t captured)
    : Entry{ line_index, raw_line }
{
    if (const auto& value = captured.get<"S">())
    {
        S = utils::stringConvert<double>(value.to_view());
    }
    else
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", line_index, raw_line));
    }
}

InitialTemperatureExtruder::InitialTemperatureExtruder(size_t line_index, const std::string& raw_line, regex_result_t captured)
    : Entry{ line_index, raw_line }
{
    // clang-format off
    if (const auto& value = captured.get<"T">()) { T = utils::stringConvert<double>(value.to_view()); }
    if (const auto& value = captured.get<"S">()) { S = utils::stringConvert<double>(value.to_view()); }
    // clang-format on
    if (T == std::nullopt && S == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", line_index, raw_line));
    }
}

Layer::Layer(size_t line_index, const std::string& raw_line, regex_result_t captured)
    : Entry{ line_index, raw_line }
{
    if (const auto& value = captured.get<"L">())
    {
        L = utils::stringConvert<int64_t>(value.to_view());
    }
    else
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", line_index, raw_line));
    }
}

M104::M104(size_t line_index, const std::string& raw_line, regex_result_t captured)
    : Entry{ line_index, raw_line }
{
    // clang-format off
    if (const auto& value = captured.get<"S">()) { S = utils::stringConvert<double>(value.to_view()); }
    if (const auto& value = captured.get<"T">()) { T = utils::stringConvert<size_t>(value.to_view()); }
    // clang-format on
    if (S == std::nullopt && T == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", line_index, raw_line));
    }
}

M106::M106(size_t line_index, const std::string& raw_line, regex_result_t captured)
    : Entry{ line_index, raw_line }
{
    // clang-format off
    if (const auto& value = captured.get<"S">()) { S = utils::stringConvert<double>(value.to_view()); }
    if (const auto& value = captured.get<"P">()) { P = utils::stringConvert<size_t>(value.to_view()); }
    // clang-format on
    if (S == std::nullopt && P == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", line_index, raw_line));
    }
}

M107::M107(size_t line_index, const std::string& raw_line, regex_result_t captured)
    : Entry{ line_index, raw_line }
{
    // clang-format off
    if (const auto& value = captured.get<"P">()) { P = utils::stringConvert<size_t>(value.to_view()); }
    // clang-format on
}

M109::M109(size_t line_index, const std::string& raw_line, regex_result_t captured)
    : Entry{ line_index, raw_line }
{
    // clang-format off
    if (const auto& value = captured.get<"S">()) { S = utils::stringConvert<double>(value.to_view()); }
    if (const auto& value = captured.get<"R">()) { R = utils::stringConvert<double>(value.to_view()); }
    if (const auto& value = captured.get<"T">()) { T = utils::stringConvert<size_t>(value.to_view()); }
    // clang-format on
    if (S == std::nullopt && R == std::nullopt && T == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", line_index, raw_line));
    }
}

M140::M140(size_t line_index, const std::string& raw_line, regex_result_t captured)
    : Entry{ line_index, raw_line }
{
    if (const auto& value = captured.get<"S">())
    {
        S = utils::stringConvert<double>(value.to_view());
    }
    else
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", line_index, raw_line));
    }
}

M190::M190(size_t line_index, const std::string& raw_line, regex_result_t captured)
    : Entry{ line_index, raw_line }
{
    // clang-format off
    if (const auto& value = captured.get<"S">()) { S = utils::stringConvert<double>(value.to_view()); }
    if (const auto& value = captured.get<"R">()) { R = utils::stringConvert<double>(value.to_view()); }
    // clang-format on
    if (S == std::nullopt && R == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", line_index, raw_line));
    }
}

M204::M204(size_t line_index, const std::string& raw_line, regex_result_t captured)
    : Entry{ line_index, raw_line }
{
    // clang-format off
    if (const auto& value = captured.get<"P">()) { P = utils::stringConvert<double>(value.to_view()); }
    if (const auto& value = captured.get<"T">()) { T = utils::stringConvert<double>(value.to_view()); }
    if (const auto& value = captured.get<"S">()) { S = utils::stringConvert<double>(value.to_view()); }
    // clang-format on
    if (P == std::nullopt && T == std::nullopt && S == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", line_index, raw_line));
    }
}

M205::M205(size_t line_index, const std::string& raw_line, regex_result_t captured)
    : Entry{ line_index, raw_line }
{
    // clang-format off
    if (const auto& value = captured.get<"X">()) { X = utils::stringConvert<double>(value.to_view()); }
    if (const auto& value = captured.get<"Y">()) { Y = utils::stringConvert<double>(value.to_view()); }
    if (const auto& value = captured.get<"Z">()) { Z = utils::stringConvert<double>(value.to_view()); }
    if (const auto& value = captured.get<"E">()) { E = utils::stringConvert<double>(value.to_view()); }
    // clang-format on
    if (X == std::nullopt && Y == std::nullopt && Z == std::nullopt && E == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", line_index, raw_line));
    }
}

Mesh::Mesh(size_t line_index, const std::string& raw_line, regex_result_t captured)
    : Entry{ line_index, raw_line }
{
    if (const auto& value = captured.get<"M">())
    {
        M = value.to_view();
    }
    else
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", line_index, raw_line));
    }
}

Comment::Comment(size_t line_index, const std::string& raw_line, regex_result_t captured)
    : Entry{ line_index, raw_line }
{
    if (const auto& value = captured.get<"C">())
    {
        C = value.to_view();
    }
    else
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", line_index, raw_line));
    }
}

T::T(size_t line_index, const std::string& raw_line, regex_result_t captured)
    : Entry{ line_index, raw_line }
{
    if (const auto& value = captured.get<"S">())
    {
        S = utils::stringConvert<size_t>(value.to_view());
    }
    else
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", line_index, raw_line));
    }
}

} // namespace dulcificum::gcode::ast
