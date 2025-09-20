#include "cli_input_handler.hxx"
#include <iostream>
#include <vector>
#include <memory>
#include <boost/algorithm/string.hpp>

cli_input_handler::cli_input_handler() {
    setup_readline();
}

cli_input_handler::~cli_input_handler() {
    cleanup_readline();
}

void cli_input_handler::setup_readline() {
    rl_attempted_completion_function = custom_completion;
    rl_bind_key('\t', rl_complete);
    rl_completion_append_character = '\0';
    rl_completion_suppress_append = 0;

    // Enable readline history
    using_history();
}

void cli_input_handler::cleanup_readline() {
    // Clear readline history
    clear_history();
}

std::string cli_input_handler::read_line(const std::string& prompt) {
    char* input_line = readline(prompt.c_str());

    if (!input_line) {
        return "";
    }

    std::string input(input_line);
    free(input_line);

    // Trim whitespace
    boost::algorithm::trim(input);

    return input;
}

void cli_input_handler::add_to_readline_history(const std::string& command) {
    if (!command.empty()) {
        add_history(command.c_str());
    }
}

char** cli_input_handler::custom_completion(const char* text, int start, int end) {
    if (start == 0) {
        return rl_completion_matches(text, custom_generator);
    }
    return nullptr;
}

char* cli_input_handler::custom_generator(const char* text, int state) {
    static std::vector<std::string> matches;
    static size_t match_index = 0;

    if (state == 0) {
        matches.clear();
        match_index = 0;

        std::vector<std::string> commands = {
            "help", "history", "clear", "exit", "quit", "detach"
        };

        for (const auto& cmd : commands) {
            if (cmd.find(text) == 0) {
                matches.push_back(cmd);
            }
        }
    }

    if (match_index < matches.size()) {
        return strdup(matches[match_index++].c_str());
    }

    return nullptr;
}
