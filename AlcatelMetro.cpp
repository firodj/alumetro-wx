///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 26 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>

#include "CRouterListPanel.h"
#include "CTerminalViewPanel.h"
#include "CTopologyViewPanel.h"

#include "AlcatelMetro.h"

///////////////////////////////////////////////////////////////////////////

CMainFrameBase::CMainFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_DEFAULT);
	
	m_menubar2 = new wxMenuBar( 0 );
	m_menu3 = new wxMenu();
	wxMenuItem* m_menuItem1;
	m_menuItem1 = new wxMenuItem( m_menu3, wxID_OPEN, wxString( wxT("Open Network...") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menuItem1->SetBitmaps( wxArtProvider::GetBitmap( wxART_FILE_OPEN, wxART_MENU ) );
	#elif defined( __WXGTK__ )
	m_menuItem1->SetBitmap( wxArtProvider::GetBitmap( wxART_FILE_OPEN, wxART_MENU ) );
	#endif
	m_menu3->Append( m_menuItem1 );
	
	m_menu3->AppendSeparator();
	
	wxMenuItem* m_menuItem2;
	m_menuItem2 = new wxMenuItem( m_menu3, wxID_CLOSE, wxString( wxT("E&xit") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem2 );
	
	m_menubar2->Append( m_menu3, wxT("&File") ); 
	
	m_menu2 = new wxMenu();
	wxMenuItem* m_menuItem3;
	m_menuItem3 = new wxMenuItem( m_menu2, wxID_ABOUT, wxString( wxT("&About") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem3 );
	
	wxMenuItem* m_menuItem4;
	m_menuItem4 = new wxMenuItem( m_menu2, ID_OPENSSH, wxString( wxT("Connect SSH") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem4 );
	
	wxMenuItem* m_menuItem5;
	m_menuItem5 = new wxMenuItem( m_menu2, ID_CLOSESSH, wxString( wxT("Close SSH") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem5 );
	
	m_menu2->AppendSeparator();
	
	wxMenuItem* m_menuItem6;
	m_menuItem6 = new wxMenuItem( m_menu2, ID_TESTING, wxString( wxT("Testing...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem6 );
	
	m_menubar2->Append( m_menu2, wxT("Help") ); 
	
	this->SetMenuBar( m_menubar2 );
	
	mp_toolbar = new wxAuiToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_HORZ_LAYOUT ); 
	mp_toolbar->AddTool( wxID_OPEN, wxT("tool"), wxArtProvider::GetBitmap( wxART_FILE_OPEN, wxART_BUTTON ), wxNullBitmap, wxITEM_NORMAL, wxT("Open"), wxEmptyString, NULL ); 
	
	mp_toolbar->AddTool( wxID_CLOSE, wxT("tool"), wxArtProvider::GetBitmap( wxART_FILE_SAVE, wxART_BUTTON ), wxNullBitmap, wxITEM_NORMAL, wxT("Save"), wxEmptyString, NULL ); 
	
	mp_toolbar->Realize();
	m_mgr.AddPane( mp_toolbar, wxAuiPaneInfo().Top().CaptionVisible( false ).PinButton( true ).Gripper().Dock().Resizable().FloatingSize( wxSize( -1,28 ) ).DockFixed( false ).Layer( 10 ) );
	
	m_statusBar2 = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	mp_treectrl = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
	m_mgr.AddPane( mp_treectrl, wxAuiPaneInfo() .Left() .CaptionVisible( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxSize( 200,-1 ) ).DockFixed( false ).BestSize( wxSize( 200,-1 ) ).MinSize( wxSize( 200,-1 ) ) );
	
	mp_notebook = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_DEFAULT_STYLE );
	m_mgr.AddPane( mp_notebook, wxAuiPaneInfo() .Center() .CaptionVisible( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxSize( -1,-1 ) ).DockFixed( false ).Layer( 1 ) );
	
	m_panelRouterList = new CRouterListPanel( mp_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	mp_notebook->AddPage( m_panelRouterList, wxT("Router List"), false, wxNullBitmap );
	m_panelTopologyView = new CTopologyViewPanel( mp_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	mp_notebook->AddPage( m_panelTopologyView, wxT("Topology"), true, wxNullBitmap );
	m_panelTerminalView = new CTerminalViewPanel( mp_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	mp_notebook->AddPage( m_panelTerminalView, wxT("Terminal Console"), false, wxNullBitmap );
	
	mp_textctrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_mgr.AddPane( mp_textctrl, wxAuiPaneInfo() .Center() .Caption( wxT("Log") ).PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).DockFixed( false ).Row( 0 ).Layer( 1 ) );
	
	
	m_mgr.Update();
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CMainFrameBase::OnClose ) );
	this->Connect( m_menuItem2->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CMainFrameBase::OnQuit ) );
	this->Connect( m_menuItem3->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CMainFrameBase::OnAbout ) );
	this->Connect( m_menuItem4->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CMainFrameBase::OnOpenConnect ) );
	this->Connect( m_menuItem5->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CMainFrameBase::OnCloseConnect ) );
	this->Connect( m_menuItem6->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CMainFrameBase::OnTest ) );
}

CMainFrameBase::~CMainFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CMainFrameBase::OnClose ) );
	this->Disconnect( wxID_CLOSE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CMainFrameBase::OnQuit ) );
	this->Disconnect( wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CMainFrameBase::OnAbout ) );
	this->Disconnect( ID_OPENSSH, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CMainFrameBase::OnOpenConnect ) );
	this->Disconnect( ID_CLOSESSH, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CMainFrameBase::OnCloseConnect ) );
	this->Disconnect( ID_TESTING, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CMainFrameBase::OnTest ) );
	
	m_mgr.UnInit();
	
}

CRouterListPanelBase::CRouterListPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_DEFAULT);
	
	m_panel4 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_panel4, wxAuiPaneInfo() .Top() .CaptionVisible( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).DockFixed( false ).MinSize( wxSize( 28,28 ) ).Layer( 10 ).ToolbarPane() );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	m_auiToolBar5 = new wxAuiToolBar( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_HORZ_LAYOUT ); 
	m_auiToolBar5->AddTool( ID_ADD, wxT("tool"), wxBitmap( wxT("res/tools/add.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Add Router"), wxEmptyString, NULL ); 
	
	m_auiToolBar5->AddTool( ID_DEL, wxT("tool"), wxBitmap( wxT("res/tools/delete.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Delete Router"), wxEmptyString, NULL ); 
	
	m_auiToolBar5->AddSeparator(); 
	
	m_auiToolBar5->AddTool( ID_IMPORT, wxT("tool"), wxBitmap( wxT("res/tools/document_import.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Import from CSV"), wxEmptyString, NULL ); 
	
	m_auiToolBar5->Realize(); 
	
	bSizer4->Add( m_auiToolBar5, 1, wxEXPAND, 5 );
	
	
	m_panel4->SetSizer( bSizer4 );
	m_panel4->Layout();
	bSizer4->Fit( m_panel4 );
	m_listRouters = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	m_mgr.AddPane( m_listRouters, wxAuiPaneInfo() .Center() .CaptionVisible( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).DockFixed( false ) );
	
	m_scrolledWindow1 = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_scrolledWindow1->SetScrollRate( 5, 5 );
	m_mgr.AddPane( m_scrolledWindow1, wxAuiPaneInfo() .Center() .Caption( wxT("Filters") ).PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).DockFixed( false ) );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1 = new wxStaticText( m_scrolledWindow1, wxID_ANY, wxT("Hostname"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer1->Add( m_staticText1, 0, wxALL, 5 );
	
	m_textCtrl2 = new wxTextCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_textCtrl2, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText2 = new wxStaticText( m_scrolledWindow1, wxID_ANY, wxT("IP System"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer1->Add( m_staticText2, 0, wxALL, 5 );
	
	m_textCtrl3 = new wxTextCtrl( m_scrolledWindow1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_textCtrl3, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText3 = new wxStaticText( m_scrolledWindow1, wxID_ANY, wxT("Area"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer1->Add( m_staticText3, 0, wxALL, 5 );
	
	m_comboAreas = new wxBitmapComboBox( m_scrolledWindow1, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer1->Add( m_comboAreas, 0, wxALL|wxEXPAND, 5 );
	
	
	m_scrolledWindow1->SetSizer( fgSizer1 );
	m_scrolledWindow1->Layout();
	fgSizer1->Fit( m_scrolledWindow1 );
	
	m_mgr.Update();
}

CRouterListPanelBase::~CRouterListPanelBase()
{
	m_mgr.UnInit();
	
}

CTopologyViewPanelBase::CTopologyViewPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_DEFAULT);
	
	m_panel2 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_panel2, wxAuiPaneInfo() .Top() .CaptionVisible( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxSize( 400,485 ) ).DockFixed( false ).MinSize( wxSize( 26,26 ) ).Layer( 10 ).ToolbarPane() );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_auiToolBar4 = new wxAuiToolBar( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_HORZ_LAYOUT ); 
	m_auiToolBar4->Realize(); 
	
	bSizer3->Add( m_auiToolBar4, 1, wxEXPAND, 5 );
	
	
	m_panel2->SetSizer( bSizer3 );
	m_panel2->Layout();
	bSizer3->Fit( m_panel2 );
	m_aggcanvas = new wxAggCanvas(this, wxID_ANY);
	m_aggcanvas->AddLine(10, 10, 200, 300);
	m_aggcanvas->AddRectangle(250.5, 100, 450, 200);
	m_mgr.AddPane( m_aggcanvas, wxAuiPaneInfo() .Center() .CaptionVisible( false ).PinButton( true ).Movable( false ).Dock().Resizable().FloatingSize( wxDefaultSize ) );
	
	
	m_mgr.Update();
}

CTopologyViewPanelBase::~CTopologyViewPanelBase()
{
	m_mgr.UnInit();
	
}

CTerminalViewPanelBase::CTerminalViewPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_DEFAULT);
	
	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_panel1, wxAuiPaneInfo() .Top() .CaptionVisible( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxSize( 400,485 ) ).DockFixed( false ).MinSize( wxSize( 26,26 ) ).Layer( 10 ).ToolbarPane() );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_auiToolBar1 = new wxAuiToolBar( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_HORZ_LAYOUT ); 
	m_auiToolBar1->Realize(); 
	
	bSizer1->Add( m_auiToolBar1, 1, wxEXPAND, 5 );
	
	
	m_panel1->SetSizer( bSizer1 );
	m_panel1->Layout();
	bSizer1->Fit( m_panel1 );
	m_terminal = new CTerminal(this, wxID_ANY);
	m_mgr.AddPane( m_terminal, wxAuiPaneInfo() .Center() .CaptionVisible( false ).PinButton( true ).Movable( false ).Dock().Resizable().FloatingSize( wxDefaultSize ) );
	
	
	m_mgr.Update();
}

CTerminalViewPanelBase::~CTerminalViewPanelBase()
{
	m_mgr.UnInit();
	
}
