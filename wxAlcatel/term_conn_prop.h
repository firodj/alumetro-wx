#pragma once

#include <wx/socket.h>
#include "libssh2.h"

typedef enum {TERM_CONN_NONE=0,
	TERM_CONN_COM,
	TERM_CONN_SSH,
	TERM_CONN_TELNET
} t_termconn;

class MyTerminal;

class MyTermConnProp
{
public:
	MyTermConnProp(MyTerminal *p_terminal);

	void OpenCOM(wxString comport);
	void OpenSSH(wxString hostname, wxString username, wxString password, int port);
	void CloseSSH();
	
	void SendToTerminal(wxString str);
	int SendToRemote(wxString str);

public:
	t_termconn m_conntype;
	wxString m_comport;
	wxString m_hostname;
	wxString m_username;
	wxString m_password;
	int m_port;
	bool m_stopping;

	MyTerminal *mp_terminal;
	LIBSSH2_SESSION *mp_session;
	LIBSSH2_CHANNEL *mp_channel;
	wxSocketClient m_sock;
	SOCKET m_socket;
};

