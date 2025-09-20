#include "version.hxx"
#include "cli/cli_app.hxx"
#include <boost/lexical_cast.hpp>
#include "socket/async_client.hxx"
#include <thread>
#include <condition_variable>

//std::condition_variable cv;
//std::mutex m;

//void input_loop(behin_socket::async_client& client) {
//    //    std::string message {"Hello world!\n\n"};

//    int counter = 0;
//    while (true) {

//        std::unique_lock<std::mutex> lk(m);
//        cv.wait(lk, [](){ std::cout << "ok" << std::endl; });

//        std::string message = boost::lexical_cast<std::string>(counter++) + ") " + "Hello world!\n";

//        if (!message.empty()) {
//            client.write(message);
//        }

//        // Small delay to avoid overwhelming the console
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
//    }

//    client.disconnect();
//}

//void run_me() {
//    try {
//        boost::asio::io_context io_context;
//        behin_socket::async_client client(io_context, "localhost", "8080");

//        // Start connection
//        client.connect();

//        // Run IO context in background thread
//        std::thread io_thread([&io_context]() {
//            try {
//                io_context.run();
//            } catch (const std::exception& e) {
//                std::cerr << "IO context error: " << e.what() << std::endl;
//            }
//        });

//        // Wait a bit for connection to establish
//        std::this_thread::sleep_for(std::chrono::seconds(1));

//        if (client.is_connected()) {
//            std::cout << "Connection established. Start typing messages..." << std::endl;
//            input_loop(client);
//        } else {
//            std::cerr << "Failed to connect to server" << std::endl;
//        }

//        // Stop IO context and wait for thread
//        //        io_context.stop();
//        if (io_thread.joinable()) {
//            io_thread.join();
//        }

//    } catch (std::exception& e) {
//        std::cerr << "Exception: " << e.what() << std::endl;
//    }




//}


int main(int argc, char **argv)
{
    (void)::behin::version::print_version();

    try {
        boost::asio::io_context io_context;
        behin_socket::async_client client(io_context, "localhost", "8585");

        // Start connection
        client.connect();

        // Run IO context in background thread
        std::thread io_thread([&io_context]() {
            try {
                io_context.run();
            } catch (const std::exception& e) {
                std::cerr << "IO context error: " << e.what() << std::endl;
            }
        });

        //    std::thread t1(&run_me);
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
