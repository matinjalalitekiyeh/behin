#ifndef CLI_APP_HXX
#define CLI_APP_HXX

#include <string>
#include <vector>
#include <map>
#include <deque>
#include <memory>
#include "../socket/async_client.hxx"


class cli_parser;
class cli_history_manager;
class cli_input_handler;

class cli_app {
public:
    cli_app(behin_socket::async_client& client);
    ~cli_app();

    void exec();
    void stop();
    void print_welcome() const;
    void print_help() const;

private:
    std::unique_ptr<cli_parser> m_parser;
    std::unique_ptr<cli_history_manager> m_history;
    std::unique_ptr<cli_input_handler> m_input_handler;

    behin_socket::async_client& m_client;
    bool m_running;
};

#endif // CLI_APP_HXX
