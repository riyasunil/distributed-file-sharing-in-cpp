#ifndef CLIENT_SESSION_H
#define CLIENT_SESSION_H

#pragma once
#include <boost/asio.hpp>
#include <string>

class ClientSession {
public:
    using tcp = boost::asio::ip::tcp;
    explicit ClientSession(tcp::socket socket);
    void start();

private:

//server needs to read and write to the client

    tcp::socket socket_;
    std::string user_id_;

    std::string read_(){
    boost::asio::streambuf buf;
    boost::asio::read_until( socket_, buf, "\n" );
    std::string data(
        boost::asio::buffers_begin(buf.data()),
        boost::asio::buffers_begin(buf.data()) + buf.size());
    buf.consume(buf.size()); // Clear the buffer
    return data;
    }

    void send_(const std::string& message){
    const std::string msg = message + "\n";
    boost::asio::write( socket_, boost::asio::buffer(msg) );
    }

    // std::string read_line();
    // void send_line(const std::string& message);
    // void handle_command(const std::string& command);
    void handle_upload();
    void handle_list();
    void handle_preview();



};

#endif
