#ifndef INCLUDE_DULCIFICUM_COMMAND_TYPES_H
#define INCLUDE_DULCIFICUM_COMMAND_TYPES_H

#include <cmath>
#include <memory>
#include <string>
#include <vector>

namespace dulcificum
{

// x, y, z, a, b
using ParamPoint = std::vector<double>;

namespace botcmd
{

enum class CommandType
{
    Invalid,
    Move, // most commands are move commands
    ActiveFanEnable, // look at m_fan_state
    ActiveFanDuty, // look at m_fan_speed
    SetTemperature, // change temperature for active tool
    ChangeTool, // change active tool
    Comment, // do nothing, only emit comments
    Delay, // delay - command to wait for event
    LayerChange, // layer - denoting a layer is changed
    WaitForTemperature, // firmware delays until temp reached
    Pause, // Command to allow for user defined pause.
};

#define MB_BEAD_MODE_TAG(TAG_NAME) \
    TAG_NAME##_0, \
    TAG_NAME##_1

enum class Tag
{
    Invalid,
    Infill,
    Inset,
    Purge,
    QuickToggle,
    Raft,
    Restart,
    Retract,
    Roof,
    Support,
    Sparse,
    TravelMove,
    MB_BEAD_MODE_TAG(Fill),
    MB_BEAD_MODE_TAG(PrimeTower),
    MB_BEAD_MODE_TAG(TopSurface),
    MB_BEAD_MODE_TAG(Support),
    MB_BEAD_MODE_TAG(SupportInterface),
    MB_BEAD_MODE_TAG(WallOuter),
    MB_BEAD_MODE_TAG(WallInner),
    MB_BEAD_MODE_TAG(Skirt)
};

#undef MB_BEAD_MODE_TAG

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
        : Command(CommandType::Comment)
    {
    }

    std::string comment{};
};

struct Move : public Command
{
    Move() noexcept
        : Command(CommandType::Move)
    {
    }

    ParamPoint point{ NAN, NAN, NAN, NAN, NAN };
    double feedrate{ NAN };
    std::vector<bool> is_point_relative{ false, false, false, true, true };
};

struct FanDuty : public Command
{
    FanDuty() noexcept
        : Command(CommandType::ActiveFanDuty)
    {
    }

    ExtruderIndex index{ 0 };
    double duty{ 0.0 };
};

struct FanToggle : public Command
{
    FanToggle() noexcept
        : Command(CommandType::ActiveFanEnable)
    {
    }

    ExtruderIndex index{ 0 };
    bool is_on{ false };
};

struct SetTemperature : public Command
{
    SetTemperature() noexcept
        : Command(CommandType::SetTemperature)
    {
    }

    ExtruderIndex index{ 0 };
    double temperature{ 0.0 };
};

struct WaitForTemperature : public Command
{
    WaitForTemperature() noexcept
        : Command(CommandType::WaitForTemperature)
    {
    }

    ExtruderIndex index{ 0 };
};

struct ChangeTool : public Command
{
    ChangeTool() noexcept
        : Command(CommandType::ChangeTool)
    {
    }

    ExtruderIndex index{ 0 };
    ParamPoint position{ NAN, NAN, NAN, NAN, NAN }; // Badman
};

struct Delay : public Command
{
    Delay() noexcept
        : Command(CommandType::Delay)
    {
    }

    double seconds{ 0.0 };
};

struct LayerChange : public Command
{
    LayerChange() noexcept
        : Command(CommandType::LayerChange)
    {
    }

    int64_t layer{ 0 };
};

CommandPtr spawnCommandPtr(const CommandType& type) noexcept;

} // namespace botcmd
} // namespace dulcificum

#endif // INCLUDE_DULCIFICUM_COMMAND_TYPES_H
