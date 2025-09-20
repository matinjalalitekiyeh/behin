#include "ver/version.hxx"
#include "cli/cli_app.hxx"
#include "socket/async_client.hxx"

int main(int argc, char **argv)
{
    (void)::behin::version::print_version();

    try {
        boost::asio::io_context io_context;
        behin_socket::async_client client(io_context, "localhost", "8585");
        client.connect();
        std::thread io_thread([&io_context]() {
            try {
                io_context.run();
            } catch (const std::exception& e) {
                std::cerr << "IO context error: " << e.what() << std::endl;
            }
        });

        cli_app app(client);
        app.exec();

        if (io_thread.joinable()) {
            io_thread.join();
        }

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }


    return 0;


}
