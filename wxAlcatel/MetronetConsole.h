#pragma once

#include <wx/docview.h>
#include "metronet.h"
#include "MyTerminal.h"

class CMetronetConsole: public wxView
{
public:
	CMetronetConsole();
	void OnDraw(wxDC *dc);
	bool OnCreate(wxDocument *doc, long flags);
	bool OnClose(bool deleteWindow);
	void OnUpdate(wxView *sender, wxObject *hint=0);

private:
	MyTerminal *m_terminal;

	DECLARE_EVENT_TABLE()
	DECLARE_DYNAMIC_CLASS(CMetronetView)
};

