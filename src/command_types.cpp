#include "dulcificum/command_types.h"

namespace dulcificum::botcmd
{
CommandPtr spawnCommandPtr(const CommandType& type) noexcept
{
    switch (type)
    {
    case CommandType::Move:
        return std::make_shared<Move>();
    case CommandType::ActiveFanEnable:
        return std::make_shared<FanToggle>();
    case CommandType::ActiveFanDuty:
        return std::make_shared<FanDuty>();
    case CommandType::Delay:
        return std::make_shared<Delay>();
    case CommandType::ChangeTool:
        return std::make_shared<ChangeTool>();
    case CommandType::SetTemperature:
        return std::make_shared<SetTemperature>();
    case CommandType::WaitForTemperature:
        return std::make_shared<WaitForTemperature>();
    default:
        return std::make_shared<Command>(type);
    }
}
} // namespace dulcificum::botcmd
