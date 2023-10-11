#ifndef DULCIFICUM_MIRACLE_GRUE_JSONTOOLPATH_JSON_KEYS_H
#define DULCIFICUM_MIRACLE_GRUE_JSONTOOLPATH_JSON_KEYS_H

#include "dulcificum/miracle_jtp/mgjtp_command_to_json.h"

namespace dulcificum {

namespace botcmd {

NLOHMANN_JSON_SERIALIZE_ENUM(CommandType, {
    { CommandType::kInvalid, "invalid" },
    { CommandType::kMove, "move" },
    { CommandType::kActiveFanEnable, "toggle_fan" },
    { CommandType::kActiveFanDuty, "fan_duty" },
    { CommandType::kSetTemperature, "set_toolhead_temperature" },
    { CommandType::kChangeTool, "change_toolhead" },
    { CommandType::kComment, "comment" },
    { CommandType::kDelay, "delay" },
    { CommandType::kWaitForTemperature, "wait_for_temperature" },
    { CommandType::kPause, "pause" }
})

NLOHMANN_JSON_SERIALIZE_ENUM(Tag, {
    { Tag::Invalid, "Invalid" },
    { Tag::Infill, "Infill" },
    { Tag::Inset, "Inset" },
    { Tag::Purge, "Purge" },
    { Tag::QuickToggle, "Quick Toggle" },
    { Tag::Raft, "Raft" },
    { Tag::Restart, "Restart" },
    { Tag::Roof, "Roof" },
    { Tag::Sparse, "Sparse" },
    { Tag::Support, "Support" },
    { Tag::TravelMove, "Travel Move" }
})

}

namespace miracle_jtp {

namespace kKeyStr {

const static std::string a = "a";
const static std::string b = "b";
const static std::string command = "command";
const static std::string comment = "comment";
const static std::string feedrate = "feedrate";
const static std::string function = "function";
const static std::string index = "index";
const static std::string metadata = "metadata";
const static std::string parameters = "parameters";
const static std::string seconds = "seconds";
const static std::string tags = "tags";
const static std::string temperature = "temperature";
const static std::string value = "value";
const static std::string x = "x";
const static std::string y = "y";
const static std::string z = "z";
const static std::array<const std::string, 5> kParamPointNames{
        x, y, z, a, b
};

}

}
}

#endif //DULCIFICUM_MIRACLE_GRUE_JSONTOOLPATH_JSON_KEYS_H
