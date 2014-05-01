//
// file name: my_main_frame.h
//
#pragma once

#include <wx/aui/framemanager.h>
#include <wx/aui/auibar.h>
#include <wx/aui/auibook.h>
#include <wx/dataview.h>
#include "Document.h"

///////////////////////////////////////////////////////////////////////////////

class CTerminal;

// Define a new frame type: this is going to be our main frame
class CMainFrame : public wxFrame
{
public:
    // ctor(s)
    CMainFrame();
	~CMainFrame();
	
    // event handlers (these functions should _not_ be virtual)
	void OnOpenConnect(wxCommandEvent& event);
	void OnCloseConnect(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	void OnTest(wxCommandEvent& event);
	void OnClose(wxCloseEvent& evt);
	
	wxDataViewTreeCtrl *mp_treectrl;
	wxDataViewListCtrl *mp_listctrl;
	
	wxTextCtrl *mp_textctrl;
	static wxLogTextCtrl *g_logtextctrl;

private:
	wxAuiManager m_mgr;
	wxAuiToolBar *mp_toolbar;
	wxAuiNotebook *mp_notebook;
	CTerminal *mp_terminal;
	CDocument m_doc;
	
    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()
};
