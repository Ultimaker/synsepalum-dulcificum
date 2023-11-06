#include "dulcificum/miracle_jtp/mgjtp_command_file_stream.h"

namespace dulcificum::miracle_jtp {

    std::istream &getCmdLineTillCurlyBracket(std::istream &in_stream, std::string &line, char &end_char) {
        line.clear();
        std::istream::sentry stream_sentry(in_stream, true);
        std::streambuf *buffer = in_stream.rdbuf();
        while (true) {
            if (buffer->sgetc() == std::streambuf::traits_type::eof()) {
                break;
            }
            end_char = buffer->sbumpc();
            line += end_char;
            if (end_char == CommandFileStream::K_CMD_OPEN) {
                return in_stream;
            }
            if (end_char == CommandFileStream::K_CMD_CLOSE) {
                return in_stream;
            }
        }
        if (line.empty()) {
            in_stream.setstate(std::ios::eofbit);
        }
        return in_stream;
    }

    std::string CommandFileStream::getCommandLine() {
        std::string line;
        if (!stream.is_open()) return line;
        while (!stream.eof()) {
            char last_char = 0;
            std::string line_till_bracket;
            getCmdLineTillCurlyBracket(stream, line_till_bracket, last_char);
            if (bracket_depth > 0) {
                line += line_till_bracket;
            }
            if (last_char == CommandFileStream::K_CMD_OPEN) {
                if (bracket_depth == 0) {
                    line += CommandFileStream::K_CMD_OPEN;
                }
                bracket_depth++;
            }
            if (last_char == CommandFileStream::K_CMD_CLOSE) {
                bracket_depth--;
                if (bracket_depth <= 0) return line;
            }
        }
        return line;
    }

} // dulcificum::miracle_jtp

