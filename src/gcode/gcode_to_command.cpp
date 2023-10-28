#include "dulcificum/command_types.h"
#include "dulcificum/gcode/ast/acceleration.h"
#include "dulcificum/gcode/ast/ast.h"
#include "dulcificum/gcode/ast/bed_temperature.h"
#include "dulcificum/gcode/ast/comment_commands.h"
#include "dulcificum/gcode/ast/delay.h"
#include "dulcificum/gcode/ast/extruder_temperature.h"
#include "dulcificum/gcode/ast/fan.h"
#include "dulcificum/gcode/ast/position.h"
#include "dulcificum/gcode/ast/positioning_mode.h"
#include "dulcificum/gcode/ast/purge.h"
#include "dulcificum/gcode/ast/toolchange.h"
#include "dulcificum/gcode/ast/translate.h"
#include "dulcificum/state.h"

#include <spdlog/spdlog.h>

#include <memory>
#include <unordered_map>
#include <variant>
#include <vector>

namespace dulcificum::gcode
{

struct VisitCommand
{
    state_t state{};
    std::vector<state_t> previous_states{ state_t{} };
    dulcificum::botcmd::CommandList proto_path = {};

    void operator()(const auto& command)
    {
        update_state(command);
        to_proto_path(command);
        previous_states.emplace_back(state);
    }

    [[maybe_unused]] void update_state(const gcode::ast::G0_G1& command)
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
            state.E[state.active_tool] = state.E_positioning == Positioning::Absolute ? *command.E : *command.E + state.E[state.active_tool];
        }

        if (command.F)
        {
            state.F[state.active_tool] = *command.F;
        }
    }
    [[maybe_unused]] void update_state(const gcode::ast::G4& command)
    {
    }
    [[maybe_unused]] void update_state([[maybe_unused]] const gcode::ast::G90& command)
    {
        state.X_positioning = Positioning::Absolute;
        state.Y_positioning = Positioning::Absolute;
        state.Z_positioning = Positioning::Absolute;
        state.E_positioning = Positioning::Absolute;
    }
    [[maybe_unused]] void update_state([[maybe_unused]] const gcode::ast::G91& command)
    {
        state.X_positioning = Positioning::Relative;
        state.Y_positioning = Positioning::Relative;
        state.Z_positioning = Positioning::Relative;
        state.E_positioning = Positioning::Relative;
    }
    [[maybe_unused]] void update_state(const gcode::ast::G92& command)
    {
        state.origin_x = state.X - (command.X ? *command.X : 0.0);
        state.origin_y = state.Y - (command.Y ? *command.Y : 0.0);
        state.origin_z = state.Z - (command.Z ? *command.Z : 0.0);
        state.X = command.X ? *command.X : 0.0;
        state.Y = command.Y ? *command.Y : 0.0;
        state.Z = command.X ? *command.Z : 0.0;
    }
    [[maybe_unused]] void update_state(const gcode::ast::G280& command)
    {
    }
    [[maybe_unused]] void update_state([[maybe_unused]] const gcode::ast::M82& command)
    {
        state.E_positioning = Positioning::Absolute;
    }
    [[maybe_unused]] void update_state([[maybe_unused]] const gcode::ast::M83& command)
    {
        state.E_positioning = Positioning::Relative;
    }
    [[maybe_unused]] void update_state(const gcode::ast::M104& command)
    {
        state.extruder_temperatures[state.active_tool] = *command.S;
    }
    [[maybe_unused]] void update_state(const gcode::ast::M106& command)
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
    [[maybe_unused]] void update_state([[maybe_unused]] const gcode::ast::M107& command)
    {
        state.fan_speed = 0;
    }
    [[maybe_unused]] void update_state(const gcode::ast::M109& command)
    {
        state.extruder_temperatures[state.active_tool] = *command.S;
    }
    [[maybe_unused]] void update_state(const gcode::ast::M140& command)
    {
        state.build_plate_temperature = *command.S;
    }
    [[maybe_unused]] void update_state(const gcode::ast::M190& command)
    {
        state.build_plate_temperature = *command.S;
    }
    [[maybe_unused]] void update_state([[maybe_unused]] const gcode::ast::M204& command)
    {
    }
    [[maybe_unused]] void update_state([[maybe_unused]] const gcode::ast::M205& command)
    {
    }
    [[maybe_unused]] void update_state(const gcode::ast::Layer& command)
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
    [[maybe_unused]] void update_state(const gcode::ast::Mesh& command)
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
    [[maybe_unused]] void update_state(const gcode::ast::FeatureType& command)
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
    [[maybe_unused]] void update_state(const gcode::ast::InitialTemperatureExtruder& command)
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
    [[maybe_unused]] void update_state(const gcode::ast::InitialTemperatureBuildPlate& command)
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
    [[maybe_unused]] void update_state(const gcode::ast::BuildVolumeTemperature& command)
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
    [[maybe_unused]] void update_state([[maybe_unused]] const gcode::ast::Comment& command)
    {
    }
    [[maybe_unused]] void update_state(const gcode::ast::T& command)
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
    [[maybe_unused]] void update_state([[maybe_unused]] const gcode::ast::Unknown& command)
    {
    }

    std::vector<double> getMovePoints(const gcode::ast::G0_G1& command) const
    {
        return {
            command.X ? *command.X : 0.0,
            command.Y ? *command.Y : 0.0,
            command.Z ? *command.Z : 0.0,
            state.active_tool == 0 && command.E ? *command.E : 0.0,
            state.active_tool == 1 && command.E ? *command.E : 0.0,
        };
    }

    std::vector<bool> getPointRelativeness(const gcode::ast::G0_G1& command) const
    {
        return {
            command.X ? state.X_positioning == Positioning::Relative : true,
            command.Y ? state.Y_positioning == Positioning::Relative : true,
            command.Z ? state.Z_positioning == Positioning::Relative : true,
            state.active_tool == 0 && command.E ? state.E_positioning == Positioning::Relative : true,
            state.active_tool == 1 && command.E ? state.E_positioning == Positioning::Relative : true,
        };
    }

    double calculateDeltaE(const gcode::ast::G0_G1& command)
    {
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
        return delta_e;
    }

    void generateCommandTags(double delta_e, const std::shared_ptr<botcmd::Move>& move)
    {
        static const std::unordered_map<std::string, botcmd::Tag> tagMap = {
            { "WALL-OUTER", botcmd::Tag::Inset },
            { "INNER-OUTER", botcmd::Tag::Inset },
            { "SKIN", botcmd::Tag::Roof },
            { "TOP-SURFACE", botcmd::Tag::Roof },
            { "PRIME-TOWER", botcmd::Tag::Purge },
            { "FILL", botcmd::Tag::Infill },
            { "FILL", botcmd::Tag::Sparse },
            { "Purge", botcmd::Tag::Raft },
            { "SUPPORT", botcmd::Tag::Sparse },
            { "SUPPORT", botcmd::Tag::Support },
            { "SUPPORT-INTERFACE", botcmd::Tag::Support },
        };

        auto iter = tagMap.find(state.feature_type);
        if (iter == tagMap.end())
        {
            move->tags.emplace_back(iter->second);
        }
        else
        {
            spdlog::warn("Unknown feature type: {}", state.feature_type);
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
    }

    [[maybe_unused]] void to_proto_path(const gcode::ast::G0_G1& command)
    {
        const auto move = std::make_shared<botcmd::Move>();

        move->point = getMovePoints(command);
        move->is_point_relative = getPointRelativeness(command);

        const double delta_e = calculateDeltaE(command);
        generateCommandTags(delta_e, move);

        proto_path.emplace_back(move);
    }

    [[maybe_unused]] void to_proto_path(const gcode::ast::G4& command)
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
    [[maybe_unused]] void to_proto_path([[maybe_unused]] const gcode::ast::G90& command)
    {
        // absolute positioning
    }
    [[maybe_unused]] void to_proto_path([[maybe_unused]] const gcode::ast::G91& command)
    {
        // relative positioning
    }
    [[maybe_unused]] void to_proto_path([[maybe_unused]] const gcode::ast::G92& command)
    {
    }
    [[maybe_unused]] void to_proto_path([[maybe_unused]] const gcode::ast::G280& command)
    {
        // prime blob; TODO: implement
    }
    [[maybe_unused]] void to_proto_path([[maybe_unused]] const gcode::ast::M82& command)
    {
        // extruder absolute positioning
    }
    [[maybe_unused]] void to_proto_path([[maybe_unused]] const gcode::ast::M83& command)
    {
        // extruder relative positioning
    }
    [[maybe_unused]] void to_proto_path([[maybe_unused]] const gcode::ast::M104& command)
    {
        const auto set_temperature = std::make_shared<botcmd::SetTemperature>();
        set_temperature->temperature = state.extruder_temperatures[state.active_tool];
        set_temperature->index = state.active_tool;
        proto_path.emplace_back(set_temperature);
    }
    [[maybe_unused]] void to_proto_path([[maybe_unused]] const gcode::ast::M106& command)
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
    [[maybe_unused]] void to_proto_path([[maybe_unused]] const gcode::ast::M107& command)
    {
        const auto toggle_fan = std::make_shared<botcmd::FanToggle>();
        toggle_fan->is_on = false;
        toggle_fan->index = 0; // TODO: implement fan index
        proto_path.emplace_back(toggle_fan);
    }
    [[maybe_unused]] void to_proto_path([[maybe_unused]] const gcode::ast::M109& command)
    {
        const auto set_temperature = std::make_shared<botcmd::SetTemperature>();
        set_temperature->temperature = state.build_plate_temperature;
        set_temperature->index = state.active_tool;
        proto_path.emplace_back(set_temperature);
    }
    [[maybe_unused]] void to_proto_path([[maybe_unused]] const gcode::ast::M140& command)
    {
        // TODO set heated bed temperature
    }
    [[maybe_unused]] void to_proto_path([[maybe_unused]] const gcode::ast::M190& command)
    {
        const auto set_temperature = std::make_shared<botcmd::SetTemperature>();
        set_temperature->temperature = state.build_plate_temperature;
        set_temperature->index = state.active_tool;
        proto_path.emplace_back(set_temperature);

        const auto wait_temperature = std::make_shared<botcmd::WaitForTemperature>();
        wait_temperature->index = state.active_tool;
        proto_path.emplace_back(wait_temperature);
    }
    [[maybe_unused]] void to_proto_path([[maybe_unused]] const gcode::ast::M204& command)
    {
        // TODO set acceleration
    }
    [[maybe_unused]] void to_proto_path([[maybe_unused]] const gcode::ast::M205& command)
    {
    }
    [[maybe_unused]] void to_proto_path(const gcode::ast::Layer& command)
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
    [[maybe_unused]] void to_proto_path([[maybe_unused]] const gcode::ast::Mesh& command)
    {
    }
    [[maybe_unused]] void to_proto_path([[maybe_unused]] const gcode::ast::FeatureType& command)
    {
    }
    [[maybe_unused]] void to_proto_path([[maybe_unused]] const gcode::ast::InitialTemperatureExtruder& command)
    {
    }
    [[maybe_unused]] void to_proto_path([[maybe_unused]] const gcode::ast::InitialTemperatureBuildPlate& command)
    {
    }
    [[maybe_unused]] void to_proto_path([[maybe_unused]] const gcode::ast::BuildVolumeTemperature& command)
    {
    }
    [[maybe_unused]] void to_proto_path(const gcode::ast::Comment& command)
    {
        const auto comment = std::make_shared<botcmd::Comment>();
        comment->comment = command.C;
        proto_path.emplace_back(comment);
    }
    [[maybe_unused]] void to_proto_path([[maybe_unused]] const gcode::ast::T& command)
    {
        const auto tool_change = std::make_shared<botcmd::ChangeTool>();
        tool_change->index = state.active_tool;
        proto_path.emplace_back(tool_change);
    }
    [[maybe_unused]] void to_proto_path([[maybe_unused]] const gcode::ast::Unknown& command)
    {
    }
};

botcmd::CommandList toCommand(gcode::ast::ast_t& gcode)
{
    spdlog::info("Translating the GCode AST to CommandList");

    VisitCommand visit_command{};

    for (const auto& instruction : gcode)
    {
        std::visit(visit_command, instruction);
    }

    return visit_command.proto_path;
}

} // namespace dulcificum::gcode