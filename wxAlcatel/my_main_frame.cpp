//
// file name: my_main_frame.cpp
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

#include "app.h"
#include "my_main_frame.h"
#include "MyTerminal.h"
#include "wx_serenum.h"

///////////////////////////////////////////////////////////////////////////////

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyMainFrame, wxFrame)
    EVT_MENU(XRCID("menu_exit"),  MyMainFrame::OnQuit)
    EVT_MENU(XRCID("menu_about"), MyMainFrame::OnAbout)
	EVT_MENU(XRCID("menu_test"), MyMainFrame::OnTest)
	EVT_CLOSE( MyMainFrame::OnClose )
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////

wxLogTextCtrl *MyMainFrame::g_logtextctrl = NULL;

// frame constructor
MyMainFrame::MyMainFrame(): m_mgr(this)
{
    // Load from Resources
    wxXmlResource::Get()->LoadFrame(this, NULL, "main_frame");
    wxXmlResource::Get()->LoadMenuBar(this, "main_menu");
    // set the frame icon
    SetIcon(wxICON(APPICON));
	SetClientSize(800, 600);
	
	mp_textctrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 
		wxTE_MULTILINE | wxTE_READONLY);
		
	mp_treectrl = new wxDataViewTreeCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxDV_NO_HEADER);
	mp_notebook = new wxAuiNotebook(this, wxID_ANY);
	mp_listctrl = new wxDataViewListCtrl(this, wxID_ANY);
	mp_toolbar = new wxAuiToolBar(this, wxID_ANY);

	mp_treectrl->SetSize(300, 600);
	mp_textctrl->SetSize(800, 200);

	mp_terminal = new MyTerminal(this, wxID_ANY);
	mp_notebook->AddPage( mp_terminal, "Term 1");

	wxVideoTerminal *p_terminal = new wxVideoTerminal(this, wxID_ANY);
	mp_notebook->AddPage( p_terminal, "Termi 2");

	mp_notebook->AddPage( mp_listctrl, "List");

	mp_toolbar->AddTool(1, "New", wxXmlResource::Get()->LoadBitmap("my_new_bitmap"), "Create new document");
	mp_toolbar->Realize();
	mp_treectrl->AppendContainer(wxDataViewItem(), "Root");

	m_mgr.AddPane(mp_toolbar, wxAuiPaneInfo().Top().ToolbarPane());
	m_mgr.AddPane(mp_treectrl, wxAuiPaneInfo().Left().Dock().CaptionVisible(false));
	m_mgr.AddPane(mp_notebook, wxAuiPaneInfo().Center().Dock().CaptionVisible(false));
	m_mgr.AddPane(mp_textctrl, wxAuiPaneInfo().Bottom().Dock().Caption("Log"));

	m_mgr.Update();
	
	MyMainFrame::g_logtextctrl = new wxLogTextCtrl(mp_textctrl);
	wxLog::SetActiveTarget( g_logtextctrl );
	wxLogMessage("Ready.");
}

MyMainFrame::~MyMainFrame()
{
	m_mgr.UnInit();
}

// event handlers

void MyMainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void MyMainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
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
                 this);
}


void MyMainFrame::OnTest(wxCommandEvent& WXUNUSED(event))
{
	//mp_terminal->OpenSSH("logingw01.datacomm.co.id", "fadhil.mandaga", "P>9pejEJ12_<", 22);
	//mp_terminal->SetFocus();
	//mp_terminal->OpenSSH("localhost", "fadhil.mandaga", "muhammad", 22);
	mp_terminal->Process("1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30");
	wxString gero("12345678\x00z", 10);
	const char *test = gero.c_str();
	wxLogMessage( "Len:%d, %s", gero.Len(), test+9);
}

void MyMainFrame::OnClose(wxCloseEvent& evt)
{
	if (mp_terminal)
		mp_terminal->CloseSSH();
	evt.Skip();
}