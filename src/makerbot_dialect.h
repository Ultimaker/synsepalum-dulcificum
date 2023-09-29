#ifndef DULCIFICUM_MAKERBOT_DIALECT_H_
#define DULCIFICUM_MAKERBOT_DIALECT_H_

#include "nlohmann/json.hpp"

#include "../include/command_types.h"

namespace dulcificum {

    using namespace botcmd;

    namespace miraclegrue {
        class CommandParser {
            // Converts miraclgrue commands with limited history

//            static nlohmann::json toJson(const CommandPtr& cmd);

//            CommandPtr toCommand(const nlohmann::json& jin);
        };
    }
}

#endif //DULCIFICUM_MAKERBOT_DIALECT_H_
