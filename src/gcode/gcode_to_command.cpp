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

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace dulcificum::gcode
{

void VisitCommand::update_state(const gcode::ast::G0_G1& command)
{
    spdlog::debug("Updating state for {}: {}", command.index, command.line);
    if (command.X)
    {
        state.X = state.X_positioning == Positioning::Absolute ? *command.X : *command.X + state.X;
    }
    if (command.Y)
    {
        state.Y = state.Y_positioning == Positioning::Absolute ? *command.Y : *command.Y + state.Y;
    }
    if (command.Z)
    {
        state.Z = state.Z_positioning == Positioning::Absolute ? *command.Z : *command.Z + state.Z;
    }
    if (command.E)
    {
        state.E[state.active_tool] = state.E_positioning == Positioning::Absolute ? *command.E : *command.E + state.E[state.active_tool];
    }

    if (command.F)
    {
        state.F[state.active_tool] = *command.F;
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
    state.origin_x = state.X - (command.X ? *command.X : 0.0);
    state.origin_y = state.Y - (command.Y ? *command.Y : 0.0);
    state.origin_z = state.Z - (command.Z ? *command.Z : 0.0);
    state.X = command.X ? *command.X : 0.0;
    state.Y = command.Y ? *command.Y : 0.0;
    state.Z = command.X ? *command.Z : 0.0;
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
    state.extruder_temperatures[state.active_tool] = *command.S;
}

void VisitCommand::update_state(const gcode::ast::M106& command)
{
    if (command.S)
    {
        state.fan_speed = *command.S;
    }
    else
    {
        state.fan_speed = 255;
    }
}

void VisitCommand::update_state([[maybe_unused]] const gcode::ast::M107& command)
{
    state.fan_speed = 0;
}

void VisitCommand::update_state(const gcode::ast::M109& command)
{
    state.extruder_temperatures[state.active_tool] = *command.S;
}

void VisitCommand::update_state(const gcode::ast::M140& command)
{
    state.build_plate_temperature = *command.S;
}

void VisitCommand::update_state(const gcode::ast::M190& command)
{
    state.build_plate_temperature = *command.S;
}

void VisitCommand::update_state(const gcode::ast::Layer& command)
{
    if (command.L)
    {
        state.layer = *command.L;
    }
    else
    {
        spdlog::warn("Layer command without layer index");
    }
}

void VisitCommand::update_state(const gcode::ast::Mesh& command)
{
    if (command.M)
    {
        state.mesh = *command.M;
    }
    else
    {
        spdlog::warn("Mesh command without mesh index");
    }
}

void VisitCommand::update_state(const gcode::ast::FeatureType& command)
{
    if (command.T)
    {
        state.feature_type = *command.T;
    }
    else
    {
        spdlog::warn("FeatureType command without feature type index");
    }
}

void VisitCommand::update_state(const gcode::ast::InitialTemperatureExtruder& command)
{
    if (command.T && command.S)
    {
        state.extruder_temperatures[*command.T] = *command.S;
    }
    else
    {
        spdlog::warn("InitialTemperatureExtruder command without tool index or temperature");
    }
}

void VisitCommand::update_state(const gcode::ast::InitialTemperatureBuildPlate& command)
{
    if (command.S)
    {
        state.build_plate_temperature = *command.S;
    }
    else
    {
        spdlog::warn("InitialTemperatureBuildPlate command without temperature");
    }
}

void VisitCommand::update_state(const gcode::ast::BuildVolumeTemperature& command)
{
    if (command.S)
    {
        state.chamber_temperature = *command.S;
    }
    else
    {
        spdlog::warn("BuildVolumeTemperature command without temperature");
    }
}

void VisitCommand::update_state(const gcode::ast::T& command)
{
    if (command.S)
    {
        state.active_tool = *command.S;
    }
    else
    {
        spdlog::warn("Tool change command without tool index");
    }
}

void VisitCommand::to_proto_path(const gcode::ast::G0_G1& command)
{
    const auto move = std::make_shared<botcmd::Move>();
    // if position is not specified for an axis, move with a relative delta 0 move
    move->point = {
        command.X ? *command.X : 0.0,
        command.Y ? *command.Y : 0.0,
        command.Z ? *command.Z : 0.0,
        state.active_tool == 0 && command.E ? *command.E : 0.0,
        state.active_tool == 1 && command.E ? *command.E : 0.0,
    };
    # gcode is in mm/min, bot cmd uses mm/sec
    move->feedrate = state.F[state.active_tool] / 60.0;
    move->is_point_relative = {
        command.X ? state.X_positioning == Positioning::Relative : true,
        command.Y ? state.Y_positioning == Positioning::Relative : true,
        command.Z ? state.Z_positioning == Positioning::Relative : true,
        state.active_tool == 0 && command.E ? state.E_positioning == Positioning::Relative : true,
        state.active_tool == 1 && command.E ? state.E_positioning == Positioning::Relative : true,
    };

    double delta_e{};
    if (! command.E)
    {
        delta_e = 0.0;
    }
    else if (state.E_positioning == Positioning::Relative)
    {
        delta_e = *command.E;
    }
    else
    {
        const auto last_e = previous_states[previous_states.size() - 1].E[state.active_tool];
        delta_e = *command.E - last_e;
    }

    if (state.is_retracted)
    {
        if (delta_e > 0)
        {
            move->tags.emplace_back(botcmd::Tag::Restart);
            state.is_retracted = false;
        }
        else
        {
            move->tags.emplace_back(botcmd::Tag::TravelMove);
        }
    }
    else if (delta_e < 0)
    {
        move->tags.emplace_back(botcmd::Tag::Retract);
        state.is_retracted = true;
    }
    else if (delta_e == 0)
    {
        move->tags.emplace_back(botcmd::Tag::TravelMove);
    }
    if (state.feature_type == "WALL-OUTER" || state.feature_type == "INNER-OUTER")
    {
        move->tags.emplace_back(botcmd::Tag::Inset);
    }
    else if (state.feature_type == "SKIN")
    {
        move->tags.emplace_back(botcmd::Tag::Roof);
    }
    else if (state.feature_type == "TOP-SURFACE")
    {
        move->tags.emplace_back(botcmd::Tag::Roof);
    }
    else if (state.feature_type == "PRIME-TOWER")
    {
        move->tags.emplace_back(botcmd::Tag::Purge);
    }
    else if (state.feature_type == "FILL")
    {
        move->tags.emplace_back(botcmd::Tag::Infill);
        move->tags.emplace_back(botcmd::Tag::Sparse);
    }
    else if (state.feature_type == "Purge")
    {
        move->tags.emplace_back(botcmd::Tag::Raft);
    }
    else if (state.feature_type == "SUPPORT")
    {
        move->tags.emplace_back(botcmd::Tag::Sparse);
        move->tags.emplace_back(botcmd::Tag::Support);
    }
    else if (state.feature_type == "SUPPORT-INTERFACE")
    {
        move->tags.emplace_back(botcmd::Tag::Support);
    }
    proto_path.emplace_back(move);
}

void VisitCommand::to_proto_path(const gcode::ast::G4& command)
{
    const auto delay = std::make_shared<botcmd::Delay>();
    if (command.S)
    {
        delay->seconds = *command.S;
    }
    else if (command.P)
    {
        delay->seconds = *command.P * 1000.0;
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
    set_temperature->temperature = state.extruder_temperatures[state.active_tool];
    set_temperature->index = state.active_tool;
    proto_path.emplace_back(set_temperature);
}

void VisitCommand::to_proto_path([[maybe_unused]] const gcode::ast::M106& command)
{
    // turn fan on
    const auto toggle_fan = std::make_shared<botcmd::FanToggle>();
    toggle_fan->is_on = false;
    toggle_fan->index = 0; // TODO: implement fan index
    proto_path.emplace_back(toggle_fan);

    // set fan speed
    const auto set_fan_speed = std::make_shared<botcmd::FanDuty>();
    set_fan_speed->duty = state.fan_speed;
    set_fan_speed->index = 0; // TODO; implement fan index
    proto_path.emplace_back(set_fan_speed);
}

void VisitCommand::to_proto_path([[maybe_unused]] const gcode::ast::M107& command)
{
    const auto toggle_fan = std::make_shared<botcmd::FanToggle>();
    toggle_fan->is_on = false;
    toggle_fan->index = 0; // TODO: implement fan index
    proto_path.emplace_back(toggle_fan);
}

void VisitCommand::to_proto_path([[maybe_unused]] const gcode::ast::M109& command)
{
    const auto set_temperature = std::make_shared<botcmd::SetTemperature>();
    set_temperature->temperature = state.build_plate_temperature;
    set_temperature->index = state.active_tool;
    proto_path.emplace_back(set_temperature);
}

void VisitCommand::to_proto_path([[maybe_unused]] const gcode::ast::M190& command)
{
    const auto set_temperature = std::make_shared<botcmd::SetTemperature>();
    set_temperature->temperature = state.build_plate_temperature;
    set_temperature->index = state.active_tool;
    proto_path.emplace_back(set_temperature);

    const auto wait_temperature = std::make_shared<botcmd::WaitForTemperature>();
    wait_temperature->index = state.active_tool;
    proto_path.emplace_back(wait_temperature);
}

void VisitCommand::to_proto_path([[maybe_unused]] const gcode::ast::Layer& command)
{
    const auto layer = std::make_shared<botcmd::LayerChange>();
    if (command.L)
    {
        layer->layer = *command.L;
    }
    else
    {
        spdlog::warn("Layer command without layer number");
    }
    proto_path.emplace_back(layer);
}

void VisitCommand::to_proto_path([[maybe_unused]] const gcode::ast::Comment& command)
{
    const auto comment = std::make_shared<botcmd::Comment>();
    comment->comment = command.C;
    proto_path.emplace_back(comment);
}

void VisitCommand::to_proto_path([[maybe_unused]] const gcode::ast::T& command)
{
    const auto tool_change = std::make_shared<botcmd::ChangeTool>();
    tool_change->index = state.active_tool;
    proto_path.emplace_back(tool_change);
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