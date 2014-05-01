/* File: DbSchema.cxx - Auto Generated */

#include "wx/wxprec.h"

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif

#include "DbSchema.hxx"
#include <wx/wxsqlite3.h>

/******************************************************************************
 * Class CCablesBase
 ******************************************************************************/

// (static) create table if not exists
bool CCablesBase::InitTable(wxSQLite3Database& _db)
{
	wxString _sql = 
"CREATE TABLE IF NOT EXISTS [cables] (\
  [cable_id] INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
  [name] CHAR(30) NOT NULL);";
	_db.ExecuteUpdate( _sql );
	return true;
}

// c-tors
CCablesBase::CCablesBase(wxSQLite3Database *_db):
		CDbTableBase(_db) 
{
	m_tablename = TCables;
	cable_id	= 0;
	name	= wxEmptyString;
}

// fetch results
int CCablesBase::FetchResult(wxSQLite3ResultSet& _q, int _c)
{
	cable_id	= _q.GetInt64(_c++).ToLong();
	name	= _q.GetAsString(_c++);	return _c;
}
	
// inserts
bool CCablesBase::Insert(bool _autoinc) 
{
	if (!BeforeInsert()) return false;
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	if (!_autoinc) {
		_sql.Format("INSERT INTO [cables] ([cable_id], [name]) \
		VALUES (%d, '%q')",
			cable_id,
			WXSQLGET_S(name)
		);
	} else {
		_sql.Format("INSERT INTO [cables] ([name]) \
		VALUES ('%q')",
			WXSQLGET_S(name)
		);
	}

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CCablesBase::Insert()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}
	
	if (_affected > 0) {
		long _rowid = m_db->GetLastRowId().ToLong();
		if (_rowid != 0) cable_id = _rowid;
		return true;
	}

	return false;
}

// updates
bool CCablesBase::Update()
{
	if (!BeforeUpdate()) return false;
	
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("UPDATE cables SET [cable_id] = %d, [name] = '%q' \
		WHERE [cable_id] = %d",
		cable_id,
		WXSQLGET_S(name),
		cable_id
	);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CCablesBase::Update()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}

	if (_affected > 0) {
		return true;
	}

	return false;
}
	
// deletes
bool CCablesBase::Delete()
{
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("DELETE FROM [cables] WHERE [cable_id] = %d",
		cable_id
	);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CCablesBase::Delete()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}

	if (_affected > 0)
		return AfterDelete();

	return false;
}
	
// (static) Selects one record
bool CCablesBase::Select(long& cable_id_,
	CCablesBase& _ret, wxSQLite3Database& _db)
{
	bool _retok = false;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("SELECT * FROM [cables] WHERE [cable_id] = %d",
		cable_id_
	);

	wxSQLite3ResultSet _q = _db.ExecuteQuery(_sql);
	while (_q.NextRow()) {
		_ret.SetDb(&_db);
		_ret.FetchResult(_q);
		_retok = true;
		break;
	}
	
	_q.Finalize();
	return _retok;
}

/******************************************************************************
 * Class CChassisBase
 ******************************************************************************/

// (static) create table if not exists
bool CChassisBase::InitTable(wxSQLite3Database& _db)
{
	wxString _sql = 
"CREATE TABLE IF NOT EXISTS [chassis] (\
  [chassis_id] INTEGER NOT NULL PRIMARY KEY,\
  [name] CHAR(20) NOT NULL,\
  [slots] TINYINT,\
  [imagefile] CHAR(40));";
	_db.ExecuteUpdate( _sql );
	return true;
}

// c-tors
CChassisBase::CChassisBase(wxSQLite3Database *_db):
		CDbTableBase(_db) 
{
	m_tablename = TChassis;
	chassis_id	= 0;
	name	= wxEmptyString;
	slots	= 0;
	imagefile	= wxEmptyString;
}

// fetch results
int CChassisBase::FetchResult(wxSQLite3ResultSet& _q, int _c)
{
	chassis_id	= _q.GetInt64(_c++).ToLong();
	name	= _q.GetAsString(_c++);
	slots	= _q.GetInt64(_c++).ToLong();
	imagefile	= _q.GetAsString(_c++);	return _c;
}
	
// inserts
bool CChassisBase::Insert(bool _autoinc) 
{
	if (!BeforeInsert()) return false;
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
		_sql.Format("INSERT INTO [chassis] ([chassis_id], [name], [slots], [imagefile]) \
		VALUES (%d, '%q', %d, '%q')",
			chassis_id,
			WXSQLGET_S(name),
			slots,
			WXSQLGET_S(imagefile)
		);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CChassisBase::Insert()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}
	
	if (_affected > 0) {
		return true;
	}

	return false;
}

// updates
bool CChassisBase::Update()
{
	if (!BeforeUpdate()) return false;
	
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("UPDATE chassis SET [chassis_id] = %d, [name] = '%q', [slots] = %d, [imagefile] = '%q' \
		WHERE [chassis_id] = %d",
		chassis_id,
		WXSQLGET_S(name),
		slots,
		WXSQLGET_S(imagefile),
		chassis_id
	);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CChassisBase::Update()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}

	if (_affected > 0) {
		return true;
	}

	return false;
}
	
// deletes
bool CChassisBase::Delete()
{
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("DELETE FROM [chassis] WHERE [chassis_id] = %d",
		chassis_id
	);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CChassisBase::Delete()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}

	if (_affected > 0)
		return AfterDelete();

	return false;
}
	
// (static) Selects one record
bool CChassisBase::Select(long& chassis_id_,
	CChassisBase& _ret, wxSQLite3Database& _db)
{
	bool _retok = false;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("SELECT * FROM [chassis] WHERE [chassis_id] = %d",
		chassis_id_
	);

	wxSQLite3ResultSet _q = _db.ExecuteQuery(_sql);
	while (_q.NextRow()) {
		_ret.SetDb(&_db);
		_ret.FetchResult(_q);
		_retok = true;
		break;
	}
	
	_q.Finalize();
	return _retok;
}

/******************************************************************************
 * Class CRoutersBase
 ******************************************************************************/

// (static) create table if not exists
bool CRoutersBase::InitTable(wxSQLite3Database& _db)
{
	wxString _sql = 
"CREATE TABLE IF NOT EXISTS [routers] (\
  [router_id] INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
  [hostname] TEXT(50) NOT NULL,\
  [chassis_id] INTEGER NOT NULL REFERENCES [chassis]([chassis_id]),\
  [partnumber] CHAR(15) NOT NULL,\
  [serialnumber] CHAR(15) NOT NULL,\
  [manufacturing] DATE NOT NULL);";
	_db.ExecuteUpdate( _sql );
	return true;
}

// c-tors
CRoutersBase::CRoutersBase(wxSQLite3Database *_db):
		CDbTableBase(_db) 
{
	m_tablename = TRouters;
	router_id	= 0;
	hostname	= wxEmptyString;
	chassis_id	= 0;
	partnumber	= wxEmptyString;
	serialnumber	= wxEmptyString;
	manufacturing.SetToCurrent();
}

// fetch results
int CRoutersBase::FetchResult(wxSQLite3ResultSet& _q, int _c)
{
	router_id	= _q.GetInt64(_c++).ToLong();
	hostname	= _q.GetAsString(_c++);
	chassis_id	= _q.GetInt64(_c++).ToLong();
	partnumber	= _q.GetAsString(_c++);
	serialnumber	= _q.GetAsString(_c++);
	manufacturing.ParseISOCombined( _q.GetAsString(_c++) );	return _c;
}
	
// inserts
bool CRoutersBase::Insert(bool _autoinc) 
{
	if (!BeforeInsert()) return false;
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	if (!_autoinc) {
		_sql.Format("INSERT INTO [routers] ([router_id], [hostname], [chassis_id], [partnumber], [serialnumber], [manufacturing]) \
		VALUES (%d, '%q', %d, '%q', '%q', '%s')",
			router_id,
			WXSQLGET_S(hostname),
			chassis_id,
			WXSQLGET_S(partnumber),
			WXSQLGET_S(serialnumber),
			WXSQLGET_DT(manufacturing)
		);
	} else {
		_sql.Format("INSERT INTO [routers] ([hostname], [chassis_id], [partnumber], [serialnumber], [manufacturing]) \
		VALUES ('%q', %d, '%q', '%q', '%s')",
			WXSQLGET_S(hostname),
			chassis_id,
			WXSQLGET_S(partnumber),
			WXSQLGET_S(serialnumber),
			WXSQLGET_DT(manufacturing)
		);
	}

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CRoutersBase::Insert()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}
	
	if (_affected > 0) {
		long _rowid = m_db->GetLastRowId().ToLong();
		if (_rowid != 0) router_id = _rowid;
		return true;
	}

	return false;
}

// updates
bool CRoutersBase::Update()
{
	if (!BeforeUpdate()) return false;
	
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("UPDATE routers SET [router_id] = %d, [hostname] = '%q', [chassis_id] = %d, [partnumber] = '%q', [serialnumber] = '%q', [manufacturing] = '%s' \
		WHERE [router_id] = %d",
		router_id,
		WXSQLGET_S(hostname),
		chassis_id,
		WXSQLGET_S(partnumber),
		WXSQLGET_S(serialnumber),
		WXSQLGET_DT(manufacturing),
		router_id
	);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CRoutersBase::Update()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}

	if (_affected > 0) {
		return true;
	}

	return false;
}
	
// deletes
bool CRoutersBase::Delete()
{
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("DELETE FROM [routers] WHERE [router_id] = %d",
		router_id
	);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CRoutersBase::Delete()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}

	if (_affected > 0)
		return AfterDelete();

	return false;
}
	
// (static) Selects one record
bool CRoutersBase::Select(long& router_id_,
	CRoutersBase& _ret, wxSQLite3Database& _db)
{
	bool _retok = false;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("SELECT * FROM [routers] WHERE [router_id] = %d",
		router_id_
	);

	wxSQLite3ResultSet _q = _db.ExecuteQuery(_sql);
	while (_q.NextRow()) {
		_ret.SetDb(&_db);
		_ret.FetchResult(_q);
		_retok = true;
		break;
	}
	
	_q.Finalize();
	return _retok;
}

/******************************************************************************
 * Class CGrfNodesBase
 ******************************************************************************/

// (static) create table if not exists
bool CGrfNodesBase::InitTable(wxSQLite3Database& _db)
{
	wxString _sql = 
"CREATE TABLE IF NOT EXISTS [grf_nodes] (\
  [node_id] INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
  [router_id] INTEGER NOT NULL REFERENCES [routers]([router_id]),\
  [x] INTEGER,\
  [y] INTEGER,\
  [w] INTEGER,\
  [h] INTEGER);";
	_db.ExecuteUpdate( _sql );
	return true;
}

// c-tors
CGrfNodesBase::CGrfNodesBase(wxSQLite3Database *_db):
		CDbTableBase(_db) 
{
	m_tablename = TGrfNodes;
	node_id	= 0;
	router_id	= 0;
	x	= 0;
	y	= 0;
	w	= 0;
	h	= 0;
}

// fetch results
int CGrfNodesBase::FetchResult(wxSQLite3ResultSet& _q, int _c)
{
	node_id	= _q.GetInt64(_c++).ToLong();
	router_id	= _q.GetInt64(_c++).ToLong();
	x	= _q.GetInt64(_c++).ToLong();
	y	= _q.GetInt64(_c++).ToLong();
	w	= _q.GetInt64(_c++).ToLong();
	h	= _q.GetInt64(_c++).ToLong();	return _c;
}
	
// inserts
bool CGrfNodesBase::Insert(bool _autoinc) 
{
	if (!BeforeInsert()) return false;
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	if (!_autoinc) {
		_sql.Format("INSERT INTO [grf_nodes] ([node_id], [router_id], [x], [y], [w], [h]) \
		VALUES (%d, %d, %d, %d, %d, %d)",
			node_id,
			router_id,
			x,
			y,
			w,
			h
		);
	} else {
		_sql.Format("INSERT INTO [grf_nodes] ([router_id], [x], [y], [w], [h]) \
		VALUES (%d, %d, %d, %d, %d)",
			router_id,
			x,
			y,
			w,
			h
		);
	}

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CGrfNodesBase::Insert()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}
	
	if (_affected > 0) {
		long _rowid = m_db->GetLastRowId().ToLong();
		if (_rowid != 0) node_id = _rowid;
		return true;
	}

	return false;
}

// updates
bool CGrfNodesBase::Update()
{
	if (!BeforeUpdate()) return false;
	
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("UPDATE grf_nodes SET [node_id] = %d, [router_id] = %d, [x] = %d, [y] = %d, [w] = %d, [h] = %d \
		WHERE [node_id] = %d",
		node_id,
		router_id,
		x,
		y,
		w,
		h,
		node_id
	);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CGrfNodesBase::Update()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}

	if (_affected > 0) {
		return true;
	}

	return false;
}
	
// deletes
bool CGrfNodesBase::Delete()
{
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("DELETE FROM [grf_nodes] WHERE [node_id] = %d",
		node_id
	);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CGrfNodesBase::Delete()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}

	if (_affected > 0)
		return AfterDelete();

	return false;
}
	
// (static) Selects one record
bool CGrfNodesBase::Select(long& node_id_,
	CGrfNodesBase& _ret, wxSQLite3Database& _db)
{
	bool _retok = false;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("SELECT * FROM [grf_nodes] WHERE [node_id] = %d",
		node_id_
	);

	wxSQLite3ResultSet _q = _db.ExecuteQuery(_sql);
	while (_q.NextRow()) {
		_ret.SetDb(&_db);
		_ret.FetchResult(_q);
		_retok = true;
		break;
	}
	
	_q.Finalize();
	return _retok;
}

/******************************************************************************
 * Class CPortsBase
 ******************************************************************************/

// (static) create table if not exists
bool CPortsBase::InitTable(wxSQLite3Database& _db)
{
	wxString _sql = 
"CREATE TABLE IF NOT EXISTS [ports] (\
  [router_id] INTEGER NOT NULL REFERENCES [routers]([router_id]),\
  [port_id] INTEGER NOT NULL,\
  [name] CHAR(15) NOT NULL,\
  [flags] CHAR(4),\
  [description] TEXT,\
  [ethernetmode] CHAR(1),\
  [ethernetencap] CHAR(1),\
  CONSTRAINT [sqlite_autoindex_ports_1] PRIMARY KEY ([router_id], [port_id]));\
CREATE UNIQUE INDEX IF NOT EXISTS [port_name] ON [ports] ([name]);";
	_db.ExecuteUpdate( _sql );
	return true;
}

// c-tors
CPortsBase::CPortsBase(wxSQLite3Database *_db):
		CDbTableBase(_db) 
{
	m_tablename = TPorts;
	router_id	= 0;
	port_id	= 0;
	name	= wxEmptyString;
	flags	= wxEmptyString;
	description	= wxEmptyString;
	ethernetmode	= wxEmptyString;
	ethernetencap	= wxEmptyString;
}

// fetch results
int CPortsBase::FetchResult(wxSQLite3ResultSet& _q, int _c)
{
	router_id	= _q.GetInt64(_c++).ToLong();
	port_id	= _q.GetInt64(_c++).ToLong();
	name	= _q.GetAsString(_c++);
	flags	= _q.GetAsString(_c++);
	description	= _q.GetAsString(_c++);
	ethernetmode	= _q.GetAsString(_c++);
	ethernetencap	= _q.GetAsString(_c++);	return _c;
}
	
// inserts
bool CPortsBase::Insert(bool _autoinc) 
{
	if (!BeforeInsert()) return false;
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
		_sql.Format("INSERT INTO [ports] ([router_id], [port_id], [name], [flags], [description], [ethernetmode], [ethernetencap]) \
		VALUES (%d, %d, '%q', '%q', '%q', '%q', '%q')",
			router_id,
			port_id,
			WXSQLGET_S(name),
			WXSQLGET_S(flags),
			WXSQLGET_S(description),
			WXSQLGET_S(ethernetmode),
			WXSQLGET_S(ethernetencap)
		);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CPortsBase::Insert()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}
	
	if (_affected > 0) {
		return true;
	}

	return false;
}

// updates
bool CPortsBase::Update()
{
	if (!BeforeUpdate()) return false;
	
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("UPDATE ports SET [router_id] = %d, [port_id] = %d, [name] = '%q', [flags] = '%q', [description] = '%q', [ethernetmode] = '%q', [ethernetencap] = '%q' \
		WHERE [router_id] = %d AND [port_id] = %d",
		router_id,
		port_id,
		WXSQLGET_S(name),
		WXSQLGET_S(flags),
		WXSQLGET_S(description),
		WXSQLGET_S(ethernetmode),
		WXSQLGET_S(ethernetencap),
		router_id,
		port_id
	);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CPortsBase::Update()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}

	if (_affected > 0) {
		return true;
	}

	return false;
}
	
// deletes
bool CPortsBase::Delete()
{
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("DELETE FROM [ports] WHERE [router_id] = %d AND [port_id] = %d",
		router_id,
			port_id
	);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CPortsBase::Delete()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}

	if (_affected > 0)
		return AfterDelete();

	return false;
}
	
// (static) Selects one record
bool CPortsBase::Select(long& router_id_, long& port_id_,
	CPortsBase& _ret, wxSQLite3Database& _db)
{
	bool _retok = false;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("SELECT * FROM [ports] WHERE [router_id] = %d AND [port_id] = %d",
		router_id_,
			port_id_
	);

	wxSQLite3ResultSet _q = _db.ExecuteQuery(_sql);
	while (_q.NextRow()) {
		_ret.SetDb(&_db);
		_ret.FetchResult(_q);
		_retok = true;
		break;
	}
	
	_q.Finalize();
	return _retok;
}

/******************************************************************************
 * Class CSpeedsBase
 ******************************************************************************/

// (static) create table if not exists
bool CSpeedsBase::InitTable(wxSQLite3Database& _db)
{
	wxString _sql = 
"CREATE TABLE IF NOT EXISTS [speeds] (\
  [speed_id] INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
  [name] CHAR(30) NOT NULL);";
	_db.ExecuteUpdate( _sql );
	return true;
}

// c-tors
CSpeedsBase::CSpeedsBase(wxSQLite3Database *_db):
		CDbTableBase(_db) 
{
	m_tablename = TSpeeds;
	speed_id	= 0;
	name	= wxEmptyString;
}

// fetch results
int CSpeedsBase::FetchResult(wxSQLite3ResultSet& _q, int _c)
{
	speed_id	= _q.GetInt64(_c++).ToLong();
	name	= _q.GetAsString(_c++);	return _c;
}
	
// inserts
bool CSpeedsBase::Insert(bool _autoinc) 
{
	if (!BeforeInsert()) return false;
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	if (!_autoinc) {
		_sql.Format("INSERT INTO [speeds] ([speed_id], [name]) \
		VALUES (%d, '%q')",
			speed_id,
			WXSQLGET_S(name)
		);
	} else {
		_sql.Format("INSERT INTO [speeds] ([name]) \
		VALUES ('%q')",
			WXSQLGET_S(name)
		);
	}

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CSpeedsBase::Insert()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}
	
	if (_affected > 0) {
		long _rowid = m_db->GetLastRowId().ToLong();
		if (_rowid != 0) speed_id = _rowid;
		return true;
	}

	return false;
}

// updates
bool CSpeedsBase::Update()
{
	if (!BeforeUpdate()) return false;
	
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("UPDATE speeds SET [speed_id] = %d, [name] = '%q' \
		WHERE [speed_id] = %d",
		speed_id,
		WXSQLGET_S(name),
		speed_id
	);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CSpeedsBase::Update()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}

	if (_affected > 0) {
		return true;
	}

	return false;
}
	
// deletes
bool CSpeedsBase::Delete()
{
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("DELETE FROM [speeds] WHERE [speed_id] = %d",
		speed_id
	);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CSpeedsBase::Delete()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}

	if (_affected > 0)
		return AfterDelete();

	return false;
}
	
// (static) Selects one record
bool CSpeedsBase::Select(long& speed_id_,
	CSpeedsBase& _ret, wxSQLite3Database& _db)
{
	bool _retok = false;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("SELECT * FROM [speeds] WHERE [speed_id] = %d",
		speed_id_
	);

	wxSQLite3ResultSet _q = _db.ExecuteQuery(_sql);
	while (_q.NextRow()) {
		_ret.SetDb(&_db);
		_ret.FetchResult(_q);
		_retok = true;
		break;
	}
	
	_q.Finalize();
	return _retok;
}

/******************************************************************************
 * Class CLinksBase
 ******************************************************************************/

// (static) create table if not exists
bool CLinksBase::InitTable(wxSQLite3Database& _db)
{
	wxString _sql = 
"CREATE TABLE IF NOT EXISTS [links] (\
  [link_id] INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,\
  [router_a_id] INTEGER NOT NULL,\
  [port_a_id] INTEGER NOT NULL,\
  [router_b_id] INTEGER NOT NULL,\
  [port_b_id] INTEGER NOT NULL,\
  [cable_id] INTEGER NOT NULL REFERENCES [cables]([cable_id]),\
  [speed_id] INTEGER NOT NULL REFERENCES [speeds],\
  CONSTRAINT [router_b_port] FOREIGN KEY([router_b_id], [port_b_id]) REFERENCES [ports]([router_id], [port_id]),\
  CONSTRAINT [router_a_port] FOREIGN KEY([router_a_id], [port_a_id]) REFERENCES [ports]([router_id], [port_id]));";
	_db.ExecuteUpdate( _sql );
	return true;
}

// c-tors
CLinksBase::CLinksBase(wxSQLite3Database *_db):
		CDbTableBase(_db) 
{
	m_tablename = TLinks;
	link_id	= 0;
	router_a_id	= 0;
	port_a_id	= 0;
	router_b_id	= 0;
	port_b_id	= 0;
	cable_id	= 0;
	speed_id	= 0;
}

// fetch results
int CLinksBase::FetchResult(wxSQLite3ResultSet& _q, int _c)
{
	link_id	= _q.GetInt64(_c++).ToLong();
	router_a_id	= _q.GetInt64(_c++).ToLong();
	port_a_id	= _q.GetInt64(_c++).ToLong();
	router_b_id	= _q.GetInt64(_c++).ToLong();
	port_b_id	= _q.GetInt64(_c++).ToLong();
	cable_id	= _q.GetInt64(_c++).ToLong();
	speed_id	= _q.GetInt64(_c++).ToLong();	return _c;
}
	
// inserts
bool CLinksBase::Insert(bool _autoinc) 
{
	if (!BeforeInsert()) return false;
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	if (!_autoinc) {
		_sql.Format("INSERT INTO [links] ([link_id], [router_a_id], [port_a_id], [router_b_id], [port_b_id], [cable_id], [speed_id]) \
		VALUES (%d, %d, %d, %d, %d, %d, %d)",
			link_id,
			router_a_id,
			port_a_id,
			router_b_id,
			port_b_id,
			cable_id,
			speed_id
		);
	} else {
		_sql.Format("INSERT INTO [links] ([router_a_id], [port_a_id], [router_b_id], [port_b_id], [cable_id], [speed_id]) \
		VALUES (%d, %d, %d, %d, %d, %d)",
			router_a_id,
			port_a_id,
			router_b_id,
			port_b_id,
			cable_id,
			speed_id
		);
	}

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CLinksBase::Insert()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}
	
	if (_affected > 0) {
		long _rowid = m_db->GetLastRowId().ToLong();
		if (_rowid != 0) link_id = _rowid;
		return true;
	}

	return false;
}

// updates
bool CLinksBase::Update()
{
	if (!BeforeUpdate()) return false;
	
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("UPDATE links SET [link_id] = %d, [router_a_id] = %d, [port_a_id] = %d, [router_b_id] = %d, [port_b_id] = %d, [cable_id] = %d, [speed_id] = %d \
		WHERE [link_id] = %d",
		link_id,
		router_a_id,
		port_a_id,
		router_b_id,
		port_b_id,
		cable_id,
		speed_id,
		link_id
	);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CLinksBase::Update()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}

	if (_affected > 0) {
		return true;
	}

	return false;
}
	
// deletes
bool CLinksBase::Delete()
{
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("DELETE FROM [links] WHERE [link_id] = %d",
		link_id
	);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CLinksBase::Delete()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}

	if (_affected > 0)
		return AfterDelete();

	return false;
}
	
// (static) Selects one record
bool CLinksBase::Select(long& link_id_,
	CLinksBase& _ret, wxSQLite3Database& _db)
{
	bool _retok = false;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("SELECT * FROM [links] WHERE [link_id] = %d",
		link_id_
	);

	wxSQLite3ResultSet _q = _db.ExecuteQuery(_sql);
	while (_q.NextRow()) {
		_ret.SetDb(&_db);
		_ret.FetchResult(_q);
		_retok = true;
		break;
	}
	
	_q.Finalize();
	return _retok;
}

/******************************************************************************
 * Class CGrfPointsBase
 ******************************************************************************/

// (static) create table if not exists
bool CGrfPointsBase::InitTable(wxSQLite3Database& _db)
{
	wxString _sql = 
"CREATE TABLE IF NOT EXISTS [grf_points] (\
  [link_id] INTEGER REFERENCES [links]([link_id]),\
  [no] TINYINT,\
  [x] INTEGER,\
  [y] INTEGER,\
  [flags] CHAR(4),\
  CONSTRAINT [sqlite_autoindex_grf_points_1] PRIMARY KEY ([link_id], [no]));";
	_db.ExecuteUpdate( _sql );
	return true;
}

// c-tors
CGrfPointsBase::CGrfPointsBase(wxSQLite3Database *_db):
		CDbTableBase(_db) 
{
	m_tablename = TGrfPoints;
	link_id	= 0;
	no	= 0;
	x	= 0;
	y	= 0;
	flags	= wxEmptyString;
}

// fetch results
int CGrfPointsBase::FetchResult(wxSQLite3ResultSet& _q, int _c)
{
	link_id	= _q.GetInt64(_c++).ToLong();
	no	= _q.GetInt64(_c++).ToLong();
	x	= _q.GetInt64(_c++).ToLong();
	y	= _q.GetInt64(_c++).ToLong();
	flags	= _q.GetAsString(_c++);	return _c;
}
	
// inserts
bool CGrfPointsBase::Insert(bool _autoinc) 
{
	if (!BeforeInsert()) return false;
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
		_sql.Format("INSERT INTO [grf_points] ([link_id], [no], [x], [y], [flags]) \
		VALUES (%d, %d, %d, %d, '%q')",
			link_id,
			no,
			x,
			y,
			WXSQLGET_S(flags)
		);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CGrfPointsBase::Insert()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}
	
	if (_affected > 0) {
		return true;
	}

	return false;
}

// updates
bool CGrfPointsBase::Update()
{
	if (!BeforeUpdate()) return false;
	
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("UPDATE grf_points SET [link_id] = %d, [no] = %d, [x] = %d, [y] = %d, [flags] = '%q' \
		WHERE [link_id] = %d AND [no] = %d",
		link_id,
		no,
		x,
		y,
		WXSQLGET_S(flags),
		link_id,
		no
	);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CGrfPointsBase::Update()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}

	if (_affected > 0) {
		return true;
	}

	return false;
}
	
// deletes
bool CGrfPointsBase::Delete()
{
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("DELETE FROM [grf_points] WHERE [link_id] = %d AND [no] = %d",
		link_id,
			no
	);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CGrfPointsBase::Delete()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}

	if (_affected > 0)
		return AfterDelete();

	return false;
}
	
// (static) Selects one record
bool CGrfPointsBase::Select(long& link_id_, long& no_,
	CGrfPointsBase& _ret, wxSQLite3Database& _db)
{
	bool _retok = false;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("SELECT * FROM [grf_points] WHERE [link_id] = %d AND [no] = %d",
		link_id_,
			no_
	);

	wxSQLite3ResultSet _q = _db.ExecuteQuery(_sql);
	while (_q.NextRow()) {
		_ret.SetDb(&_db);
		_ret.FetchResult(_q);
		_retok = true;
		break;
	}
	
	_q.Finalize();
	return _retok;
}

/******************************************************************************
 * Class CInterfacesBase
 ******************************************************************************/

// (static) create table if not exists
bool CInterfacesBase::InitTable(wxSQLite3Database& _db)
{
	wxString _sql = 
"CREATE TABLE IF NOT EXISTS [interfaces] (\
  [router_id] INTEGER NOT NULL REFERENCES [routers]([router_id]),\
  [interface_id] INTEGER NOT NULL,\
  [name] CHAR(50),\
  [ipaddress] CHAR(15),\
  [ipaddressmask] CHAR(15),\
  [port_id] INTEGER,\
  [lag_id] INTEGER,\
  CONSTRAINT [sqlite_autoindex_interfaces_1] PRIMARY KEY ([router_id], [interface_id]));\
CREATE INDEX IF NOT EXISTS [router_lag] ON [interfaces] ([router_id], [lag_id]);\
CREATE UNIQUE INDEX IF NOT EXISTS [router_port] ON [interfaces] ([router_id], [port_id]);";
	_db.ExecuteUpdate( _sql );
	return true;
}

// c-tors
CInterfacesBase::CInterfacesBase(wxSQLite3Database *_db):
		CDbTableBase(_db) 
{
	m_tablename = TInterfaces;
	router_id	= 0;
	interface_id	= 0;
	name	= wxEmptyString;
	ipaddress	= wxEmptyString;
	ipaddressmask	= wxEmptyString;
	port_id	= 0;
	lag_id	= 0;
}

// fetch results
int CInterfacesBase::FetchResult(wxSQLite3ResultSet& _q, int _c)
{
	router_id	= _q.GetInt64(_c++).ToLong();
	interface_id	= _q.GetInt64(_c++).ToLong();
	name	= _q.GetAsString(_c++);
	ipaddress	= _q.GetAsString(_c++);
	ipaddressmask	= _q.GetAsString(_c++);
	port_id	= _q.GetInt64(_c++).ToLong();
	lag_id	= _q.GetInt64(_c++).ToLong();	return _c;
}
	
// inserts
bool CInterfacesBase::Insert(bool _autoinc) 
{
	if (!BeforeInsert()) return false;
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
		_sql.Format("INSERT INTO [interfaces] ([router_id], [interface_id], [name], [ipaddress], [ipaddressmask], [port_id], [lag_id]) \
		VALUES (%d, %d, '%q', '%q', '%q', %d, %d)",
			router_id,
			interface_id,
			WXSQLGET_S(name),
			WXSQLGET_S(ipaddress),
			WXSQLGET_S(ipaddressmask),
			port_id,
			lag_id
		);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CInterfacesBase::Insert()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}
	
	if (_affected > 0) {
		return true;
	}

	return false;
}

// updates
bool CInterfacesBase::Update()
{
	if (!BeforeUpdate()) return false;
	
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("UPDATE interfaces SET [router_id] = %d, [interface_id] = %d, [name] = '%q', [ipaddress] = '%q', [ipaddressmask] = '%q', [port_id] = %d, [lag_id] = %d \
		WHERE [router_id] = %d AND [interface_id] = %d",
		router_id,
		interface_id,
		WXSQLGET_S(name),
		WXSQLGET_S(ipaddress),
		WXSQLGET_S(ipaddressmask),
		port_id,
		lag_id,
		router_id,
		interface_id
	);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CInterfacesBase::Update()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}

	if (_affected > 0) {
		return true;
	}

	return false;
}
	
// deletes
bool CInterfacesBase::Delete()
{
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("DELETE FROM [interfaces] WHERE [router_id] = %d AND [interface_id] = %d",
		router_id,
			interface_id
	);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CInterfacesBase::Delete()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}

	if (_affected > 0)
		return AfterDelete();

	return false;
}
	
// (static) Selects one record
bool CInterfacesBase::Select(long& router_id_, long& interface_id_,
	CInterfacesBase& _ret, wxSQLite3Database& _db)
{
	bool _retok = false;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("SELECT * FROM [interfaces] WHERE [router_id] = %d AND [interface_id] = %d",
		router_id_,
			interface_id_
	);

	wxSQLite3ResultSet _q = _db.ExecuteQuery(_sql);
	while (_q.NextRow()) {
		_ret.SetDb(&_db);
		_ret.FetchResult(_q);
		_retok = true;
		break;
	}
	
	_q.Finalize();
	return _retok;
}

/******************************************************************************
 * Class CLagsBase
 ******************************************************************************/

// (static) create table if not exists
bool CLagsBase::InitTable(wxSQLite3Database& _db)
{
	wxString _sql = 
"CREATE TABLE IF NOT EXISTS [lags] (\
  [router_id] INTEGER NOT NULL,\
  [lag_id] INTEGER NOT NULL,\
  [flags] CHAR(4),\
  CONSTRAINT [sqlite_autoindex_lags_1] PRIMARY KEY ([router_id], [lag_id]));";
	_db.ExecuteUpdate( _sql );
	return true;
}

// c-tors
CLagsBase::CLagsBase(wxSQLite3Database *_db):
		CDbTableBase(_db) 
{
	m_tablename = TLags;
	router_id	= 0;
	lag_id	= 0;
	flags	= wxEmptyString;
}

// fetch results
int CLagsBase::FetchResult(wxSQLite3ResultSet& _q, int _c)
{
	router_id	= _q.GetInt64(_c++).ToLong();
	lag_id	= _q.GetInt64(_c++).ToLong();
	flags	= _q.GetAsString(_c++);	return _c;
}
	
// inserts
bool CLagsBase::Insert(bool _autoinc) 
{
	if (!BeforeInsert()) return false;
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
		_sql.Format("INSERT INTO [lags] ([router_id], [lag_id], [flags]) \
		VALUES (%d, %d, '%q')",
			router_id,
			lag_id,
			WXSQLGET_S(flags)
		);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CLagsBase::Insert()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}
	
	if (_affected > 0) {
		return true;
	}

	return false;
}

// updates
bool CLagsBase::Update()
{
	if (!BeforeUpdate()) return false;
	
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("UPDATE lags SET [router_id] = %d, [lag_id] = %d, [flags] = '%q' \
		WHERE [router_id] = %d AND [lag_id] = %d",
		router_id,
		lag_id,
		WXSQLGET_S(flags),
		router_id,
		lag_id
	);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CLagsBase::Update()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}

	if (_affected > 0) {
		return true;
	}

	return false;
}
	
// deletes
bool CLagsBase::Delete()
{
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("DELETE FROM [lags] WHERE [router_id] = %d AND [lag_id] = %d",
		router_id,
			lag_id
	);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CLagsBase::Delete()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}

	if (_affected > 0)
		return AfterDelete();

	return false;
}
	
// (static) Selects one record
bool CLagsBase::Select(long& router_id_, long& lag_id_,
	CLagsBase& _ret, wxSQLite3Database& _db)
{
	bool _retok = false;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("SELECT * FROM [lags] WHERE [router_id] = %d AND [lag_id] = %d",
		router_id_,
			lag_id_
	);

	wxSQLite3ResultSet _q = _db.ExecuteQuery(_sql);
	while (_q.NextRow()) {
		_ret.SetDb(&_db);
		_ret.FetchResult(_q);
		_retok = true;
		break;
	}
	
	_q.Finalize();
	return _retok;
}

/******************************************************************************
 * Class CLagportsBase
 ******************************************************************************/

// (static) create table if not exists
bool CLagportsBase::InitTable(wxSQLite3Database& _db)
{
	wxString _sql = 
"CREATE TABLE IF NOT EXISTS [lagports] (\
  [router_id] INTEGER NOT NULL,\
  [lag_id] INTEGER NOT NULL,\
  [port_id] INTEGER NOT NULL,\
  [flags] CHAR(4),\
  CONSTRAINT [router_port] FOREIGN KEY([router_id], [port_id]) REFERENCES [ports]([router_id], [port_id]),\
  CONSTRAINT [router_lag] FOREIGN KEY([router_id], [lag_id]) REFERENCES [lags]([router_id], [lag_id]),\
  CONSTRAINT [sqlite_autoindex_lagports_1] PRIMARY KEY ([router_id], [lag_id], [port_id]));";
	_db.ExecuteUpdate( _sql );
	return true;
}

// c-tors
CLagportsBase::CLagportsBase(wxSQLite3Database *_db):
		CDbTableBase(_db) 
{
	m_tablename = TLagports;
	router_id	= 0;
	lag_id	= 0;
	port_id	= 0;
	flags	= wxEmptyString;
}

// fetch results
int CLagportsBase::FetchResult(wxSQLite3ResultSet& _q, int _c)
{
	router_id	= _q.GetInt64(_c++).ToLong();
	lag_id	= _q.GetInt64(_c++).ToLong();
	port_id	= _q.GetInt64(_c++).ToLong();
	flags	= _q.GetAsString(_c++);	return _c;
}
	
// inserts
bool CLagportsBase::Insert(bool _autoinc) 
{
	if (!BeforeInsert()) return false;
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
		_sql.Format("INSERT INTO [lagports] ([router_id], [lag_id], [port_id], [flags]) \
		VALUES (%d, %d, %d, '%q')",
			router_id,
			lag_id,
			port_id,
			WXSQLGET_S(flags)
		);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CLagportsBase::Insert()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}
	
	if (_affected > 0) {
		return true;
	}

	return false;
}

// updates
bool CLagportsBase::Update()
{
	if (!BeforeUpdate()) return false;
	
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("UPDATE lagports SET [router_id] = %d, [lag_id] = %d, [port_id] = %d, [flags] = '%q' \
		WHERE [router_id] = %d AND [lag_id] = %d AND [port_id] = %d",
		router_id,
		lag_id,
		port_id,
		WXSQLGET_S(flags),
		router_id,
		lag_id,
		port_id
	);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CLagportsBase::Update()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}

	if (_affected > 0) {
		return true;
	}

	return false;
}
	
// deletes
bool CLagportsBase::Delete()
{
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("DELETE FROM [lagports] WHERE [router_id] = %d AND [lag_id] = %d AND [port_id] = %d",
		router_id,
			lag_id,
			port_id
	);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - CLagportsBase::Delete()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}

	if (_affected > 0)
		return AfterDelete();

	return false;
}
	
// (static) Selects one record
bool CLagportsBase::Select(long& router_id_, long& lag_id_, long& port_id_,
	CLagportsBase& _ret, wxSQLite3Database& _db)
{
	bool _retok = false;
	wxSQLite3StatementBuffer _sql;
	_sql.Format("SELECT * FROM [lagports] WHERE [router_id] = %d AND [lag_id] = %d AND [port_id] = %d",
		router_id_,
			lag_id_,
			port_id_
	);

	wxSQLite3ResultSet _q = _db.ExecuteQuery(_sql);
	while (_q.NextRow()) {
		_ret.SetDb(&_db);
		_ret.FetchResult(_q);
		_retok = true;
		break;
	}
	
	_q.Finalize();
	return _retok;
}

