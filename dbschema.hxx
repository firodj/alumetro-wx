/* File: DbSchema.hxx - Auto Generated */
#pragma once

#include <wx/datetime.h>

class wxSQLite3Database;
class wxSQLite3ResultSet;

// Define default conversion macros
#ifndef WXSQLGET_S
    #define WXSQLGET_S(m_mber) (const char*)(m_mber).utf8_str()
#endif
#ifndef WXSQLGET_LL
    #define WXSQLGET_LL(m_mber) (const char*)(m_mber).ToString()
#endif
#ifndef WXSQLGET_DT
    #define WXSQLGET_DT(m_mber) (const char*)(m_mber).FormatISOCombined()
#endif

class CDbTableBase {
public:
    CDbTableBase(wxSQLite3Database *_db = NULL): m_tablename(TNone), m_db(_db) {};
    wxSQLite3Database* GetDb() const { return m_db; }
    void SetDb(wxSQLite3Database *_db) { m_db = _db; }
    
public:
    enum {
		TNone	= 0,
		TCables	= 1,
		TChassis	= 2,
		TRouters	= 3,
		TGrfNodes	= 4,
		TPorts	= 5,
		TSpeeds	= 6,
		TLinks	= 7,
		TGrfPoints	= 8,
		TInterfaces	= 9,
		TLags	= 10,
		TLagports	= 11,
    } m_tablename;
    
protected:
    wxSQLite3Database* m_db;
};

///////////////////////////////////////////////////////////////////////////////
// Class CCablesBase
class CCablesBase: public CDbTableBase {
public:
	// members
	long	cable_id;
	wxString	name;

	// virtuals
	virtual bool BeforeInsert()	{ return true; };
	virtual bool BeforeUpdate()	{ return true; };
	virtual bool AfterDelete()	{ return true; };
	
	// statics
	// create table
	static bool InitTable(wxSQLite3Database& _db);
	// select
	static bool Select(long& cable_id_,
		CCablesBase& _ret, wxSQLite3Database& _db);
	
	// c-tors
	CCablesBase(wxSQLite3Database *_db=NULL);

	int FetchResult(wxSQLite3ResultSet& _q, int _c=0);
	bool Insert(bool _autoinc=false);
	bool Update();
	bool Delete();
};

// Class CChassisBase
class CChassisBase: public CDbTableBase {
public:
	// members
	long	chassis_id;
	wxString	name;
	long	slots;
	wxString	imagefile;

	// virtuals
	virtual bool BeforeInsert()	{ return true; };
	virtual bool BeforeUpdate()	{ return true; };
	virtual bool AfterDelete()	{ return true; };
	
	// statics
	// create table
	static bool InitTable(wxSQLite3Database& _db);
	// select
	static bool Select(long& chassis_id_,
		CChassisBase& _ret, wxSQLite3Database& _db);
	
	// c-tors
	CChassisBase(wxSQLite3Database *_db=NULL);

	int FetchResult(wxSQLite3ResultSet& _q, int _c=0);
	bool Insert(bool _autoinc=false);
	bool Update();
	bool Delete();
};

// Class CRoutersBase
class CRoutersBase: public CDbTableBase {
public:
	// members
	long	router_id;
	wxString	hostname;
	long	chassis_id;
	wxString	partnumber;
	wxString	serialnumber;
	wxDateTime	manufacturing;

	// virtuals
	virtual bool BeforeInsert()	{ return true; };
	virtual bool BeforeUpdate()	{ return true; };
	virtual bool AfterDelete()	{ return true; };
	
	// statics
	// create table
	static bool InitTable(wxSQLite3Database& _db);
	// select
	static bool Select(long& router_id_,
		CRoutersBase& _ret, wxSQLite3Database& _db);
	
	// c-tors
	CRoutersBase(wxSQLite3Database *_db=NULL);

	int FetchResult(wxSQLite3ResultSet& _q, int _c=0);
	bool Insert(bool _autoinc=false);
	bool Update();
	bool Delete();
};

// Class CGrfNodesBase
class CGrfNodesBase: public CDbTableBase {
public:
	// members
	long	node_id;
	long	router_id;
	long	x;
	long	y;
	long	w;
	long	h;

	// virtuals
	virtual bool BeforeInsert()	{ return true; };
	virtual bool BeforeUpdate()	{ return true; };
	virtual bool AfterDelete()	{ return true; };
	
	// statics
	// create table
	static bool InitTable(wxSQLite3Database& _db);
	// select
	static bool Select(long& node_id_,
		CGrfNodesBase& _ret, wxSQLite3Database& _db);
	
	// c-tors
	CGrfNodesBase(wxSQLite3Database *_db=NULL);

	int FetchResult(wxSQLite3ResultSet& _q, int _c=0);
	bool Insert(bool _autoinc=false);
	bool Update();
	bool Delete();
};

// Class CPortsBase
class CPortsBase: public CDbTableBase {
public:
	// members
	long	router_id;
	long	port_id;
	wxString	name;
	wxString	flags;
	wxString	description;
	wxString	ethernetmode;
	wxString	ethernetencap;

	// virtuals
	virtual bool BeforeInsert()	{ return true; };
	virtual bool BeforeUpdate()	{ return true; };
	virtual bool AfterDelete()	{ return true; };
	
	// statics
	// create table
	static bool InitTable(wxSQLite3Database& _db);
	// select
	static bool Select(long& router_id_, long& port_id_,
		CPortsBase& _ret, wxSQLite3Database& _db);
	
	// c-tors
	CPortsBase(wxSQLite3Database *_db=NULL);

	int FetchResult(wxSQLite3ResultSet& _q, int _c=0);
	bool Insert(bool _autoinc=false);
	bool Update();
	bool Delete();
};

// Class CSpeedsBase
class CSpeedsBase: public CDbTableBase {
public:
	// members
	long	speed_id;
	wxString	name;

	// virtuals
	virtual bool BeforeInsert()	{ return true; };
	virtual bool BeforeUpdate()	{ return true; };
	virtual bool AfterDelete()	{ return true; };
	
	// statics
	// create table
	static bool InitTable(wxSQLite3Database& _db);
	// select
	static bool Select(long& speed_id_,
		CSpeedsBase& _ret, wxSQLite3Database& _db);
	
	// c-tors
	CSpeedsBase(wxSQLite3Database *_db=NULL);

	int FetchResult(wxSQLite3ResultSet& _q, int _c=0);
	bool Insert(bool _autoinc=false);
	bool Update();
	bool Delete();
};

// Class CLinksBase
class CLinksBase: public CDbTableBase {
public:
	// members
	long	link_id;
	long	router_a_id;
	long	port_a_id;
	long	router_b_id;
	long	port_b_id;
	long	cable_id;
	long	speed_id;

	// virtuals
	virtual bool BeforeInsert()	{ return true; };
	virtual bool BeforeUpdate()	{ return true; };
	virtual bool AfterDelete()	{ return true; };
	
	// statics
	// create table
	static bool InitTable(wxSQLite3Database& _db);
	// select
	static bool Select(long& link_id_,
		CLinksBase& _ret, wxSQLite3Database& _db);
	
	// c-tors
	CLinksBase(wxSQLite3Database *_db=NULL);

	int FetchResult(wxSQLite3ResultSet& _q, int _c=0);
	bool Insert(bool _autoinc=false);
	bool Update();
	bool Delete();
};

// Class CGrfPointsBase
class CGrfPointsBase: public CDbTableBase {
public:
	// members
	long	link_id;
	long	no;
	long	x;
	long	y;
	wxString	flags;

	// virtuals
	virtual bool BeforeInsert()	{ return true; };
	virtual bool BeforeUpdate()	{ return true; };
	virtual bool AfterDelete()	{ return true; };
	
	// statics
	// create table
	static bool InitTable(wxSQLite3Database& _db);
	// select
	static bool Select(long& link_id_, long& no_,
		CGrfPointsBase& _ret, wxSQLite3Database& _db);
	
	// c-tors
	CGrfPointsBase(wxSQLite3Database *_db=NULL);

	int FetchResult(wxSQLite3ResultSet& _q, int _c=0);
	bool Insert(bool _autoinc=false);
	bool Update();
	bool Delete();
};

// Class CInterfacesBase
class CInterfacesBase: public CDbTableBase {
public:
	// members
	long	router_id;
	long	interface_id;
	wxString	name;
	wxString	ipaddress;
	wxString	ipaddressmask;
	long	port_id;
	long	lag_id;

	// virtuals
	virtual bool BeforeInsert()	{ return true; };
	virtual bool BeforeUpdate()	{ return true; };
	virtual bool AfterDelete()	{ return true; };
	
	// statics
	// create table
	static bool InitTable(wxSQLite3Database& _db);
	// select
	static bool Select(long& router_id_, long& interface_id_,
		CInterfacesBase& _ret, wxSQLite3Database& _db);
	
	// c-tors
	CInterfacesBase(wxSQLite3Database *_db=NULL);

	int FetchResult(wxSQLite3ResultSet& _q, int _c=0);
	bool Insert(bool _autoinc=false);
	bool Update();
	bool Delete();
};

// Class CLagsBase
class CLagsBase: public CDbTableBase {
public:
	// members
	long	router_id;
	long	lag_id;
	wxString	flags;

	// virtuals
	virtual bool BeforeInsert()	{ return true; };
	virtual bool BeforeUpdate()	{ return true; };
	virtual bool AfterDelete()	{ return true; };
	
	// statics
	// create table
	static bool InitTable(wxSQLite3Database& _db);
	// select
	static bool Select(long& router_id_, long& lag_id_,
		CLagsBase& _ret, wxSQLite3Database& _db);
	
	// c-tors
	CLagsBase(wxSQLite3Database *_db=NULL);

	int FetchResult(wxSQLite3ResultSet& _q, int _c=0);
	bool Insert(bool _autoinc=false);
	bool Update();
	bool Delete();
};

// Class CLagportsBase
class CLagportsBase: public CDbTableBase {
public:
	// members
	long	router_id;
	long	lag_id;
	long	port_id;
	wxString	flags;

	// virtuals
	virtual bool BeforeInsert()	{ return true; };
	virtual bool BeforeUpdate()	{ return true; };
	virtual bool AfterDelete()	{ return true; };
	
	// statics
	// create table
	static bool InitTable(wxSQLite3Database& _db);
	// select
	static bool Select(long& router_id_, long& lag_id_, long& port_id_,
		CLagportsBase& _ret, wxSQLite3Database& _db);
	
	// c-tors
	CLagportsBase(wxSQLite3Database *_db=NULL);

	int FetchResult(wxSQLite3ResultSet& _q, int _c=0);
	bool Insert(bool _autoinc=false);
	bool Update();
	bool Delete();
};

