#include <dulcificum.h>
#include <emscripten/bind.h>
#include <filesystem>
#include <string>

using namespace emscripten;

extern "C"
{
    const std::string gcode_2_miracle_jtp(const std::string& content)
    {
        return dulcificum::GCode2Miracle_JTP(content);
    }
}

EMSCRIPTEN_BINDINGS(dulcificum)
{
    function("gcode_2_miracle_jtp", &gcode_2_miracle_jtp);
}