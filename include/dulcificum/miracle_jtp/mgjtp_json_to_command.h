#ifndef DULCIFICUM_MIRACLE_GRUE_JSONTOOLPATH_IN_H
#define DULCIFICUM_MIRACLE_GRUE_JSONTOOLPATH_IN_H

#include <dulcificum/miracle_jtp/mgjtp_mappings_json_key_to_str.h>

namespace dulcificum::miracle_jtp {

botcmd::CommandPtr toCommand(const nlohmann::json &jcmd);

}

#endif //DULCIFICUM_MIRACLE_GRUE_JSONTOOLPATH_IN_H
