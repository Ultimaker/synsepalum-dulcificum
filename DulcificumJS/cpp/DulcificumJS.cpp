#include <dulcificum.h>
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(dulcificum)
{
    emscripten::function("gcode_2_miracle_jtp", &dulcificum::GCode2Miracle_JTP);
}