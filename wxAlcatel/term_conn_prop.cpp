//
// file name: my_main_frame.cpp
//

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

///////////////////////////////////////////////////////////////////////////////

#include "MyTerminal.h"
#include "term_conn_prop.h"


MyTermConnProp::MyTermConnProp(MyTerminal *p_terminal):
	  mp_terminal(p_terminal), m_stopping(false), m_conntype(TERM_CONN_NONE)
{
	mp_session = NULL;
	mp_channel = NULL;
	m_socket = NULL;
};

void MyTermConnProp::OpenCOM(wxString comport)
{
	m_conntype = TERM_CONN_COM;
	m_comport = comport;
}

void MyTermConnProp::OpenSSH(wxString hostname, wxString username, wxString password, int port)
{
	m_conntype = TERM_CONN_SSH;
	m_hostname = hostname;
	m_port = port;
	m_username = username;
	m_password = password;
	m_stopping = false;

	//m_sock.SetEventHandler(*mp_terminal, wxID_ANY);
	//m_sock.SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG | wxSOCKET_OUTPUT_FLAG);
	//m_sock.Notify(true);
}

void MyTermConnProp::SendToTerminal(wxString str)
{
	wxCommandEvent *evt = new wxCommandEvent(wxEVT_COMMAND_TERM_RECEIVE, wxID_ANY);
	evt->SetString(str);
	wxQueueEvent(mp_terminal, evt);
}

int MyTermConnProp::SendToRemote(wxString str)
{
	int nwritten = 0;
	// TODO: check SSH first
	if (!mp_channel) 
		return -1;
	//buf = m_connprop.mp_terminal->EatSendBuffer();
	int nbytes = str.Len();
	if (nbytes > 0)
		nwritten = libssh2_channel_write( mp_channel, str.c_str(), nbytes);
	return nwritten;
}

void MyTermConnProp::CloseSSH()
{
	int rc;

	if (mp_session)
	{
		if (mp_channel)
		{
			while ( (rc = libssh2_channel_close(mp_channel)) == LIBSSH2_ERROR_EAGAIN ) ;

			while ( (rc = libssh2_channel_free(mp_channel)) == LIBSSH2_ERROR_EAGAIN ) ;
		}
		
		while ((rc = libssh2_session_disconnect(mp_session,
								   "Normal Shutdown, Thank you for playing")) == LIBSSH2_ERROR_EAGAIN );
		
		libssh2_session_free(mp_session);
	
		SendToTerminal(wxString::Format("Done connection to %s.\n", m_hostname));
	}

	mp_session = NULL;
	mp_channel = NULL;
}

