#include "dulcificum/command_types.h"
#include "dulcificum/gcode/ast/ast.h"
#include "dulcificum/state.h"

#include <spdlog/spdlog.h>
#include <range/v3/view/zip.hpp>

namespace dulcificum::gcode
{

struct VisitCommand
{
    state_t state {};
    std::vector<state_t> previous_states { state_t{} };
    dulcificum::botcmd::CommandList proto_path = {};

    void operator()(const auto& command)
    {
        update_state(command);
        to_proto_path(command);
        previous_states.emplace_back(state);
    }

    void update_state(const gcode::ast::G0_G1& command)
    {
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
            state.E[state.active_tool] = state.E_positioning == Positioning::Absolute
                                           ? *command.E
                                           : *command.E + state.E[state.active_tool];
        }

        if (command.F)
        {
            state.F[state.active_tool] = *command.F;
        }
    }
    void update_state(const gcode::ast::G4& command)
    {}
    void update_state(const gcode::ast::G90& command)
    {
        state.X_positioning = Positioning::Absolute;
        state.Y_positioning = Positioning::Absolute;
        state.Z_positioning = Positioning::Absolute;
        state.E_positioning = Positioning::Absolute;
    }
    void update_state(const gcode::ast::G91& command)
    {
        state.X_positioning = Positioning::Relative;
        state.Y_positioning = Positioning::Relative;
        state.Z_positioning = Positioning::Relative;
        state.E_positioning = Positioning::Relative;
    }
    void update_state(const gcode::ast::G92& command)
    {
        if (command.X)
        {
            state.X = *command.X;
        }
        if (command.Y)
        {
            state.Y = *command.Y;
        }
        if (command.Z)
        {
            state.Z = *command.Z;
        }
        if (command.E)
        {
            state.E[state.active_tool] = *command.E;
        }
    }
    void update_state(const gcode::ast::G280& command)
    {}
    void update_state(const gcode::ast::M82& command)
    {
        state.E_positioning = Positioning::Absolute;
    }
    void update_state(const gcode::ast::M83& command)
    {
        state.E_positioning = Positioning::Relative;
    }
    void update_state(const gcode::ast::M104& command)
    {
        state.extruder_temperatures[state.active_tool] = *command.S;
    }
    void update_state(const gcode::ast::M106& command)
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
    void update_state(const gcode::ast::M107& command)
    {
        state.fan_speed = 0;
    }
    void update_state(const gcode::ast::M109& command)
    {
        state.extruder_temperatures[state.active_tool] = *command.S;
    }
    void update_state(const gcode::ast::M140& command)
    {
        state.build_plate_temperature = *command.S;
    }
    void update_state(const gcode::ast::M190& command)
    {
        state.build_plate_temperature = *command.S;
    }
    void update_state(const gcode::ast::M204& command)
    {}
    void update_state(const gcode::ast::M205& command)
    {}
    void update_state(const gcode::ast::Layer& command)
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
    void update_state(const gcode::ast::Mesh& command)
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
    void update_state(const gcode::ast::FeatureType& command)
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
    void update_state(const gcode::ast::InitialTemperatureExtruder& command)
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
    void update_state(const gcode::ast::InitialTemperatureBuildPlate& command)
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
    void update_state(const gcode::ast::BuildVolumeTemperature& command)
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
    void update_state(const gcode::ast::Comment& command)
    {}
    void update_state(const gcode::ast::T& command)
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
    void update_state(const gcode::ast::Unknown& command)
    {}
    void to_proto_path(const gcode::ast::G0_G1& command)
    {
        const auto move = std::make_shared<botcmd::Move>();
        move->point = {
            state.X + state.origin_x,
            state.Y + state.origin_y,
            state.Z + state.origin_z,
            state.E[0],
            state.E[1],
        };
        move->feedrate = state.F[state.active_tool];
        move->is_point_relative = { false, false, false, false, false };
        proto_path.emplace_back(move);
    }
    void to_proto_path(const gcode::ast::G4& command)
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
    void to_proto_path(const gcode::ast::G90& command)
    {
        // absolute positioning
    }
    void to_proto_path(const gcode::ast::G91& command)
    {
        // relative positioning
    }
    void to_proto_path(const gcode::ast::G92& command)
    {
        state.origin_x = state.X - (command.X ? *command.X : 0.0);
        state.origin_y = state.Y - (command.Y ? *command.Y : 0.0);
        state.origin_z = state.Z - (command.Z ? *command.Z : 0.0);
        state.X = command.X ? *command.X : 0.0;
        state.Y = command.Y ? *command.Y : 0.0;
        state.Z = command.X ? *command.Z : 0.0;
    }
    void to_proto_path(const gcode::ast::G280& command)
    {
        // prime blob; TODO: implement
    }
    void to_proto_path(const gcode::ast::M82& command)
    {
        // extruder absolute positioning
    }
    void to_proto_path(const gcode::ast::M83& command)
    {
        // extruder relative positioning
    }
    void to_proto_path(const gcode::ast::M104& command)
    {
        const auto set_temperature = std::make_shared<botcmd::SetTemperature>();
        set_temperature->temperature = state.extruder_temperatures[state.active_tool];
        set_temperature->index = state.active_tool;
        proto_path.emplace_back(set_temperature);
    }
    void to_proto_path(const gcode::ast::M106& command)
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
    void to_proto_path(const gcode::ast::M107& command)
    {
        const auto toggle_fan = std::make_shared<botcmd::FanToggle>();
        toggle_fan->is_on = false;
        toggle_fan->index = 0; // TODO: implement fan index
        proto_path.emplace_back(toggle_fan);
    }
    void to_proto_path(const gcode::ast::M109& command)
    {
        const auto set_temperature = std::make_shared<botcmd::SetTemperature>();
        set_temperature->temperature = state.build_plate_temperature;
        set_temperature->index = state.active_tool;
        proto_path.emplace_back(set_temperature);
    }
    void to_proto_path(const gcode::ast::M140& command)
    {
        // TODO set heated bed temperature
    }
    void to_proto_path(const gcode::ast::M190& command)
    {
        const auto set_temperature = std::make_shared<botcmd::SetTemperature>();
        set_temperature->temperature = state.build_plate_temperature;
        set_temperature->index = state.active_tool;
        proto_path.emplace_back(set_temperature);

        const auto wait_temperature = std::make_shared<botcmd::WaitForTemperature>();
        wait_temperature->index = state.active_tool;
        proto_path.emplace_back(wait_temperature);
    }
    void to_proto_path(const gcode::ast::M204& command)
    {
        // TODO set acceleration
    }
    void to_proto_path(const gcode::ast::M205& command)
    {}
    void to_proto_path(const gcode::ast::Layer& command)
    {}
    void to_proto_path(const gcode::ast::Mesh& command)
    {}
    void to_proto_path(const gcode::ast::FeatureType& command)
    {}
    void to_proto_path(const gcode::ast::InitialTemperatureExtruder& command)
    {}
    void to_proto_path(const gcode::ast::InitialTemperatureBuildPlate& command)
    {}
    void to_proto_path(const gcode::ast::BuildVolumeTemperature& command)
    {}
    void to_proto_path(const gcode::ast::Comment& command)
    {
        const auto comment = std::make_shared<botcmd::Comment>();
        comment->comment = command.C;
        proto_path.emplace_back(comment);
    }
    void to_proto_path(const gcode::ast::T& command)
    {
        const auto tool_change = std::make_shared<botcmd::ChangeTool>();
        tool_change->index = state.active_tool;
        proto_path.emplace_back(tool_change);
    }
    void to_proto_path(const gcode::ast::Unknown& command)
    {}
};

botcmd::CommandList toCommand(gcode::ast::ast_t& gcode)
{
    spdlog::info("Translating the GCode AST to CommandList");

    VisitCommand visit_command {};

    for (const auto& instruction : gcode)
    {
        std::visit(visit_command, instruction);
    }

    return visit_command.proto_path;
}

} // namespace dulcificum::gcode