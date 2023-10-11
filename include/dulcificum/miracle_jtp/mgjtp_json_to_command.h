#ifndef DULCIFICUM_MIRACLE_GRUE_JSONTOOLPATH_IN_H
#define DULCIFICUM_MIRACLE_GRUE_JSONTOOLPATH_IN_H


#include "dulcificum/command_types.h"

#include <nlohmann/json.hpp>

namespace dulcificum::miracle_jtp
{

botcmd::CommandPtr toCommand(const nlohmann::json& jcmd);

} // namespace dulcificum::miracle_jtp

#endif // DULCIFICUM_MIRACLE_GRUE_JSONTOOLPATH_IN_H
