#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_IO_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_IO_H

#include <filesystem>
#include <optional>
#include <string>

namespace dulcificum::utils
{

namespace details
{
[[nodiscard]] std::optional<std::string> readGcodeFile(const std::filesystem::path& filename) noexcept;
} // namespace details

[[nodiscard]] std::optional<std::string> readFile(const std::filesystem::path& filename) noexcept;

} // namespace dulcificum::utils

#endif // DULCIFICUM_INCLUDE_DULCIFICUM_IO_H
