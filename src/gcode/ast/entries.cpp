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
#include "dulcificum/utils/svtod.h"

#include <fmt/format.h>
#include <spdlog/spdlog.h>


namespace dulcificum::gcode::ast
{

BuildVolumeTemperature::BuildVolumeTemperature(size_t index, std::string line, regex_result_t captured)
    : Entry{ index, line }
{
    if (const auto& value = captured.get<"S">())
    {
        S = utils::StringViewToDouble(value.to_view());
    }
    else
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", index, line));
    }
}

FeatureType::FeatureType(size_t index, std::string line, regex_result_t captured)
    : Entry{ index, line }
{
    if (const auto& value = captured.get<"T">())
    {
        T = value.to_view();
    }
    else
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", index, line));
    }
}

G0_G1::G0_G1(size_t index, std::string line, regex_result_t captured)
    : Entry{ index, line }
{
    // clang-format off
    if (const auto& value = captured.get<"X">()) { X = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = captured.get<"Y">()) { Y = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = captured.get<"Z">()) { Z = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = captured.get<"E">()) { E = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = captured.get<"F">()) { F = utils::StringViewToDouble(value.to_view()); }
    // clang-format on
    if (X == std::nullopt && Y == std::nullopt && Z == std::nullopt && E == std::nullopt && F == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", index, line));
    }
}

G4::G4(size_t index, std::string line, regex_result_t captured)
    : Entry{ index, line }
{
    if (const auto& value = captured.get<"P">())
        P = utils::StringViewToDouble(value.to_view());
    if (const auto& value = captured.get<"S">())
        S = utils::StringViewToDouble(value.to_view());

    if (P == std::nullopt && S == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", index, line));
    }
}

G280::G280(size_t index, std::string line, regex_result_t captured)
    : Entry{ index, line }
{
    if (const auto& value = captured.get<"S">())
    {
        S = utils::StringViewToDouble(value.to_view());
    }
    else
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", index, line));
    }
}

G92::G92(size_t index, std::string line, regex_result_t captured)
    : Entry{ index, line }
{
    // clang-format off
    if (const auto& value = captured.get<"X">()) { X = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = captured.get<"Y">()) { Y = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = captured.get<"Z">()) { Z = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = captured.get<"E">()) { E = utils::StringViewToDouble(value.to_view()); }
    // clang-format on
    if (X == std::nullopt && Y == std::nullopt && Z == std::nullopt && E == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", index, line));
    }
}

InitialTemperatureBuildPlate::InitialTemperatureBuildPlate(size_t index, std::string line, regex_result_t captured)
    : Entry{ index, line }
{
    if (const auto& value = captured.get<"S">())
    {
        S = utils::StringViewToDouble(value.to_view());
    }
    else
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", index, line));
    }
}

InitialTemperatureExtruder::InitialTemperatureExtruder(size_t index, std::string line, regex_result_t captured)
    : Entry{ index, line }
{
    // clang-format off
    if (const auto& value = captured.get<"T">()) { T = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = captured.get<"S">()) { S = utils::StringViewToDouble(value.to_view()); }
    // clang-format on
    if (T == std::nullopt && S == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", index, line));
    }
}

Layer::Layer(size_t index, std::string line, regex_result_t captured)
    : Entry{ index, line }
{
    if (const auto& value = captured.get<"L">())
    {
        L = static_cast<int64_t>(utils::StringViewToDouble(value.to_view()));
    }
    else
    {
        // throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", index, line));
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", index, ""));
    }
}

M104::M104(size_t index, std::string line, regex_result_t captured)
    : Entry{ index, line }
{
    // clang-format off
    if (const auto& value = captured.get<"S">()) { S = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = captured.get<"T">()) { T = utils::StringViewToDouble(value.to_view()); }
    // clang-format on
    if (S == std::nullopt && T == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", index, line));
    }
}

M106::M106(size_t index, std::string line, regex_result_t captured)
    : Entry{ index, line }
{
    // clang-format off
    if (const auto& value = captured.get<"S">()) { S = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = captured.get<"P">()) { P = static_cast<size_t>(utils::StringViewToDouble(value.to_view())); }
    // clang-format on
    if (S == std::nullopt && P == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", index, line));
    }
}

M107::M107(size_t index, std::string line, regex_result_t captured)
    : Entry{ index, line }
{
    // clang-format off
    if (const auto& value = captured.get<"P">()) { P = static_cast<size_t>(utils::StringViewToDouble(value.to_view())); }
    // clang-format on
}

M109::M109(size_t index, std::string line, regex_result_t captured)
    : Entry{ index, line }
{
    // clang-format off
    if (const auto& value = captured.get<"S">()) { S = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = captured.get<"R">()) { R = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = captured.get<"T">()) { T = utils::StringViewToDouble(value.to_view()); }
    // clang-format on
    if (S == std::nullopt && R == std::nullopt && T == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", index, line));
    }
}

M140::M140(size_t index, std::string line, regex_result_t captured)
    : Entry{ index, line }
{
    if (const auto& value = captured.get<"S">())
    {
        S = utils::StringViewToDouble(value.to_view());
    }
    else
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", index, line));
    }
}

M190::M190(size_t index, std::string line, regex_result_t captured)
    : Entry{ index, line }
{
    // clang-format off
    if (const auto& value = captured.get<"S">()) { S = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = captured.get<"R">()) { R = utils::StringViewToDouble(value.to_view()); }
    // clang-format on
    if (S == std::nullopt && R == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", index, line));
    }
}

M204::M204(size_t index, std::string line, regex_result_t captured)
    : Entry{ index, line }
{
    // clang-format off
    if (const auto& value = captured.get<"P">()) { P = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = captured.get<"T">()) { T = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = captured.get<"S">()) { S = utils::StringViewToDouble(value.to_view()); }
    // clang-format on
    if (P == std::nullopt && T == std::nullopt && S == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", index, line));
    }
}

M205::M205(size_t index, std::string line, regex_result_t captured)
    : Entry{ index, line }
{
    // clang-format off
    if (const auto& value = captured.get<"X">()) { X = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = captured.get<"Y">()) { Y = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = captured.get<"Z">()) { Z = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = captured.get<"E">()) { E = utils::StringViewToDouble(value.to_view()); }
    // clang-format on
    if (X == std::nullopt && Y == std::nullopt && Z == std::nullopt && E == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", index, line));
    }
}

Mesh::Mesh(size_t index, std::string line, regex_result_t captured)
    : Entry{ index, line }
{
    if (const auto& value = captured.get<"M">())
    {
        M = value.to_view();
    }
    else
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", index, line));
    }
}

Comment::Comment(size_t index, std::string line, regex_result_t captured)
    : Entry{ index, line }
{
    if (const auto& value = captured.get<"C">())
    {
        C = value.to_view();
    }
    else
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", index, line));
    }
}

T::T(size_t index, std::string line, regex_result_t captured)
    : Entry{ index, line }
{
    if (const auto& value = captured.get<"S">())
    {
        S = utils::StringViewToDouble(value.to_view());
    }
    else
    {
        throw std::runtime_error(fmt::format("Unable to parse: [{}] {}", index, line));
    }
}

} // namespace dulcificum::gcode::ast
