#ifndef DULCIFICUM_TEST_DATA_DIR_H
#define DULCIFICUM_TEST_DATA_DIR_H

#include <filesystem>
#include <source_location>

namespace dulcificum
{
static const auto kTestDataDir = std::filesystem::path{ std::source_location::current().file_name() }.parent_path().append("data");
} // namespace dulcificum

#endif // DULCIFICUM_TEST_DATA_DIR_H
