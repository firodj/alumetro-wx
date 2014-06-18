//
// file name: wxAlcatel.h
//
#pragma once

class CApp: public wxApp
{
public:
	virtual bool OnInit();
    
    virtual int OnExit();

private:

	//DECLARE_EVENT_TABLE()
};

wxDECLARE_APP(CApp);
