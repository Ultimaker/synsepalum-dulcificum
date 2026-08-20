#include <dulcificum.h>
#include <emscripten/bind.h>
#include <exception>
#include <nlohmann/json.hpp>
#include <string>

struct info_t
{
    std::string dulcificum_version;
    std::string dulcificum_hash;
};

info_t get_info()
{
    return { DULCIFICUM_VERSION, DULCIFICUM_HASH };
}

std::string gcode_2_miracle_jtp_safe(const std::string& content, size_t nb_extruders)
{
    try
    {
        return dulcificum::GCode2Miracle_JTP(content, nb_extruders);
    }
    catch (const std::exception& e)
    {
        const nlohmann::json err = { { "error", true }, { "message", e.what() } };
        return err.dump();
    }
    catch (...)
    {
        const nlohmann::json err = { { "error", true }, { "message", "Unknown error during GCode translation" } };
        return err.dump();
    }
}

EMSCRIPTEN_BINDINGS(dulcificum)
{
    emscripten::function("gcode_2_miracle_jtp", &gcode_2_miracle_jtp_safe);
    // Binding for info_t structure
    emscripten::value_object<info_t>("info_t").field("dulcificum_version", &info_t::dulcificum_version).field("dulcificum_hash", &info_t::dulcificum_hash);

    // Binding for get_info function
    emscripten::function("dulcificum_info", &get_info);
}
