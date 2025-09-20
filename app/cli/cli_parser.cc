#include "cli_parser.hxx"
#include <boost/algorithm/string.hpp>

namespace ba = boost::algorithm;

command cli_parser::parse(const std::string& input) const {
    command cmd;
    std::vector<std::string> tokens = tokenize(input);

    if (!tokens.empty()) {
        cmd.name = tokens[0];
        if (tokens.size() > 1) {
            cmd.args.assign(tokens.begin() + 1, tokens.end());
        }
    }
    return cmd;
}

std::vector<std::string> cli_parser::tokenize(const std::string& input) {
    std::vector<std::string> tokens;
    ba::split(tokens, input, ba::is_any_of(" "), ba::token_compress_on);
    return tokens;
}