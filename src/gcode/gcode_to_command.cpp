#include "dulcificum/gcode/gcode_to_command.h"

#include "dulcificum/command_types.h"
#include "dulcificum/gcode/ast/ast.h"
#include "dulcificum/gcode/ast/bed_temperature.h"
#include "dulcificum/gcode/ast/comment_commands.h"
#include "dulcificum/gcode/ast/delay.h"
#include "dulcificum/gcode/ast/extruder_temperature.h"
#include "dulcificum/gcode/ast/fan.h"
#include "dulcificum/gcode/ast/position.h"
#include "dulcificum/gcode/ast/positioning_mode.h"
#include "dulcificum/gcode/ast/toolchange.h"
#include "dulcificum/gcode/ast/translate.h"
#include "dulcificum/state.h"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <array>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace dulcificum::gcode
{

void VisitCommand::update_state(const gcode::ast::G0_G1& command)
{
    if (command.X.has_value())
    {
        state.X = state.X_positioning == Positioning::Absolute ? command.X.value() : command.X.value() + state.X;
    }
    if (command.Y.has_value())
    {
        state.Y = state.Y_positioning == Positioning::Absolute ? command.Y.value() : command.Y.value() + state.Y;
    }
    if (command.Z.has_value())
    {
        state.Z = state.Z_positioning == Positioning::Absolute ? command.Z.value() : command.Z.value() + state.Z;
    }
    if (command.E.has_value())
    {
        state.E[state.active_tool] = state.E_positioning == Positioning::Absolute ? command.E.value() : command.E.value() + state.E[state.active_tool];
    }

    if (command.F.has_value())
    {
        state.F[state.active_tool] = command.F.value();
    }
}

void VisitCommand::update_state([[maybe_unused]] const gcode::ast::G90& command)
{
    state.X_positioning = Positioning::Absolute;
    state.Y_positioning = Positioning::Absolute;
    state.Z_positioning = Positioning::Absolute;
    state.E_positioning = Positioning::Absolute;
}

void VisitCommand::update_state([[maybe_unused]] const gcode::ast::G91& command)
{
    state.X_positioning = Positioning::Relative;
    state.Y_positioning = Positioning::Relative;
    state.Z_positioning = Positioning::Relative;
    state.E_positioning = Positioning::Relative;
}

void VisitCommand::update_state(const gcode::ast::G92& command)
{
    if (command.X.has_value())
    {
        state.origin_x = state.X - command.X.value();
        state.X = command.X.value();
    }

    if (command.Y.has_value())
    {
        state.origin_y = state.Y - command.Y.value();
        state.Y = command.Y.value();
    }

    if (command.Z.has_value())
    {
        state.origin_z = state.Z - command.Z.value();
        state.Z = command.Z.value();
    }

    if (command.E.has_value())
    {
        for (size_t idx = 0; idx < state.num_extruders; idx++)
        {
            state.origin_e[idx] = state.E[idx] - command.E.value();
            state.E[idx] = command.E.value();
        }
    }
}

void VisitCommand::update_state([[maybe_unused]] const gcode::ast::M82& command)
{
    state.E_positioning = Positioning::Absolute;
}

void VisitCommand::update_state([[maybe_unused]] const gcode::ast::M83& command)
{
    state.E_positioning = Positioning::Relative;
}

void VisitCommand::update_state(const gcode::ast::M104& command)
{
    if (command.S.has_value())
    {
        state.extruder_temperatures[command.T.has_value() ? command.T.value() : state.active_tool] = command.S.value();
    }
}

void VisitCommand::update_state(const gcode::ast::M109& command)
{
    const auto temp = command.S.has_value() ? command.S : command.R;
    if (temp.has_value())
    {
        state.extruder_temperatures[command.T.has_value() ? command.T.value() : state.active_tool] = temp.value();
    }
}

void VisitCommand::update_state(const gcode::ast::M140& command)
{
    if (command.S.has_value())
    {
        state.build_plate_temperature = command.S.value();
    }
}

void VisitCommand::update_state(const gcode::ast::M190& command)
{
    const auto temp = command.S.has_value() ? command.S : command.R;
    if (temp.has_value())
    {
        state.build_plate_temperature = temp.value();
    }
}

void VisitCommand::update_state(const gcode::ast::Layer& command)
{
    if (command.L.has_value()) [[likely]]
    {
        if (! state.layer_index_offset.has_value()) [[unlikely]]
        {
            state.layer_index_offset = -1 * command.L.value();
        }
        state.layer = command.L.value();
    }
    else
    {
        spdlog::warn("Layer command without layer index");
    }
}

void VisitCommand::update_state(const gcode::ast::Mesh& command)
{
    if (command.M.has_value())
    {
        state.mesh = command.M.value();
    }
    else
    {
        spdlog::warn("Mesh command without mesh index");
    }
}

void VisitCommand::update_state(const gcode::ast::FeatureType& command)
{
    if (command.T.has_value())
    {
        state.feature_type = command.T.value();
    }
    else
    {
        spdlog::warn("FeatureType command without feature type index");
    }
}

void VisitCommand::update_state(const gcode::ast::InitialTemperatureExtruder& command)
{
    if (command.S.has_value())
    {
        state.extruder_temperatures[command.T.has_value() ? command.T.value() : state.active_tool] = command.S.value();
    }
    else
    {
        spdlog::warn("InitialTemperatureExtruder command without temperature");
    }
}

void VisitCommand::update_state(const gcode::ast::InitialTemperatureBuildPlate& command)
{
    if (command.S.has_value())
    {
        state.build_plate_temperature = command.S.value();
    }
    else
    {
        spdlog::warn("InitialTemperatureBuildPlate command without temperature");
    }
}

void VisitCommand::update_state(const gcode::ast::BuildVolumeTemperature& command)
{
    if (command.S.has_value())
    {
        state.chamber_temperature = command.S.value();
    }
    else
    {
        spdlog::warn("BuildVolumeTemperature command without temperature");
    }
}

void VisitCommand::update_state(const gcode::ast::T& command)
{
    if (command.S.has_value())
    {
        state.active_tool = command.S.value();
    }
    else
    {
        spdlog::warn("Tool change command without tool index");
    }
}

namespace
{
constexpr double k_ms_per_second = 1000.0;
constexpr double k_max_pwm_duty = 255.0;

struct TagMapping
{
    std::string_view feature_type;
    botcmd::Tag tool0_tag;
    botcmd::Tag tool1_tag;
};

constexpr std::array<TagMapping, 10> k_tag_mappings = { {
    { "WALL-OUTER", botcmd::Tag::WallOuter_0, botcmd::Tag::WallOuter_1 },
    { "WALL-INNER", botcmd::Tag::WallInner_0, botcmd::Tag::WallInner_1 },
    { "SKIN", botcmd::Tag::TopSurface_0, botcmd::Tag::TopSurface_1 },
    { "TOP-SURFACE", botcmd::Tag::TopSurface_0, botcmd::Tag::TopSurface_1 },
    { "PRIME-TOWER", botcmd::Tag::PrimeTower_0, botcmd::Tag::PrimeTower_1 },
    { "FILL", botcmd::Tag::Fill_0, botcmd::Tag::Fill_1 },
    { "Purge", botcmd::Tag::Raft, botcmd::Tag::Raft },
    { "SUPPORT", botcmd::Tag::Support_0, botcmd::Tag::Support_1 },
    { "SUPPORT-INTERFACE", botcmd::Tag::SupportInterface_0, botcmd::Tag::SupportInterface_1 },
    { "SKIRT", botcmd::Tag::Skirt_0, botcmd::Tag::Skirt_1 },
} };

std::optional<botcmd::Tag> getFeatureTag(std::string_view feature_type, size_t active_tool)
{
    for (const auto& mapping : k_tag_mappings)
    {
        if (mapping.feature_type == feature_type)
        {
            return active_tool == 0 ? mapping.tool0_tag : mapping.tool1_tag;
        }
    }
    return std::nullopt;
}

std::optional<botcmd::Tag> determineMoveTag(double delta_e, bool& is_retracted, std::string_view feature_type, size_t active_tool)
{
    if (is_retracted)
    {
        if (delta_e > 0.0)
        {
            is_retracted = false;
            return botcmd::Tag::Restart;
        }
        return botcmd::Tag::TravelMove;
    }
    if (delta_e < 0.0)
    {
        is_retracted = true;
        return botcmd::Tag::Retract;
    }
    if (delta_e == 0.0)
    {
        return botcmd::Tag::TravelMove;
    }
    return getFeatureTag(feature_type, active_tool);
}
} // namespace

void VisitCommand::to_proto_path(const gcode::ast::G0_G1& command)
{
    const auto move = std::make_shared<botcmd::Move>();

    double delta_e{};
    if (! command.E.has_value())
    {
        delta_e = 0.0;
    }
    else if (state.E_positioning == Positioning::Relative)
    {
        delta_e = command.E.value();
    }
    else
    {
        const auto e_val = command.E.value();
        delta_e = e_val - state.E[state.active_tool];
        state.E[state.active_tool] = e_val;
    }

    const auto feedrate = command.F.has_value() ? command.F : state.F[state.active_tool];
    if (feedrate.has_value())
    {
        state.F[state.active_tool] = feedrate.value();
    }
    move->feedrate = state.F[state.active_tool] / 60.0;

    // if position is not specified for an axis, move with a relative delta 0 move
    move->point = {
        command.X.has_value() ? (command.X.value() + (state.X_positioning == Positioning::Absolute ? state.origin_x : 0.0)) : 0.0,
        command.Y.has_value() ? (command.Y.value() + (state.Y_positioning == Positioning::Absolute ? state.origin_y : 0.0)) : 0.0,
        state.Z + state.origin_z,
        state.active_tool == 0 && command.E.has_value() ? delta_e : 0.0,
        state.active_tool == 1 && command.E.has_value() ? delta_e : 0.0,
    };
    // gcode is in mm / min, bot cmd uses mm / sec
    move->feedrate = state.F[state.active_tool] / 60.0;

    // Since MakerBot doesn't support "reset extruder" commands commutative errors
    // occur when setting extruders to absolute. As the printer can't keep up with
    // the increasing extruder position it starts over extruding. As a solution
    // every absolute extruder position to a relative move.
    move->is_point_relative = {
        command.X.has_value() ? state.X_positioning == Positioning::Relative : true, command.Y.has_value() ? state.Y_positioning == Positioning::Relative : true, false, true, true,
    };

    if (const auto tag = determineMoveTag(delta_e, state.is_retracted, state.feature_type, state.active_tool); tag.has_value())
    {
        move->tags.emplace_back(*tag);
    }
    proto_path.emplace_back(move);
}

void VisitCommand::to_proto_path(const gcode::ast::G4& command)
{
    const auto delay = std::make_shared<botcmd::Delay>();
    if (command.S.has_value())
    {
        delay->seconds = command.S.value();
    }
    else if (command.P.has_value())
    {
        delay->seconds = command.P.value() / k_ms_per_second;
    }
    else
    {
        spdlog::warn("Delay command without duration");
    }
    proto_path.emplace_back(delay);
}

void VisitCommand::to_proto_path([[maybe_unused]] const gcode::ast::M104& command)
{
    const auto set_temperature = std::make_shared<botcmd::SetTemperature>();
    set_temperature->temperature = command.S.has_value() ? command.S.value() : 0.0;
    set_temperature->index = command.T.has_value() ? command.T.value() : state.active_tool;
    proto_path.emplace_back(set_temperature);
}

void VisitCommand::to_proto_path([[maybe_unused]] const gcode::ast::M106& command)
{
    // turn fan on
    const auto toggle_fan = std::make_shared<botcmd::FanToggle>();
    toggle_fan->is_on = true;
    toggle_fan->index = command.P.has_value() ? command.P.value() : 0;
    proto_path.emplace_back(toggle_fan);

    // set fan speed
    const auto set_fan_speed = std::make_shared<botcmd::FanDuty>();
    double duty = 1.0;
    if (command.S.has_value())
    {
        duty = command.S.value() <= 1.0 ? command.S.value() : (command.S.value() / k_max_pwm_duty);
        duty = std::clamp(duty, 0.0, 1.0);
    }
    set_fan_speed->duty = duty;
    set_fan_speed->index = toggle_fan->index;
    proto_path.emplace_back(set_fan_speed);
}

void VisitCommand::to_proto_path([[maybe_unused]] const gcode::ast::M107& command)
{
    const auto toggle_fan = std::make_shared<botcmd::FanToggle>();
    toggle_fan->is_on = false;
    toggle_fan->index = command.P.has_value() ? command.P.value() : 0;
    proto_path.emplace_back(toggle_fan);
}

void VisitCommand::to_proto_path([[maybe_unused]] const gcode::ast::M109& command)
{
    const auto set_temperature = std::make_shared<botcmd::SetTemperature>();
    const auto temp = command.S.has_value() ? command.S : command.R;
    set_temperature->temperature = temp.has_value() ? temp.value() : 0.0;
    set_temperature->index = command.T.has_value() ? command.T.value() : state.active_tool;
    proto_path.emplace_back(set_temperature);

    const auto wait_temperature = std::make_shared<botcmd::WaitForTemperature>();
    wait_temperature->index = command.T.has_value() ? command.T.value() : state.active_tool;
    proto_path.emplace_back(wait_temperature);
}

void VisitCommand::to_proto_path([[maybe_unused]] const gcode::ast::Layer& command)
{
    const auto layer = std::make_shared<botcmd::LayerChange>();
    if (command.L.has_value())
    {
        layer->layer = command.L.value() + state.layer_index_offset.value();
    }
    else
    {
        spdlog::warn("Layer command without layer number");
    }
    proto_path.emplace_back(layer);
}

void VisitCommand::to_proto_path([[maybe_unused]] const gcode::ast::InitialTemperatureExtruder& command)
{
    const auto set_temperature = std::make_shared<botcmd::SetTemperature>();
    set_temperature->temperature = command.S.has_value() ? command.S.value() : 0.0;
    set_temperature->index = command.T.has_value() ? command.T.value() : state.active_tool;
    proto_path.emplace_back(set_temperature);
}

void VisitCommand::to_proto_path([[maybe_unused]] const gcode::ast::Comment& command)
{
    const auto comment = std::make_shared<botcmd::Comment>();
    comment->comment = command.C;
    proto_path.emplace_back(comment);
}

void VisitCommand::to_proto_path([[maybe_unused]] const gcode::ast::T& command)
{
    if (command.S.has_value())
    {
        const auto tool_change = std::make_shared<botcmd::ChangeTool>();
        tool_change->index = command.S.value();
        proto_path.emplace_back(tool_change);
    }
}

botcmd::CommandList toCommand(gcode::ast::ast_t& gcode)
{
    spdlog::info("Translating the GCode AST to CommandList");

    VisitCommand visit_command;

    for (const auto& instruction : gcode)
    {
        std::visit(visit_command, instruction);
    }

    return visit_command.proto_path;
}

} // namespace dulcificum::gcode
