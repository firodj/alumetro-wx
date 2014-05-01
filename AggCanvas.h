#pragma once

class wxAggCanvasImpl;

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
	void StrokeColor(unsigned r, unsigned g, unsigned b, unsigned a=255);
	void AddLine(double x1, double y1, double x2, double y2);
	void AddRectangle(double x1, double y1, double x2, double y2);

private:
	wxAggCanvasImpl& impl;

	DECLARE_EVENT_TABLE()
};
