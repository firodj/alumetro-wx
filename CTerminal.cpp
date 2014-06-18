#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "VideoTerminal.h"
#include "CTerminal.h"
#include "term_thread_ssh.h"

///////////////////////////////////////////////////////////////////////////////
wxDEFINE_EVENT(wxEVT_COMMAND_TERM_RECEIVE, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_COMMAND_TERM_THREAD_DONE, wxCommandEvent);

///////////////////////////////////////////////////////////////////////////////
// Events

BEGIN_EVENT_TABLE(CTerminal, wxVideoTerminal)
	EVT_RIGHT_DOWN(CTerminal::OnMouseRightClick)
	EVT_COMMAND(wxID_ANY, wxEVT_COMMAND_TERM_RECEIVE, CTerminal::OnTermReceive)
	EVT_COMMAND(wxID_ANY, wxEVT_COMMAND_TERM_THREAD_DONE, CTerminal::OnTermThreadDone)
	//EVT_SOCKET(wxID_ANY, CTerminal::OnSocket)
END_EVENT_TABLE()

CTerminal::CTerminal(wxWindow *parent, wxWindowID id):
	  wxVideoTerminal(parent, id),
		  mp_term_thread(NULL),
	m_local_answer(false)
{
	mp_menu = new wxMenu();
	mp_menu->Append(wxID_COPY, "Copy");
	mp_menu->Append(wxID_PASTE, "Paste");
	mp_menu->Append(wxID_OPEN, "Open");
	mp_menu->Append(wxID_CLOSE, "Close");

	mp_session = NULL;
	mp_channel = NULL;
	m_socket = NULL;
	m_sshSession = NULL;
	m_sshChannel = NULL;

	m_use_lib_ver = 1; // 1=libssh, 2=libssh2
};

CTerminal::~CTerminal()
{
  	{
		wxCriticalSectionLocker enter(mcs_term_thread);
		mp_term_thread = NULL;
	}
	if (mp_menu) delete mp_menu;
};


void CTerminal::OnMouseRightClick(wxMouseEvent &evt)
{
	PopupMenu(mp_menu, evt.GetPosition());
}

#if 0
void CTerminal::OnSocket(wxSocketEvent &evt)
{
	char buffer[256];
	int byteread;

	switch (evt.GetSocketEvent())
	{ 
		case wxSOCKET_CONNECTION:
			Process( "Connected.\n" );
			StartInitSSH();
			break;
		case wxSOCKET_INPUT:
			if (mp_conn_prop->mp_channel)
			{
				wxLogMessage("Input occured - channelized!");
				libssh2_channel_set_blocking(mp_conn_prop->mp_channel,0);
				do
				{
					byteread = libssh2_channel_read( mp_conn_prop->mp_channel, buffer, sizeof(buffer) );
					if (byteread == LIBSSH2_ERROR_EAGAIN) continue;
					if (byteread > 0)
						Process( wxString( (const char*) buffer, (size_t) byteread ));
				} while (byteread > 0);
				if (libssh2_channel_eof( mp_conn_prop->mp_channel ))
					wxQueueEvent(this, new wxCommandEvent(wxEVT_COMMAND_MENU_SELECTED, wxID_CLOSE));
				libssh2_channel_set_blocking(mp_conn_prop->mp_channel,1);
			} else
				wxLogMessage("Input occured, no Channel!");
			break;
		case wxSOCKET_LOST:
			wxLogMessage("Lost occured!");
			break;
		case wxSOCKET_OUTPUT:
			wxLogMessage("Output occured!");
			break;
		default:
			wxLogMessage("unknown %d", evt.GetSocketEvent());
	};	
}
#endif

void CTerminal::PutSendBuffer(wxString str)
{
	wxString copy_buf;
	bool send_remote = false;
	{
		wxCriticalSectionLocker enter(mcs_send_buffer);
		m_send_buffer += str;
		copy_buf = m_send_buffer;
		send_remote = !m_local_answer;
	}

	if (m_use_lib_ver == 2)
	{
		if (send_remote)
		{
			wxCriticalSectionLocker enter(mcs_send_buffer);
			if (mp_channel) SendToRemote(m_send_buffer);	
			m_send_buffer.Empty();
		}
	}

}

wxString CTerminal::EatSendBuffer()
{
	wxString copy_buf;
	{
		wxCriticalSectionLocker enter(mcs_send_buffer);
		copy_buf = m_send_buffer;
		m_send_buffer.Empty();
	}
	return copy_buf;
}

//////////

bool CTerminal::StartLocalAnswer()
{	
	wxCriticalSectionLocker enter(mcs_send_buffer);

	if (m_local_answer) return false;
	
	m_send_buffer.Empty();
	m_local_answer = true;

	return true;
}

bool CTerminal::WhileLocalAnswer()
{
	wxCriticalSectionLocker enter(mcs_send_buffer);
	return m_local_answer;
}


//////////////////////////////

bool CTerminal::OpenSSH(wxString hostname, wxString username, wxString password, int port)
{
	{
		wxCriticalSectionLocker enter(mcs_term_thread);
		if (mp_term_thread) return false;
	}
	
	m_conntype = TERM_CONN_SSH;
	m_hostname = hostname;
	m_port = port;
	m_username = username;
	m_password = password;
	m_stopping = false;

	SendToTerminal( wxString::Format("Connecting to %s...\n", m_hostname), true );

	if (m_use_lib_ver == 2) {

		m_socket = socket(AF_INET, SOCK_STREAM, 0);
		
		struct sockaddr_in sin;
		struct hostent *remoteHost = remoteHost = gethostbyname(m_hostname.c_str());
		if (!remoteHost)
		{
			SendToTerminal("\x1b[31;1mUnable to retrive remote-host address!\x1b[m\n");
			return false;
		}
		if (remoteHost->h_addrtype == AF_INET && remoteHost->h_addr_list[0] != 0)
		{	
			sin.sin_family = remoteHost->h_addrtype;
			sin.sin_addr.s_addr = *(u_long *)remoteHost->h_addr_list[0];
			sin.sin_port = htons(m_port);
		} else
		{
			SendToTerminal("\x1b[31;1mUnsupported address type!\x1b[m\n");
			return false;
		}

		//unsigned long hostaddr = htonl(0x7F000001);
	    
		if (connect(m_socket, 
					(struct sockaddr*)(&sin),
					sizeof(struct sockaddr_in)) != 0) {
			SendToTerminal("\x1b[31;1mFailed to connect!\x1b[m\n");
			return false;
		}
	} else { //if (m_use_lib_ver == 1)
		m_sshSession = ssh_new();
		if (m_sshSession == NULL) {
			SendToTerminal("\x1b[31;1mUnable to create ssh-session instance!\x1b[m\n");
			return false;
		}

		if (ssh_options_set(m_sshSession, SSH_OPTIONS_USER, m_username.c_str()) < 0) {
			ssh_free(m_sshSession);
			//SendToTerminal("\x1b[31;1mUnable to retrive remote-host address!\x1b[m\n");
			return false;
		}

		if (ssh_options_set(m_sshSession, SSH_OPTIONS_HOST, m_hostname.c_str()) < 0) {
			ssh_free(m_sshSession);
			//SendToTerminal("\x1b[31;1mUnable to retrive remote-host address!\x1b[m\n");
			return false;
		}

		if(ssh_connect(m_sshSession)){
				SendToTerminal(
					wxString::Format("\x1b[31;1mConnection failed : %s!\x1b[m\n", ssh_get_error(m_sshSession))
					);
			ssh_disconnect(m_sshSession);
			ssh_free(m_sshSession);
			return false;
		}
	}

	return StartInitSSH();
}

bool CTerminal::StartInitSSH()
{
	wxCriticalSectionLocker enter(mcs_term_thread);
	if (mp_term_thread) return false;

	mp_term_thread = new MyTermThread(this);

	if ( mp_term_thread->Create() != wxTHREAD_NO_ERROR )
    {
        wxLogError("Can't create the thread!");
        delete mp_term_thread;
        mp_term_thread = NULL;
    }
    else
    {
        if (mp_term_thread->Run() != wxTHREAD_NO_ERROR )
        {
            wxLogError("Can't create the thread!");
            delete mp_term_thread;
            mp_term_thread = NULL;
        }

        // after the call to wxThread::Run(), the m_pThread pointer is "unsafe":
        // at any moment the thread may cease to exist (because it completes its work).
        // To avoid dangling pointers OnThreadExit() will set m_pThread
        // to NULL when the thread dies.
    }

	return true;
}

bool CTerminal::Destroy()
{
	{
		wxCriticalSectionLocker enter(mcs_term_thread);
		if (mp_term_thread)
		{
			if (mp_term_thread->Delete() != wxTHREAD_NO_ERROR)
				wxLogError("Can't delete the terminal-thread!");
		}
	}
	return wxWindow::Destroy();
}


void CTerminal::DoSendChar(wxChar chr, int code)
{
	if (m_local_answer)
	{
		if (chr < 32 || chr > 126)
		{
			switch (code) {
			case WXK_BACK:
				{
					wxCriticalSectionLocker enter(mcs_send_buffer);
					if (m_send_buffer.Len() > 0)
					{
						m_send_buffer.RemoveLast();
						Process("\x8\x1b[\x50");
					}
				}
				break;
			case WXK_TAB:
				//Process("\x9");
				break;
			case WXK_UP:
				//Process("\x1b[\x41");
				break;
			case WXK_DOWN:
				//Process("\x1b[\x42");
				break;
			case WXK_RIGHT:
				//Process("\x1b[\x43");
				break;
			case WXK_LEFT:
				//Process("\x1b[\x44");
				break;		
			case WXK_RETURN:
				{
					wxCriticalSectionLocker enter(mcs_send_buffer);
					m_local_answer = false;
					Process("\r\n");
				}
				break;
			}
		}
		else
		{
			PutSendBuffer(chr);
			Process(chr);
		}

	} 
	else if (m_conntype == TERM_CONN_NONE)
	{
		wxBell();
	}
	else
	{
		if (chr == 0)
		{
			switch (code) {
				case WXK_UP:
					PutSendBuffer("\x1b[A");
					break;
				case WXK_DOWN:
					PutSendBuffer("\x1b[B");
					break;
				case WXK_RIGHT:
					PutSendBuffer("\x1b[C");
					break;
				case WXK_LEFT:
					PutSendBuffer("\x1b[D");
					break;
				case WXK_PAGEUP:
					PutSendBuffer("\x1b[5~");
					break;
				case WXK_PAGEDOWN:
					PutSendBuffer("\x1b[6~");
					break;
				case WXK_HOME:
					PutSendBuffer(wxString("\x1b[\x00",3));
					break;
				case WXK_END:
					PutSendBuffer("\x1b[e");
					break;
				case WXK_INSERT:
					PutSendBuffer("\x1b[2~");
					break;
				case WXK_DELETE:
					PutSendBuffer("\x7f");
					break;
				case WXK_F1:
					PutSendBuffer("\x1b[11~");
					break;
				case WXK_F2:
					PutSendBuffer("\x1b[12~");
					break;
				case WXK_F3:
					PutSendBuffer("\x1b[13~");
					break;
				case WXK_F4:
					PutSendBuffer("\x1b[14~");
					break;
				case WXK_F5:
					PutSendBuffer("\x1b[15~");
					break;
				case WXK_F6:
					PutSendBuffer("\x1b[17~");
					break;
				case WXK_F7:
					PutSendBuffer("\x1b[18~");
					break;
				case WXK_F8:
					PutSendBuffer("\x1b[19~");
					break;
				case WXK_F9:
					PutSendBuffer("\x1b[20~");
					break;
				case WXK_F10:
					PutSendBuffer("\x1b[21~");
					break;
				case WXK_F11:
					PutSendBuffer("\x1b[23~");
					break;
				case WXK_F12:
					PutSendBuffer("\x1b[24~");
					break;
				default:
					wxBell();
			}
		} else
		{
			PutSendBuffer(chr);
		}
	}
}

void CTerminal::OnTermReceive(wxCommandEvent &evt)
{
	// called by `SendToTerminal`
	wxString str = evt.GetString();
	Process(str);
}

void CTerminal::OnTermThreadDone(wxCommandEvent &evt)
{
	wxCriticalSectionLocker enter(mcs_term_thread);
	if (mp_term_thread)
		mp_term_thread = NULL;
}

// never call this function from thread itself !!!
bool CTerminal::WaitThreadStop()
{
	int rc;

	if (mp_term_thread)
	{
		if (mp_term_thread->IsAlive())
		{
			SendToTerminal( "Disconnecting...\n" );
			CloseSSH();
			mp_term_thread->Delete();
		}
	}
	return true;
}

bool CTerminal::CloseSSH()
{
	int rc;

	if (m_use_lib_ver == 2) {
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
	} else { // if (m_use_lib_ver == 1)

		if (m_sshSession) {
			if (m_sshChannel) {
				ssh_channel_close(m_sshChannel);
				ssh_channel_send_eof(m_sshChannel);
				ssh_channel_free(m_sshChannel);
				m_sshChannel = NULL;
			}
			ssh_disconnect(m_sshSession);
			ssh_free(m_sshSession);
			m_sshSession = NULL;

			SendToTerminal(wxString::Format("Done connection to %s.\n", m_hostname));
		}
	}

	return true;
}

void CTerminal::SendToTerminal(wxString str, bool force)
{
	wxCommandEvent *evt = new wxCommandEvent(wxEVT_COMMAND_TERM_RECEIVE, wxID_ANY);
	evt->SetString(str);

	if (force) {
		ProcessEvent( *evt );
		Update();
		delete evt;
	} else
		wxQueueEvent(this, evt);
}

int CTerminal::SendToRemote(wxString str)
{
	int nwritten = 0;
	int nbytes = str.Len();

	if (m_use_lib_ver == 2) {
		// TODO: check SSH first
		if (!mp_channel) 
			return -1;
		//buf = m_connprop.mp_terminal->EatSendBuffer();
		if (nbytes > 0)
			nwritten = libssh2_channel_write( mp_channel, str.c_str(), nbytes);
	} else {
		// FOR THIS IS DEPRECATED
		if (!m_sshChannel) return -1;
		if (nbytes > 0)
			nwritten = ssh_channel_write( m_sshChannel, str.c_str(), nbytes);
	}

	return nwritten;
}