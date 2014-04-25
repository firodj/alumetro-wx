#pragma once

#include "VideoTerminal.h"

#include "libssh2.h"
//#include "libssh/libsshpp.hpp"
#include "libssh/libssh.h"

class MyTermThread;

typedef enum {TERM_CONN_NONE=0,
	TERM_CONN_COM,
	TERM_CONN_SSH,
	TERM_CONN_TELNET
} t_termconn;

///////////////////////////////////////////////////////////////////////////////

class CTerminal: public wxVideoTerminal
{
public:
	CTerminal(wxWindow *parent, wxWindowID id);
	~CTerminal();

	virtual bool Destroy();
	virtual void DoSendChar(wxChar chr, int code);

	bool OpenSSH(wxString hosname, wxString username=wxEmptyString, wxString password=wxEmptyString, int port=22);
	bool CloseSSH();

	void OnCloseConnect(wxCommandEvent &evt);
	void OnOpenConnect(wxCommandEvent &evt);
	void OnTermReceive(wxCommandEvent &evt);
	
	//void OnSocket(wxSocketEvent &evt);
	void OnMouseRightClick(wxMouseEvent &evt);

	void OnTermThreadDone(wxCommandEvent &evt);

	bool StartInitSSH();
	bool WaitThreadStop();

	void PutSendBuffer(wxString str);
	wxString EatSendBuffer();

	bool StartLocalAnswer();
	bool WhileLocalAnswer();

	void SendToTerminal(wxString str, bool force =false);
	int SendToRemote(wxString str);

	t_termconn m_conntype;
	wxString m_comport;
	wxString m_hostname;
	wxString m_username;
	wxString m_password;
	int m_port;
	bool m_stopping;

	int m_use_lib_ver;

	LIBSSH2_SESSION *mp_session;
	LIBSSH2_CHANNEL *mp_channel;
	SOCKET m_socket;

	ssh_session m_sshSession;
	ssh_channel m_sshChannel;

	wxCriticalSection mcs_send_buffer;
	wxString m_send_buffer;
	bool m_local_answer;

private:
	int m_connecting_state;

	MyTermThread *mp_term_thread;
	
	wxCriticalSection mcs_term_thread;    // protects the m_pThread pointer

	
	
	wxMenu *mp_menu;



	DECLARE_EVENT_TABLE()

};


wxDECLARE_EVENT(wxEVT_COMMAND_TERM_RECEIVE, wxCommandEvent);
wxDECLARE_EVENT(wxEVT_COMMAND_TERM_THREAD_DONE, wxCommandEvent);