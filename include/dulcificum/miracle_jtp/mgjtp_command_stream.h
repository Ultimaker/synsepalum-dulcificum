#ifndef DULCIFICUM_MGJTP_COMMAND_STREAM_H
#define DULCIFICUM_MGJTP_COMMAND_STREAM_H

#include "../command_types.h"
#include "mgjtp_command_file_stream.h"
#include "mgjtp_json_to_command.h"
#include "mgjtp_command_file_stream.h"
namespace dulcificum::miracle_jtp
{

struct CommandStream
{
    CommandFileStream fstream;
    public:
        void open(const std::filesystem::path &path) {
            fstream.open(path);
        }

        void close() {
            fstream.close();
        }

        bool eof() {
            return fstream.eof();
        }

        nlohmann::json getCommandJson();

        botcmd::CommandPtr getCommand();

        botcmd::CommandList getCommandList();
 };
}

#endif // DULCIFICUM_MGJTP_COMMAND_STREAM_H
