#ifndef INCLUDE_DULCIFICUM_STATE_H
#define INCLUDE_DULCIFICUM_STATE_H

#include <cstddef>
#include <map>
#include <memory>
#include <string_view>
#include <variant>
#include <vector>
#include <set>

namespace dulcificum
{

enum class Positioning
{
    Absolute,
    Relative,
};

struct State
{
    unsigned int num_extruders{ 2 };
    unsigned int active_tool{ 0 };

    unsigned int layer{ 0 };
    std::string_view mesh{ "" };
    std::string_view feature_type{ "" };

    // needed for g92 commands, which set the current position
    // of the extruder, and thereby changing the origin
    double origin_x{ 0.0 };
    double origin_y{ 0.0 };
    double origin_z{ 0.0 };

    double X{ 0.0 };
    Positioning X_positioning{ Positioning::Absolute };

    double Y{ 0.0 };
    Positioning Y_positioning{ Positioning::Absolute };

    double Z{ 0.0 };
    Positioning Z_positioning{ Positioning::Absolute };

    std::vector<double> E = std::vector<double>(num_extruders, 0.0);
    Positioning E_positioning{ Positioning::Absolute };

    std::vector<double> F = std::vector<double>(num_extruders, 0.0);

    std::vector<double> extruder_temperatures = std::vector<double>(num_extruders, 0.0);

    double fan_speed { 0.0 };

    double build_plate_temperature{ 0.0 };

    double chamber_temperature{ 0.0 };
};

using state_t = State;

} // namespace dulcificum

#endif // INCLUDE_DULCIFICUM_STATE_H
