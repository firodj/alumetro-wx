//
// file name: wxAlcatel.h
//
#pragma once

// Define a new application type, each program should derive a class from wxApp
class MyApp: public wxApp
{
public:
	// override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();
    
    // this is called after destroying all application windows and controls, but 
    // before wxWidgets cleanup. Note that it is not called at all if OnInit() failed.
    // The return value of this function is currently ignored, return the same value 
    // as returned by the base class method.
    virtual int OnExit();
	void OnAbout(wxCommandEvent& event);

private:
	//struct ssh_threads_callbacks_struct m_ssh_threads_cbs;

	DECLARE_EVENT_TABLE()
};

wxDECLARE_APP(MyApp);
