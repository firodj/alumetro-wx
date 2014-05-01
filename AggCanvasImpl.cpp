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

#include "AggCanvasImpl.h"

wxAggCanvasImpl::wxAggCanvasImpl():
	mp_bitmap(NULL),
	m_rbuf(),
    m_pixFormat(m_rbuf),
    m_renBase(m_pixFormat),
	m_renPrim(m_renBase),
	m_renSolid(m_renBase),
	m_strokeColor(0xFF,0xAE,0xAE),
	m_fillColor(0xB4,0xD8,0xE7)
{
	
}

wxAggCanvasImpl::~wxAggCanvasImpl() 
{
	if (mp_bitmap)
		delete mp_bitmap;

	for (size_t i = 0; i < m_shapes.GetCount(); ++i) {
		wxAggShape* p_shape = (wxAggShape*) m_shapes[i];
		delete p_shape;
	}
}


///////

void wxAggCanvasImpl::DrawShape(wxAggShape& shape) 
{
	switch (shape.m_shape_type) {
		case wxAggShape::AGG_SHAPE_LINE:
			//p_graphic->lineColor(m_stroke_color);
			//p_graphic->line(m_params[0], m_params[1], m_params[2], m_params[3]);
			m_renPrim.line_color( m_strokeColor );
			m_renPrim.line(m_renPrim.coord( shape.m_params[0]), 
				m_renPrim.coord( shape.m_params[1]),
				m_renPrim.coord( shape.m_params[2]),
				m_renPrim.coord( shape.m_params[3]));
			break;
		case wxAggShape::AGG_SHAPE_RECT:
			//p_graphic->lineColor(m_stroke_color);
			//p_graphic->fillColor(m_fill_color);
			//p_graphic->rectangle(m_params[0], m_params[1], m_params[2], m_params[3]);
			break;
	}
}

bool wxAggShape::IsHit(int x, int y) {
	switch (m_shape_type) {
		case wxAggShape::AGG_SHAPE_LINE:
			break;
	}
	return false;

}