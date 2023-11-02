#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_H

#include <nlohmann/json_fwd.hpp>
#ifndef DULCIFICUM_VERSION
#define DULCIFICUM_VERSION "0.1.0"
#endif

#include "sstream"

#include "range/v3/view/enumerate.hpp"

#include "dulcificum/gcode/gcode_to_command.h"
#include "dulcificum/gcode/parse.h"
#include "dulcificum/miracle_jtp/mgjtp_command_to_json.h"

namespace dulcificum
{

[[nodiscard]] std::string GCode2Miracle_JTP(const std::string& content)
{
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
