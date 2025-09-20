#ifndef ASYNC_CLIENT_HXX
#define ASYNC_CLIENT_HXX

#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <thread>
#include <string>

using boost::asio::ip::tcp;


namespace behin_socket {

class async_client {
public:
    async_client(boost::asio::io_context& io_context,
                 const std::string& host, const std::string& port)
        : io_context_(io_context), socket_(io_context),
          resolver_(io_context), host_(host), port_(port),
          connected_(false) {}

    void connect() {
        resolver_.async_resolve(host_, port_,
                                [this](const boost::system::error_code& ec,
                                tcp::resolver::results_type endpoints) {
            if (!ec) {
                async_connect(endpoints);
            } else {
                std::cerr << "Resolve error: " << ec.message() << std::endl;
            }
        });
    }

    ~async_client() {
        io_context_.stop();
    }


    void async_connect(tcp::resolver::results_type endpoints) {
        boost::asio::async_connect(socket_, endpoints,
                                   [this](const boost::system::error_code& ec,
                                   const tcp::endpoint& endpoint) {
            if (!ec) {
                std::cout << "Connected to " << endpoint << std::endl;
                connected_ = true;
                start_read();
            } else {
                std::cerr << "Connect error: " << ec.message() << std::endl;
            }
        });
    }

    void write(const std::string& message) {
        if (!connected_) {
            std::cerr << "Not connected, cannot send message" << std::endl;
            return;
        }

        auto shared_msg = std::make_shared<std::string>(message);
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(*shared_msg),
                                 [this, shared_msg](const boost::system::error_code& ec,
                                 std::size_t /*length*/) {
            if (!ec) {
                //                    std::cout << "Message sent: " << *shared_msg << std::endl;
            } else {
                std::cerr << "Write error: " << ec.message() << std::endl;
                connected_ = false;
            }
        });
    }

    void write(const char* message, size_t len) {
        if (!connected_) {
            std::cerr << "Not connected, cannot send message" << std::endl;
            return;
        }

        // Create a shared_ptr to manage the buffer lifetime
        auto shared_buffer = std::make_shared<std::vector<char>>(message, message + len);

        boost::asio::async_write(socket_,
                                 boost::asio::buffer(*shared_buffer),
                                 [this, shared_buffer](const boost::system::error_code& ec,
                                 std::size_t /*length*/) {
            if (!ec) {
                // std::cout << "Message sent: " << std::string(shared_buffer->data(), shared_buffer->size()) << std::endl;
            } else {
                std::cerr << "Write error: " << ec.message() << std::endl;
                connected_ = false;
            }
        });
    }

    bool is_connected() const {
        return connected_;
    }



    void disconnect() {
        if (socket_.is_open()) {
            boost::system::error_code ec;
            socket_.shutdown(tcp::socket::shutdown_both, ec);
            socket_.close(ec);
            connected_ = false;
        }
    }

private:
    void start_read() {
        socket_.async_read_some(boost::asio::buffer(read_buffer_),
                                [this](const boost::system::error_code& ec, std::size_t length) {
            if (!ec) {
                std::cout << "Received: "
                          << std::string(read_buffer_.data(), length)
                          << std::endl;
                start_read();
            } else {
                std::cerr << "Read error: " << ec.message() << std::endl;
                connected_ = false;
            }
        });
    }

    boost::asio::io_context& io_context_;
    tcp::socket socket_;
    tcp::resolver resolver_;
    std::string host_;
    std::string port_;
    std::array<char, 1024> read_buffer_;
    std::atomic<bool> connected_;

};


}


#endif /*ASYNC_CLIENT_HXX*/
