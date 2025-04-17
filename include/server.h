#pragma once //only include this file once in a single compilation
#include <boost/asio.hpp>
#include <thread>

class Server{
    public :
        Server(const std::string &address, unsigned short port);
        void start();

    private:
        void accept_loop(); //to accept clients

        boost::asio::io_context io_context_;
        boost::asio::ip::tcp::acceptor acceptor_;

};
