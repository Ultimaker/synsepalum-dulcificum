#include "dulcificum/miracle_jtp/mgjtp_command_stream.h"

namespace dulcificum::miracle_jtp {

    nlohmann::json dulcificum::miracle_jtp::CommandStream::getCommandJson() {
        const auto& line = fstream.getCommandLine();
        nlohmann::json cmd_json;
        if (!line.empty()) {
            cmd_json = nlohmann::json::parse(line);
        }
        return cmd_json;
    }

    botcmd::CommandPtr CommandStream::getCommand() {
        const auto& cmd_json = getCommandJson();
        return toCommand(getCommandJson());
    }

    botcmd::CommandList CommandStream::getCommandList() {
        botcmd::CommandList list;
        while (!fstream.eof()) {
            const auto& jcmd = getCommandJson();
            if (!jcmd.empty()) {
                list.emplace_back(toCommand(jcmd));
            }
        }
        return list;
    }


} // dulcificum::miracle_jtp

