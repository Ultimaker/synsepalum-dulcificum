#ifndef INCLUDE_DULCIFICUM_MIRACLE_JTP_MGJTP_MAPPINGS_JSON_KEY_TO_STR_H
#define INCLUDE_DULCIFICUM_MIRACLE_JTP_MGJTP_MAPPINGS_JSON_KEY_TO_STR_H

#include "dulcificum/miracle_jtp/mgjtp_command_to_json.h"

#include <nlohmann/json.hpp>
#include <string_view>

namespace dulcificum
{

namespace botcmd
{

NLOHMANN_JSON_SERIALIZE_ENUM(
    CommandType,
    { { CommandType::Invalid, "invalid" },
      { CommandType::Move, "move" },
      { CommandType::ActiveFanEnable, "toggle_fan" },
      { CommandType::ActiveFanDuty, "fan_duty" },
      { CommandType::SetTemperature, "set_toolhead_temperature" },
      { CommandType::ChangeTool, "change_toolhead" },
      { CommandType::Comment, "comment" },
      { CommandType::LayerChange, "comment" },
      { CommandType::Delay, "delay" },
      { CommandType::WaitForTemperature, "wait_for_temperature" },
      { CommandType::Pause, "pause" } })

NLOHMANN_JSON_SERIALIZE_ENUM(
    Tag,
    { { Tag::Invalid, "Invalid" },
      { Tag::Infill, "Infill" },
      { Tag::Inset, "Inset" },
      { Tag::Purge, "Purge" },
      { Tag::QuickToggle, "Quick Toggle" },
      { Tag::Raft, "Raft" },
      { Tag::Restart, "Restart" },
      { Tag::Retract, "Retract" },
      { Tag::Roof, "Roof" },
      { Tag::Sparse, "Sparse" },
      { Tag::Support, "Support" },
      { Tag::TravelMove, "Travel Move" } })

} // namespace botcmd

namespace miracle_jtp::k_key_str
{

constexpr static std::string_view a{ "a" };
constexpr static std::string_view b{ "b" };
constexpr static std::string_view command{ "command" };
constexpr static std::string_view comment{ "comment" };
constexpr static std::string_view feedrate{ "feedrate" };
constexpr static std::string_view function{ "function" };
constexpr static std::string_view index{ "index" };
constexpr static std::string_view metadata{ "metadata" };
constexpr static std::string_view parameters{ "parameters" };
constexpr static std::string_view seconds{ "seconds" };
constexpr static std::string_view tags{ "tags" };
constexpr static std::string_view temperature{ "temperature" };
constexpr static std::string_view value{ "value" };
constexpr static std::string_view x{ "x" };
constexpr static std::string_view y{ "y" };
constexpr static std::string_view z{ "z" };
constexpr static std::array<const std::string_view, 5> k_param_point_names{ x, y, z, a, b };

} // namespace miracle_jtp::k_key_str


} // namespace dulcificum

#endif // INCLUDE_DULCIFICUM_MIRACLE_JTP_MGJTP_MAPPINGS_JSON_KEY_TO_STR_H
