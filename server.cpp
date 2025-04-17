#include <iostream>
#include <unordered_map>
#include <boost/asio.hpp>
#include <string>
#include "auth.h"
#include "utils.h"
#include <fstream>
#include <vector>
#include <filesystem>

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

                cout<<user_id;
                cout<<password;
                if(auth::validate_user(user_id, password)){
                    // cout<<"logged in";
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

                            send_(sock, "Enter file path and file name for the file to upload...\n");

                            //get file name from user
                            boost::asio::streambuf name_buf;
                            boost::asio::read_until(sock, name_buf, "\n");  
                            
                            string file_name(
                                boost::asio::buffers_begin(name_buf.data()), 
                                boost::asio::buffers_begin(name_buf.data()) + name_buf.size() - 1); // -1 to remove the \n
                            name_buf.consume(name_buf.size());

                            cout << "Received file name: " << file_name << endl;


                            cout << "Waiting for file size..." << endl;


                            //get file from the client:
                            streambuf size_buf;
                            read_until(sock, size_buf, "\n");
                            string size_str(
                                buffers_begin(size_buf.data()), buffers_begin(size_buf.data()) + size_buf.size() -1
                            );
                            size_buf.consume(size_buf.size());

                            cout << "Received file size: " << size_str << " bytes" << endl;
                            
                            //convert the size of the file we get from client (in string) to size_t data type
                            size_t file_size;
                            try{
                                file_size = std::stoull(size_str);
                            }catch( const std::exception& e){
                                cout << "Exception here :" << e.what();
                                send_(sock, "Error : Invalid file size recieved \n");
                                break;
                            }
                           

                            //download that file path onto server at <uid>/file_name
                            
                            //create dir:
                            string user_dir = user_id + "/";
                            std::filesystem::create_directories(user_dir);

                            string output_path =  user_dir+file_name;
                            std::ofstream output_file(output_path, std::ios::binary);

                            if(!output_file){
                                cout<<"Error: Could not create output file in server";
                                send_(sock, "Error: Could not create output file in server");
                                break;
                            }

                            //recieve and write the file content into the output_file
                            std::vector<char> buffer(1024);
                            size_t bytes_recieved = 0;

                            cout << "Starting to receive file..." << endl;

                            
                            while(bytes_recieved < file_size){
                                // cout << "hi";
                                size_t bytes_to_read = std::min(buffer.size(), file_size- bytes_recieved);
                                size_t bytes = sock.read_some(boost::asio::buffer(buffer,  bytes_to_read));

                                cout<<"Receieved" <<bytes <<"bytes" << std::endl;

                                if(bytes > 0){
                                    cout <<"Here \n";
                                    cout << bytes;
                                    output_file.write(buffer.data(), bytes);
                                    bytes_recieved += bytes;
                                                cout << "Total received: " << bytes_recieved << "/" << file_size << endl;
                                }
                                else{
                                     cout << "Read returned 0 bytes, breaking" << endl;           
                                     break;
                                }
                            }

                            output_file.close();

                            if (bytes_recieved == file_size) {
                                send_(sock, "File uploaded successfully: " + file_name + "\n");
                            } else {
                                send_(sock, "Warning: Incomplete file transfer. Expected " + 
                                    std::to_string(file_size) + " bytes but received " + 
                                    std::to_string(bytes_recieved) + " bytes\n");
                            }


                            // sock.close();
                        }
                        else if(command == "-l"){
                            send_(sock,  "Listing Files : \n");
                            string file_list;
                            string user_dir = user_id + "/";
                            for(const auto &entry : std::filesystem::directory_iterator(user_dir)){
                                file_list += entry.path().string() + "\n";

                            }

                            if(file_list.empty()){
                                send_(sock, "No files uploaded\n");
                            }else{
                                send_(sock, file_list);
                            }
                        }
                        else if (command == "v"){
                            send_(sock, "Preparing to Preview");
                            
                        }
                        else{
                            send_(sock, "Unknown Command\n");
                            send_(sock, "Enter : -u for file upload, -l to list, -q for quit\n");

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
