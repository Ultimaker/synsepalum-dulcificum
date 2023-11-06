#ifndef DULCIFICUM_MGJTP_COMMAND_STREAM_H
#define DULCIFICUM_MGJTP_COMMAND_STREAM_H

#include "../command_types.h"
<<<<<<< HEAD
#include "mgjtp_json_to_command.h"

    =======
#include "mgjtp_command_file_stream.h"
    >>>>>>> 02de50b5febf71709526393981a1651516bc337a

    namespace dulcificum::miracle_jtp
{
    struct CommandStream
    {
        CommandFileStream fstream;
    };

<<<<<<< HEAD

public:
    void open(const std::filesystem::path& path)
    {
        fstream.open(path);
    }

    void close()
    {
        fstream.close();
    }

    bool eof()
    {
        return fstream.eof();
    }

    nlohmann::json getCommandJson();

    botcmd::CommandPtr getCommand();

    botcmd::CommandList getCommandList();
};
}
=======
} // namespace dulcificum::miracle_jtp
>>>>>>> 02de50b5febf71709526393981a1651516bc337a

#endif // DULCIFICUM_MGJTP_COMMAND_STREAM_H
