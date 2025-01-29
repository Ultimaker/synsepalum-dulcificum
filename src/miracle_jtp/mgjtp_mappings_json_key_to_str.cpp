#include "dulcificum/miracle_jtp/mgjtp_mappings_json_key_to_str.h"

#include <spdlog/spdlog.h>

namespace dulcificum::miracle_jtp
{

namespace k_key_str
{

std::vector<std::string_view> k_param_point_names = {};

void init(const size_t nb_extruders)
{
    spdlog::info("Initializing with {} extruder(s)", nb_extruders);

    k_param_point_names = { x, y, z };

    if (nb_extruders >= 1)
    {
        k_param_point_names.push_back(a);
    }
    if (nb_extruders >= 2)
    {
        k_param_point_names.push_back(b);
    }
    if (nb_extruders >= 3)
    {
        spdlog::error("Using more than 2 extruders is not supported, using 2 instead");
    }
}

} // namespace k_key_str

} // namespace dulcificum::miracle_jtp
