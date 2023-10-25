#include "dulcificum/utils/svtod.h"

#include <string_view>
#include <string>

namespace dulcificum::utils
{
double StringViewToDouble(std::string_view sv)
{
    return std::stod(std::string(sv));
}
} // namespace dulcificum::utils