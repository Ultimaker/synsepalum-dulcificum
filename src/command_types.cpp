#include "dulcificum/command_types.h"

namespace dulcificum::botcmd {
CommandPtr spawnCommandPtr(CommandType type) {
    switch (type) {
        case CommandType::kMove:
            return std::make_shared<Move>();
        case CommandType::kActiveFanEnable:
            return std::make_shared<FanToggle>();
        case CommandType::kActiveFanDuty:
            return std::make_shared<FanDuty>();
        case CommandType::kDelay:
            return std::make_shared<Delay>();
        case CommandType::kChangeTool:
            return std::make_shared<ChangeTool>();
        case CommandType::kSetTemperature:
            return std::make_shared<SetTemperature>();
        case CommandType::kWaitForTemperature:
            return std::make_shared<WaitForTemperature>();
        default:
            return std::make_shared<Command>(type);
    }
}
}
