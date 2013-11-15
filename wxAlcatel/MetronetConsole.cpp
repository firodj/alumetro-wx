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

#include "wx/docview.h"
#include "wx/docmdi.h"
#include <wx/app.h> 

#include "app.h"
#include "MetronetConsole.h"

///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(CMetronetConsole, wxView)

BEGIN_EVENT_TABLE(CMetronetConsole, wxView)
END_EVENT_TABLE()

CMetronetConsole::CMetronetConsole(): wxView()
{


}

bool CMetronetConsole::OnCreate(wxDocument *doc, long flags)
{
	if ( !wxView::OnCreate(doc, flags) )
        return false;
	
	wxDocMDIChildFrame  *frame = new wxDocMDIChildFrame(
        doc,	// wxDocument
        this,	// wxView
        wxStaticCast(wxGetApp().GetTopWindow(), wxDocMDIParentFrame),	// wxMDIParentFrame
        wxID_ANY,	// wxWindowID
        "Console Frame",	// wxString title
        wxDefaultPosition,	// wxPoint pos
        wxSize(300, 300)	// wxSize size
		);

	m_terminal = new MyTerminal(frame, wxID_ANY);

	frame->Show();

	return true;

}

void CMetronetConsole::OnDraw(wxDC *dc)
{
}

bool CMetronetConsole::OnClose(bool deleteWindow)
{
	if ( !wxView::OnClose(deleteWindow) )
        return false;

    Activate(false);

	if ( deleteWindow )
    {
        GetFrame()->Destroy();
        SetFrame(NULL);
    }

	return true;
}

void CMetronetConsole::OnUpdate(wxView *sender, wxObject *hint)
{
	wxView::OnUpdate(sender, hint);
}