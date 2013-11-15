#pragma once

#include "vidterm.h"
#include <wx/socket.h>

class MyTermThread;
class MyTermConnProp;

///////////////////////////////////////////////////////////////////////////////

class MyTerminal: public wxVideoTerminal
{
public:
	MyTerminal(wxWindow *parent, wxWindowID id);
	~MyTerminal();

	virtual bool Destroy();
	virtual void DoSendChar(wxChar chr, int code);

	bool OpenSSH(wxString hosname, wxString username=wxEmptyString, wxString password=wxEmptyString, int port=22);
	bool CloseSSH();

	void OnCloseConnect(wxCommandEvent &evt);
	void OnOpenConnect(wxCommandEvent &evt);
	void OnTermReceive(wxCommandEvent &evt);
	
	void OnSocket(wxSocketEvent &evt);
	void OnMouseRightClick(wxMouseEvent &evt);

	void OnTermThreadDone(wxCommandEvent &evt);

	bool StartInitSSH();

	void PutSendBuffer(wxString str);
	wxString EatSendBuffer();

	bool StartLocalAnswer();
	bool WhileLocalAnswer();

private:
	int m_connecting_state;

	MyTermConnProp *mp_conn_prop;
	MyTermThread *mp_term_thread;
	
	wxCriticalSection mcs_term_thread;    // protects the m_pThread pointer

	wxCriticalSection mcs_send_buffer;
	wxString m_send_buffer;
	
	wxMenu *mp_menu;
	bool m_local_answer;

	DECLARE_EVENT_TABLE()

};


wxDECLARE_EVENT(wxEVT_COMMAND_TERM_RECEIVE, wxCommandEvent);
wxDECLARE_EVENT(wxEVT_COMMAND_TERM_THREAD_DONE, wxCommandEvent);