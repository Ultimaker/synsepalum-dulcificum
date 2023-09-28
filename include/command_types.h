#ifndef DULCIFICUM_COMMAND_TYPES_H_
#define DULCIFICUM_COMMAND_TYPES_H_

#include <cmath>
#include <vector>
#include <memory>

namespace dulcificum {

    // x, y, z, a, b
    typedef std::array<double, 5> ParamPoint;

    namespace botcommand {

        enum class CommandType {
            ct_move = 0,            // most commands are move commands
            ct_fan_toggle,          // look at m_fan_state
            ct_fan_speed,           // look at m_fan_speed
            ct_gradient_fan_toggle, // turn on gradient fan
            ct_gradient_fan_speed,  // set speed for gradient fan
            ct_temperature,         // change temperature for active tool
            ct_tool_change_speed,   // set speed for tool change
            ct_tool_change,         // change active tool
            ct_progress,            // set progress display
            ct_comment_only,        // do nothing, only emit comments
            ct_invalid,
            ct_delay,               // delay - command to wait for event
            ct_wait_for_temp,       // firmware delays until temp reached
            ct_pause,               // Command to allow for user defined pause.
            // It will be typically triggered at
            // pre-specified z-height.
            ct_enable_purge         // set whether purging on jam is allowed for a
            // given z-height
        };

        enum class Tag {
            Support,
            Roof,
            Raft,
            Infill,
            Sparse,
            Restart
        };

        typedef size_t ExtruderIndex;

        struct Command {
            Command() = delete;
            Command(CommandType type) : type(type) {}
            const CommandType type;
        };

        typedef std::shared_ptr<Command> CommandPtr;
        typedef std::vector<CommandPtr> CommandList;

        struct Comment : public Command {
            Comment()
                    : Command(CommandType::ct_comment_only) {}
            std::string comment;
        };

        struct Move : public Command {
            Move()
              : Command(CommandType::ct_move) {}
            ParamPoint point = {NAN, NAN, NAN, NAN, NAN};
            double feedrate = NAN;
            std::array<bool, 5> is_point_relative = {false, false, false, true,
                                                     true};
            std::vector<Tag> tags;
        };


        struct FanDuty : public Command {
            FanDuty() : Command(CommandType::ct_fan_speed) {}
            ExtruderIndex index = 0;
            float duty = 0.0;
        };

        struct FanToggle : public Command {
            FanToggle() : Command(CommandType::ct_fan_toggle) {}
            ExtruderIndex index = 0;
            bool is_on = false;
        };


        struct SetTemperature : public Command {
            SetTemperature() : Command(CommandType::ct_temperature) {}
            ExtruderIndex index = 0;
            double temperature = 0.0;
        };

        struct ToolChange : public Command {
            ToolChange() : Command(CommandType::ct_tool_change) {}
            ParamPoint position = {NAN, NAN, NAN, NAN, NAN};
        };

        struct Delay : public Command {
            Delay() : Command(CommandType::ct_delay) {}
            double seconds = 0.0;
        };
    }
}

#endif //DULCIFICUM_COMMAND_TYPES_H_
