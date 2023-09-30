#include "mgjtp_command_to_json.h"
#include "mgjtp_mappings_json_key_to_str.h"

namespace dulcificum::miracle_jtp {
    using namespace botcmd;

    template<typename KeyListType, typename ValListType>
    nlohmann::json
    zipListsToJson(const KeyListType &keys, const ValListType &vals) {
        nlohmann::json jout;
        for (size_t key_ii = 0; key_ii < keys.size(); key_ii++) {
            jout[keys[key_ii]] = vals[key_ii];
        }
        return jout;
    }

    template<typename KeyListType, typename ValListType>
    nlohmann::json
    zipListsIgnoreNan(const KeyListType &keys, const ValListType &vals) {
        nlohmann::json jout;
        for (size_t key_ii = 0; key_ii < keys.size(); key_ii++) {
            const auto &val = vals[key_ii];
            if (!std::isnan(val)) {
                jout[keys[key_ii]] = vals[key_ii];
            }
        }
        return jout;
    }


    nlohmann::json getCommandMetadata(const Command &cmd) {
        nlohmann::json jmetadata({});
        if (cmd.type == CommandType::kMove) {
            auto move = static_cast<const Move &>(cmd);
            jmetadata["relative"] = zipListsToJson(
                    kKeyStr::kParamPointNames,
                    move.is_point_relative
            );
            return jmetadata;
        }
        return jmetadata;
    }

    nlohmann::json getCommandParameters(const Command &cmd) {
        nlohmann::json jparams({});
        if (cmd.type == CommandType::kMove) {
            const auto move = static_cast<const Move &>(cmd);
            jparams = zipListsToJson(
                    kKeyStr::kParamPointNames,
                    move.point
            );
            jparams[kKeyStr::feedrate] = move.feedrate;
            return jparams;
        }
        if (cmd.type == CommandType::kComment) {
            const auto com = static_cast<const Comment &>(cmd);
            jparams[kKeyStr::comment] = com.comment;
            return jparams;
        }
        if (cmd.type == CommandType::kActiveFanDuty) {
            const auto dut = static_cast<const FanDuty &>(cmd);
            jparams[kKeyStr::index] = dut.index;
            jparams[kKeyStr::value] = dut.duty;
            return jparams;
        }
        if (cmd.type == CommandType::kActiveFanEnable) {
            const auto fan = static_cast<const FanToggle &>(cmd);
            jparams[kKeyStr::index] = fan.index;
            jparams[kKeyStr::value] = fan.is_on;
            return jparams;
        }
        if (cmd.type == CommandType::kSetTemperature) {
            const auto dcmd = static_cast<const SetTemperature &>(cmd);
            jparams[kKeyStr::index] = dcmd.index;
            jparams[kKeyStr::temperature] = dcmd.temperature;
            return jparams;
        }
        if (cmd.type == CommandType::kWaitForTemperature) {
            const auto dcmd = static_cast<const WaitForTemperature &>(cmd);
            jparams[kKeyStr::index] = dcmd.index;
            return jparams;
        }
        if (cmd.type == CommandType::kChangeTool) {
            const auto dcmd = static_cast<const ChangeTool &>(cmd);
            jparams = zipListsIgnoreNan(
                    kKeyStr::kParamPointNames,
                    dcmd.position
            );
            jparams[kKeyStr::index] = dcmd.index;
            return jparams;
        }
        if (cmd.type == CommandType::kDelay) {
            const auto dcmd = static_cast<const Delay &>(cmd);
            jparams[kKeyStr::seconds] = dcmd.seconds;
            return jparams;
        }
        return jparams;
    }

    nlohmann::json getCommandTags(const Command& cmd) {
        if (cmd.type == CommandType::kMove) {
            const auto &move = static_cast<const Move &>(cmd);
            return nlohmann::json(move.tags);
        }
        if (cmd.type == CommandType::kChangeTool) {
            const auto &changetool = static_cast<const ChangeTool &>(cmd);
            return nlohmann::json(changetool.tags);
        }
        nlohmann::json jtags = std::vector<Tag>();
        return jtags;
    }

    nlohmann::json toJson(const Command &cmd) {
        nlohmann::json jcmd;
        jcmd[kKeyStr::function] = cmd.type;
        jcmd[kKeyStr::metadata] = getCommandMetadata(cmd);
        jcmd[kKeyStr::parameters] = getCommandParameters(cmd);
        jcmd[kKeyStr::tags] = getCommandTags(cmd);
        nlohmann::json jout;
        jout[kKeyStr::command] = jcmd;
        return jout;
    }
}