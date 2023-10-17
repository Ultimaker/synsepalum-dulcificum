#ifndef INCLUDE_DULCIFICUM_MIRACLE_JTP_MGJTP_COMMAND_TO_JSON_H
#define INCLUDE_DULCIFICUM_MIRACLE_JTP_MGJTP_COMMAND_TO_JSON_H

#include "dulcificum/command_types.h"

#include <nlohmann/json.hpp>

namespace dulcificum::miracle_jtp
{

nlohmann::json toJson(const botcmd::Command& cmd);

} // namespace dulcificum::miracle_jtp

#endif // INCLUDE_DULCIFICUM_MIRACLE_JTP_MGJTP_COMMAND_TO_JSON_H
