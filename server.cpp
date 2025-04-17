/*
Structuring the class in the following architecture:
[main.cpp]
    |
    v
[Server class] -> handles incoming client connections
    |
    v
[ClientSession class] -> handles per client
    |
    v
[auth/utils/fs]
*/


#include <server.h>
#include <client_session.h>
#include <iostream>
#include <boost/asio.hpp>
#include <string>


using namespace boost::asio;
using ip::tcp;
using std::cout;
using std::cin;
using std::endl;
using std::string;


Server::Server(const std::string& address, unsigned short port)
    : acceptor_(io_context_)
        {
             boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(address), port);
 acceptor_.open(endpoint.protocol());  // open with protocol
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true)); // set reuse
    acceptor_.bind(endpoint);  // bind manually
    acceptor_.listen();          }


void Server::start() {
    std::cout << "Server is running. Waiting for clients...\n";
    accept_loop();
}

void Server::accept_loop() {
    while (true) {
        boost::asio::ip::tcp::socket socket(io_context_);
        acceptor_.accept(socket);

        std::thread client_thread([sock = std::move(socket)]() mutable {
            ClientSession session(std::move(sock));
            session.start();
        });

        client_thread.detach();
    }
}
