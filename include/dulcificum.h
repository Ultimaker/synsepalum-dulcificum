#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_H

#include <nlohmann/json_fwd.hpp>
#include <string>
#ifndef DULCIFICUM_VERSION
#define DULCIFICUM_VERSION "0.1.0"
#endif

#include <fmt/ranges.h>
#include <range/v3/to_container.hpp>
#include <range/v3/view/c_str.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>

#include <dulcificum/gcode/gcode_to_command.h>
#include <dulcificum/gcode/parse.h>
#include <dulcificum/miracle_jtp/mgjtp_command_to_json.h>

namespace dulcificum
{

[[nodiscard]] std::string GCode2Miracle_JTP(const std::string& content)
{
    auto gcode_ast = dulcificum::gcode::parse(content);
    auto command_list = dulcificum::gcode::toCommand(gcode_ast);
    auto commands = command_list
                  | ranges::views::transform(
                        [](const auto& command)
                        {
                            return dulcificum::miracle_jtp::toJson(*command).dump();
                        })
                  | ranges::views::join(ranges::views::c_str(",\n")) | ranges::to<std::string>();
    return "[\n" + commands + "}\n]";
}


} // namespace dulcificum

#endif // DULCIFICUM_INCLUDE_DULCIFICUM_H
