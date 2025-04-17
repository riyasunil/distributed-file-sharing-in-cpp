#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <thread>

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
        socket.connect(tcp::endpoint(boost::asio::ip::make_address("127.0.0.1"), 1234));
        
        string user_id, password;
        cout << "Enter User ID: ";
        cin >> user_id;
        cout << "Enter Password: ";
        cin >> password;
        
        cout << user_id;
        cout << password;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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
                cout << "Enter : -u <file-path> for file upload, -l for listing files, -v for previewing files -q for quit\n";

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

                if (server_response.find("Listing Files") != std::string::npos){
                    boost::asio::streambuf list_response_buffer;
                    read_until(socket, list_response_buffer, "\n");
                    string list_response(
                        buffers_begin(list_response_buffer.data()), buffers_begin(list_response_buffer.data()) + list_response_buffer.size()
                    );
                    list_response_buffer.consume(list_response_buffer.size());

                    std::cout << "Upload Response" << list_response;
                }

                if (server_response.find("Enter file path and file name for the file to upload...\n") != std::string::npos){
                    string file_path;
                    cout << "Enter the file path";
                    std::getline(cin, file_path);


                    string file_name;
                    cout << "enter the file name";
                    std::getline(cin, file_name);

                    //sending the file name to server:
                    boost::asio::write(socket, boost::asio::buffer(file_name+"\n"));

                    //open the file :
                    std::ifstream file(file_path, std::ios::binary);
                    if(!file){
                        std::cout<<"Error, Could not open file" << file_path << std::endl;
                        continue;
                    }

                    //read the file content
                    std::vector<char> file_contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

                    //sending file size:
                    std::string size_str = std::to_string(file_contents.size()) + "\n";
                    write(socket, buffer(size_str));

                    // After sending the file size:
                    std::cout << "Sending file content..." << std::endl;

                    // Add a small delay to ensure server is ready
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));


                    //sending the file content:
                    if(!file_contents.empty()){
                        write(socket, buffer(file_contents));
                    }

                    std::cout << "Sent " << file_contents.size() << " bytes" << std::endl;

                    //get upload confirmation:
                    boost::asio::streambuf list_response_buffer;
                    read_until(socket, list_response_buffer, "\n");
                    string list_response(
                        buffers_begin(list_response_buffer.data()), buffers_begin(list_response_buffer.data()) + list_response_buffer.size()
                    );
                    list_response_buffer.consume(list_response_buffer.size());

                    std::cout << "Upload Response" << list_response;
                }

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