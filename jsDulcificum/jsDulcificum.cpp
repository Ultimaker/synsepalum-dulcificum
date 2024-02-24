#include <dulcificum.h>
#include <emscripten/bind.h>
#include <filesystem>
#include <string>

using namespace emscripten;
using namespace dulcificum;

EMSCRIPTEN_BINDINGS(dulcificum)
{
    function("gcode_2_miracle_jtp", &GCode2Miracle_JTP);
}
