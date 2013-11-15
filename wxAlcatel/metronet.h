#pragma once

#include <wx/docview.h>
#include <wx/scrolwin.h>

class CMetronetDoc: public wxDocument
{
public:
	CMetronetDoc(): wxDocument() {};

private:

	DECLARE_DYNAMIC_CLASS(CMetronetDoc)
};

class CMetronetCanvas;

class CMetronetView: public wxView
{
public:
	CMetronetView(): wxView() {};
	void OnDraw(wxDC *dc);
	bool OnCreate(wxDocument *doc, long flags);
	bool OnClose(bool deleteWindow);
	void OnUpdate(wxView *sender, wxObject *hint=0);

private:
	CMetronetCanvas* m_canvas;

	DECLARE_EVENT_TABLE()
	DECLARE_DYNAMIC_CLASS(CMetronetView)
};

class CMetronetCanvas : public wxScrolledCanvas
{
public:
	CMetronetCanvas(CMetronetView *view);
	void OnDraw(wxDC& dc);

private:
	CMetronetView *m_view;
};