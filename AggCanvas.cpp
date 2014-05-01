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
#include "AggCanvasImpl.h"

//#include "platform/agg_platform_support.h"
//#include "agg2d.h"
//#include "platform/win32/agg_win32_bmp.h"

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
	impl(*new wxAggCanvasImpl)
	//mp_graphic(new Agg2D())
{

}

wxAggCanvas::~wxAggCanvas()
{
	//delete mp_graphic;
	delete &impl;
}

void wxAggCanvas::OnPaint(wxPaintEvent& WXUNUSED(evt))
{
	wxPaintDC dc(this);

	Draw();

	if (impl.mp_bitmap)
	{
		wxMemoryDC dc_mem(*impl.mp_bitmap);
		dc.Blit( wxPoint(0, 0), dc_mem.GetSize(), &dc_mem, wxPoint(0, 0) );
	}
}

void wxAggCanvas::OnMouseLeftDown(wxMouseEvent &evt)
{
	wxPoint pt_mouse = evt.GetPosition();
	double x = pt_mouse.x, y = pt_mouse.y;
	//mp_graphic->screenToWorld(x,y);

	for (size_t i=0; i<impl.m_shapes.GetCount(); ++i) {
		wxAggShape* p_shape = (wxAggShape*) impl.m_shapes[i];
		p_shape->IsHit(pt_mouse.x, pt_mouse.y);
	}

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

	if (impl.mp_bitmap) {
		if (w != impl.mp_bitmap->GetWidth() || h != impl.mp_bitmap->GetHeight()) {
			delete impl.mp_bitmap;
			impl.mp_bitmap = NULL;
		}
	}

	if (!impl.mp_bitmap && h>0 && w>0) {
		impl.mp_bitmap = new wxBitmap(w, h, 32);	//rgba
		dirty = true;
	}

	if (impl.mp_bitmap) {
		//expand from: wxNativePixelData data(*mp_bitmap);
		CPixelData pix_data;
		unsigned char *p_buf = (unsigned char *)impl.mp_bitmap->GetRawData(pix_data, impl.mp_bitmap->GetDepth());

		// reverse this equation: bits += (h - 1)*bytesPerRow;
		p_buf -= (pix_data.GetHeight()-1)*-pix_data.GetRowStride();

		impl.m_rbuf.attach(p_buf, pix_data.GetWidth(), pix_data.GetHeight(), pix_data.GetRowStride());
		impl.m_renBase.reset_clipping(true);
		impl.m_renBase.clear( AggColor(255,255,255) );

		//impl.m_renPrim.square( 50, 50, 5 );
		//mp_graphic->attach(p_buf, pix_data.GetWidth(), pix_data.GetHeight(), pix_data.GetRowStride());
		//mp_graphic->clearAll(255, 255, 255);

		//mp_graphic->viewport(0, 0, w*2, h*2,
        //                    0, 0, w, h,
        //                     Agg2D::XMidYMid);//Agg2D::Anisotropic

		AggPathStorage path;
		AggConvCurve convcurve(path);
		AggConvStroke convstroke(convcurve);

		path.remove_all();
		path.move_to(20, 100);
		path.line_to(300, 20);
		path.line_to(200, 220);
		convstroke.width(2.0);
		
		impl.m_rasterizer.reset();
		impl.m_rasterizer.add_path(convcurve);
		impl.m_renSolid.color( impl.m_fillColor );

		agg::render_scanlines(impl.m_rasterizer, impl.m_scanline, impl.m_renSolid);

		impl.m_rasterizer.reset();
		impl.m_rasterizer.add_path(convstroke);
		impl.m_renSolid.color( impl.m_strokeColor );
		
		agg::render_scanlines(impl.m_rasterizer, impl.m_scanline, impl.m_renSolid);

		for (size_t i=0; i < impl.m_shapes.GetCount(); ++i) {
			impl.DrawShape( *(wxAggShape*)impl.m_shapes[i]);
		}

		//mp_graphic->flipText(true);
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
	impl.m_strokeColor.r = r;
	impl.m_strokeColor.g = g;
	impl.m_strokeColor.b = b;
	impl.m_strokeColor.a = a;
}

void wxAggCanvas::AddLine(double x1, double y1, double x2, double y2)
{
	wxAggShape *p_shape = new wxAggShape(wxAggShape::AGG_SHAPE_LINE);
	p_shape->m_params.Add( x1 );
	p_shape->m_params.Add( y1 );
	p_shape->m_params.Add( x2 );
	p_shape->m_params.Add( y2 );
	p_shape->m_stroke_color = impl.m_strokeColor;
	impl.m_shapes.Add( p_shape );
}

void wxAggCanvas::AddRectangle(double x1, double y1, double x2, double y2)
{
	wxAggShape *p_shape = new wxAggShape(wxAggShape::AGG_SHAPE_RECT);
	p_shape->m_params.Add( x1 );
	p_shape->m_params.Add( y1 );
	p_shape->m_params.Add( x2 );
	p_shape->m_params.Add( y2 );
	p_shape->m_stroke_color = impl.m_strokeColor;
	p_shape->m_fill_color	= impl.m_fillColor; 
	impl.m_shapes.Add( p_shape );
}
