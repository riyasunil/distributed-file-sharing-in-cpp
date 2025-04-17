#include <wx/wx.h>

class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        wxFrame *frame = new wxFrame(nullptr, wxID_ANY, "Hello, my name is Riya!");
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
