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

#include "wx/docview.h"
#include "wx/docmdi.h"
#include <wx/app.h> 

#include "app.h"
#include "metronet.h"

IMPLEMENT_DYNAMIC_CLASS(CMetronetView, wxView)

BEGIN_EVENT_TABLE(CMetronetView, wxView)
    //EVT_MENU(wxID_CUT, DrawingView::OnCut)
END_EVENT_TABLE()

bool CMetronetView::OnCreate(wxDocument *doc, long flags)
{
	if ( !wxView::OnCreate(doc, flags) )
        return false;
	
	MyApp& app = wxGetApp();
	wxFrame *frame = new wxDocMDIChildFrame
                       (
                            doc,
                            this,
                            wxStaticCast(app.GetTopWindow(), wxDocMDIParentFrame),
                            wxID_ANY,
                            "Child Frame",
                            wxDefaultPosition,
                            wxSize(300, 300)
                       );

	m_canvas = new CMetronetCanvas(this);

	frame->Show();

	return true;
}

void CMetronetView::OnDraw(wxDC *dc)
{
	dc->SetBrush(*wxBLACK_BRUSH);
	dc->DrawRectangle(10, 10, 100, 100);
}

bool CMetronetView::OnClose(bool deleteWindow)
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

void CMetronetView::OnUpdate(wxView *sender, wxObject *hint)
{
	 wxView::OnUpdate(sender, hint);
}

///////////////////////////////////////////////////////////////////////////////

CMetronetCanvas::CMetronetCanvas(CMetronetView *view):
	wxScrolledCanvas(view->GetFrame()),
	m_view(view)
{
	SetVirtualSize(1000, 1000);
    SetScrollRate(20, 20);

    SetBackgroundColour(*wxWHITE);
}

void CMetronetCanvas::OnDraw(wxDC& dc)
{
    if ( m_view )
        m_view->OnDraw(& dc);
}