#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_H

#include <nlohmann/json_fwd.hpp>
#ifndef DULCIFICUM_VERSION
#define DULCIFICUM_VERSION "0.3.0"
#endif

#include "dulcificum/gcode/gcode_to_command.h"
#include "dulcificum/gcode/parse.h"
#include "dulcificum/miracle_jtp/mgjtp_command_to_json.h"
#include "dulcificum/miracle_jtp/mgjtp_mappings_json_key_to_str.h"
#include "range/v3/view/enumerate.hpp"
#include "sstream"

namespace dulcificum
{

[[nodiscard]] std::string GCode2Miracle_JTP(const std::string& content, const size_t nb_extruders = 2)
{
    miracle_jtp::k_key_str::init(nb_extruders);

    auto gcode_ast = dulcificum::gcode::parse(content);
    auto command_list = dulcificum::gcode::toCommand(gcode_ast);
    std::stringstream stream;

    stream << "[\n";

    for (auto [command_id, command] : command_list | ranges::view::enumerate)
    {
        stream << dulcificum::miracle_jtp::toJson(*command).dump();
        if (command_id < command_list.size() - 1)
        {
            stream << ",\n";
        }
    }

    stream << "\n]";

    return stream.str();
}


} // namespace dulcificum

#endif // DULCIFICUM_INCLUDE_DULCIFICUM_H
