#include "dulcificum/miracle_jtp/mgjtp_command_to_json.h"
#include "dulcificum/miracle_jtp/mgjtp_mappings_json_key_to_str.h"

#include <range/v3/view/zip.hpp>

namespace dulcificum::miracle_jtp
{

std::shared_ptr<botcmd::Move> toMove(const nlohmann::json& jmove)
{
    auto move = std::make_shared<botcmd::Move>();
    const auto& jparams = jmove.at(k_key_str::parameters);
    move->feedrate = jparams.at(k_key_str::feedrate);
    const auto& jrelative = jmove.at(k_key_str::metadata).at("relative");
    for (size_t param_ii = 0; param_ii < k_key_str::k_param_point_names.size(); param_ii++)
    {
        const auto& key = k_key_str::k_param_point_names[param_ii];
        double pval = jparams.at(key);
        move->point[param_ii] = pval;
        bool is_rel = jrelative.at(key);
        move->is_point_relative[param_ii] = is_rel;
    }
    std::vector<botcmd::Tag> tags = jmove.at(k_key_str::tags);
    move->tags = std::move(tags);
    return move;
}

botcmd::CommandPtr toParamOnlyCommand(const botcmd::CommandType type, const nlohmann::json& jparam)
{
    if (type == botcmd::CommandType::Comment)
    {
        auto com = std::make_shared<botcmd::Comment>();
        com->comment = jparam.at(k_key_str::comment);
        return com;
    }
    if (type == botcmd::CommandType::ActiveFanDuty)
    {
        auto cmd = std::make_shared<botcmd::FanDuty>();
        cmd->index = jparam[k_key_str::index];
        cmd->duty = jparam[k_key_str::value];
        return cmd;
    }
    if (type == botcmd::CommandType::ActiveFanEnable)
    {
        auto cmd = std::make_shared<botcmd::FanToggle>();
        cmd->index = jparam[k_key_str::index];
        cmd->is_on = jparam[k_key_str::value];
        return cmd;
    }
    if (type == botcmd::CommandType::SetTemperature)
    {
        auto cmd = std::make_shared<botcmd::SetTemperature>();
        cmd->index = jparam[k_key_str::index];
        cmd->temperature = jparam[k_key_str::temperature];
        return cmd;
    }
    if (type == botcmd::CommandType::WaitForTemperature)
    {
        auto cmd = std::make_shared<botcmd::WaitForTemperature>();
        cmd->index = jparam[k_key_str::index];
        return cmd;
    }
    if (type == botcmd::CommandType::ChangeTool)
    {
        auto cmd = std::make_shared<botcmd::SetTemperature>();
        cmd->index = jparam[k_key_str::index];
        cmd->temperature = jparam[k_key_str::temperature];
        return cmd;
    }
    if (type == botcmd::CommandType::Delay)
    {
        auto cmd = std::make_shared<botcmd::Delay>();
        cmd->seconds = jparam[k_key_str::seconds];
        return cmd;
    }
    return spawnCommandPtr(type);
}

botcmd::CommandPtr toChangeTool(const nlohmann::json& jcmd)
{
    auto cmd = std::make_shared<botcmd::ChangeTool>();
    auto jparam = jcmd.at(k_key_str::parameters);
    cmd->index = jparam.at(k_key_str::index);
    for (size_t param_ii = 0; param_ii < k_key_str::k_param_point_names.size(); param_ii++)
    {
        const auto& key = k_key_str::k_param_point_names[param_ii];
        if (jparam.contains(key))
        {
            cmd->position[param_ii] = jparam.at(key);
        }
    }
    std::vector<botcmd::Tag> tags = jcmd.at(k_key_str::tags);
    cmd->tags = std::move(tags);
    return cmd;
}

botcmd::CommandPtr toCommand(const nlohmann::json& jin)
{
    if (jin.empty()) return botcmd::spawnCommandPtr(botcmd::CommandType::Invalid);
    auto jcmd = jin[k_key_str::command];
    botcmd::CommandType type = jcmd[k_key_str::function];
    if (type == botcmd::CommandType::Move)
    {
        return toMove(jcmd);
    }
    if (type == botcmd::CommandType::ChangeTool)
    {
        return toChangeTool(jcmd);
    }
    if (jcmd.contains(k_key_str::parameters))
    {
        auto jparam = jcmd[k_key_str::parameters];
        auto cmd = toParamOnlyCommand(type, jparam);
        std::vector<botcmd::Tag> tags = jcmd.at(k_key_str::tags);
        cmd->tags = std::move(tags);
        return cmd;
    }
    auto cmd = spawnCommandPtr(type);
    return cmd;
}

} // namespace dulcificum::miracle_jtp
