#ifndef DULCIFICUM_INCLUDE_DULCIFICUM_UTILS_CHAR_RANGE_LITERAL_H
#define DULCIFICUM_INCLUDE_DULCIFICUM_UTILS_CHAR_RANGE_LITERAL_H

#include <algorithm>
#include <cstddef>

namespace dulcificum::utils
{

// NOLINTBEGIN
template<std::size_t N>
struct CharRangeLiteral
{
    constexpr CharRangeLiteral(const char (&str)[N]) noexcept
    {
        std::copy_n(str, N, value);
    }
    char value[N]; ///< The character array holding the literal
};
// NOLINTEND

} // namespace dulcificum::utils

#endif // DULCIFICUM_INCLUDE_DULCIFICUM_UTILS_CHAR_RANGE_LITERAL_H
