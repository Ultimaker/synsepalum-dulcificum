#ifndef DULCIFICUM_MGJTP_COMMAND_STREAM_H
#define DULCIFICUM_MGJTP_COMMAND_STREAM_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

namespace dulcificum::miracle_jtp {

    struct CommandFStream {
    private:
        std::ifstream stream;
        size_t bracket_depth = 0;


    public:
        static const char K_CMD_OPEN = '{';
        static const char K_CMD_CLOSE = '}';

        void open(const std::filesystem::path &path) {
            stream.open(path);
        }

        std::string get();
    };

} // dulcificum::miracle_jtp

#endif //DULCIFICUM_MGJTP_COMMAND_STREAM_H
