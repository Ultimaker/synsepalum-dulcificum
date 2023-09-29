#ifndef DULCIFICUM_MIRACLEGRUE_JSONTOOLPATH_H_
#define DULCIFICUM_MIRACLEGRUE_JSONTOOLPATH_H_

#include "nlohmann/json.hpp"

#include "../include/command_types.h"

namespace dulcificum {

    using namespace botcmd;

    // miraclegrue jsontoolpath dialect
    namespace miracle_jtp {
        CommandPtr toCommand(const nlohmann::json& jin);
        nlohmann::json toJson(const Command& cmd);

        //        class CommandBookeeper {
//            // Converts miraclgrue commands with limited history
//
////            static nlohmann::json toJson(const CommandPtr& cmd);
//
////            CommandPtr toCommand(const nlohmann::json& jin);
//        };
    }
}

#endif //DULCIFICUM_MIRACLEGRUE_JSONTOOLPATH_H_
