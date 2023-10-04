#ifndef DULCIFICUM_MIRACLEGRUE_JSONTOOLPATH_H_
#define DULCIFICUM_MIRACLEGRUE_JSONTOOLPATH_H_

#include "nlohmann/json.hpp"

#include "../../include/command_types.h"

namespace dulcificum::miracle_jtp {

nlohmann::json toJson(const botcmd::Command &cmd);

}

#endif //DULCIFICUM_MIRACLEGRUE_JSONTOOLPATH_H_
