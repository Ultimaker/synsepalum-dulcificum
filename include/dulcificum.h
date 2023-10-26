#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_H

#ifndef DULCIFICUM_VERSION
#define DULCIFICUM_VERSION "0.1.0"
#endif

#include <dulcificum/gcode/gcode_to_command.h>
#include <dulcificum/gcode/parse.h>
#include <dulcificum/miracle_jtp/mgjtp_command_to_json.h>

namespace dulcificum
{

[[nodiscard]] std::string GCode2Miracle_JTP(const std::string& content)
{
    auto gcode_ast = dulcificum::gcode::parse(content);
    auto command_list = dulcificum::gcode::toCommand(gcode_ast);
    auto json_commands = nlohmann::json::array();
    for (const auto& command : command_list)
    {
        json_commands.emplace_back(dulcificum::miracle_jtp::toJson(*command));
    }
    return json_commands.dump();
}


} // namespace dulcificum

#endif // DULCIFICUM_INCLUDE_DULCIFICUM_H
