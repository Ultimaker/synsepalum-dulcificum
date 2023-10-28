#ifndef INCLUDE_DULCIFICUM_GCODE_TO_COMMAND_H
#define INCLUDE_DULCIFICUM_GCODE_TO_COMMAND_H

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

#include <vector>

namespace dulcificum::gcode
{

botcmd::CommandList toCommand(gcode::ast::ast_t& gcode);

struct VisitCommand
{
    state_t state{};
    std::vector<state_t> previous_states{};
    dulcificum::botcmd::CommandList proto_path = {};

    VisitCommand()
        : previous_states{ state } {};

    void operator()(const auto& command)
    {
        update_state(command);
        to_proto_path(command);
        previous_states.emplace_back(state);
    };

private:
    constexpr void update_state([[maybe_unused]] const auto& command) noexcept
    {
        spdlog::debug("Not updating state for {}: {}", command.index, command.line);
        // TODO: G4
        // TODO: G280
        // TODO: M204
        // TODO: Comment
    };
    void update_state(const ast::G0_G1& command);
    void update_state(const ast::G90& command);
    void update_state(const ast::G91& command);
    void update_state(const ast::G92& command);
    void update_state(const ast::M82& command);
    void update_state(const ast::M83& command);
    void update_state(const ast::M104& command);
    void update_state(const ast::M106& command);
    void update_state(const ast::M107& command);
    void update_state(const ast::M109& command);
    void update_state(const ast::M140& command);
    void update_state(const ast::M190& command);
    void update_state(const ast::Layer& command);
    void update_state(const ast::Mesh& command);
    void update_state(const ast::FeatureType& command);
    void update_state(const ast::InitialTemperatureExtruder& command);
    void update_state(const ast::InitialTemperatureBuildPlate& command);
    void update_state(const ast::BuildVolumeTemperature& command);
    void update_state(const ast::T& command);

    constexpr void to_proto_path(const auto& command)
    {
        spdlog::debug("Not converting to proto_path {}: {}", command.index, command.line);
        // TODO: G90
        // TODO: G91
        // TODO: G92
        // TODO: G280
        // TODO: M82
        // TODO: M83
        // TODO: M140
        // TODO: M204
        // TODO: M205
        // TODO: Mesh
        // TODO: FeatureType
        // TODO: InitialTemperatureExtruder
        // TODO: InitialTemperatureBuildPlate
        // TODO: BuildVolumeTemperature
        // TODO: Unknown
    };
    void to_proto_path(const ast::G0_G1& command);
    void to_proto_path(const ast::G4& command);
    void to_proto_path(const ast::M104& command);
    void to_proto_path(const ast::M106& command);
    void to_proto_path(const ast::M107& command);
    void to_proto_path(const ast::M109& command);
    void to_proto_path(const ast::M190& command);
    //    void to_proto_path(const ast::Layer& command);
    //    void to_proto_path(const ast::Comment& command);
    //    void to_proto_path(const ast::T& command);
};

} // namespace dulcificum::gcode

#endif // INCLUDE_DULCIFICUM_GCODE_TO_COMMAND_H
