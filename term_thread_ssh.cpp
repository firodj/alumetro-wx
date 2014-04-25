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

#include "Terminal.h"
#include "term_thread_ssh.h"

///////////////////////////////////////////////////////////////////////////////

const char *keyfile1="./id_rsa.pub";
const char *keyfile2="./id_rsa";

static void kbd_callback(const char *name, int name_len,
                         const char *instruction, int instruction_len,
                         int num_prompts,
                         const LIBSSH2_USERAUTH_KBDINT_PROMPT *prompts,
                         LIBSSH2_USERAUTH_KBDINT_RESPONSE *responses,
                         void **abstract)
{
    (void)name;
    (void)name_len;
    (void)instruction;
    (void)instruction_len;
    if (num_prompts == 1) {
        //responses[0].text = strdup(password);
        //responses[0].length = strlen(password);
    }
    (void)prompts;
    (void)abstract;
}

static int waitsocket(int socket_fd, LIBSSH2_SESSION *session, long sec=10)
{
    struct timeval timeout;
    int rc;
    fd_set fd;
    fd_set *writefd = NULL;
    fd_set *readfd = NULL;
    int dir;
 
    timeout.tv_sec = sec;
    timeout.tv_usec = 0;
 
    FD_ZERO(&fd);
 
    FD_SET(socket_fd, &fd);
 
    /* now make sure we wait in the correct direction */ 
    dir = libssh2_session_block_directions(session);

    if(dir & LIBSSH2_SESSION_BLOCK_INBOUND)
        readfd = &fd;
 
    if(dir & LIBSSH2_SESSION_BLOCK_OUTBOUND)
        writefd = &fd;
 
    rc = select(socket_fd + 1, readfd, writefd, NULL, &timeout);
 
    return rc;
}

static int waitchannel(ssh_session session, ssh_channel channel, long sec=10)
{
	struct timeval timeout;
    int rc;
    fd_set fds;
	ssh_channel in_channels[2]={0}, out_channels[2]={0};
    int maxfd;
 
    timeout.tv_sec = sec;
    timeout.tv_usec = 0;
 
	in_channels[0] = channel;
	in_channels[1] = 0;

    FD_ZERO(&fds);
 
    FD_SET(0, &fds);
	FD_SET(ssh_get_fd(session), &fds);
	maxfd = ssh_get_fd(session)+1;

	rc = ssh_select(in_channels, out_channels, maxfd, &fds, &timeout);
 
    return rc;
}

MyTermThread::MyTermThread(CTerminal *pTerminal):
		wxThread(wxTHREAD_DETACHED),
		m_pTerminal(pTerminal)
{
	
};

MyTermThread::~MyTermThread()
{
	
};
	
wxThread::ExitCode MyTermThread::Entry()
{
	wxThread::ExitCode result;
	if (m_pTerminal->m_conntype == TERM_CONN_SSH)
	{
		if (m_pTerminal->m_use_lib_ver == 2) {
			result = EntryForLIBSSH2();
		} else {
			result = EntryForSSH();
		}
	}
	else 
		return (wxThread::ExitCode)0;

	wxQueueEvent(m_pTerminal, new wxCommandEvent(wxEVT_COMMAND_TERM_THREAD_DONE,wxID_ANY));
	return result;
};
	
wxThread::ExitCode MyTermThread::EntryForSSH()
{
	int rc;
	int auth_method;
	char *banner;

	// Try to authenticate
	rc = ssh_userauth_none(m_pTerminal->m_sshSession, NULL);
	if (rc == SSH_AUTH_ERROR) {
		m_pTerminal->SendToTerminal(wxString::Format(
				"\x1b[31;1mAuthentication failed: %s!\x1b[m\n", 
				ssh_get_error(m_pTerminal->m_sshSession)));
		goto shutdown;
	}

	// Show Banner
	banner = ssh_get_issue_banner(m_pTerminal->m_sshSession);
	if (banner) {
		m_pTerminal->SendToTerminal((const char*)banner);
		ssh_string_free_char(banner);
	}

	// Auth
	auth_method = ssh_auth_list(m_pTerminal->m_sshSession);
	if (auth_method & SSH_AUTH_METHOD_PASSWORD)
	{
		rc = ssh_userauth_password(m_pTerminal->m_sshSession, 
			NULL, 
			m_pTerminal->m_password.c_str());
		if (rc == SSH_AUTH_ERROR) {
			m_pTerminal->SendToTerminal(wxString::Format(
				"\x1b[31;1mAuthentication failed: %s!\x1b[m\n", 
				ssh_get_error(m_pTerminal->m_sshSession)));
			goto shutdown;
		} else if (rc == SSH_AUTH_DENIED) {
			m_pTerminal->SendToTerminal("\x1b[31;1mAuthentication denied!\x1b[m\n");
			goto shutdown;
		}
	} else {
		m_pTerminal->SendToTerminal("\x1b[31;1mUnsupported authentication method!\x1b[m\n");
		goto shutdown;
	}

	//m_pTerminal->SendToTerminal("Authentication successfully!\n");
	
	m_pTerminal->m_sshChannel = ssh_channel_new(m_pTerminal->m_sshSession);
	if (m_pTerminal->m_sshChannel == NULL) {
		m_pTerminal->SendToTerminal("\x1b[31;1mFail creating channel!\x1b[m\n");
		goto shutdown;
	}

	rc = ssh_channel_open_session(m_pTerminal->m_sshChannel);
	if (rc != SSH_OK) {
		m_pTerminal->SendToTerminal("\x1b[31;1mFail opening channel!\x1b[m\n");
		goto shutdown;
	}

	//rc = ssh_channel_request_pty(m_pTerminal->m_sshChannel);
	rc = ssh_channel_request_pty_size(m_pTerminal->m_sshChannel, "vt100", 80, 24);
	if (rc != SSH_OK) {
		m_pTerminal->SendToTerminal("\x1b[31;1mFail request pty size for vt100!\x1b[m\n");
		goto shutdown;
	}
	//rc = ssh_channel_change_pty_size(m_pTerminal->m_sshChannel, 80, 24);
	
	rc = ssh_channel_request_shell(m_pTerminal->m_sshChannel);
	if (rc != SSH_OK) {
		m_pTerminal->SendToTerminal("\x1b[31;1mFail request shell!\x1b[m\n");
		goto shutdown;
	}

	return (wxThread::ExitCode) sshStartInteractiveShell();

shutdown:
	m_pTerminal->CloseSSH();

	return (wxThread::ExitCode)-1;
}

wxThread::ExitCode MyTermThread::EntryForLIBSSH2()
{
	int rc;

	m_pTerminal->mp_session = libssh2_session_init();
	
	libssh2_session_set_blocking(m_pTerminal->mp_session, 0);

	while( (rc = libssh2_session_handshake(m_pTerminal->mp_session, m_pTerminal->m_socket )) == LIBSSH2_ERROR_EAGAIN)
	{
		if (TestDestroy())
		{
			m_pTerminal->SendToTerminal("\x1b[31mFailure establishing SSH session\x1b[0m\n");
			goto shutdown;
		}
	}
	
    if (rc) {
		m_pTerminal->SendToTerminal("Failure establishing SSH session\n");
        goto shutdown;
    }
	
	//const char *banner = libssh2_session_banner_get(m_pTerminal->mp_session);
	//m_pTerminal->SendToTerminal( banner );
	//m_pTerminal->SendToTerminal( "\n" );

	LIBSSH2_KNOWNHOSTS *nh = libssh2_knownhost_init(m_pTerminal->mp_session);
	if(!nh) {
		goto shutdown;
	}

	libssh2_knownhost_readfile(nh, "known_hosts", LIBSSH2_KNOWNHOST_FILE_OPENSSH);
	libssh2_knownhost_writefile(nh, "dumpfile", LIBSSH2_KNOWNHOST_FILE_OPENSSH);

	//const char *fingerprint = libssh2_hostkey_hash(m_pTerminal->mp_session, LIBSSH2_HOSTKEY_HASH_SHA1);
	size_t len;
	int type;
	const char *fingerprint = libssh2_session_hostkey(m_pTerminal->mp_session, &len, &type);
	if (fingerprint)
	{
		m_pTerminal->SendToTerminal("Fingerprint: ");
		for(int i = 0; i < 20; i++) {
			m_pTerminal->SendToTerminal( wxString::Format("%02X ", (unsigned char)fingerprint[i]) );
		}
		m_pTerminal->SendToTerminal("\n");

		struct libssh2_knownhost *host;
        int check = libssh2_knownhost_checkp(nh, m_pTerminal->m_hostname.c_str(), 
											m_pTerminal->m_port,

                                             fingerprint, len,
                                             LIBSSH2_KNOWNHOST_TYPE_PLAIN|
                                             LIBSSH2_KNOWNHOST_KEYENC_RAW,
                                             &host);
		m_pTerminal->SendToTerminal(wxString::Format("Host check: %d, key: %s\n", check,
                (check <= LIBSSH2_KNOWNHOST_CHECK_MISMATCH)?host->key:"<none>") );
	}
	libssh2_knownhost_free(nh);

	char *userauthlist = NULL;
	do
	{
		userauthlist = libssh2_userauth_list(m_pTerminal->mp_session, 
			m_pTerminal->m_username.c_str(), m_pTerminal->m_username.Len());
		if (userauthlist == NULL)
		{
			if ((rc = libssh2_session_last_errno(m_pTerminal->mp_session)) != LIBSSH2_ERROR_EAGAIN)
			{
				goto shutdown;
			}
		}
	} while (userauthlist == NULL);

	m_pTerminal->SendToTerminal(wxString::Format("Authentication methods: %s\n", userauthlist) );

	int auth_pw = 0;
    if (strstr(userauthlist, "password") != NULL) {
        auth_pw |= 1;
    }
    if (strstr(userauthlist, "keyboard-interactive") != NULL) {
        auth_pw |= 2;
    }
    if (strstr(userauthlist, "publickey") != NULL) {
        auth_pw |= 4;
    }

	if (auth_pw & 1) {
        /* We could authenticate via password */
		wxString passwd;
		if (m_pTerminal->m_password.IsEmpty())
		{
			m_pTerminal->SendToTerminal("Password: ");
			passwd = WaitLocalAnswer();
		} else
			passwd = m_pTerminal->m_password;

        while ((rc = libssh2_userauth_password(m_pTerminal->mp_session, 
			m_pTerminal->m_username.c_str(), passwd.c_str() )) == LIBSSH2_ERROR_EAGAIN)
			;
		
		if (rc) {
            m_pTerminal->SendToTerminal("\tAuthentication by password failed!\n");
            goto shutdown;
        } else {
            m_pTerminal->SendToTerminal( "\tAuthentication by password succeeded.\n");
        }
    } else if (auth_pw & 2) {
        /* Or via keyboard-interactive */
        if (libssh2_userauth_keyboard_interactive(m_pTerminal->mp_session, m_pTerminal->m_username.c_str(),
                                                  &kbd_callback) ) {
            m_pTerminal->SendToTerminal(
                "\tAuthentication by keyboard-interactive failed!\n");
            goto shutdown;
        } else {
            m_pTerminal->SendToTerminal(
                "\tAuthentication by keyboard-interactive succeeded.\n");
        }
    } else if (auth_pw & 4) {
        /* Or by public key */
        if (libssh2_userauth_publickey_fromfile(m_pTerminal->mp_session, m_pTerminal->m_username.c_str(), keyfile1,
                                                keyfile2, m_pTerminal->m_password.c_str())) {
            m_pTerminal->SendToTerminal( "\tAuthentication by public key failed!\n");
            goto shutdown;
        } else {
            m_pTerminal->SendToTerminal( "\tAuthentication by public key succeeded.\n");
        }
    } else {
        m_pTerminal->SendToTerminal( "No supported authentication methods found!\n");
        goto shutdown;
    }

	//m_pTerminal->m_sock.SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG | wxSOCKET_OUTPUT_FLAG);

	while ( (m_pTerminal->mp_channel = libssh2_channel_open_session(m_pTerminal->mp_session)) == NULL &&
		libssh2_session_last_error(m_pTerminal->mp_session, NULL, NULL ,0) == LIBSSH2_ERROR_EAGAIN )
	{
		waitsocket(m_pTerminal->m_socket, m_pTerminal->mp_session);
	}

	if (!m_pTerminal->mp_channel)
	{
		m_pTerminal->SendToTerminal("Unable to open a session\n");
		goto shutdown;
	}

	while ( (rc = libssh2_channel_request_pty(m_pTerminal->mp_channel, "vt100")) == LIBSSH2_ERROR_EAGAIN)
		;

	if (rc)
	{
		m_pTerminal->SendToTerminal("Failed requesting pty\n");
		goto shutdown;
	}

	while ( (rc = libssh2_channel_shell(m_pTerminal->mp_channel)) == LIBSSH2_ERROR_EAGAIN)
		;

	if (rc) {
        m_pTerminal->SendToTerminal("Unable to request shell on allocated pty\n");
        goto shutdown;
    }

	return (wxThread::ExitCode) sshStartInteractiveShellLIBSSH2();
	
shutdown:

    m_pTerminal->CloseSSH();

	return (wxThread::ExitCode)-1;
}

wxString MyTermThread::WaitLocalAnswer()
{
	CTerminal &terminal = *m_pTerminal;
	
	terminal.EatSendBuffer();

	if (!terminal.StartLocalAnswer()) 
		return wxEmptyString;
	
	while ( terminal.WhileLocalAnswer() && !m_pTerminal->m_stopping)
	{
		wxThread::This()->Sleep(200);

		if (TestDestroy())
			m_pTerminal->m_stopping = true;
	}
		
	return terminal.EatSendBuffer();
}

///////////////////////////////////////////////////////////////////////////////

void MyTermThread::sshDisplayBanner()
{
	
}

int MyTermThread::sshVerifyKnownHost()
{
	
	return 0;
}

int MyTermThread::sshTestSeveralAuthMethods()
{
	return 0;
}

int MyTermThread::sshStartInteractiveShell()
{
	char buffer[0x1000];
	int rc;
	int nbytes;

	for (;;)
	{
		/* loop until we block */
		do {
			nbytes = ssh_channel_read_nonblocking(m_pTerminal->m_sshChannel, 
				buffer, sizeof(buffer), 0);
			if (nbytes > 0) {
				// process
				wxString recv_chan( (const char*) buffer, wxMBConvUTF8() , (size_t) nbytes  );
				m_pTerminal->SendToTerminal( recv_chan );
			}
		} while (nbytes > 0);

		if (!ssh_channel_is_open(m_pTerminal->m_sshChannel) ||
			ssh_channel_is_eof(m_pTerminal->m_sshChannel))
		{
			m_pTerminal->SendToTerminal("\x1b[31mDisconnect by remote.\x1b[m\n");
			break;
		}

		if (TestDestroy())
			break;
	
		// TODO: wait Socket or BREAK
		{
			wxCriticalSectionLocker enter(m_pTerminal->mcs_send_buffer);
			if (m_pTerminal->m_send_buffer.Len() > 0 && !m_pTerminal->m_local_answer) {
				ssh_channel_write(m_pTerminal->m_sshChannel,  
					m_pTerminal->m_send_buffer.c_str(),
					m_pTerminal->m_send_buffer.Len());
				m_pTerminal->m_send_buffer.Empty();
			}			
		}
		
		wxThread::This()->Sleep(50);
		
	}

	m_pTerminal->CloseSSH();
	return 0;
}

int MyTermThread::sshStartInteractiveShellLIBSSH2()
{
	char buffer[0x4000];
	int rc;

	for (;;)
	{
		/* loop until we block */ 
		int rc;
        do
		{
			rc = libssh2_channel_read( m_pTerminal->mp_channel, buffer, sizeof(buffer) );
			if (rc > 0)
			{
				// process
				wxString recv_chan( (const char*) buffer, wxMBConvUTF8() , (size_t) rc  );
				m_pTerminal->SendToTerminal( recv_chan );
			}
			else 
			{
				if (rc != LIBSSH2_ERROR_EAGAIN) 
					;	// error
			}
		} while (rc > 0);

		if (libssh2_channel_eof( m_pTerminal->mp_channel ))
		{
			m_pTerminal->SendToTerminal("\x1b[31mDisconnect by remote.\x1b[m\n");
			//wxQueueEvent(m_pTerminal->mp_terminal, new wxCommandEvent(wxEVT_COMMAND_MENU_SELECTED, wxID_CLOSE));
			break;
		}

		if (TestDestroy())
			break;

		if(rc == LIBSSH2_ERROR_EAGAIN )
        {
            waitsocket(m_pTerminal->m_socket, m_pTerminal->mp_session);
        }
        else
            break;		
	};
	
	m_pTerminal->CloseSSH();
	return 0;
}

void MyTermThread::sshStop()
{
	int rc;
	if (m_pTerminal->mp_channel)
	{
		m_pTerminal->SendToTerminal( "Disconnecting..." );
		while ( (rc = libssh2_channel_send_eof(m_pTerminal->mp_channel)) == LIBSSH2_ERROR_EAGAIN) ;
		m_pTerminal->SendToRemote("");		
	}
};


void MyTermThread::OnDelete() 
{
	wxThread::OnDelete();	
};
