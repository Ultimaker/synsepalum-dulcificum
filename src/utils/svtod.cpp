#include "dulcificum/utils/svtod.h"

#include <string>
#include <string_view>

namespace dulcificum::utils
{
double StringViewToDouble(std::string_view sv)
{
    return std::stod(std::string(sv));
}
} // namespace dulcificum::utils