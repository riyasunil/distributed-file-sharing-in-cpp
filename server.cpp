#include<iostream>
#include <unordered_map>
#include <boost/asio.hpp>
#include <string>
#include "auth.h"
#include "utils.h"

// using std::string;
// using std::cout;
// using std::cin;
using auth::validate_user;
using utils::trim;
using namespace boost::asio;
using ip::tcp;
using std::cout;
using std::cin;
using std::endl;
using std::string;

//server needs to read and write to the client
string read_(tcp::socket & socket){
  boost::asio::streambuf buf;
  boost::asio::read_until( socket, buf, "\n" );
  std::string data(
    boost::asio::buffers_begin(buf.data()),
    boost::asio::buffers_begin(buf.data()) + buf.size());
buf.consume(buf.size()); // Clear the buffer
  return data;
}

void send_(tcp::socket & socket, const string& message){
  const string msg = message + "\n";
  boost::asio::write( socket, boost::asio::buffer(msg) );
}



void start_server()
{
    boost::asio::io_context io_context;
    //listen for new connection
    tcp::acceptor acceptor_(io_context, tcp::endpoint(boost::asio::ip::make_address("0.0.0.0"), 1234));

    cout << "Server is running. Waiting for clients...\n";

    while(true){

        tcp::socket socket_(io_context);

        acceptor_.accept(socket_);

        std::thread client_thread([sock = std::move(socket_)]() mutable {
            try{
                string user_id = read_(sock);
                string password = read_(sock);

                if(auth::validate_user(user_id, password)){
                    send_(sock, "Login Successful");

                    //start session loop
                    try{
                    while(true){
                        string command = trim(read_(sock));
                        if(command == "-q"){
                            send_(sock, "Closing connection...\n");
                            sock.close();
                            break;
                        }
                        else if(command == "-u"){
                            send_(sock, "Upload function otw\n");
                            // sock.close();
                        }else{
                            send_(sock, "Unknown Command\n");
                            send_(sock, "Enter : -u for file upload, -q for quit\n");

                        }
                    }
                    }
                    catch(std::exception& e){
                        std::cerr << "Error during session: " << e.what() << std::endl;
                        sock.close();
                    }
                }
                else{
                    send_(sock, "Login Failed");

                    //close the connection immedeatly if the user login credentials are wrong or cant be logged in without retries
                    sock.close();
                }
            }catch(const std::exception& e){
                std::cerr << "Client thread handler exception (in server.cpp):" << e.what() << std::endl;
            }
        });

        client_thread.detach(); //to make sure the main thread does not lose track of this thread (could lead to resource leaks if unchecked)

        
    }
    
 

}
