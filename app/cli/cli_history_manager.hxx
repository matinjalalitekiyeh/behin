#ifndef HISTORYMANAGER_H
#define HISTORYMANAGER_H

#include <string>
#include <deque>
#include <vector>

class cli_history_manager {
private:
    std::deque<std::string> m_command_history;
    const size_t MAX_HISTORY_SIZE = 100;

public:
    cli_history_manager();

    void add(const std::string& command);
    void show() const;
    std::string get(int index) const;
    std::string expand_references(const std::string& input) const;
    size_t size() const;

private:
    int parse_history_reference(const std::string& ref) const;
    std::string handle_bang_number(const std::string& input) const;
    std::string handle_bang_bang(const std::string& input) const;
    std::string handle_bang_minus(const std::string& input) const;
    std::string handle_bang_string(const std::string& input) const;
};

#endif // HISTORYMANAGER_H