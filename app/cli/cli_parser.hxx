#ifndef CLI_PARSER_HXX
#define CLI_PARSER_HXX

#include <string>
#include <vector>

struct command {
    std::string name;
    std::vector<std::string> args;
};

class cli_parser {
public:
    command parse(const std::string& input) const;
    static std::vector<std::string> tokenize(const std::string& input);
};

#endif // CLI_PARSER_HXX