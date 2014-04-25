#pragma once

#include <wx/dynarray.h>

class Agg2D;

class wxAggCanvas: public wxWindow
{
public:
	wxAggCanvas();
	wxAggCanvas(wxWindow *parent, wxWindowID id);
	~wxAggCanvas();

	// Event Handler
	void OnPaint(wxPaintEvent& WXUNUSED(evt));
	void OnEraseBackground(wxEraseEvent &evt);
	void OnSize(wxSizeEvent &evt);
	void OnScrollBar(wxScrollWinEvent &evt);
	void OnMouseLeftDown(wxMouseEvent &evt);
	void OnMouseMove(wxMouseEvent &evt);

	// Methods
	void Draw();
	void StrokeColor(unsigned r, unsigned g, unsigned b, unsigned a);
	void AddLine(double x1, double y1, double x2, double y2);
	void AddRectangle(double x1, double y1, double x2, double y2);

private:
	// forward declaration -- pimpl
	Agg2D *mp_graphic;

	wxBitmap *mp_bitmap;
	wxArrayPtrVoid m_shapes;

	unsigned m_stroke_r;
	unsigned m_stroke_g;
	unsigned m_stroke_b;
	unsigned m_stroke_a;

	unsigned m_fill_r;
	unsigned m_fill_g;
	unsigned m_fill_b;
	unsigned m_fill_a;

	DECLARE_EVENT_TABLE()
};
