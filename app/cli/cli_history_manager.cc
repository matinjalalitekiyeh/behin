#include "cli_history_manager.hxx"
#include <iostream>
#include <regex>
#include <boost/algorithm/string.hpp>

cli_history_manager::cli_history_manager() = default;

void cli_history_manager::add(const std::string& command) {
    if (command.empty()) return;

    // Don't add duplicate consecutive commands
    if (!m_command_history.empty() && m_command_history.back() == command) {
        return;
    }

    m_command_history.push_back(command);

    if (m_command_history.size() > MAX_HISTORY_SIZE) {
        m_command_history.pop_front();
    }
}

void cli_history_manager::show() const {
    if (m_command_history.empty()) {
        std::cout << "No command history yet." << std::endl;
        return;
    }

    std::cout << "Command History:" << std::endl;
    std::cout << "  #  Command" << std::endl;
    std::cout << "──── ──────────────────────────────────" << std::endl;

    for (size_t i = 0; i < m_command_history.size(); ++i) {
        std::cout << " " << (i + 1) << "  " << m_command_history[i] << std::endl;
    }

    std::cout << "\nUse !<number> to execute a command from history" << std::endl;
    std::cout << "Examples: !3, !!, !-2, !set" << std::endl;
}

std::string cli_history_manager::get(int index) const {
    if (index < 1 || index > static_cast<int>(m_command_history.size())) {
        return "";
    }
    return m_command_history[index - 1];
}

size_t cli_history_manager::size() const {
    return m_command_history.size();
}

std::string cli_history_manager::expand_references(const std::string& input) const {
    std::string result = input;

    if (input.find('!') == std::string::npos) {
        return input;
    }

    std::string temp = handle_bang_bang(result);
    if (temp.empty()) return "";
    if (temp != result) return temp;

    temp = handle_bang_number(result);
    if (temp.empty()) return "";
    if (temp != result) return temp;

    temp = handle_bang_minus(result);
    if (temp.empty()) return "";
    if (temp != result) return temp;

    temp = handle_bang_string(result);
    if (temp.empty()) return "";

    return temp;
}

std::string cli_history_manager::handle_bang_bang(const std::string& input) const {
    if (input == "!!") {
        if (!m_command_history.empty()) {
            return m_command_history.back();
        } else {
            std::cout << "No previous command in history." << std::endl;
            return "";
        }
    }
    return input;
}

std::string cli_history_manager::handle_bang_number(const std::string& input) const {
    std::regex pattern_bang_number(R"(!(\d+))");
    std::smatch matches;

    if (std::regex_search(input, matches, pattern_bang_number)) {
        try {
            int index = std::stoi(matches[1].str());
            std::string hist_cmd = get(index);
            if (!hist_cmd.empty()) {
                return hist_cmd;
            } else {
                std::cout << "History command " << index << " not found." << std::endl;
                return "";
            }
        } catch (const std::exception& e) {
            std::cout << "Invalid history reference: " << matches[1].str() << std::endl;
            return "";
        }
    }
    return input;
}

std::string cli_history_manager::handle_bang_minus(const std::string& input) const {
    std::regex pattern_bang_minus(R"(!-(\d+))");
    std::smatch matches;

    if (std::regex_search(input, matches, pattern_bang_minus)) {
        try {
            int offset = std::stoi(matches[1].str());
            if (offset <= m_command_history.size() && offset > 0) {
                int index = m_command_history.size() - offset;
                std::string hist_cmd = get(index + 1);
                if (!hist_cmd.empty()) {
                    return hist_cmd;
                }
            }
            std::cout << "History offset too large: !-" << offset << std::endl;
            return "";
        } catch (const std::exception& e) {
            std::cout << "Invalid history reference: !-" << matches[1].str() << std::endl;
            return "";
        }
    }
    return input;
}

std::string cli_history_manager::handle_bang_string(const std::string& input) const {
    std::regex pattern_bang_string(R"(!([a-zA-Z][a-zA-Z0-9_]*))");
    std::smatch matches;

    if (std::regex_search(input, matches, pattern_bang_string)) {
        std::string prefix = matches[1].str();
        for (auto it = m_command_history.rbegin(); it != m_command_history.rend(); ++it) {
            if (it->find(prefix) == 0) {
                return *it;
            }
        }
        std::cout << "No command in history starting with: " << prefix << std::endl;
        return "";
    }
    return input;
}
