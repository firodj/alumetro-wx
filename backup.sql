/****************/
/*              */
/* document.sql */
/*              */
/****************/

/* Disable Foreign Keys */
pragma foreign_keys = off;
/* Begin Transaction */
begin transaction;

/* Database [indosat] */
pragma auto_vacuum=0;
pragma default_cache_size=2000;
pragma encoding='UTF-8';
pragma page_size=1024;

/* Drop table [cables] */
drop table if exists [cables];

/* Table structure [cables] */
CREATE TABLE [cables] (
  [cable_id] INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, 
  [name] CHAR(30) NOT NULL);

/* Data [cables] */
insert into [cables] values(1, 'SM-PC');
insert into [cables] values(2, 'MM-PC');
insert into [cables] values(3, 'UTP');


/* Drop table [chassis] */
drop table if exists [chassis];

/* Table structure [chassis] */
CREATE TABLE [chassis] (
  [chassis_id] INTEGER NOT NULL PRIMARY KEY, 
  [name] CHAR(20) NOT NULL, 
  [slots] TINYINT, 
  [imagefile] CHAR(40));

/* Data [chassis] */
insert into [chassis] values(7750120, '7750 SR-12', 12, '7750sr12.png');


/* Drop table [routers] */
drop table if exists [routers];

/* Table structure [routers] */
CREATE TABLE [routers] (
  [router_id] INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, 
  [hostname] TEXT(50) NOT NULL, 
  [chassis_id] INTEGER NOT NULL REFERENCES [chassis]([chassis_id]), 
  [partnumber] CHAR(15) NOT NULL, 
  [serialnumber] CHAR(15) NOT NULL, 
  [manufacturing] DATE NOT NULL);

/* Data [routers] */
insert into [routers] values(1, 'JKT-KPI-CN3-SR12', 7750120, '3HE00104AAAC02', 'NS104360355', '2010-10-22');
insert into [routers] values(4, 'JKT-KPI-CN2-SR12', 7750120, '3HE00104AAAC02', 'NS092662785', '2009-06-27');


/* Drop table [grf_nodes] */
drop table if exists [grf_nodes];

/* Table structure [grf_nodes] */
CREATE TABLE [grf_nodes] (
  [node_id] INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, 
  [router_id] INTEGER NOT NULL REFERENCES [routers]([router_id]), 
  [x] INTEGER, 
  [y] INTEGER, 
  [w] INTEGER, 
  [h] INTEGER);

/* Data [grf_nodes] */
insert into [grf_nodes] values(1, 1, 10, 10, 20, 20);
insert into [grf_nodes] values(2, 4, 100, 10, 20, 20);


/* Drop table [ports] */
drop table if exists [ports];

/* Table structure [ports] */
CREATE TABLE [ports] (
  [router_id] INTEGER NOT NULL REFERENCES [routers]([router_id]), 
  [port_id] INTEGER NOT NULL, 
  [name] CHAR(15) NOT NULL, 
  [flags] CHAR(4), 
  [description] TEXT, 
  [ethernetmode] CHAR(1), 
  [ethernetencap] CHAR(1), 
  CONSTRAINT [sqlite_autoindex_ports_1] PRIMARY KEY ([router_id], [port_id]));
CREATE UNIQUE INDEX [port_name] ON [ports] ([name]);

/* Data [ports] */
insert into [ports] values(1, 102015, '1/2/15', 'AO', 'to-JKT-KPI-CN2-SR12', 'N', 'N');
insert into [ports] values(1, 102016, '1/2/16', 'AO', 'to-JKT-KPI-CN2-SR12', 'N', 'N');
insert into [ports] values(4, 202006, '2/2/6', 'AO', 'to-JKT-KPI-CN3-SR12', 'N', 'N');
insert into [ports] values(4, 902004, '9/2/4', 'AO', 'to-JKT-KPI-CN3-SR12', 'N', 'N');


/* Drop table [speeds] */
drop table if exists [speeds];

/* Table structure [speeds] */
CREATE TABLE [speeds] (
  [speed_id] INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, 
  [name] CHAR(30) NOT NULL);

/* Data [speeds] */
insert into [speeds] values(1000, '1GE');
insert into [speeds] values(10000, '10GE');


/* Drop table [links] */
drop table if exists [links];

/* Table structure [links] */
CREATE TABLE [links] (
  [link_id] INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, 
  [router_a_id] INTEGER NOT NULL, 
  [port_a_id] INTEGER NOT NULL, 
  [router_b_id] INTEGER NOT NULL, 
  [port_b_id] INTEGER NOT NULL, 
  [cable_id] INTEGER NOT NULL REFERENCES [cables]([cable_id]), 
  [speed_id] INTEGER NOT NULL REFERENCES [speeds], 
  CONSTRAINT [router_b_port] FOREIGN KEY([router_b_id], [port_b_id]) REFERENCES [ports]([router_id], [port_id]), 
  CONSTRAINT [router_a_port] FOREIGN KEY([router_a_id], [port_a_id]) REFERENCES [ports]([router_id], [port_id]));

/* Data [links] */
insert into [links] values(1, 1, 102015, 4, 202006, 1, 10000);
insert into [links] values(2, 1, 102016, 4, 902004, 1, 10000);


/* Drop table [grf_points] */
drop table if exists [grf_points];

/* Table structure [grf_points] */
CREATE TABLE [grf_points] (
  [link_id] INTEGER REFERENCES [links]([link_id]), 
  [no] TINYINT, 
  [x] INTEGER, 
  [y] INTEGER, 
  [flags] CHAR(4), 
  CONSTRAINT [sqlite_autoindex_grf_points_1] PRIMARY KEY ([link_id], [no]));

/* Data [grf_points] */
insert into [grf_points] values(1, 1, 10, 10, 'A');
insert into [grf_points] values(1, 2, 100, 10, 'B');
insert into [grf_points] values(2, 1, 10, 12, 'A');
insert into [grf_points] values(2, 2, 100, 12, 'B');


/* Drop table [interfaces] */
drop table if exists [interfaces];

/* Table structure [interfaces] */
CREATE TABLE [interfaces] (
  [router_id] INTEGER NOT NULL REFERENCES [routers]([router_id]), 
  [interface_id] INTEGER NOT NULL, 
  [name] CHAR(50), 
  [ipaddress] CHAR(15), 
  [ipaddressmask] CHAR(15), 
  [port_id] INTEGER, 
  [lag_id] INTEGER, 
  CONSTRAINT [sqlite_autoindex_interfaces_1] PRIMARY KEY ([router_id], [interface_id]));
CREATE INDEX [router_lag] ON [interfaces] ([router_id], [lag_id]);
CREATE UNIQUE INDEX [router_port] ON [interfaces] ([router_id], [port_id]);

/* Data [interfaces] */
insert into [interfaces] values(1, 1, 'system', '114.0.36.1', '255.255.255.255', null, null);
insert into [interfaces] values(1, 2, 'to-JKT-KPI-CN2-SR12', '124.195.90.154', '255.255.255.252', null, 50);
insert into [interfaces] values(4, 1, 'system', '124.195.96.246', '255.255.255.255', null, null);
insert into [interfaces] values(4, 2, 'to-JKT-KPI-CN3-SR12', '124.195.90.153', '255.255.255.252', null, 50);


/* Drop table [lags] */
drop table if exists [lags];

/* Table structure [lags] */
CREATE TABLE [lags] (
  [router_id] INTEGER NOT NULL, 
  [lag_id] INTEGER NOT NULL, 
  [flags] CHAR(4), 
  CONSTRAINT [sqlite_autoindex_lags_1] PRIMARY KEY ([router_id], [lag_id]));

/* Data [lags] */
insert into [lags] values(1, 50, 'AO');
insert into [lags] values(4, 50, 'AO');


/* Drop table [lagports] */
drop table if exists [lagports];

/* Table structure [lagports] */
CREATE TABLE [lagports] (
  [router_id] INTEGER NOT NULL, 
  [lag_id] INTEGER NOT NULL, 
  [port_id] INTEGER NOT NULL, 
  [flags] CHAR(4), 
  CONSTRAINT [router_port] FOREIGN KEY([router_id], [port_id]) REFERENCES [ports]([router_id], [port_id]), 
  CONSTRAINT [router_lag] FOREIGN KEY([router_id], [lag_id]) REFERENCES [lags]([router_id], [lag_id]), 
  CONSTRAINT [sqlite_autoindex_lagports_1] PRIMARY KEY ([router_id], [lag_id], [port_id]));

/* Data [lagports] */
insert into [lagports] values(1, 50, 102015, 'AOP');
insert into [lagports] values(1, 50, 102016, 'AO');
insert into [lagports] values(4, 50, 202006, 'AOP');
insert into [lagports] values(4, 50, 902004, 'AO');


/* Commit Transaction */
commit transaction;

/* Enable Foreign Keys */
pragma foreign_keys = on;
