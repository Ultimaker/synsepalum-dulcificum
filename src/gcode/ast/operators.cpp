#include "dulcificum/gcode/ast/acceleration.h"
#include "dulcificum/gcode/ast/bed_temperature.h"
#include "dulcificum/gcode/ast/comment_commands.h"
#include "dulcificum/gcode/ast/extruder_temperature.h"
#include "dulcificum/gcode/ast/fan.h"
#include "dulcificum/gcode/ast/position.h"
#include "dulcificum/gcode/ast/purge.h"
#include "dulcificum/gcode/ast/toolchange.h"
#include "dulcificum/gcode/ast/translate.h"
#include "dulcificum/utils/svtod.h"

#include <fmt/format.h>

namespace dulcificum::gcode::ast
{
BuildVolumeTemperature::BuildVolumeTemperature(size_t index, std::string line)
    : Entry{ index, std::move(line) }
{
    const auto& matches = get();
    if (const auto& value = matches.get<"S">())
    {
        S = utils::StringViewToDouble(value.to_view());
    }
    else
    {
        throw std::runtime_error(fmt::format("Unable to parse lino: {}", index));
    }
}

FeatureType::FeatureType(size_t index, std::string line)
    : Entry{ index, std::move(line) }
{
    const auto& matches = get();
    if (const auto& value = matches.get<"T">())
    {
        T = value.to_view();
    }
    else
    {
        throw std::runtime_error(fmt::format("Unable to parse lino: {}", index));
    }
}

G0::G0(size_t index, std::string line)
    : Entry{ index, std::move(line) }
{
    const auto& matches = get();
    // clang-format off
    if (const auto& value = matches.get<"X">()) { X = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = matches.get<"Y">()) { Y = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = matches.get<"Z">()) { Z = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = matches.get<"E">()) { E = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = matches.get<"F">()) { F = utils::StringViewToDouble(value.to_view()); }
    // clang-format on
    if (X == std::nullopt && Y == std::nullopt && Z == std::nullopt && E == std::nullopt && F == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse lino: {}", index));
    }
}

G1::G1(size_t index, std::string line)
    : Entry{ index, std::move(line) }
{
    const auto& matches = get();
    // clang-format off
    if (const auto& value = matches.get<"X">()) { X = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = matches.get<"Y">()) { Y = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = matches.get<"Z">()) { Z = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = matches.get<"E">()) { E = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = matches.get<"F">()) { F = utils::StringViewToDouble(value.to_view()); }
    // clang-format on
    if (X == std::nullopt && Y == std::nullopt && Z == std::nullopt && E == std::nullopt && F == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse lino: {}", index));
    }
}

G280::G280(size_t index, std::string line)
    : Entry{ index, std::move(line) }
{
    const auto& matches = get();
    if (const auto& value = matches.get<"S">())
    {
        S = utils::StringViewToDouble(value.to_view());
    }
    else
    {
        throw std::runtime_error(fmt::format("Unable to parse lino: {}", index));
    }
}

G92::G92(size_t index, std::string line)
    : Entry{ index, std::move(line) }
{
    const auto& matches = get();
    // clang-format off
    if (const auto& value = matches.get<"X">()) { X = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = matches.get<"Y">()) { Y = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = matches.get<"Z">()) { Z = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = matches.get<"E">()) { E = utils::StringViewToDouble(value.to_view()); }
    // clang-format on
    if (X == std::nullopt && Y == std::nullopt && Z == std::nullopt && E == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse lino: {}", index));
    }
}

InitialTemperatureBuildPlate::InitialTemperatureBuildPlate(size_t index, std::string line)
    : Entry{ index, std::move(line) }
{
    const auto& matches = get();
    if (const auto& value = matches.get<"S">())
    {
        S = utils::StringViewToDouble(value.to_view());
    }
    else
    {
        throw std::runtime_error(fmt::format("Unable to parse lino: {}", index));
    }
}

InitialTemperatureExtruder::InitialTemperatureExtruder(size_t index, std::string line)
    : Entry{ index, std::move(line) }
{
    const auto& matches = get();
    // clang-format off
    if (const auto& value = matches.get<"T">()) { T = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = matches.get<"S">()) { S = utils::StringViewToDouble(value.to_view()); }
    // clang-format on
    if (T == std::nullopt && S == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse lino: {}", index));
    }
}

Layer::Layer(size_t index, std::string line)
    : Entry{ index, std::move(line) }
{
    const auto& matches = get();
    if (const auto& value = matches.get<"L">())
    {
        L = utils::StringViewToDouble(value.to_view());
    }
    else
    {
        throw std::runtime_error(fmt::format("Unable to parse lino: {}", index));
    }
}

M104::M104(size_t index, std::string line)
    : Entry{ index, std::move(line) }
{
    const auto& matches = get();
    // clang-format off
    if (const auto& value = matches.get<"S">()) { S = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = matches.get<"T">()) { T = utils::StringViewToDouble(value.to_view()); }
    // clang-format on
    if (S == std::nullopt && T == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse lino: {}", index));
    }
}

M106::M106(size_t index, std::string line)
    : Entry{ index, std::move(line) }
{
    const auto& matches = get();
    if (const auto& value = matches.get<"S">())
    {
        S = utils::StringViewToDouble(value.to_view());
    }
    else
    {
        throw std::runtime_error(fmt::format("Unable to parse lino: {}", index));
    }
}

M109::M109(size_t index, std::string line)
    : Entry{ index, std::move(line) }
{
    const auto& matches = get();
    // clang-format off
    if (const auto& value = matches.get<"S">()) { S = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = matches.get<"R">()) { R = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = matches.get<"T">()) { T = utils::StringViewToDouble(value.to_view()); }
    // clang-format on
    if (S == std::nullopt && R == std::nullopt && T == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse lino: {}", index));
    }
}

M140::M140(size_t index, std::string line)
    : Entry{ index, std::move(line) }
{
    const auto& matches = get();
    if (const auto& value = matches.get<"S">())
    {
        S = utils::StringViewToDouble(value.to_view());
    }
    else
    {
        throw std::runtime_error(fmt::format("Unable to parse lino: {}", index));
    }
}

M190::M190(size_t index, std::string line)
    : Entry{ index, std::move(line) }
{
    const auto& matches = get();
    // clang-format off
    if (const auto& value = matches.get<"S">()) { S = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = matches.get<"R">()) { R = utils::StringViewToDouble(value.to_view()); }
    // clang-format on
    if (S == std::nullopt && R == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse lino: {}", index));
    }
}

M204::M204(size_t index, std::string line)
    : Entry{ index, std::move(line) }
{
    const auto& matches = get();
    // clang-format off
    if (const auto& value = matches.get<"P">()) { P = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = matches.get<"T">()) { T = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = matches.get<"S">()) { S = utils::StringViewToDouble(value.to_view()); }
    // clang-format on
    if (P == std::nullopt && T == std::nullopt && S == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse lino: {}", index));
    }
}

M205::M205(size_t index, std::string line)
    : Entry{ index, std::move(line) }
{
    const auto& matches = get();
    // clang-format off
    if (const auto& value = matches.get<"X">()) { X = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = matches.get<"Y">()) { Y = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = matches.get<"Z">()) { Z = utils::StringViewToDouble(value.to_view()); }
    if (const auto& value = matches.get<"E">()) { E = utils::StringViewToDouble(value.to_view()); }
    // clang-format on
    if (X == std::nullopt && Y == std::nullopt && Z == std::nullopt && E == std::nullopt)
    {
        throw std::runtime_error(fmt::format("Unable to parse lino: {}", index));
    }
}

Mesh::Mesh(size_t index, std::string line)
    : Entry{ index, std::move(line) }
{
    const auto& matches = get();
    if (const auto& value = matches.get<"M">())
    {
        M = value.to_view();
    }
    else
    {
        throw std::runtime_error(fmt::format("Unable to parse lino: {}", index));
    }
}

T::T(size_t index, std::string line)
    : Entry{ index, std::move(line) }
{
    const auto& matches = get();
    if (const auto& value = matches.get<"S">())
    {
        S = utils::StringViewToDouble(value.to_view());
    }
    else
    {
        throw std::runtime_error(fmt::format("Unable to parse lino: {}", index));
    }
}

} // namespace dulcificum::gcode::ast