#include <wx/wx.h>

class MyFrame : public wxFrame
{
    public:
        MyFrame() : wxFrame(nullptr, wxID_ANY, "CalicoFS", wxDefaultPosition, wxSize(800,600)){
        wxPanel* panel = new wxPanel(this, wxID_ANY);

        wxTextCtrl* textCtrl = new wxTextCtrl(panel, wxID_ANY, wxT("1"), wxPoint(50, 50), wxSize(200, 40));
    }

};

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
