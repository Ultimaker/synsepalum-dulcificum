#ifndef DULCIFICUM_MIRACLE_GRUE_JSONTOOLPATH_IN_H
#define DULCIFICUM_MIRACLE_GRUE_JSONTOOLPATH_IN_H

#include "dulcificum/miracle_jtp/mgjtp_mappings_json_key_to_str.h"

#include <nlohmann/json.hpp>

namespace dulcificum::miracle_jtp
{

botcmd::CommandPtr toCommand(const nlohmann::json& jcmd);

} // namespace dulcificum::miracle_jtp

#endif // DULCIFICUM_MIRACLE_GRUE_JSONTOOLPATH_IN_H
