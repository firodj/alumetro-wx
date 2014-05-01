//
// file name: wxAlcatel.cpp
//
//   purpose: wxAlcatel wxWidgets application
//

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

///////////////////////////////////////////////////////////////////////////////

#include <wx/xrc/xmlres.h>

#include "libssh2.h"

#include "App.h"
#include "MainFrame.h"
#include "wx/wxsqlite3.h"

///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_APP(CApp)

BEGIN_EVENT_TABLE(CApp, wxApp)
    EVT_MENU(wxID_ABOUT, CApp::OnAbout)
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////

bool CApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

#ifdef WIN32
	WSADATA wsadata;
    WSAStartup(MAKEWORD(2,0), &wsadata);
#endif
	libssh2_init(0);

    // init Image Handlre and XML Resource
    wxImage::AddHandler(new wxGIFHandler);
    wxXmlResource::Get()->InitAllHandlers();
    if (!wxXmlResource::Get()->Load("*.xrc"))
        wxLogError("Couldn't load resources!");

	// create the main application window
    CMainFrame *pMainframe = new CMainFrame();

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    pMainframe->Show(true);
    SetTopWindow(pMainframe);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
	
	wxSQLite3Database::InitializeSQLite();

    return true;
};

int CApp::OnExit()
{
	wxSQLite3Database::ShutdownSQLite();

	libssh2_exit();
	
    // return the standard exit code
    return wxApp::OnExit();
}

void CApp::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxString::Format(
                    wxT("Welcome to %s!\n")
                    wxT("\n")
                    wxT("This is the wxAlcatel wxWidgets application\n")
                    wxT("running under %s."),
                    wxVERSION_STRING,
                    wxGetOsDescription().c_str()
                 ),
                 wxT("About wxAlcatel wxWidgets application"),
                 wxOK | wxICON_INFORMATION,
                 NULL);
};

