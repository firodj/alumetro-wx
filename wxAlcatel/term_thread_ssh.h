#pragma once

class MyTermThread: public wxThread
{
public:
	MyTermThread(MyTermConnProp *connprop);
	~MyTermThread();
	void sshStop();
	virtual void OnDelete();

protected:
	MyTermConnProp *mp_conn_prop;
	//ssh_session m_ssh_session;

	wxMutex *mp_mutex;
	wxCondition *mp_condition;
	
	virtual wxThread::ExitCode Entry();
	wxThread::ExitCode EntryForSSH();
	wxString WaitLocalAnswer();
	
protected:
	void sshDisplayBanner();
	int sshVerifyKnownHost();
	int sshTestSeveralAuthMethods();
	int sshStartInteractiveShell();

};
