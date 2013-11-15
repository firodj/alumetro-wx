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

#include <wx/socket.h>
#include "libssh2.h"

#include "MyTerminal.h"
#include "term_conn_prop.h"
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

MyTermThread::MyTermThread(MyTermConnProp *connprop):
		wxThread(wxTHREAD_DETACHED),
		mp_conn_prop(connprop)
{
	
};

MyTermThread::~MyTermThread()
{
	
};
	
wxThread::ExitCode MyTermThread::Entry()
{
	wxThread::ExitCode result;
	if (mp_conn_prop->m_conntype == TERM_CONN_SSH)
	{
		result = EntryForSSH();
	}
	else 
		return (wxThread::ExitCode)0;

	wxQueueEvent(mp_conn_prop->mp_terminal, new wxCommandEvent(wxEVT_COMMAND_TERM_THREAD_DONE,wxID_ANY));
	return result;
};
	
wxThread::ExitCode MyTermThread::EntryForSSH()
{
	int rc;

	mp_conn_prop->mp_session = libssh2_session_init();
	
	libssh2_session_set_blocking(mp_conn_prop->mp_session, 0);

	while( (rc = libssh2_session_handshake(mp_conn_prop->mp_session, mp_conn_prop->m_socket )) == LIBSSH2_ERROR_EAGAIN)
	{
		if (TestDestroy())
		{
			mp_conn_prop->SendToTerminal("\x1b[31mFailure establishing SSH session\x1b[0m\n");
			goto shutdown;
		}
	}
	
    if (rc) {
		mp_conn_prop->SendToTerminal("Failure establishing SSH session\n");
        goto shutdown;
    }
	
	const char *banner = libssh2_session_banner_get(mp_conn_prop->mp_session);
	mp_conn_prop->SendToTerminal( banner );
	mp_conn_prop->SendToTerminal( "\n" );

	LIBSSH2_KNOWNHOSTS *nh = libssh2_knownhost_init(mp_conn_prop->mp_session);
	if(!nh) {
		goto shutdown;
	}

	libssh2_knownhost_readfile(nh, "known_hosts", LIBSSH2_KNOWNHOST_FILE_OPENSSH);
	libssh2_knownhost_writefile(nh, "dumpfile", LIBSSH2_KNOWNHOST_FILE_OPENSSH);

	//const char *fingerprint = libssh2_hostkey_hash(mp_conn_prop->mp_session, LIBSSH2_HOSTKEY_HASH_SHA1);
	size_t len;
	int type;
	const char *fingerprint = libssh2_session_hostkey(mp_conn_prop->mp_session, &len, &type);
	if (fingerprint)
	{
		mp_conn_prop->SendToTerminal("Fingerprint: ");
		for(int i = 0; i < 20; i++) {
			mp_conn_prop->SendToTerminal( wxString::Format("%02X ", (unsigned char)fingerprint[i]) );
		}
		mp_conn_prop->SendToTerminal("\n");

		struct libssh2_knownhost *host;
        int check = libssh2_knownhost_checkp(nh, mp_conn_prop->m_hostname.c_str(), 
											mp_conn_prop->m_port,

                                             fingerprint, len,
                                             LIBSSH2_KNOWNHOST_TYPE_PLAIN|
                                             LIBSSH2_KNOWNHOST_KEYENC_RAW,
                                             &host);
		mp_conn_prop->SendToTerminal(wxString::Format("Host check: %d, key: %s\n", check,
                (check <= LIBSSH2_KNOWNHOST_CHECK_MISMATCH)?host->key:"<none>") );
	}
	libssh2_knownhost_free(nh);

	char *userauthlist = NULL;
	do
	{
		userauthlist = libssh2_userauth_list(mp_conn_prop->mp_session, mp_conn_prop->m_username.c_str(), mp_conn_prop->m_username.Len());
		if (userauthlist == NULL)
		{
			if ((rc = libssh2_session_last_errno(mp_conn_prop->mp_session)) != LIBSSH2_ERROR_EAGAIN)
			{
				goto shutdown;
			}
		}
	} while (userauthlist == NULL);

	mp_conn_prop->SendToTerminal(wxString::Format("Authentication methods: %s\n", userauthlist) );

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
		if (mp_conn_prop->m_password.IsEmpty())
		{
			mp_conn_prop->SendToTerminal("Password: ");
			passwd = WaitLocalAnswer();
		} else
			passwd = mp_conn_prop->m_password;

        while ((rc = libssh2_userauth_password(mp_conn_prop->mp_session, mp_conn_prop->m_username.c_str(), passwd.c_str() )) == LIBSSH2_ERROR_EAGAIN)
			;
		
		if (rc) {
            mp_conn_prop->SendToTerminal("\tAuthentication by password failed!\n");
            goto shutdown;
        } else {
            mp_conn_prop->SendToTerminal( "\tAuthentication by password succeeded.\n");
        }
    } else if (auth_pw & 2) {
        /* Or via keyboard-interactive */
        if (libssh2_userauth_keyboard_interactive(mp_conn_prop->mp_session, mp_conn_prop->m_username.c_str(),
                                                  &kbd_callback) ) {
            mp_conn_prop->SendToTerminal(
                "\tAuthentication by keyboard-interactive failed!\n");
            goto shutdown;
        } else {
            mp_conn_prop->SendToTerminal(
                "\tAuthentication by keyboard-interactive succeeded.\n");
        }
    } else if (auth_pw & 4) {
        /* Or by public key */
        if (libssh2_userauth_publickey_fromfile(mp_conn_prop->mp_session, mp_conn_prop->m_username.c_str(), keyfile1,
                                                keyfile2, mp_conn_prop->m_password.c_str())) {
            mp_conn_prop->SendToTerminal( "\tAuthentication by public key failed!\n");
            goto shutdown;
        } else {
            mp_conn_prop->SendToTerminal( "\tAuthentication by public key succeeded.\n");
        }
    } else {
        mp_conn_prop->SendToTerminal( "No supported authentication methods found!\n");
        goto shutdown;
    }

	//mp_conn_prop->m_sock.SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG | wxSOCKET_OUTPUT_FLAG);

	while ( (mp_conn_prop->mp_channel = libssh2_channel_open_session(mp_conn_prop->mp_session)) == NULL &&
		libssh2_session_last_error(mp_conn_prop->mp_session, NULL, NULL ,0) == LIBSSH2_ERROR_EAGAIN )
	{
		waitsocket(mp_conn_prop->m_socket, mp_conn_prop->mp_session);
	}

	if (!mp_conn_prop->mp_channel)
	{
		mp_conn_prop->SendToTerminal("Unable to open a session\n");
		goto shutdown;
	}

	while ( (rc = libssh2_channel_request_pty(mp_conn_prop->mp_channel, "vt100")) == LIBSSH2_ERROR_EAGAIN)
		;

	if (rc)
	{
		mp_conn_prop->SendToTerminal("Failed requesting pty\n");
		goto shutdown;
	}

	while ( (rc = libssh2_channel_shell(mp_conn_prop->mp_channel)) == LIBSSH2_ERROR_EAGAIN)
		;

	if (rc) {
        mp_conn_prop->SendToTerminal("Unable to request shell on allocated pty\n");
        goto shutdown;
    }

	return (wxThread::ExitCode) sshStartInteractiveShell();
	
shutdown:

    mp_conn_prop->CloseSSH();

	return (wxThread::ExitCode)-1;
}

wxString MyTermThread::WaitLocalAnswer()
{
	MyTerminal &terminal = *mp_conn_prop->mp_terminal;
	
	terminal.EatSendBuffer();

	if (!terminal.StartLocalAnswer()) 
		return wxEmptyString;
	
	while ( terminal.WhileLocalAnswer() && !mp_conn_prop->m_stopping)
	{
		wxThread::This()->Sleep(200);

		if (TestDestroy())
			mp_conn_prop->m_stopping = true;
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
	char buffer[0x4000];
	int rc;

	for (;;)
	{
		/* loop until we block */ 
		int rc;
        do
		{
			rc = libssh2_channel_read( mp_conn_prop->mp_channel, buffer, sizeof(buffer) );
			if (rc > 0)
			{
				// process
				wxString recv_chan( (const char*) buffer, wxMBConvUTF8() , (size_t) rc  );
				mp_conn_prop->SendToTerminal( recv_chan );
			}
			else 
			{
				if (rc != LIBSSH2_ERROR_EAGAIN) 
					;	// error
			}
		} while (rc > 0);

		if (libssh2_channel_eof( mp_conn_prop->mp_channel ))
		{
			mp_conn_prop->SendToTerminal("\x1b[31mDisconnect by remote.\x1b[m\n");
			//wxQueueEvent(mp_conn_prop->mp_terminal, new wxCommandEvent(wxEVT_COMMAND_MENU_SELECTED, wxID_CLOSE));
			break;
		}

		if (TestDestroy())
			break;

		if(rc == LIBSSH2_ERROR_EAGAIN )
        {
            waitsocket(mp_conn_prop->m_socket, mp_conn_prop->mp_session);
        }
        else
            break;		
	};
	
	mp_conn_prop->CloseSSH();
	return 0;
}

void MyTermThread::sshStop()
{
	int rc;
	if (mp_conn_prop->mp_channel)
	{
		mp_conn_prop->SendToTerminal( "Disconnecting..." );
		while ( (rc = libssh2_channel_send_eof(mp_conn_prop->mp_channel)) == LIBSSH2_ERROR_EAGAIN) ;
		mp_conn_prop->SendToRemote("");		
	}
};


void MyTermThread::OnDelete() 
{
	wxThread::OnDelete();	
};
