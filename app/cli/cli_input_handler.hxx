#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <string>
#include <readline/readline.h>
#include <readline/history.h>

class cli_input_handler {
public:
    cli_input_handler();
    ~cli_input_handler();

    std::string read_line(const std::string& prompt);
    void setup_readline();
    void cleanup_readline();
    void add_to_readline_history(const std::string& command);

    // Readline completion functions
    static char** custom_completion(const char* text, int start, int end);
    static char* custom_generator(const char* text, int state);
};

#endif // INPUTHANDLER_H