#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "vidterm.h"
#include "term_conn_prop.h"
#include "term_thread_ssh.h"
#include "MyTerminal.h"

///////////////////////////////////////////////////////////////////////////////
wxDEFINE_EVENT(wxEVT_COMMAND_TERM_RECEIVE, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_COMMAND_TERM_THREAD_DONE, wxCommandEvent);

///////////////////////////////////////////////////////////////////////////////
// Events

BEGIN_EVENT_TABLE(MyTerminal, wxVideoTerminal)
	EVT_RIGHT_DOWN(MyTerminal::OnMouseRightClick)
	EVT_MENU(wxID_OPEN, MyTerminal::OnOpenConnect)
	EVT_MENU(wxID_CLOSE, MyTerminal::OnCloseConnect)
	EVT_COMMAND(wxID_ANY, wxEVT_COMMAND_TERM_RECEIVE, MyTerminal::OnTermReceive)
	EVT_COMMAND(wxID_ANY, wxEVT_COMMAND_TERM_THREAD_DONE, MyTerminal::OnTermThreadDone)
	//EVT_SOCKET(wxID_ANY, MyTerminal::OnSocket)
END_EVENT_TABLE()

MyTerminal::MyTerminal(wxWindow *parent, wxWindowID id):
	  wxVideoTerminal(parent, id),
		  mp_term_thread(NULL),
	m_local_answer(false),
	mp_conn_prop(NULL)
{
	mp_conn_prop = new MyTermConnProp(this);

	mp_menu = new wxMenu();
	mp_menu->Append(wxID_COPY, "Copy");
	mp_menu->Append(wxID_PASTE, "Paste");
	mp_menu->Append(wxID_OPEN, "Open");
	mp_menu->Append(wxID_CLOSE, "Close");
};

MyTerminal::~MyTerminal()
{
  	{
		wxCriticalSectionLocker enter(mcs_term_thread);
		mp_term_thread = NULL;
	}
	if (mp_menu) delete mp_menu;
	if (mp_conn_prop) delete mp_conn_prop;
};


void MyTerminal::OnMouseRightClick(wxMouseEvent &evt)
{
	PopupMenu(mp_menu, evt.GetPosition());
}

void MyTerminal::OnCloseConnect(wxCommandEvent &evt)
{
	CloseSSH();
}

void MyTerminal::OnOpenConnect(wxCommandEvent &evt)
{
	//OpenSSH("logingw01.datacomm.co.id", "fadhil.mandaga", "5+jMhrHR24_/", 22);
	OpenSSH("localhost", "cyg_server");

	SetFocus();
}

#if 0
void MyTerminal::OnSocket(wxSocketEvent &evt)
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

void MyTerminal::PutSendBuffer(wxString str)
{
	wxString copy_buf;
	bool send_remote = false;
	{
		wxCriticalSectionLocker enter(mcs_send_buffer);
		m_send_buffer += str;
		copy_buf = m_send_buffer;
		send_remote = !m_local_answer;
	}

	if (send_remote)
	{
		wxCriticalSectionLocker enter(mcs_send_buffer);
		if (mp_conn_prop->mp_channel)
		{
			mp_conn_prop->SendToRemote(m_send_buffer);
		}
		m_send_buffer.Empty();
	}
}

wxString MyTerminal::EatSendBuffer()
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



bool MyTerminal::StartLocalAnswer()
{	
	wxCriticalSectionLocker enter(mcs_send_buffer);

	if (m_local_answer) return false;
	
	m_send_buffer.Empty();
	m_local_answer = true;

	return true;
}

bool MyTerminal::WhileLocalAnswer()
{
	wxCriticalSectionLocker enter(mcs_send_buffer);
	return m_local_answer;
}


//////////////////////////////

bool MyTerminal::OpenSSH(wxString hostname, wxString username, wxString password, int port)
{
	{
		wxCriticalSectionLocker enter(mcs_term_thread);
		if (mp_term_thread) return false;
	}
	
	mp_conn_prop->OpenSSH(hostname, username, password, port);
	mp_conn_prop->mp_terminal = this;

#if 1

	Process( wxString::Format("Connecting to %s...\n", mp_conn_prop->m_hostname));

	mp_conn_prop->m_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in sin;
	struct hostent *remoteHost = remoteHost = gethostbyname(mp_conn_prop->m_hostname.c_str());
	if (!remoteHost)
	{
		mp_conn_prop->SendToTerminal("\x1b[31;1mUnable to retrive remote-host address!\x1b[m\n");
        return false;
	}
	if (remoteHost->h_addrtype == AF_INET && remoteHost->h_addr_list[0] != 0)
	{	
		sin.sin_family = remoteHost->h_addrtype;
		sin.sin_addr.s_addr = *(u_long *)remoteHost->h_addr_list[0];
		sin.sin_port = htons(mp_conn_prop->m_port);
	} else
	{
		mp_conn_prop->SendToTerminal("\x1b[31;1mUnsupported address type!\x1b[m\n");
        return false;
	}

	//unsigned long hostaddr = htonl(0x7F000001);
    
	if (connect(mp_conn_prop->m_socket, 
				(struct sockaddr*)(&sin),
                sizeof(struct sockaddr_in)) != 0) {
        mp_conn_prop->SendToTerminal("\x1b[31;1mFailed to connect!\x1b[m\n");
        return false;
    }

	return StartInitSSH();

#else
	wxIPV4address sock_addr;

	sock_addr.Hostname(mp_conn_prop->m_hostname);
	sock_addr.Service(mp_conn_prop->m_port);

	Process( wxString::Format("Connecting to %s...\n", mp_conn_prop->m_hostname));

	//mp_conn_prop->m_sock.SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_LOST_FLAG);
	return mp_conn_prop->m_sock.Connect(sock_addr);
#endif
}

bool MyTerminal::StartInitSSH()
{
	wxCriticalSectionLocker enter(mcs_term_thread);
	if (mp_term_thread) return false;

	mp_term_thread = new MyTermThread(mp_conn_prop);

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

bool MyTerminal::Destroy()
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


void MyTerminal::DoSendChar(wxChar chr, int code)
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
	
	else if (mp_conn_prop)
	{
		if (mp_conn_prop->m_conntype == TERM_CONN_NONE)
			wxBell();
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
				PutSendBuffer(chr);
		}
	}
}

void MyTerminal::OnTermReceive(wxCommandEvent &evt)
{
	// SendToTerminal
	wxString str = evt.GetString();
	Process(str);
}

void MyTerminal::OnTermThreadDone(wxCommandEvent &evt)
{
	wxCriticalSectionLocker enter(mcs_term_thread);
	if (mp_term_thread)
		mp_term_thread = NULL;
}

bool MyTerminal::CloseSSH()
{
	if (mp_term_thread)
	{
		if (mp_term_thread->IsAlive())
		{
			mp_conn_prop->SendToTerminal( "Disconnecting...\n" );

			mp_term_thread->Pause();

			mp_conn_prop->CloseSSH();
			
			mp_term_thread->Delete();
			
			//mp_term_thread->sshStop();
		}
	}
	return true;
}