#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/dir.h>
#include <wx/filename.h>
#include "wx/wxsqlite3.h"
#include "Document.h"
#include "dbschema.hxx"

// Class CDocumentImpl
class CDocument::CDocumentImpl {
public:
	// Constructor
	CDocumentImpl() {
	}

	// Desctuctor
	~CDocumentImpl() {
	}

	// Create tables
	void CreateTables() {
		CCablesBase::InitTable(m_db);
		CChassisBase::InitTable(m_db);
		CRoutersBase::InitTable(m_db);
		CPortsBase::InitTable(m_db);
		CSpeedsBase::InitTable(m_db);
		CLinksBase::InitTable(m_db);
		CInterfacesBase::InitTable(m_db);
		CLagsBase::InitTable(m_db);
		CLagportsBase::InitTable(m_db);
		CGrfNodesBase::InitTable(m_db);
		CGrfPointsBase::InitTable(m_db);
	}

	wxString m_strFilename;
	wxSQLite3Database m_db;
};

CDocument::CDocument():
	impl(*new CDocumentImpl())
{
}

CDocument::~CDocument()
{
	delete &impl;
}

bool CDocument::Open(wxString filename, bool create)
{
	impl.m_strFilename = filename;

	if (impl.m_db.IsOpen())
		impl.m_db.Close();

	if (!create && !wxFile::Exists( filename )) 
		return false;

	try {
		impl.m_db.Open( filename );
		impl.m_db.EnableForeignKeySupport(true);
		impl.CreateTables();
		
	}
	catch (wxSQLite3Exception e)
	{
		wxMessageBox(e.GetMessage(), "Error Open/Create Catalog");
		return false;
	}

	return true;
}

bool CDocument::IsOpen()
{
	return impl.m_db.IsOpen();
}

void CDocument::Close()
{
	return impl.m_db.Close();
}

void CDocument::Init()
{

}

void CDocument::GetAllRouters()
{
	wxSQLite3StatementBuffer _sql;
	CRoutersBase _ret;
	_sql.Format("SELECT * FROM [routers]");

	wxSQLite3ResultSet _q = impl.m_db.ExecuteQuery(_sql);
	while (_q.NextRow()) {
		_ret.SetDb( &impl.m_db);
		_ret.FetchResult(_q);
		wxLogDebug("Hostname = %s", _ret.hostname);
	}
	_q.Finalize();
}