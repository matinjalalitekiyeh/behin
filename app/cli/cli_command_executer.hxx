#ifndef COMMAND_EXECUTOR_HXX
#define COMMAND_EXECUTOR_HXX

#include "cli_parser.hxx"
#include "cli_history_manager.hxx"
#include <algorithm>
#include <string.h>

struct detach_message_t {
    bool is_clean_up;
    size_t count;
    unsigned long long imsi;
};
struct trace_message_t {
    bool is_clean_up;
    size_t count;
    unsigned long long imsi;
};

#define MESSAGE_SIZE 1024 * 32
#define MESSAGE_METADATA 5*sizeof(uint32_t)
struct message_t {
    uint32_t header;
    uint32_t size;
    uint32_t type;
    uint8_t data[MESSAGE_SIZE - MESSAGE_METADATA];
    uint32_t crc;
    uint32_t footer;
};

class cli_command_executor {
private:
    cli_history_manager& m_history;

public:
    cli_command_executor(cli_history_manager& hist);
    message_t execute(const command& cmd);

private:
    void execute_history() const;
    void execute_clear() const;
    void execute_help() const;

    void execute_detach(const std::vector<std::string>& args, detach_message_t &tm);
};

#endif /* COMMAND_EXECUTOR_HXX */
