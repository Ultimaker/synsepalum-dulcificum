#include "dulcificum/miracle_jtp/mgjtp_command_to_json.h"

#include "dulcificum/miracle_jtp/mgjtp_mappings_json_key_to_str.h"

#include <range/v3/view/remove_if.hpp>
#include <range/v3/view/zip.hpp>

#include <spdlog/spdlog.h>

namespace dulcificum::miracle_jtp
{

template<typename KeyListType, typename ValListType>
nlohmann::json zipListsToJson(const KeyListType& keys, const ValListType& vals)
{
    nlohmann::json jout;
    for (auto const& item : ranges::views::zip(keys, vals))
    {
        jout[std::get<0>(item)] = std::get<1>(item);
    }
    return jout;
}

template<typename KeyListType, typename ValListType>
nlohmann::json zipListsIgnoreNan(const KeyListType& keys, const ValListType& vals)
{
    nlohmann::json jout;
    auto filter_key_value_view = ranges::views::zip(keys, vals)
                               | ranges::views::remove_if(
                                     [](const auto& key_value)
                                     {
                                         return std::isnan(std::get<1>(key_value));
                                     });
    for (auto const& item : filter_key_value_view)
    {
        jout[std::get<0>(item)] = std::get<1>(item);
    }
    return jout;
}

nlohmann::json getCommandMetadata(const botcmd::Command& cmd)
{
    nlohmann::json jmetadata({});
    if (cmd.type == botcmd::CommandType::kMove)
    {
        auto move = static_cast<const botcmd::Move&>(cmd);
        jmetadata["relative"] = zipListsToJson(k_key_str::k_param_point_names, move.is_point_relative);
        return jmetadata;
    }
    return jmetadata;
}

nlohmann::json getCommandParameters(const botcmd::Command& cmd)
{
    nlohmann::json jparams({});
    if (cmd.type == botcmd::CommandType::kMove)
    {
        const auto move = static_cast<const botcmd::Move&>(cmd);
        jparams = zipListsToJson(k_key_str::k_param_point_names, move.point);
        jparams[k_key_str::feedrate] = move.feedrate;
        return jparams;
    }
    if (cmd.type == botcmd::CommandType::kComment)
    {
        const auto com = static_cast<const botcmd::Comment&>(cmd);
        jparams[k_key_str::comment] = com.comment;
        return jparams;
    }
    if (cmd.type == botcmd::CommandType::kActiveFanDuty)
    {
        const auto dut = static_cast<const botcmd::FanDuty&>(cmd);
        jparams[k_key_str::index] = dut.index;
        jparams[k_key_str::value] = dut.duty;
        return jparams;
    }
    if (cmd.type == botcmd::CommandType::kActiveFanEnable)
    {
        const auto fan = static_cast<const botcmd::FanToggle&>(cmd);
        jparams[k_key_str::index] = fan.index;
        jparams[k_key_str::value] = fan.is_on;
        return jparams;
    }
    if (cmd.type == botcmd::CommandType::kSetTemperature)
    {
        const auto dcmd = static_cast<const botcmd::SetTemperature&>(cmd);
        jparams[k_key_str::index] = dcmd.index;
        // Methods require temperature commands to be expressed in whole degrees
        jparams[k_key_str::temperature] = static_cast<size_t>(dcmd.temperature);
        return jparams;
    }
    if (cmd.type == botcmd::CommandType::kWaitForTemperature)
    {
        const auto dcmd = static_cast<const botcmd::WaitForTemperature&>(cmd);
        jparams[k_key_str::index] = dcmd.index;
        return jparams;
    }
    if (cmd.type == botcmd::CommandType::kChangeTool)
    {
        const auto dcmd = static_cast<const botcmd::ChangeTool&>(cmd);
        jparams = zipListsIgnoreNan(k_key_str::k_param_point_names, dcmd.position);
        jparams[k_key_str::index] = dcmd.index;
        return jparams;
    }
    if (cmd.type == botcmd::CommandType::kDelay)
    {
        const auto dcmd = static_cast<const botcmd::Delay&>(cmd);
        jparams[k_key_str::seconds] = dcmd.seconds;
        return jparams;
    }
    if (cmd.type == botcmd::CommandType::kLayerChange)
    {
        const auto dcmd = static_cast<const botcmd::LayerChange&>(cmd);
        auto comment = botcmd::Comment{};
        // TODO what data should be placed in the parenthesis?
        comment.comment = "Layer Section " + std::to_string(dcmd.layer) + " (0)";
        const auto com = static_cast<const botcmd::Comment&>(comment);
        jparams[k_key_str::comment] = com.comment;
        return jparams;
    }
    return jparams;
}

nlohmann::json toJson(const botcmd::Command& cmd)
{
    nlohmann::json jcmd;
    jcmd[k_key_str::function] = cmd.type;
    jcmd[k_key_str::metadata] = getCommandMetadata(cmd);
    jcmd[k_key_str::parameters] = getCommandParameters(cmd);
    jcmd[k_key_str::tags] = cmd.tags;
    nlohmann::json jout;
    jout[k_key_str::command] = jcmd;
    return jout;
}
} // namespace dulcificum::miracle_jtp