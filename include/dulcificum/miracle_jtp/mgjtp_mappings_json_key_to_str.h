#ifndef DULCIFICUM_MIRACLE_GRUE_JSONTOOLPATH_JSON_KEYS_H
#define DULCIFICUM_MIRACLE_GRUE_JSONTOOLPATH_JSON_KEYS_H

#include "dulcificum/command_types.h"

#include <nlohmann/json.hpp>
#include <string_view>

namespace dulcificum
{

namespace botcmd
{

NLOHMANN_JSON_SERIALIZE_ENUM(
    CommandType,
    { { CommandType::kInvalid, "invalid" },
      { CommandType::kMove, "move" },
      { CommandType::kActiveFanEnable, "toggle_fan" },
      { CommandType::kActiveFanDuty, "fan_duty" },
      { CommandType::kSetTemperature, "set_toolhead_temperature" },
      { CommandType::kChangeTool, "change_toolhead" },
      { CommandType::kComment, "comment" },
      { CommandType::kDelay, "delay" },
      { CommandType::kWaitForTemperature, "wait_for_temperature" },
      { CommandType::kPause, "pause" } })

NLOHMANN_JSON_SERIALIZE_ENUM(
    Tag,
    { { Tag::Invalid, "Invalid" },
      { Tag::Infill, "Infill" },
      { Tag::Inset, "Inset" },
      { Tag::Purge, "Purge" },
      { Tag::QuickToggle, "Quick Toggle" },
      { Tag::Raft, "Raft" },
      { Tag::Restart, "Restart" },
      { Tag::Roof, "Roof" },
      { Tag::Sparse, "Sparse" },
      { Tag::Support, "Support" },
      { Tag::TravelMove, "Travel Move" } })

} // namespace botcmd

namespace miracle_jtp::kKeyStr
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
constexpr static std::array<const std::string_view, 5> kParamPointNames{ x, y, z, a, b };

} // namespace miracle_jtp::kKeyStr


} // namespace dulcificum

#endif // DULCIFICUM_MIRACLE_GRUE_JSONTOOLPATH_JSON_KEYS_H
