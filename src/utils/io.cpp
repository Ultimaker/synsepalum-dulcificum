#include "dulcificum/utils/io.h"

#include <spdlog/spdlog.h>

#include <fstream>

namespace dulcificum::utils
{
namespace details
{
[[nodiscard]] std::optional<std::string> readGcodeFile(const std::filesystem::path& filename) noexcept
{
    std::ifstream file{ filename };
    std::string raw_data;
    if (file.is_open())
    {
        raw_data.assign((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
    }
    else
    {
        spdlog::error("Could not open {}", filename.string());
        return std::nullopt;
    }
    return raw_data;
}
} // namespace details

[[nodiscard]] std::optional<std::string> readFile(const std::filesystem::path& filename) noexcept
{
    // TODO: Add reading for the other formats
    return details::readGcodeFile(filename);
}

[[nodiscard]] bool writeFile(const std::filesystem::path& filename, std::string& content) noexcept
{
    std::ofstream outfile{ filename };
    outfile << content;
    outfile.close();
    return true;
}


} // namespace dulcificum::utils