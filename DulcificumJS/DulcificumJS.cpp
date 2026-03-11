#include <dulcificum.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>

struct info_t
{
    std::string dulcificum_version;
    std::string dulcificum_hash;
};

info_t get_info()
{
    return { DULCIFICUM_VERSION, DULCIFICUM_HASH };
}

emscripten::val gcode_2_miracle_jtp_bytes(const std::string& content, const size_t nb_extruders = 2)
{
    const auto json = dulcificum::GCode2Miracle_JTP(content, nb_extruders);

    // Allocate a standalone ArrayBuffer-backed Uint8Array so callers can transfer it.
    auto uint8_array_ctor = emscripten::val::global("Uint8Array");
    auto out = uint8_array_ctor.new_(json.size());
    out.call<void>("set", emscripten::val(emscripten::typed_memory_view(
                               json.size(),
                               reinterpret_cast<const unsigned char*>(json.data()))));
    return out;
}

EMSCRIPTEN_BINDINGS(dulcificum)
{
    emscripten::function("gcode_2_miracle_jtp", &dulcificum::GCode2Miracle_JTP);
    emscripten::function("gcode_2_miracle_jtp_bytes", &gcode_2_miracle_jtp_bytes);
    // Binding for info_t structure
    emscripten::value_object<info_t>("info_t")
        .field("dulcificum_version", &info_t::dulcificum_version)
        .field("dulcificum_hash", &info_t::dulcificum_hash);

    // Binding for get_info function
    emscripten::function("dulcificum_info", &get_info);
}
