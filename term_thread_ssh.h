#pragma once

class MyTermThread: public wxThread
{
public:
	MyTermThread(CTerminal *pTerminal);
	~MyTermThread();
	void sshStop();
	virtual void OnDelete();

protected:
	CTerminal *m_pTerminal;
	//ssh_session m_ssh_session;

	wxMutex *mp_mutex;
	wxCondition *mp_condition;
	
	virtual wxThread::ExitCode Entry();
	wxThread::ExitCode EntryForSSH();
	wxThread::ExitCode EntryForLIBSSH2();
	wxString WaitLocalAnswer();
	
protected:
	void sshDisplayBanner();
	int sshVerifyKnownHost();
	int sshTestSeveralAuthMethods();
	int sshStartInteractiveShell();
	int sshStartInteractiveShellLIBSSH2();
};
