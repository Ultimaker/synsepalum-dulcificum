#include "dulcificum/utils/svtod.h"

#include <charconv>

namespace dulcificum::utils
{
double StringViewToDouble(std::string_view sv)
{
    double value = 0.0;
    std::from_chars(sv.data(), sv.data() + sv.size(), value);
    return value;
}
} // namespace dulcificum::utils