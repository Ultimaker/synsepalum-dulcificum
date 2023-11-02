#ifndef INCLUDE_DULCIFICUM_UTILS_STRING_CONVERT_H
#define INCLUDE_DULCIFICUM_UTILS_STRING_CONVERT_H

#include <locale>
#include <sstream>
#include <string>
#include <type_traits>

namespace dulcificum::utils
{
template <typename T>
concept stringlike = requires (const T& t) {
    { std::is_convertible_v<T, std::string> };
};

template<class T>
T stringConvert(const std::string_view& value)
{
    std::istringstream iss{ value.data() };
    iss.imbue(std::locale::classic()); // the "C" locale
    T converted{};
    iss >> converted;
    return converted;
}

} // namespace dulcificum::utils
#endif // INCLUDE_DULCIFICUM_UTILS_STRING_CONVERT_H
