#ifndef INCLUDE_DULCIFICUM_UTILS_IO_H
#define INCLUDE_DULCIFICUM_UTILS_IO_H

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

bool writeFile(const std::filesystem::path& filename, const std::string& content);

} // namespace dulcificum::utils

#endif // INCLUDE_DULCIFICUM_UTILS_IO_H
