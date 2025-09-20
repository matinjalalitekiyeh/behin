#include "cli_app.hxx"
#include "cli_parser.hxx"
#include "cli_history_manager.hxx"
#include "cli_input_handler.hxx"
#include "cli_command_executer.hxx"
#include <iostream>

cli_app::cli_app(behin_socket::async_client &client)
    : m_client(client)
    , m_running(false)
{
    m_parser = std::make_unique<cli_parser>();
    m_history = std::make_unique<cli_history_manager>();
    m_input_handler = std::make_unique<cli_input_handler>();
}

cli_app::~cli_app() = default;

void cli_app::exec() {
    m_running = true;
    print_welcome();

    cli_command_executor executor(*m_history);

    while (m_running) {
        std::string input = m_input_handler->read_line("behin> ");

        if (input.empty()) {
            continue;
        }

        if (input == "exit" || input == "quit") {
            stop();
            continue;
        }

        std::string expanded_input = m_history->expand_references(input);
        if (expanded_input.empty()) {
            continue;
        }

        if (expanded_input != input) {
            std::cout << expanded_input << std::endl;
            input = expanded_input;
        }

        m_history->add(input);
        m_input_handler->add_to_readline_history(input);

        command cmd = m_parser->parse(input);



        message_t msg = executor.execute(cmd);
        if (m_client.is_connected()) {
            m_client.write(reinterpret_cast<const char*>(&msg), sizeof(msg));
        }

    }
}

void cli_app::stop() {
    m_running = false;
}

void cli_app::print_welcome() const {
    std::cout << "╔══════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                                                      ║" << std::endl;
    std::cout << "║                    BEHIN CLI v1.0                    ║" << std::endl;
    std::cout << "║                                                      ║" << std::endl;
    std::cout << "╠══════════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║  • Runtime Monitoring, config and Trace              ║" << std::endl;
    std::cout << "║  • Type 'help' for available commands                ║" << std::endl;
    std::cout << "║  • Type 'exit' or 'quit' to end session              ║" << std::endl;
    std::cout << "║  • Use ↑/↓ arrows for command history                ║" << std::endl;
    std::cout << "║  • Use !number to re-run history commands            ║" << std::endl;
    std::cout << "║  • Tab completion available                          ║" << std::endl;
    std::cout << "║                                                      ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;
}

void cli_app::print_help() const {
    std::cout << "\nAvailable commands:\n"
              << "  help                    - Show this help message\n"
              << "  history                 - Show command history\n"
              << "  clear                   - Clear the screen\n"
              << "  exit, quit              - Exit the program\n"
              << std::endl;
}
