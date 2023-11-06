#ifndef DULCIFICUM_MGJTP_FILE_STREAM_H
#define DULCIFICUM_MGJTP_FILE_STREAM_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace dulcificum::miracle_jtp {

    class CommandFileStream {
    private:
        std::ifstream stream;
        size_t bracket_depth = 0;


    public:
        static const char K_CMD_OPEN = '{';
        static const char K_CMD_CLOSE = '}';

        void open(const std::filesystem::path &path) {
            stream.open(path);
        }

        void close() {
            stream.close();
        }

        bool eof() {
            return stream.eof();
        }

        std::string getCommandLine();
    };
} //namespace dulcificum::miracle_jtp

#endif //DULCIFICUM_MGJTP_FILE_STREAM_H
