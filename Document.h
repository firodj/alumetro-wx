#pragma once

#include "Router.h"
#include "RouterLink.h"

class CDocument {
public:
	CDocument();
	~CDocument();

	bool Open(wxString filename, bool create=false);
	bool IsOpen();
	void Close();
	void Init();
	void GetAllRouters();

private:
	class CDocumentImpl;
	CDocumentImpl& impl;
};
