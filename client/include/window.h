#pragma once
#include <wx/wx.h>

class MyFrame : public wxFrame
{
public:
    MyFrame();

private:
    wxTextCtrl* useridCtrl;
    wxTextCtrl* passwordCtrl;

    void OnLoginClick(wxCommandEvent& event);
    void LoginWithCredentials(const std::string& user, const std::string& pass);
};
