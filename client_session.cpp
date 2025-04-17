#include "client_session.h"
#include "auth.h"
#include "utils.h"
#include <filesystem>
#include <fstream>
#include <iostream>

using boost::asio::ip::tcp;
using namespace std;

ClientSession::ClientSession(tcp::socket socket)
    : socket_(std::move(socket)) {}

void ClientSession::start() {
    try {
        std::string uid = read_();
        std::string pwd = read_();
        user_id_ = uid;

        // std::cout << "user id : " << uid; 

        if (!auth::validate_user(uid, pwd)) {
            send_("Login Failed");
            socket_.close();
            return;
        }

        send_( "Login Successful");

        while (true) {
            std::string command = utils::trim(read_());
            if (command == "-q") {
                send_( "Closing connection...");
                socket_.close();
                break;
            } else if (command == "-u") {
                handle_upload();
            } else if (command == "-l") {
                handle_list();
            } else if (command == "-p") {
                std::cout << "hello";
                handle_preview();
            } else {
                send_( "Unknown command");
            }
        }
    } catch (const std::exception& e) {
        cerr << "Client session error: " << e.what() << endl;
    }
}

void ClientSession::handle_upload(){
    send_( "Enter file path and file name for the file to upload...\n");

    //get file name from user
    boost::asio::streambuf name_buf;
    boost::asio::read_until(socket_, name_buf, "\n");  
    
    string file_name(
        boost::asio::buffers_begin(name_buf.data()), 
        boost::asio::buffers_begin(name_buf.data()) + name_buf.size() - 1); // -1 to remove the \n
    name_buf.consume(name_buf.size());

    cout << "Received file name: " << file_name << endl;


    cout << "Waiting for file size..." << endl;


    //get file from the client:
    boost::asio::streambuf size_buf;
    read_until(socket_, size_buf, "\n");
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
        send_( "Error : Invalid file size recieved \n");
        return;
    }
    

    //download that file path onto server at <uid>/file_name
    
    //create dir:
    string user_dir = user_id_ + "/";
    std::filesystem::create_directories(user_dir);

    string output_path =  user_dir+file_name;
    std::ofstream output_file(output_path, std::ios::binary);

    if(!output_file){
        cout<<"Error: Could not create output file in server";
        send_( "Error: Could not create output file in server");
        return;
    }

    //recieve and write the file content into the output_file
    std::vector<char> buffer(1024);
    size_t bytes_recieved = 0;

    cout << "Starting to receive file..." << endl;

    
    while(bytes_recieved < file_size){
        // cout << "hi";
        size_t bytes_to_read = std::min(buffer.size(), file_size- bytes_recieved);
        size_t bytes = socket_.read_some(boost::asio::buffer(buffer,  bytes_to_read));

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
        send_( "File uploaded successfully: " + file_name + "\n");
    } else {
        send_( "Warning: Incomplete file transfer. Expected " + 
            std::to_string(file_size) + " bytes but received " + 
            std::to_string(bytes_recieved) + " bytes\n");
    }


}


void ClientSession::handle_list() {
    send_(  "Listing Files : \n");
    string file_list;
    string user_dir = user_id_ + "/";
    for(const auto &entry : std::filesystem::directory_iterator(user_dir)){
        file_list += entry.path().string() + "\n";

    }

    if(file_list.empty()){
        send_( "No files uploaded\n");
    }else{
        send_( file_list);
    }
}


void ClientSession::handle_preview() {
    send_("Preparing to Preview");
    return;

}