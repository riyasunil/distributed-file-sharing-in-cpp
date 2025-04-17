#include <wx/wx.h>
#include <string>
#include <boost/asio.hpp>
#include <thread>
#include "window.h"

using namespace boost::asio;
using ip::tcp;
    
        MyFrame::MyFrame() : wxFrame(nullptr, wxID_ANY, "CalicoFS", wxDefaultPosition, wxSize(800,600)){
        wxPanel* panel = new wxPanel(this, wxID_ANY);

        useridCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(50, 50), wxSize(200, 40));
        passwordCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(50, 100), wxSize(200, 40), wxTE_PASSWORD);

        wxButton* loginBtn = new wxButton(panel, wxID_ANY, wxT("Login"), wxPoint(50, 150), wxSize(200,40));

        loginBtn->Bind(wxEVT_BUTTON, &MyFrame::OnLoginClick, this);
    }


    void MyFrame::OnLoginClick(wxCommandEvent& event)
    {
        std::string user = std::string(useridCtrl->GetValue().mb_str());
        std::string pass = std::string(passwordCtrl->GetValue().mb_str());

        std::cout << user << pass << std::endl;
        std::thread([=]() {
            LoginWithCredentials(user, pass);
        }).detach(); // run in background to avoid freezing UI
    }

    void MyFrame::LoginWithCredentials(const std::string& user, const std::string& pass)
{
    try {
        boost::asio::io_context io_context;
        tcp::socket socket(io_context);
        socket.connect(tcp::endpoint(boost::asio::ip::make_address("127.0.0.1"), 1234));

        boost::asio::write(socket, boost::asio::buffer(user + "\n"));
        boost::asio::write(socket, boost::asio::buffer(pass + "\n"));

        boost::asio::streambuf receive_buffer;
        boost::asio::read_until(socket, receive_buffer, "\n");

        std::string response(
            boost::asio::buffers_begin(receive_buffer.data()),
            boost::asio::buffers_begin(receive_buffer.data()) + receive_buffer.size());

        this->CallAfter([this, response]() {
            wxMessageBox(wxString::FromUTF8(response), "Server Response", wxICON_ERROR);
        });

    } catch (std::exception& e) {
        CallAfter([=]() {
            wxMessageBox(wxString::FromUTF8(e.what()), "Connection Error", wxICON_ERROR);
        });
    }
}

class MyApp : public wxApp
{
    public:
        virtual bool OnInit(){
            MyFrame* frame = new MyFrame();
            frame->Show(true);
            return true;
        }
};

wxIMPLEMENT_APP(MyApp);
