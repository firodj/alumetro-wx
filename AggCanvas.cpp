#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/rawbmp.h>
//#include <wx/arrimpl.cpp> // This is a magic incantation which must be done!WX_DEFINE_OBJARRAY(ArrayOfDirectories);

#include "AggCanvas.h"

#include "platform/agg_platform_support.h"
#include "agg2d.h"
//#include "platform/win32/agg_win32_bmp.h"

class wxAggShape
{
public:	
	enum e_shape {
		AGG_SHAPE_LINE,
		AGG_SHAPE_RECT
	};

	wxAggShape(enum e_shape shape_type): m_shape_type(shape_type) {};
	~wxAggShape() {};

	enum e_shape m_shape_type;
	wxArrayDouble m_params;

	Agg2D::Color m_stroke_color;
	Agg2D::Color m_fill_color;
	float m_stroke_width;

	void Draw(Agg2D *p_graphic);
	bool IsHit(int x, int y);
};

WX_DECLARE_OBJARRAY(wxAggShape, Array_AggShape);

// this is helper class
class CPixelData: public wxPixelDataBase {
public:
	CPixelData(): wxPixelDataBase() {}
};

BEGIN_EVENT_TABLE(wxAggCanvas, wxWindow)
	EVT_PAINT(wxAggCanvas::OnPaint)
	EVT_ERASE_BACKGROUND(wxAggCanvas::OnEraseBackground)
	//EVT_CHAR(wxAggCanvas::OnChar)
	//EVT_SIZE(wxAggCanvas::OnSize)
	EVT_SCROLLWIN(wxAggCanvas::OnScrollBar)
	EVT_LEFT_DOWN(wxAggCanvas::OnMouseLeftDown)
	EVT_MOTION(wxAggCanvas::OnMouseMove)
	//EVT_MOUSEWHEEL(wxAggCanvas::OnMouseWheel)
	//EVT_TIMER(TIMER_ID, wxAggCanvas::OnTimer)
	//EVT_SET_FOCUS(wxAggCanvas::OnSetFocus)
	//EVT_KILL_FOCUS(wxAggCanvas::OnKillFocus)
END_EVENT_TABLE()

wxAggCanvas::wxAggCanvas(wxWindow *parent, wxWindowID id):
	wxWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxVSCROLL| wxHSCROLL|wxWANTS_CHARS ),
	mp_bitmap(NULL),
	mp_graphic(new Agg2D())
{
	m_stroke_r = 0;
	m_stroke_g = 0;
	m_stroke_b = 0;
	m_stroke_a = 255;

	m_fill_r = 255;
	m_fill_g = 255;
	m_fill_b = 255;
	m_fill_a = 255;
}

wxAggCanvas::~wxAggCanvas()
{
	delete mp_graphic;

	if (mp_bitmap)
		delete mp_bitmap;
	for (size_t i = 0; i<m_shapes.GetCount(); ++i) {
		wxAggShape* p_shape = (wxAggShape*) m_shapes[i];
		delete p_shape;
	}
}

void wxAggCanvas::OnPaint(wxPaintEvent& WXUNUSED(evt))
{
	wxPaintDC dc(this);

	Draw();

	if (mp_bitmap)
	{
		wxMemoryDC dc_mem(*mp_bitmap);
		dc.Blit( wxPoint(0, 0), dc_mem.GetSize(), &dc_mem, wxPoint(0, 0) );
	}
}

void wxAggCanvas::OnMouseLeftDown(wxMouseEvent &evt)
{
	wxPoint pt_mouse = evt.GetPosition();
	for (size_t i=0; i<m_shapes.GetCount(); ++i) {
		wxAggShape* p_shape = (wxAggShape*) m_shapes[i];
		p_shape->IsHit(pt_mouse.x, pt_mouse.y);
	}

	double x = pt_mouse.x, y = pt_mouse.y;
	wxLogDebug("mouse (%f, %f)", x, y);
	mp_graphic->worldToScreen(x,y);
	wxLogDebug("s2w (%f, %f)", x, y);
}

void wxAggCanvas::OnMouseMove(wxMouseEvent &evt)
{

}

void wxAggCanvas::Draw()
{
	int w=0,h=0;
	bool dirty = false;

	GetClientSize(&w,&h);
	w = ((w & 0xff) == 0) ? w : (w | 0xff)+1;
	h = ((h & 0xff) == 0) ? h : (h | 0xff)+1;

	if (mp_bitmap) {
		if (w != mp_bitmap->GetWidth() || h != mp_bitmap->GetHeight()) {
			delete mp_bitmap;
			mp_bitmap = NULL;
		}
	}

	if (!mp_bitmap && h>0 && w>0) {
		mp_bitmap = new wxBitmap(w, h, 32);
		dirty = true;
	}

	if (mp_bitmap) {
		//expand from: wxNativePixelData data(*mp_bitmap);
		CPixelData pix_data;
		unsigned char *p_buf = (unsigned char *)mp_bitmap->GetRawData(pix_data, mp_bitmap->GetDepth());

		// reverse this: bits += (h - 1)*bytesPerRow;
		p_buf -= (pix_data.GetHeight()-1)*-pix_data.GetRowStride();

		mp_graphic->attach(p_buf, pix_data.GetWidth(), pix_data.GetHeight(), -pix_data.GetRowStride());
		mp_graphic->clearAll(255, 255, 255);

		mp_graphic->viewport(0, 400-h, w, 400,
                            0, 0, w, h,
                            //Agg2D::Anisotropic);
                            Agg2D::XMidYMid);

		for (size_t i=0; i<m_shapes.GetCount(); ++i) {
			wxAggShape* p_shape = (wxAggShape*) m_shapes[i];
			p_shape->Draw(mp_graphic);
		}

		//mp_graphic->font("Verdana", 20.0, false, false, Agg2D::VectorFontCache);
        //mp_graphic->lineColor(50, 0, 0);
        //mp_graphic->fillColor(180, 200, 100);
        //mp_graphic->lineWidth(0.4);
        //mp_graphic->text(100, 20, "Regular Raster Text -- Fast, but can't be rotated");
	}
}


void wxAggCanvas::OnSize(wxSizeEvent &evt)
{
	Refresh(false);
}

void wxAggCanvas::OnScrollBar(wxScrollWinEvent &evt)
{
	
}

void wxAggCanvas::OnEraseBackground(wxEraseEvent &evt)
{
	wxDC *p_dc = evt.GetDC();
	// nothing to do
}

void wxAggCanvas::StrokeColor(unsigned r, unsigned g, unsigned b, unsigned a)
{
	m_stroke_r = r;
	m_stroke_g = g;
	m_stroke_b = b;
	m_stroke_a = a;
}

void wxAggCanvas::AddLine(double x1, double y1, double x2, double y2)
{
	wxAggShape *p_shape = new wxAggShape(wxAggShape::AGG_SHAPE_LINE);
	p_shape->m_params.Add( x1 );
	p_shape->m_params.Add( y1 );
	p_shape->m_params.Add( x2 );
	p_shape->m_params.Add( y2 );
	p_shape->m_stroke_color = Agg2D::Color( m_stroke_r, m_stroke_g, m_stroke_b, m_stroke_a );
	m_shapes.Add( p_shape );
}

void wxAggCanvas::AddRectangle(double x1, double y1, double x2, double y2)
{
	wxAggShape *p_shape = new wxAggShape(wxAggShape::AGG_SHAPE_RECT);
	p_shape->m_params.Add( x1 );
	p_shape->m_params.Add( y1 );
	p_shape->m_params.Add( x2 );
	p_shape->m_params.Add( y2 );
	p_shape->m_stroke_color = Agg2D::Color( m_stroke_r, m_stroke_g, m_stroke_b, m_stroke_a );
	p_shape->m_fill_color = Agg2D::Color( m_fill_r, m_fill_g, m_fill_b, m_fill_a );
	m_shapes.Add( p_shape );
}

///////

void wxAggShape::Draw(Agg2D *p_graphic) {
	switch (m_shape_type) {
		case wxAggShape::AGG_SHAPE_LINE:
			p_graphic->lineColor(m_stroke_color);
			p_graphic->line(m_params[0], m_params[1], m_params[2], m_params[3]);
			break;
		case wxAggShape::AGG_SHAPE_RECT:
			p_graphic->lineColor(m_stroke_color);
			p_graphic->fillColor(m_fill_color);
			p_graphic->rectangle(m_params[0], m_params[1], m_params[2], m_params[3]);
			break;
	}
}

bool wxAggShape::IsHit(int x, int y) {

	return false;

}