#ifndef DULCIFICUM_MIRACLEGRUE_JSONTOOLPATH_H
#define DULCIFICUM_MIRACLEGRUE_JSONTOOLPATH_H

#include "dulcificum/command_types.h"

#include <nlohmann/json.hpp>

namespace dulcificum::miracle_jtp
{

nlohmann::json toJson(const botcmd::Command& cmd);

} // namespace dulcificum::miracle_jtp

#endif // DULCIFICUM_MIRACLEGRUE_JSONTOOLPATH_H
