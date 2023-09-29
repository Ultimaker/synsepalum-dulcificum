#include "miraclegrue_jsontoolpath_dialect.h"

#include <iostream>

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

        template<typename KeyType, typename ValType>
        KeyType inverseLookup(const std::unordered_map<KeyType, ValType> &map,
                              const ValType &val) {
            for (const auto &pair: map) {
                if (val == pair.second) {
                    return pair.first;
                }
            }
            throw std::invalid_argument("Key not in map");
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


        //    const static std::unordered_map<std::string, CommandType> kNameToCommandMap = {
        //            {"move", CommandType::kMove},
        //            {"fan_toggle", CommandType::kActiveFanEnable,},
        //            {"fan_duty", CommandType::kActiveFanDuty},
        //            {"temperature", CommandType::kSetTemperature},
        //            {"tool_change", CommandType::kChangeTool},
        //            {"comment", CommandType::kComment},
        //            {"delay", CommandType::kDelay},
        //            {"wait_for_temperature", CommandType::kWaitForTemperature},
        //            {"pause", CommandType::kPause}
        //    };

        //    const static std::unordered_map<std::string, Tag> kNameToTagMap = {
        //            {"Support", Tag::Support},
        //            {"Roof", Tag::Roof},
        //            {"Raft", Tag::Raft},
        //            {"Infill", Tag::Infill},
        //            {"Sparse", Tag::Sparse},
        //            {"Restart", Tag::Restart}
        //    };


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

        //    std::vector<std::string> nameTagList(const std::vector<Tag>& tags) {
        //        std::vector<std::string> names;
        //        std::transform(
        //                std::begin(tags),
        //                std::end(tags),
        //                std::back_inserter(names),
        //                [](const Tag& tag) -> std::string {
        //                    std::string name_of_tag = inverseLookup(kNameToTagMap, tag);
        //                    return name_of_tag;
        //                }
        //        );
        //        return names;
        //    }

        nlohmann::json toJson(const Command &cmd) {
            nlohmann::json jcmd;
            jcmd["function"] = to_string(cmd.type);
            jcmd["metadata"] = getCommandMetadata(cmd);
            jcmd["parameters"] = getCommandParameters(cmd);
            if (cmd.type == CommandType::kMove) {
                const auto move = static_cast<const Move &>(cmd);
                for (const auto &tag: move.tags) {
                    nlohmann::json jtag(tag);
                    std::cout << jtag << std::endl;
                    jcmd["tags"].push_back(jtag);
                }
            }
            nlohmann::json jout;
            jout["command"] = jcmd;
            return jout;
        }

        std::shared_ptr<Move> toMove(const nlohmann::json &jmove) {
            auto move = std::make_shared<Move>();
            const auto &jparams = jmove["parameters"];
            const auto &jmetadata = jmove["metadata"];
            for (size_t param_ii = 0;
                 param_ii < kParamPointPrintName.size(); param_ii++) {
                const auto &key = kParamPointPrintName[param_ii];

                move->point[param_ii] = jparams[key];
                move->is_point_relative[param_ii] = jmetadata[key];
            }
            move->tags = jmove["tags"].template get<std::vector<Tag>>();
            //        std::transform(
            //                std::begin(jmove["tags"]),
            //                std::end(jmove["tags"]),
            //                std::back_inserter(move->tags),
            //                [](const std::string& name) -> Tag {
            //                    return kNameToTagMap.find(name)->second;
            //                }
            //        );
        }


        CommandPtr toCommand(const nlohmann::json &jin) {
            auto jcmd = jin["command"];
            CommandType type = jcmd["function"];
            if (type == CommandType::kMove) return toMove(jcmd);
            auto cmd = spawnCommandPtr(type);
            return cmd;
        }

    }
}
