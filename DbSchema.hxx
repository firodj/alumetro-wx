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

class DbBaseTable {
public:
    DbBaseTable(wxSQLite3Database *_db = NULL): m_tablename(TbNone), m_db(_db) {};
    wxSQLite3Database* GetDb() const { return m_db; }
    void SetDb(wxSQLite3Database *_db) { m_db = _db; }
    
public:
    enum {
		TbNone	= 0,
		TbCables	= 1,
		TbChassis	= 2,
		TbRouters	= 3,
		TbGrfNodes	= 4,
		TbPorts	= 5,
		TbSpeeds	= 6,
		TbLinks	= 7,
		TbGrfPoints	= 8,
		TbInterfaces	= 9,
		TbLags	= 10,
		TbLagports	= 11,
    } m_tablename;
    
protected:
    wxSQLite3Database* m_db;
};

///////////////////////////////////////////////////////////////////////////////
// Class DbCablesBase
class DbCablesBase: public DbBaseTable {
public:
	// members
	long	cable_id;
	wxString	name;

	// virtuals
	virtual bool BeforeInsert()	{ return true; };
	virtual bool BeforeUpdate()	{ return true; };
	virtual bool AfterDelete()	{ return true; };
	virtual int  AfterFetch()	{ return 0; };
    
	// statics
	// create table
	static bool InitTable(wxSQLite3Database& _db);
	// select
	static bool Select(long& cable_id_,
		DbCablesBase& _ret, wxSQLite3Database& _db);
	
	// c-tors
	DbCablesBase(wxSQLite3Database *_db=NULL);

	int FetchResult(wxSQLite3ResultSet& _q, int _c=0);
	virtual bool Insert(bool _autoinc=false);
	virtual bool Update();
	bool Delete();
};

// Class DbChassisBase
class DbChassisBase: public DbBaseTable {
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
	virtual int  AfterFetch()	{ return 0; };
    
	// statics
	// create table
	static bool InitTable(wxSQLite3Database& _db);
	// select
	static bool Select(long& chassis_id_,
		DbChassisBase& _ret, wxSQLite3Database& _db);
	
	// c-tors
	DbChassisBase(wxSQLite3Database *_db=NULL);

	int FetchResult(wxSQLite3ResultSet& _q, int _c=0);
	virtual bool Insert(bool _autoinc=false);
	virtual bool Update();
	bool Delete();
};

// Class DbRoutersBase
class DbRoutersBase: public DbBaseTable {
public:
	// members
	long	router_id;
	wxString	hostname;
	long	chassis_id;
	wxString	partnumber;
	wxString	serialnumber;
	wxDateTime	manufacturing;
	wxString	ipsystem;

	// virtuals
	virtual bool BeforeInsert()	{ return true; };
	virtual bool BeforeUpdate()	{ return true; };
	virtual bool AfterDelete()	{ return true; };
	virtual int  AfterFetch()	{ return 0; };
    
	// statics
	// create table
	static bool InitTable(wxSQLite3Database& _db);
	// select
	static bool Select(long& router_id_,
		DbRoutersBase& _ret, wxSQLite3Database& _db);
	
	// c-tors
	DbRoutersBase(wxSQLite3Database *_db=NULL);

	int FetchResult(wxSQLite3ResultSet& _q, int _c=0);
	virtual bool Insert(bool _autoinc=false);
	virtual bool Update();
	bool Delete();
};

// Class DbGrfNodesBase
class DbGrfNodesBase: public DbBaseTable {
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
	virtual int  AfterFetch()	{ return 0; };
    
	// statics
	// create table
	static bool InitTable(wxSQLite3Database& _db);
	// select
	static bool Select(long& node_id_,
		DbGrfNodesBase& _ret, wxSQLite3Database& _db);
	
	// c-tors
	DbGrfNodesBase(wxSQLite3Database *_db=NULL);

	int FetchResult(wxSQLite3ResultSet& _q, int _c=0);
	virtual bool Insert(bool _autoinc=false);
	virtual bool Update();
	bool Delete();
};

// Class DbPortsBase
class DbPortsBase: public DbBaseTable {
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
	virtual int  AfterFetch()	{ return 0; };
    
	// statics
	// create table
	static bool InitTable(wxSQLite3Database& _db);
	// select
	static bool Select(long& router_id_, long& port_id_,
		DbPortsBase& _ret, wxSQLite3Database& _db);
	
	// c-tors
	DbPortsBase(wxSQLite3Database *_db=NULL);

	int FetchResult(wxSQLite3ResultSet& _q, int _c=0);
	virtual bool Insert(bool _autoinc=false);
	virtual bool Update();
	bool Delete();
};

// Class DbSpeedsBase
class DbSpeedsBase: public DbBaseTable {
public:
	// members
	long	speed_id;
	wxString	name;

	// virtuals
	virtual bool BeforeInsert()	{ return true; };
	virtual bool BeforeUpdate()	{ return true; };
	virtual bool AfterDelete()	{ return true; };
	virtual int  AfterFetch()	{ return 0; };
    
	// statics
	// create table
	static bool InitTable(wxSQLite3Database& _db);
	// select
	static bool Select(long& speed_id_,
		DbSpeedsBase& _ret, wxSQLite3Database& _db);
	
	// c-tors
	DbSpeedsBase(wxSQLite3Database *_db=NULL);

	int FetchResult(wxSQLite3ResultSet& _q, int _c=0);
	virtual bool Insert(bool _autoinc=false);
	virtual bool Update();
	bool Delete();
};

// Class DbLinksBase
class DbLinksBase: public DbBaseTable {
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
	virtual int  AfterFetch()	{ return 0; };
    
	// statics
	// create table
	static bool InitTable(wxSQLite3Database& _db);
	// select
	static bool Select(long& link_id_,
		DbLinksBase& _ret, wxSQLite3Database& _db);
	
	// c-tors
	DbLinksBase(wxSQLite3Database *_db=NULL);

	int FetchResult(wxSQLite3ResultSet& _q, int _c=0);
	virtual bool Insert(bool _autoinc=false);
	virtual bool Update();
	bool Delete();
};

// Class DbGrfPointsBase
class DbGrfPointsBase: public DbBaseTable {
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
	virtual int  AfterFetch()	{ return 0; };
    
	// statics
	// create table
	static bool InitTable(wxSQLite3Database& _db);
	// select
	static bool Select(long& link_id_, long& no_,
		DbGrfPointsBase& _ret, wxSQLite3Database& _db);
	
	// c-tors
	DbGrfPointsBase(wxSQLite3Database *_db=NULL);

	int FetchResult(wxSQLite3ResultSet& _q, int _c=0);
	virtual bool Insert(bool _autoinc=false);
	virtual bool Update();
	bool Delete();
};

// Class DbInterfacesBase
class DbInterfacesBase: public DbBaseTable {
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
	virtual int  AfterFetch()	{ return 0; };
    
	// statics
	// create table
	static bool InitTable(wxSQLite3Database& _db);
	// select
	static bool Select(long& router_id_, long& interface_id_,
		DbInterfacesBase& _ret, wxSQLite3Database& _db);
	
	// c-tors
	DbInterfacesBase(wxSQLite3Database *_db=NULL);

	int FetchResult(wxSQLite3ResultSet& _q, int _c=0);
	virtual bool Insert(bool _autoinc=false);
	virtual bool Update();
	bool Delete();
};

// Class DbLagsBase
class DbLagsBase: public DbBaseTable {
public:
	// members
	long	router_id;
	long	lag_id;
	wxString	flags;

	// virtuals
	virtual bool BeforeInsert()	{ return true; };
	virtual bool BeforeUpdate()	{ return true; };
	virtual bool AfterDelete()	{ return true; };
	virtual int  AfterFetch()	{ return 0; };
    
	// statics
	// create table
	static bool InitTable(wxSQLite3Database& _db);
	// select
	static bool Select(long& router_id_, long& lag_id_,
		DbLagsBase& _ret, wxSQLite3Database& _db);
	
	// c-tors
	DbLagsBase(wxSQLite3Database *_db=NULL);

	int FetchResult(wxSQLite3ResultSet& _q, int _c=0);
	virtual bool Insert(bool _autoinc=false);
	virtual bool Update();
	bool Delete();
};

// Class DbLagportsBase
class DbLagportsBase: public DbBaseTable {
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
	virtual int  AfterFetch()	{ return 0; };
    
	// statics
	// create table
	static bool InitTable(wxSQLite3Database& _db);
	// select
	static bool Select(long& router_id_, long& lag_id_, long& port_id_,
		DbLagportsBase& _ret, wxSQLite3Database& _db);
	
	// c-tors
	DbLagportsBase(wxSQLite3Database *_db=NULL);

	int FetchResult(wxSQLite3ResultSet& _q, int _c=0);
	virtual bool Insert(bool _autoinc=false);
	virtual bool Update();
	bool Delete();
};

