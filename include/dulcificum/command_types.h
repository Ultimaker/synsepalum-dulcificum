#ifndef DULCIFICUM_COMMAND_TYPES_H
#define DULCIFICUM_COMMAND_TYPES_H

#include <array>
#include <cmath>
#include <memory>
#include <string>
#include <vector>

namespace dulcificum
{

// x, y, z, a, b
using ParamPoint = std::array<double, 5>;

namespace botcmd
{

enum class CommandType
{
    kInvalid,
    kMove, // most commands are move commands
    kActiveFanEnable, // look at m_fan_state
    kActiveFanDuty, // look at m_fan_speed
    kSetTemperature, // change temperature for active tool
    kChangeTool, // change active tool
    kComment, // do nothing, only emit comments
    kDelay, // delay - command to wait for event
    kWaitForTemperature, // firmware delays until temp reached
    kPause, // Command to allow for user defined pause.
};

enum class Tag
{
    Invalid,
    Infill,
    Inset,
    Purge,
    QuickToggle,
    Raft,
    Restart,
    Roof,
    Support,
    Sparse,
    TravelMove
};

using ExtruderIndex = size_t;

struct Command
{
    Command() = delete;

    explicit Command(CommandType type) noexcept
        : type{ type }
    {
    }

    const CommandType type;
    std::vector<Tag> tags;
};

using CommandPtr = std::shared_ptr<Command>;
using CommandList = std::vector<CommandPtr>;

struct Comment : public Command
{
    Comment() noexcept
        : Command(CommandType::kComment)
    {
    }

    std::string comment{};
};

struct Move : public Command
{
    Move() noexcept
        : Command(CommandType::kMove)
    {
    }

    ParamPoint point{ NAN, NAN, NAN, NAN, NAN };
    double feedrate{ NAN };
    std::array<bool, 5> is_point_relative{ false, false, false, true, true };
};

struct FanDuty : public Command
{
    FanDuty() noexcept
        : Command(CommandType::kActiveFanDuty)
    {
    }

    ExtruderIndex index{ 0 };
    double duty{ 0.0 };
};

struct FanToggle : public Command
{
    FanToggle() noexcept
        : Command(CommandType::kActiveFanEnable)
    {
    }

    ExtruderIndex index{ 0 };
    bool is_on{ false };
};

struct SetTemperature : public Command
{
    SetTemperature() noexcept
        : Command(CommandType::kSetTemperature)
    {
    }

    ExtruderIndex index{ 0 };
    double temperature{ 0.0 };
};

struct WaitForTemperature : public Command
{
    WaitForTemperature() noexcept
        : Command(CommandType::kWaitForTemperature)
    {
    }

    ExtruderIndex index{ 0 };
};

struct ChangeTool : public Command
{
    ChangeTool() noexcept
        : Command(CommandType::kChangeTool)
    {
    }

    ExtruderIndex index{ 0 };
    ParamPoint position{ NAN, NAN, NAN, NAN, NAN };
};

struct Delay : public Command
{
    Delay() noexcept
        : Command(CommandType::kDelay)
    {
    }

    double seconds{ 0.0 };
};

CommandPtr spawnCommandPtr(const CommandType& type) noexcept;

} // namespace botcmd
} // namespace dulcificum

#endif // DULCIFICUM_COMMAND_TYPES_H