#include "APP.h"
#include "MainWindow.h"

bool CAPP::OnInit()
{
    
    CMainWindow *frame = new CMainWindow( _("Memory Manager 3000"), wxPoint(50, 50), wxSize(800, 420),wxCAPTION);
    frame->Center();
    frame->Show(true);
    frame->SetMaxSize(wxSize(800,420));
    frame->SetMinSize(wxSize(800,420));
    SetTopWindow(frame);
    return true;
}