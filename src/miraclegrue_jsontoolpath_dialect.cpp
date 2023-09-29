#include "miraclegrue_jsontoolpath_dialect.h"

namespace dulcificum {

    namespace botcmd {
        NLOHMANN_JSON_SERIALIZE_ENUM(CommandType, {
            { CommandType::kMove, "move" },
            { CommandType::kActiveFanEnable, "fan_toggle" },
            { CommandType::kActiveFanDuty, "fan_duty" },
            { CommandType::kSetTemperature, "temperature" },
            { CommandType::kChangeTool, "tool_change" },
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
            { Tag::Restart, "Restart" }
        })
    }

    namespace miracle_jtp {

        template<typename KeyListType, typename ValListType>
        nlohmann::json
        zipListsToJson(const KeyListType &keys, const ValListType &vals) {
            nlohmann::json jout;
            for (size_t key_ii = 0; key_ii < keys.size(); key_ii++) {
                jout[keys[key_ii]] = vals[key_ii];
            }
            return jout;
        }

        const static std::array<std::string, 5> kParamPointPrintName = {
                "x", "y", "z", "a", "b"
        };


        nlohmann::json getCommandMetadata(const Command &cmd) {
            nlohmann::json jmetadata;
            if (cmd.type == CommandType::kMove) {
                auto move = static_cast<const Move &>(cmd);
                jmetadata["relative"] = zipListsToJson(
                        kParamPointPrintName,
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
                        kParamPointPrintName,
                        move.point
                );
                jparams["feedrate"] = move.feedrate;
            }
            return jparams;
        }

        std::string to_string(const CommandType &type) {
            switch (type) {
                case CommandType::kMove:
                    return "move";
                default:
                    return "";
            }
        }

        nlohmann::json toJson(const Command &cmd) {
            nlohmann::json jcmd;
            jcmd["function"] = to_string(cmd.type);
            jcmd["metadata"] = getCommandMetadata(cmd);
            jcmd["parameters"] = getCommandParameters(cmd);
            if (cmd.type == CommandType::kMove) {
                const auto move = static_cast<const Move &>(cmd);
                jcmd["tags"] = nlohmann::json (move.tags);
            }
            nlohmann::json jout;
            jout["command"] = jcmd;
            return jout;
        }

        std::shared_ptr<Move> toMove(const nlohmann::json &jmove) {
            auto move = std::make_shared<Move>();
            const auto &jparams = jmove.at("parameters");
            move->feedrate = jparams.at("feedrate");
            const auto &jrelative = jmove.at("metadata").at("relative");
            for (size_t param_ii = 0;
                 param_ii < kParamPointPrintName.size(); param_ii++) {
                const auto &key = kParamPointPrintName[param_ii];
                double pval = jparams.at(key);
                move->point[param_ii] = pval;
                bool is_rel = jrelative.at(key);
                move->is_point_relative[param_ii] = is_rel;
            }
            std::vector<Tag> tags = jmove.at("tags");
            move->tags = std::move(tags);
            return move;
        }


        CommandPtr toCommand(const nlohmann::json &jin) {
            auto jcmd = jin["command"];
            CommandType type = jcmd["function"];
            if (type == CommandType::kMove) {
                return toMove(jcmd);
            }
            auto cmd = spawnCommandPtr(type);
            return cmd;
        }

    }
}
