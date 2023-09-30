#include "miraclegrue_jsontoolpath_dialect.h"

namespace dulcificum {

    namespace botcmd {
        NLOHMANN_JSON_SERIALIZE_ENUM(CommandType, {
            { CommandType::kMove, "move" },
            { CommandType::kActiveFanEnable, "fan_toggle" },
            { CommandType::kActiveFanDuty, "fan_duty" },
            { CommandType::kSetTemperature, "set_toolhead_temperature" },
            { CommandType::kChangeTool, "change_toolhead" },
            { CommandType::kComment, "comment" },
            { CommandType::kDelay, "delay" },
            { CommandType::kWaitForTemperature, "wait_for_temperature" },
            { CommandType::kPause, "pause" }
        })

        NLOHMANN_JSON_SERIALIZE_ENUM(Tag, {
            { Tag::Support, "Support" },
            { Tag::Roof, "Roof" },
            { Tag::Raft, "Raft" },
            { Tag::Infill, "Infill" },
            { Tag::Sparse, "Sparse" },
            { Tag::Restart, "Restart" },
            { Tag::QuickToggle, "Quick Toggle" },
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
            const static std::array<const std::string, 5> kParamPointNames {
                x,y,z,a,b
            };
        }


        template<typename KeyListType, typename ValListType>
        nlohmann::json
        zipListsToJson(const KeyListType &keys, const ValListType &vals) {
            nlohmann::json jout;
            for (size_t key_ii = 0; key_ii < keys.size(); key_ii++) {
                jout[keys[key_ii]] = vals[key_ii];
            }
            return jout;
        }

        template<typename KeyListType, typename ValListType, typename ValType>
        nlohmann::json
        zipListsExceptVal(const KeyListType &keys, const ValListType &vals,
                          const ValType &ignore_val) {
            nlohmann::json jout;
            for (size_t key_ii = 0; key_ii < keys.size(); key_ii++) {
                const auto &val = vals[key_ii];
                if (val != ignore_val) jout[keys[key_ii]] = vals[key_ii];
            }
            return jout;
        }


        nlohmann::json getCommandMetadata(const Command &cmd) {
            nlohmann::json jmetadata;
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
            nlohmann::json jparams;
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
                jparams = zipListsExceptVal(
                        kKeyStr::kParamPointNames,
                        dcmd.position,
                        NAN
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

        nlohmann::json toJson(const Command &cmd) {
            nlohmann::json jcmd;
            jcmd[kKeyStr::function] = cmd.type;
            jcmd[kKeyStr::metadata] = getCommandMetadata(cmd);
            jcmd[kKeyStr::parameters] = getCommandParameters(cmd);
            if (cmd.type == CommandType::kMove) {
                const auto move = static_cast<const Move &>(cmd);
                jcmd[kKeyStr::tags] = nlohmann::json(move.tags);
            }
            nlohmann::json jout;
            jout[kKeyStr::command] = jcmd;
            return jout;
        }

        // ################################ //
        // From Json to Command             //
        // ################################ //

        std::shared_ptr<Move> toMove(const nlohmann::json &jmove) {
            auto move = std::make_shared<Move>();
            const auto &jparams = jmove.at(kKeyStr::parameters);
            move->feedrate = jparams.at(kKeyStr::feedrate);
            const auto &jrelative = jmove.at(kKeyStr::metadata).at("relative");
            for (size_t param_ii = 0;
                 param_ii < kKeyStr::kParamPointNames.size(); param_ii++) {
                const auto &key = kKeyStr::kParamPointNames[param_ii];
                double pval = jparams.at(key);
                move->point[param_ii] = pval;
                bool is_rel = jrelative.at(key);
                move->is_point_relative[param_ii] = is_rel;
            }
            std::vector<Tag> tags = jmove.at(kKeyStr::tags);
            move->tags = std::move(tags);
            return move;
        }

        CommandPtr toParamOnlyCommand(const CommandType type, const nlohmann::json &jparam) {
            if (type == CommandType::kComment) {
                auto com = std::make_shared<Comment>();
                com->comment = jparam.at(kKeyStr::comment);
                return com;
            }
            if (type == CommandType::kActiveFanDuty) {
                auto cmd = std::make_shared<FanDuty>();
                cmd->index=jparam[kKeyStr::index];
                cmd->duty=jparam[kKeyStr::value];
                return cmd;
            }
            if (type == CommandType::kActiveFanEnable) {
                auto cmd = std::make_shared<FanToggle>();
                cmd->index=jparam[kKeyStr::index];
                cmd->is_on=jparam[kKeyStr::value];
                return cmd;
            }
            if (type == CommandType::kSetTemperature) {
                auto cmd = std::make_shared<SetTemperature>();
                cmd->index=jparam[kKeyStr::index];
                cmd->temperature=jparam[kKeyStr::temperature];
                return cmd;
            }
            if (type == CommandType::kWaitForTemperature) {
                auto cmd = std::make_shared<WaitForTemperature>();
                cmd->index=jparam[kKeyStr::index];
                return cmd;
            }
            if (type == CommandType::kChangeTool) {
                auto cmd = std::make_shared<SetTemperature>();
                cmd->index=jparam[kKeyStr::index];
                cmd->temperature=jparam[kKeyStr::temperature];
                return cmd;
            }
            return spawnCommandPtr(type);
        }

        CommandPtr toChangeTool(const nlohmann::json& jcmd) {
            auto cmd = std::make_shared<ChangeTool>();
            auto jparam = jcmd.at(kKeyStr::parameters);
            cmd->index = jparam.at(kKeyStr::index);
            for (size_t param_ii=0; param_ii<kKeyStr::kParamPointNames.size(); param_ii++) {
                const auto& key = kKeyStr::kParamPointNames[param_ii];
                if (jparam.contains(key)) {
                    cmd->position[param_ii] = jparam.at(key);
                }
            }
            std::vector<Tag> tags = jcmd.at(kKeyStr::tags);
            cmd->tags = std::move(tags);
            return cmd;
        }

        CommandPtr toCommand(const nlohmann::json &jin) {
            auto jcmd = jin[kKeyStr::command];
            CommandType type = jcmd[kKeyStr::function];
            if (type == CommandType::kMove) {
                return toMove(jcmd);
            }
            if (type == CommandType::kChangeTool) {
                return toChangeTool(jcmd);
            }
            if (jcmd.contains(kKeyStr::parameters)) {
                auto jparam = jcmd[kKeyStr::parameters];
                return toParamOnlyCommand(type, jparam);
            }
            auto cmd = spawnCommandPtr(type);
            return cmd;
        }

    }
}
