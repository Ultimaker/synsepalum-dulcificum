#include "dulcificum/miracle_jtp/mgjtp_command_to_json.h"
#include "dulcificum/miracle_jtp/mgjtp_mappings_json_key_to_str.h"

namespace dulcificum::miracle_jtp
{
using namespace botcmd;

std::shared_ptr<Move> toMove(const nlohmann::json& jmove)
{
    auto move = std::make_shared<Move>();
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
    std::vector<Tag> tags = jmove.at(k_key_str::tags);
    move->tags = std::move(tags);
    return move;
}

CommandPtr toParamOnlyCommand(const CommandType type, const nlohmann::json& jparam)
{
    if (type == CommandType::kComment)
    {
        auto com = std::make_shared<Comment>();
        com->comment = jparam.at(k_key_str::comment);
        return com;
    }
    if (type == CommandType::kActiveFanDuty)
    {
        auto cmd = std::make_shared<FanDuty>();
        cmd->index = jparam[k_key_str::index];
        cmd->duty = jparam[k_key_str::value];
        return cmd;
    }
    if (type == CommandType::kActiveFanEnable)
    {
        auto cmd = std::make_shared<FanToggle>();
        cmd->index = jparam[k_key_str::index];
        cmd->is_on = jparam[k_key_str::value];
        return cmd;
    }
    if (type == CommandType::kSetTemperature)
    {
        auto cmd = std::make_shared<SetTemperature>();
        cmd->index = jparam[k_key_str::index];
        cmd->temperature = jparam[k_key_str::temperature];
        return cmd;
    }
    if (type == CommandType::kWaitForTemperature)
    {
        auto cmd = std::make_shared<WaitForTemperature>();
        cmd->index = jparam[k_key_str::index];
        return cmd;
    }
    if (type == CommandType::kChangeTool)
    {
        auto cmd = std::make_shared<SetTemperature>();
        cmd->index = jparam[k_key_str::index];
        cmd->temperature = jparam[k_key_str::temperature];
        return cmd;
    }
    if (type == CommandType::kDelay)
    {
        auto cmd = std::make_shared<Delay>();
        cmd->seconds = jparam[k_key_str::seconds];
        return cmd;
    }
    return spawnCommandPtr(type);
}

CommandPtr toChangeTool(const nlohmann::json& jcmd)
{
    auto cmd = std::make_shared<ChangeTool>();
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
    std::vector<Tag> tags = jcmd.at(k_key_str::tags);
    cmd->tags = std::move(tags);
    return cmd;
}

CommandPtr toCommand(const nlohmann::json& jin)
{
    auto jcmd = jin[k_key_str::command];
    CommandType type = jcmd[k_key_str::function];
    if (type == CommandType::kMove)
    {
        return toMove(jcmd);
    }
    if (type == CommandType::kChangeTool)
    {
        return toChangeTool(jcmd);
    }
    if (jcmd.contains(k_key_str::parameters))
    {
        auto jparam = jcmd[k_key_str::parameters];
        auto cmd = toParamOnlyCommand(type, jparam);
        std::vector<Tag> tags = jcmd.at(k_key_str::tags);
        cmd->tags = std::move(tags);
        return cmd;
    }
    auto cmd = spawnCommandPtr(type);
    return cmd;
}

} // namespace dulcificum::miracle_jtp
