#include "dulcificum/miracle_jtp/mgjtp_command_to_json.h"

#include "dulcificum/miracle_jtp/mgjtp_mappings_json_key_to_str.h"

namespace dulcificum::miracle_jtp
{
using namespace botcmd;

template<typename KeyListType, typename ValListType>
nlohmann::json zipListsToJson(const KeyListType& keys, const ValListType& vals)
{
    nlohmann::json jout;
    for (size_t key_ii = 0; key_ii < keys.size(); key_ii++)
    {
        jout[keys[key_ii]] = vals[key_ii];
    }
    return jout;
}

template<typename KeyListType, typename ValListType>
nlohmann::json zipListsIgnoreNan(const KeyListType& keys, const ValListType& vals)
{
    nlohmann::json jout;
    for (size_t key_ii = 0; key_ii < keys.size(); key_ii++)
    {
        const auto& val = vals[key_ii];
        if (! std::isnan(val))
        {
            jout[keys[key_ii]] = vals[key_ii];
        }
    }
    return jout;
}


nlohmann::json getCommandMetadata(const Command& cmd)
{
    nlohmann::json jmetadata({});
    if (cmd.type == CommandType::kMove)
    {
        auto move = static_cast<const Move&>(cmd);
        jmetadata["relative"] = zipListsToJson(k_key_str::k_param_point_names, move.is_point_relative);
        return jmetadata;
    }
    return jmetadata;
}

nlohmann::json getCommandParameters(const Command& cmd)
{
    nlohmann::json jparams({});
    if (cmd.type == CommandType::kMove)
    {
        const auto move = static_cast<const Move&>(cmd);
        jparams = zipListsToJson(k_key_str::k_param_point_names, move.point);
        jparams[k_key_str::feedrate] = move.feedrate;
        return jparams;
    }
    if (cmd.type == CommandType::kComment)
    {
        const auto com = static_cast<const Comment&>(cmd);
        jparams[k_key_str::comment] = com.comment;
        return jparams;
    }
    if (cmd.type == CommandType::kActiveFanDuty)
    {
        const auto dut = static_cast<const FanDuty&>(cmd);
        jparams[k_key_str::index] = dut.index;
        jparams[k_key_str::value] = dut.duty;
        return jparams;
    }
    if (cmd.type == CommandType::kActiveFanEnable)
    {
        const auto fan = static_cast<const FanToggle&>(cmd);
        jparams[k_key_str::index] = fan.index;
        jparams[k_key_str::value] = fan.is_on;
        return jparams;
    }
    if (cmd.type == CommandType::kSetTemperature)
    {
        const auto dcmd = static_cast<const SetTemperature&>(cmd);
        jparams[k_key_str::index] = dcmd.index;
        jparams[k_key_str::temperature] = dcmd.temperature;
        return jparams;
    }
    if (cmd.type == CommandType::kWaitForTemperature)
    {
        const auto dcmd = static_cast<const WaitForTemperature&>(cmd);
        jparams[k_key_str::index] = dcmd.index;
        return jparams;
    }
    if (cmd.type == CommandType::kChangeTool)
    {
        const auto dcmd = static_cast<const ChangeTool&>(cmd);
        jparams = zipListsIgnoreNan(k_key_str::k_param_point_names, dcmd.position);
        jparams[k_key_str::index] = dcmd.index;
        return jparams;
    }
    if (cmd.type == CommandType::kDelay)
    {
        const auto dcmd = static_cast<const Delay&>(cmd);
        jparams[k_key_str::seconds] = dcmd.seconds;
        return jparams;
    }
    return jparams;
}

nlohmann::json toJson(const Command& cmd)
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