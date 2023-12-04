#ifndef INCLUDE_DULCIFICUM_GCODE_AST_AST_H
#define INCLUDE_DULCIFICUM_GCODE_AST_AST_H

#include "dulcificum/gcode/ast/acceleration.h"
#include "dulcificum/gcode/ast/bed_temperature.h"
#include "dulcificum/gcode/ast/comment_commands.h"
#include "dulcificum/gcode/ast/delay.h"
#include "dulcificum/gcode/ast/entry.h"
#include "dulcificum/gcode/ast/extruder_temperature.h"
#include "dulcificum/gcode/ast/fan.h"
#include "dulcificum/gcode/ast/position.h"
#include "dulcificum/gcode/ast/positioning_mode.h"
#include "dulcificum/gcode/ast/purge.h"
#include "dulcificum/gcode/ast/toolchange.h"
#include "dulcificum/gcode/ast/translate.h"

#include <cstddef>
#include <string>
#include <variant>
#include <vector>

namespace dulcificum::gcode::ast
{

using Unknown = Entry<R"(^(.*)$)">;

using node_t = std::variant<
    G0_G1,
    G4,
    G90,
    G91,
    G92,
    G280,
    M82,
    M83,
    M104,
    M106,
    M107,
    M109,
    M140,
    M190,
    M204,
    M205,
    Layer,
    Mesh,
    FeatureType,
    InitialTemperatureExtruder,
    InitialTemperatureBuildPlate,
    BuildVolumeTemperature,
    Comment,
    T,
    Unknown>;
using ast_t = std::vector<node_t>;

node_t factory(size_t line_index, const std::string& raw_line);

} // namespace dulcificum::gcode::ast

#endif // INCLUDE_DULCIFICUM_GCODE_AST_AST_H
