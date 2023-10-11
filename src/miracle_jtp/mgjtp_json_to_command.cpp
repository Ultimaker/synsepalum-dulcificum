#include "dulcificum/miracle_jtp/mgjtp_command_to_json.h"
#include "dulcificum/miracle_jtp/mgjtp_mappings_json_key_to_str.h"

namespace dulcificum::miracle_jtp
{
using namespace botcmd;

std::shared_ptr<Move> toMove(const nlohmann::json& jmove)
{
    auto move = std::make_shared<Move>();
    const auto& jparams = jmove.at(kKeyStr::parameters);
    move->feedrate = jparams.at(kKeyStr::feedrate);
    const auto& jrelative = jmove.at(kKeyStr::metadata).at("relative");
    for (size_t param_ii = 0; param_ii < kKeyStr::kParamPointNames.size(); param_ii++)
    {
        const auto& key = kKeyStr::kParamPointNames[param_ii];
        double pval = jparams.at(key);
        move->point[param_ii] = pval;
        bool is_rel = jrelative.at(key);
        move->is_point_relative[param_ii] = is_rel;
    }
    std::vector<Tag> tags = jmove.at(kKeyStr::tags);
    move->tags = std::move(tags);
    return move;
}

CommandPtr toParamOnlyCommand(const CommandType type, const nlohmann::json& jparam)
{
    if (type == CommandType::kComment)
    {
        auto com = std::make_shared<Comment>();
        com->comment = jparam.at(kKeyStr::comment);
        return com;
    }
    if (type == CommandType::kActiveFanDuty)
    {
        auto cmd = std::make_shared<FanDuty>();
        cmd->index = jparam[kKeyStr::index];
        cmd->duty = jparam[kKeyStr::value];
        return cmd;
    }
    if (type == CommandType::kActiveFanEnable)
    {
        auto cmd = std::make_shared<FanToggle>();
        cmd->index = jparam[kKeyStr::index];
        cmd->is_on = jparam[kKeyStr::value];
        return cmd;
    }
    if (type == CommandType::kSetTemperature)
    {
        auto cmd = std::make_shared<SetTemperature>();
        cmd->index = jparam[kKeyStr::index];
        cmd->temperature = jparam[kKeyStr::temperature];
        return cmd;
    }
    if (type == CommandType::kWaitForTemperature)
    {
        auto cmd = std::make_shared<WaitForTemperature>();
        cmd->index = jparam[kKeyStr::index];
        return cmd;
    }
    if (type == CommandType::kChangeTool)
    {
        auto cmd = std::make_shared<SetTemperature>();
        cmd->index = jparam[kKeyStr::index];
        cmd->temperature = jparam[kKeyStr::temperature];
        return cmd;
    }
    if (type == CommandType::kDelay)
    {
        auto cmd = std::make_shared<Delay>();
        cmd->seconds = jparam[kKeyStr::seconds];
        return cmd;
    }
    return spawnCommandPtr(type);
}

CommandPtr toChangeTool(const nlohmann::json& jcmd)
{
    auto cmd = std::make_shared<ChangeTool>();
    auto jparam = jcmd.at(kKeyStr::parameters);
    cmd->index = jparam.at(kKeyStr::index);
    for (size_t param_ii = 0; param_ii < kKeyStr::kParamPointNames.size(); param_ii++)
    {
        const auto& key = kKeyStr::kParamPointNames[param_ii];
        if (jparam.contains(key))
        {
            cmd->position[param_ii] = jparam.at(key);
        }
    }
    std::vector<Tag> tags = jcmd.at(kKeyStr::tags);
    cmd->tags = std::move(tags);
    return cmd;
}

CommandPtr toCommand(const nlohmann::json& jin)
{
    auto jcmd = jin[kKeyStr::command];
    CommandType type = jcmd[kKeyStr::function];
    if (type == CommandType::kMove)
    {
        return toMove(jcmd);
    }
    if (type == CommandType::kChangeTool)
    {
        return toChangeTool(jcmd);
    }
    if (jcmd.contains(kKeyStr::parameters))
    {
        auto jparam = jcmd[kKeyStr::parameters];
        auto cmd = toParamOnlyCommand(type, jparam);
        std::vector<Tag> tags = jcmd.at(kKeyStr::tags);
        cmd->tags = std::move(tags);
        return cmd;
    }
    auto cmd = spawnCommandPtr(type);
    return cmd;
}

} // namespace dulcificum::miracle_jtp
