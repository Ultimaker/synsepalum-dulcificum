#include "makerbot_dialect.h"

namespace dulcificum::miraclegrue {

    const static std::array<std::string, 5> kParamPointPrintName = {
            "x", "y", "z", "a", "b"
    };

    nlohmann::json jpackCommand(
            const std::string& function,
            const nlohmann::json& metadata = {},
            const nlohmann::json& parameters = {},
            const nlohmann::json& tags = {}
    ) {
        nlohmann::json jout;
        jout["command"] = {
                {"function" , function},
                {"metadata", metadata},
                {"parameters", parameters},
                {"tags", tags}
        };
        return jout;
    }
//
//    nlohmann::json getCommandMetadata(const CommandPtr& cmd) {
//        nlohmann::json jout;
//        if (cmd->type == CommandType::kMove) {
//            return getMoveMetadata(
//                    std::static_pointer_cast<Move>(cmd)
//                    );
//        }
//    }
//
//    nlohmann::json Comment::to_json() const {
//        nlohmann::json jmetadata;
//        jmetadata["comment"] = comment;
//        auto jout = jpackCommand(
//                "comment",
//                {},
//                jmetadata,
//                {}
//        );
//        return jout;
//    }
//
//
//    nlohmann::json Move::to_json() const {
//        nlohmann::json jparameters;
//        for (size_t foo=0; foo<6; foo++) {
//            jparameters[kParamPointPrintName[foo]] = point[static_cast<Eigen::Index>(foo)];
//        }
//        nlohmann::json jmetadata;
//        for (size_t foo=0; foo< is_point_relative.size(); foo++) {
//            jmetadata["relative"][kParamPointPrintName[foo]] = is_point_relative[foo];
//        }
//        nlohmann::json jtags = nlohmann::json(tags);
//        auto jout = packageMGCommand(
//                "move",
//                jmetadata,
//                jparameters,
//                jtags
//        );
//        return jout;
//    }
//
//
//    static nlohmann::json CommandParser::toJson(
//            const CommandPtr &cmd) {
//        return nlohmann::json();
//    }
//
//    Command CommandParser::toCommand(const nlohmann::json& jin) const {
//        return Command();
//    }

}
