///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 26 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __ALCATELMETRO_H__
#define __ALCATELMETRO_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
class CRouterListPanel;
class CTerminalViewPanel;
class CTopologyViewPanel;

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/aui/aui.h>
#include <wx/aui/auibar.h>
#include <wx/statusbr.h>
#include <wx/treectrl.h>
#include <wx/panel.h>
#include <wx/aui/auibook.h>
#include <wx/textctrl.h>
#include <wx/frame.h>
#include <wx/sizer.h>
#include <wx/listctrl.h>
#include <wx/stattext.h>
#include <wx/bmpcbox.h>
#include <wx/scrolwin.h>
#include "AggCanvas.h"
#include "CTerminal.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class CMainFrameBase
///////////////////////////////////////////////////////////////////////////////
class CMainFrameBase : public wxFrame 
{
	private:
	
	protected:
		enum
		{
			ID_OPENSSH = 1000,
			ID_CLOSESSH,
			ID_TESTING
		};
		
		wxMenuBar* m_menubar2;
		wxMenu* m_menu3;
		wxMenu* m_menu2;
		wxAuiToolBar* mp_toolbar;
		wxStatusBar* m_statusBar2;
		wxTreeCtrl* mp_treectrl;
		wxAuiNotebook* mp_notebook;
		CRouterListPanel* m_panelRouterList;
		CTopologyViewPanel* m_panelTopologyView;
		CTerminalViewPanel* m_panelTerminalView;
		wxTextCtrl* mp_textctrl;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnQuit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOpenConnect( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCloseConnect( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTest( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		CMainFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Alcatel Metro 2014"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 679,425 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		wxAuiManager m_mgr;
		
		~CMainFrameBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class CRouterListPanelBase
///////////////////////////////////////////////////////////////////////////////
class CRouterListPanelBase : public wxPanel 
{
	private:
	
	protected:
		enum
		{
			ID_ADD = 1000,
			ID_DEL,
			ID_IMPORT
		};
		
		wxPanel* m_panel4;
		wxAuiToolBar* m_auiToolBar5;
		wxListCtrl* m_listRouters;
		wxScrolledWindow* m_scrolledWindow1;
		wxStaticText* m_staticText1;
		wxTextCtrl* m_textCtrl2;
		wxStaticText* m_staticText2;
		wxTextCtrl* m_textCtrl3;
		wxStaticText* m_staticText3;
		wxBitmapComboBox* m_comboAreas;
	
	public:
		
		CRouterListPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 530,334 ), long style = wxTAB_TRAVERSAL ); wxAuiManager m_mgr;
		
		~CRouterListPanelBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class CTopologyViewPanelBase
///////////////////////////////////////////////////////////////////////////////
class CTopologyViewPanelBase : public wxPanel 
{
	private:
	
	protected:
		wxPanel* m_panel2;
		wxAuiToolBar* m_auiToolBar4;
		wxAggCanvas *m_aggcanvas;
	
	public:
		
		CTopologyViewPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL ); wxAuiManager m_mgr;
		
		~CTopologyViewPanelBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class CTerminalViewPanelBase
///////////////////////////////////////////////////////////////////////////////
class CTerminalViewPanelBase : public wxPanel 
{
	private:
	
	protected:
		wxPanel* m_panel1;
		wxAuiToolBar* m_auiToolBar1;
		CTerminal *m_terminal;
	
	public:
		
		CTerminalViewPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL ); wxAuiManager m_mgr;
		
		~CTerminalViewPanelBase();
	
};

#endif //__ALCATELMETRO_H__
