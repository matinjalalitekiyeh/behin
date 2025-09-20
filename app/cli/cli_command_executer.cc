#include "cli_command_executer.hxx"
#include <iostream>

enum message_type {
    detach = 1,
    count
};


cli_command_executor::cli_command_executor(cli_history_manager& hist)
//    : m_database(db),
    : m_history(hist)
{}

static void init_message(message_t& msg) {
    memset(&msg, 0x00, sizeof (message_t));
    msg.header      = 0xaabbccdd;
    msg.crc         = 0x00;
    msg.footer      = 0xdeaddead;
}

message_t cli_command_executor::execute(const command& cmd) {
    message_t msg;
    init_message(msg);

    if (cmd.name == "help") {
        execute_help();
    }
    else if (cmd.name == "detach") {
        detach_message_t dm;
        msg.type = message_type::detach;
        memset(&dm, 0x00, sizeof(dm));
        execute_detach(cmd.args, dm);
        memcpy(msg.data, &dm, sizeof(dm));
    }
    else if (cmd.name == "history") {
        execute_history();
    }
    else if (cmd.name == "clear") {
        execute_clear();
    }
    else {
        std::cout << "Unknown command: '" << cmd.name << "'. Type 'help' for a list." << std::endl;
    }

    return msg;
}

void cli_command_executor::execute_history() const {
    m_history.show();
}

void cli_command_executor::execute_clear() const {
    std::cout << "\033[2J\033[1;1H";
}

void cli_command_executor::execute_help() const {
    std::cout << "\nAvailable commands:\n"
              << "  help                    - Show this help message\n"
              << "  history                 - Show command history\n"
              << "  clear                   - Clear the screen\n"
              << "  exit, quit              - Exit the program\n"
              << "  detach [IMSI]           - detach UE from core forcefully\n"
              << "  detach -c [IMSI]        - detach UE from core forcefully and clear session\n"
              << std::endl;
    std::cout << "History References:\n"
              << "  !<number>               - Execute command by number (e.g., !3)\n"
              << "  !!                      - Execute previous command\n"
              << "  !-<number>              - Execute command from end (e.g., !-2)\n"
              << "  !<prefix>               - Execute last command starting with prefix\n"
              << std::endl;
}
void cli_command_executor::execute_detach(const std::vector<std::string> &args, detach_message_t &tm)
{
    if (!args.size()) return;

    for (const auto& o : args) {
        tm.imsi = std::stoull(o.c_str(), nullptr, 0);
    }
}
