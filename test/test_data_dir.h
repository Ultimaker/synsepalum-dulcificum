#ifndef DULCIFICUM_TEST_DATA_DIR_H
#define DULCIFICUM_TEST_DATA_DIR_H

#ifndef STRING
#define STRING(x) #x
#define XSTRING(x) STRING(x)
#endif

#include <filesystem>

namespace dulcificum {
    static const std::filesystem::path kTestDataDir = XSTRING(DULCIFICUM_DATA_TEST_DIR);
}

#endif //DULCIFICUM_TEST_DATA_DIR_H
