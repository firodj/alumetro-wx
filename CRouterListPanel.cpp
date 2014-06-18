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

#include "CRouterListPanel.h"

enum {
	COL_Hostname = 0,
	COL_Chassis,
	COL_IpSystem,
	COL_PartNo,
	COL_SerialNo,
	COL_MftDate,
	
};

CRouterListPanel::CRouterListPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
:
CRouterListPanelBase( parent )
{
	m_listRouters->AppendColumn( "Hostname");
	m_listRouters->AppendColumn( "Chassis");
	m_listRouters->AppendColumn( "IP System");
	m_listRouters->AppendColumn( "Part No.");
	m_listRouters->AppendColumn( "S/N");
	m_listRouters->AppendColumn( "Manufacturing");
	

}
