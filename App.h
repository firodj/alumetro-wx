//
// file name: wxAlcatel.h
//
#pragma once

class CApp: public wxApp
{
public:
	virtual bool OnInit();
    
    virtual int OnExit();

	void OnAbout(wxCommandEvent& event);

	void OnConnect(wxCommandEvent& event);

private:

	DECLARE_EVENT_TABLE()
};

wxDECLARE_APP(CApp);
