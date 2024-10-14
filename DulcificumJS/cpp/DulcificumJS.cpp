#include <dulcificum.h>
#include <emscripten/bind.h>

struct info_t
{
    std::string dulcificum_version;
    std::string dulcificum_hash;
};

info_t get_info()
{
    return { DULCIFICUM_VERSION, DULCIFICUM_HASH };
}

EMSCRIPTEN_BINDINGS(dulcificum)
{
    emscripten::function("gcode_2_miracle_jtp", &dulcificum::GCode2Miracle_JTP);
    // Binding for info_t structure
    emscripten::value_object<info_t>("info_t")
        .field("dulcificum_version", &info_t::dulcificum_version)
        .field("dulcificum_hash", &info_t::dulcificum_hash);

    // Binding for get_info function
    emscripten::function("dulcificum_info", &get_info);
}
