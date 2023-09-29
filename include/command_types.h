#ifndef DULCIFICUM_COMMAND_TYPES_H_
#define DULCIFICUM_COMMAND_TYPES_H_

#include <cmath>
#include <vector>
#include <memory>

namespace dulcificum {

    // x, y, z, a, b
    typedef std::array<double, 5> ParamPoint;

    namespace botcmd {

        enum class CommandType {
            kMove = 0,            // most commands are move commands
            kActiveFanEnable,          // look at m_fan_state
            kActiveFanDuty,           // look at m_fan_speed
            kSetTemperature,         // change temperature for active tool
            kChangeTool,         // change active tool
            kComment,        // do nothing, only emit comments
            kDelay,               // delay - command to wait for event
            kWaitForTemperature,       // firmware delays until temp reached
            kPause,               // Command to allow for user defined pause.
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
                    : Command(CommandType::kComment) {}
            std::string comment;
        };

        struct Move : public Command {
            Move()
              : Command(CommandType::kMove) {}
            ParamPoint point = {NAN, NAN, NAN, NAN, NAN};
            double feedrate = NAN;
            std::array<bool, 5> is_point_relative = {false, false, false, true,
                                                     true};
            std::vector<Tag> tags;
        };


        struct FanDuty : public Command {
            FanDuty() : Command(CommandType::kActiveFanDuty) {}
            ExtruderIndex index = 0;
            float duty = 0.0;
        };

        struct FanToggle : public Command {
            FanToggle() : Command(CommandType::kActiveFanEnable) {}
            ExtruderIndex index = 0;
            bool is_on = false;
        };


        struct SetTemperature : public Command {
            SetTemperature() : Command(CommandType::kSetTemperature) {}
            ExtruderIndex index = 0;
            double temperature = 0.0;
        };

        struct WaitForTemperature : public Command {
            WaitForTemperature() : Command(CommandType::kWaitForTemperature) {}
            ExtruderIndex index = 0;
        };

        struct ToolChange : public Command {
            ToolChange() : Command(CommandType::kChangeTool) {}
            ParamPoint position = {NAN, NAN, NAN, NAN, NAN};
        };

        struct Delay : public Command {
            Delay() : Command(CommandType::kDelay) {}
            double seconds = 0.0;
        };

        CommandPtr spawnCommandPtr(CommandType type) {
            switch(type) {
                case CommandType::kMove:
                    return std::make_shared<Move>();
                case CommandType::kActiveFanEnable:
                    return std::make_shared<FanToggle>();
                case CommandType::kActiveFanDuty:
                    return std::make_shared<FanDuty>();
                case CommandType::kDelay:
                    return std::make_shared<Delay>();
                case CommandType::kChangeTool:
                    return std::make_shared<ToolChange>();
                case CommandType::kSetTemperature:
                    return std::make_shared<SetTemperature>();
                case CommandType::kWaitForTemperature:
                    return std::make_shared<WaitForTemperature>();
                default:
                    return std::make_shared<Command>(type);
            }
        }
    }
}

#endif //DULCIFICUM_COMMAND_TYPES_H_
