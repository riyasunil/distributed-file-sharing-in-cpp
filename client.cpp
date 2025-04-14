#include <iostream>
#include <boost/asio.hpp>
#include <string>

using namespace boost::asio;
using ip::tcp;
using std::cout;
using std::cin;
using std::string;

void start_client() {
    try {
        boost::asio::io_context io_context;
        
        // Socket creation
        tcp::socket socket(io_context);
        
        // Connection
        socket.connect(tcp::endpoint(boost::asio::ip::make_address("127.0.0.0"), 1234));
        
        string user_id, password;
        cout << "Enter User ID: ";
        cin >> user_id;
        cout << "Enter Password: ";
        cin >> password;
        
        cout << user_id;
        cout << password;
        // Send credentials
        boost::asio::write(socket, boost::asio::buffer(user_id + "\n"));
        boost::asio::write(socket, boost::asio::buffer(password + "\n"));
        
        // Get response
        boost::asio::streambuf receive_buffer;
        boost::asio::read_until(socket, receive_buffer, "\n");
        
        std::string response(
            boost::asio::buffers_begin(receive_buffer.data()),
            boost::asio::buffers_begin(receive_buffer.data()) + receive_buffer.size());
        receive_buffer.consume(receive_buffer.size()); // Clear the buffer
        
        cout << "Server response: " << response;

        if (response == "Login Successful\n"){
            while(true){
                string cmd;
                cout << "Enter : -u <file-path> for file upload, -q for quit\n";

                //for file path
                std::getline(cin, cmd);

                if(cmd.empty())continue;

                boost::asio::write(socket, boost::asio::buffer(cmd+"\n"));
                boost::asio::streambuf buf;
                boost::asio::read_until(socket, buf, "\n");
                string server_response(
                    boost::asio::buffers_begin(buf.data()), boost::asio::buffers_begin(buf.data()) + buf.size());
                buf.consume(buf.size());

                cout << "Server : " << server_response;

                if (server_response.find("Closing connection") != std::string::npos) {
            break;
        }

            }
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}