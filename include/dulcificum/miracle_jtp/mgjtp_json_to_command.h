#ifndef INCLUDE_DULCIFICUM_MIRACLE_JTP_MGJTP_JSON_TO_COMMAND_H
#define INCLUDE_DULCIFICUM_MIRACLE_JTP_MGJTP_JSON_TO_COMMAND_H

#include "dulcificum/miracle_jtp/mgjtp_mappings_json_key_to_str.h"

#include <nlohmann/json.hpp>

namespace dulcificum::miracle_jtp
{

botcmd::CommandPtr toCommand(const nlohmann::json& jcmd);

} // namespace dulcificum::miracle_jtp

#endif // INCLUDE_DULCIFICUM_MIRACLE_JTP_MGJTP_JSON_TO_COMMAND_H
