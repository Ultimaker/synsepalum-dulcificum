#ifndef DULCIFICUM_MGJTP_COMMAND_STREAM_H
#define DULCIFICUM_MGJTP_COMMAND_STREAM_H

#include "../command_types.h"
#include "mgjtp_command_file_stream.h"

namespace dulcificum::miracle_jtp
{

struct CommandStream
{
    CommandFileStream fstream;
};

} // namespace dulcificum::miracle_jtp

#endif // DULCIFICUM_MGJTP_COMMAND_STREAM_H
