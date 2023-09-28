#ifndef DULCIFICUM_MAKERBOT_DIALECT_H_
#define DULCIFICUM_MAKERBOT_DIALECT_H_

#include "nlohmann/json.hpp"

#include "command_types.h"

namespace dulcificum {
    namespace miraclegrue {
        class CommandParser {
            // Converts miraclgrue commands with limited history
            nlohmann::json toJson() const;
        };
    }
}

#endif //DULCIFICUM_MAKERBOT_DIALECT_H_
