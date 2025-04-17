#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "server.h"


void start_client();

void start_server() {

    std::string address = "127.0.0.1";
        unsigned short port = 1234;


    try{
        Server server(address, port);
        server.start();

    }catch(const std::exception& e){
        std::cerr << "Server error: " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "Starting main\n";
            start_server();


    return 0;

}