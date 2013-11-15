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
#include <wx/docmdi.h>

#include "metronet.h"
#include "libssh2.h"

#include "app.h"
#include "my_main_frame.h"

///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_APP(MyApp)

BEGIN_EVENT_TABLE(MyApp, wxApp)
    EVT_MENU(wxID_ABOUT, MyApp::OnAbout)
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////

bool MyApp::OnInit()
{
    // call the base class initialization method, currently it only parses a
    // few common command-line options but it could be do more in the future
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

	wxDocManager *docManager = new wxDocManager;
	new wxDocTemplate(docManager, "Drawing", "*.met", "", "met",
                      "Drawing Doc", "Drawing View",
                      CLASSINFO(CMetronetDoc), CLASSINFO(CMetronetView));

	wxFrame *frame = new wxDocMDIParentFrame(docManager, NULL, wxID_ANY,
                                        GetAppDisplayName(),
                                        wxDefaultPosition,
                                        wxSize(500, 400));
#if 0
	// create the main application window
    MyMainFrame *main_frame = new MyMainFrame();

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    main_frame->Show(true);
    SetTopWindow(main_frame);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
#endif

	wxMenu *menuFile = new wxMenu;

    menuFile->Append(wxID_NEW);
    menuFile->Append(wxID_OPEN);
	menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

	docManager->FileHistoryUseMenu(menuFile);
    //docManager->FileHistoryLoad(*wxConfig::Get());

	wxMenuBar *menubar = new wxMenuBar;

    menubar->Append(menuFile, wxGetStockLabel(wxID_FILE));
	wxMenu *help= new wxMenu;
    help->Append(wxID_ABOUT);
    menubar->Append(help, wxGetStockLabel(wxID_HELP));

    frame->SetMenuBar(menubar);

	frame->Centre();
    frame->Show();

    return true;
};

int MyApp::OnExit()
{
	wxDocManager * const manager = wxDocManager::GetDocumentManager();
	//manager->FileHistorySave(*wxConfig::Get());
	delete manager;

	libssh2_exit();
	
    // return the standard exit code
    return wxApp::OnExit();
}

void MyApp::OnAbout(wxCommandEvent& WXUNUSED(event))
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