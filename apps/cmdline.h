#ifndef APPS_CMDLINE_H
#define APPS_CMDLINE_H

#ifndef APP_VERSION
#define APP_VERSION "0.1.0"
#endif

#include <fmt/compile.h>

#include <string_view>

namespace apps::cmdline
{

constexpr std::string_view NAME = "translator";
constexpr std::string_view VERSION = APP_VERSION;
static const auto VERSION_ID = fmt::format(FMT_COMPILE("{} {}"), NAME, VERSION);

constexpr std::string_view USAGE = R"({0}.
Dulcificum changes the flavor, or dialect, of 3d printer commands

Usage:
  translator [--quiet | --verbose] --input=FLAVOR INPUT --output=FLAVOR OUTPUT
  translator (-h | --help)
  translator --version

Options:
  -h --help                      Show this screen.
  --version                      Show version.
  --quiet                        Output no text to stdout (except errors)
  --verbose                      Output more log statements
  --input=TYPE                   'miracle_jtp' | 'griffin'
  --output=FLAVOR                'miracle_jtp' | 'griffin'
)";

} // namespace apps::cmdline

#endif // APPS_CMDLINE_H