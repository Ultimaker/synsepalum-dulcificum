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

#define MB_JTP_TAG(TAG_NAME, TAG_STR) \
    { Tag::TAG_NAME, #TAG_STR }

#define MB_BEAD_MODE_TAG_DEF(TAG_NAME, TAG_STR) \
    MB_JTP_TAG(TAG_NAME##_0, TAG_STR##_0), \
    MB_JTP_TAG(TAG_NAME##_1, TAG_STR##_1)

NLOHMANN_JSON_SERIALIZE_ENUM(
    Tag,
    {
      MB_JTP_TAG(Invalid, Invalid),
      MB_JTP_TAG(Infill, Infill),
      MB_JTP_TAG(Inset, Inset),
      MB_JTP_TAG(Purge, Purge),
      MB_JTP_TAG(QuickToggle, Quick Toggle),
      MB_JTP_TAG(Raft, Raft),
      MB_JTP_TAG(Restart, Restart),
      MB_JTP_TAG(Retract, Retract),
      MB_JTP_TAG(Roof, Roof),
      MB_JTP_TAG(Sparse, Sparse),
      MB_JTP_TAG(TravelMove, Travel Move),
      MB_BEAD_MODE_TAG_DEF(Fill, FILL),
      MB_BEAD_MODE_TAG_DEF(PrimeTower, PRIME_TOWER),
      MB_BEAD_MODE_TAG_DEF(TopSurface, TOP_SURFACE),
      MB_BEAD_MODE_TAG_DEF(Support, SUPPORT),
      MB_BEAD_MODE_TAG_DEF(SupportInterface, SUPPORT_INTERFACE),
      MB_BEAD_MODE_TAG_DEF(WallOuter, WALL_OUTER),
      MB_BEAD_MODE_TAG_DEF(WallInner, WALL_INNER),
      MB_BEAD_MODE_TAG_DEF(Skirt, SKIRT)
    })

#undef MB_BEAD_MODE_TAG
#undef MB_JTP_TAG

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
extern std::vector<std::string_view> k_param_point_names;

extern void init(const size_t nb_extruders);

} // namespace miracle_jtp::k_key_str


} // namespace dulcificum

#endif // INCLUDE_DULCIFICUM_MIRACLE_JTP_MGJTP_MAPPINGS_JSON_KEY_TO_STR_H
