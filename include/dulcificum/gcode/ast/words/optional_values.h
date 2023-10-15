#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_WORDS_OPTIONAL_VALUES_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_WORDS_OPTIONAL_VALUES_H

#include <map>
#include <string>

namespace dulcificum::gcode::ast::details
{

struct OptionalValues
{
    std::map<std::string, double> val_map;
    struct AssignCtxValToVar
    {
        template<typename T>
        auto operator()(const std::string& key, T& map) const
        {
            return [&, key](auto& ctx)
            {
                map[key] = _attr(ctx);
            };
        }
    } assign;
};

} // namespace dulcificum::gcode::ast::details


#endif // DULCIFICUM_INCLUDE_DULCIFICUM_GCODE_AST_WORDS_OPTIONAL_VALUES_H
