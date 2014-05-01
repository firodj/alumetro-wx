<?php
$typedef = array(
    'uint64' => array('type' => 'wxULongLong',
		'get' 	=> 'WXSQLGET_LL(?)',
		'set' 	=> '?	= wxULongLong( _q.GetInt64(_c++).GetValue() )',
		'init' 	=> '?	= wxULongLong( 0 )',
		'fmt' 	=> "'%s'"),
	'int64' => array('type' => 'wxLongLong',
		'get' 	=> 'WXSQLGET_LL(?)',
		'set' 	=> '?	= wxLongLong( _q.GetInt64(_c++).GetValue() )',
		'init' 	=> '?	= wxLongLong( 0 )',
		'fmt' 	=> "'%s'"),
	'uint' => array('type' => 'unsigned long',
		'get' 	=> '?',
		'set' 	=> '?	= (unsigned long)_q.GetInt(_c++)',
		'init' 	=> '?	= 0',
		'fmt' 	=> "%u"),
	'int' => array('type' => 'long',
		'get' 	=> '?',
		'set' 	=> '?	= _q.GetInt64(_c++).ToLong()',
		'init' 	=> '?	= 0',
		'fmt' 	=> "%d"),
	'string' => array('type' => 'wxString',
		'get' 	=> 'WXSQLGET_S(?)',
		'set' 	=> '?	= _q.GetAsString(_c++)',
		'init' 	=> '?	= wxEmptyString',
		'fmt' 	=> "'%q'"),
	'datetime' => array('type' => 'wxDateTime',
		'get' 	=> 'WXSQLGET_DT(?)',
		'set' 	=> '?.ParseISOCombined( _q.GetAsString(_c++) )',
		'init' 	=> '?.SetToCurrent()',
		'fmt' 	=> "'%s'"),
	'bool' => array('type' => 'bool',
		'get' 	=> '?',
		'set' 	=> '?	= _q.GetBool(_c++)',
		'init' 	=> '?	= false',
		'fmt' 	=> "%d"),
	'decimal' => array('type' => 'double',
		'get' 	=> '?',
		'set' 	=> '?	= _q.GetDouble(_c++)',
		'init' 	=> '?	= 0.0',
		'fmt' 	=> "%f"),
);

function camel_case($str) {
	return str_replace(' ','', ucwords(str_replace('_',' ',$str)));
}

if (count($_SERVER['argv']) <= 1) {
	echo 'Usage:  php '.$_SERVER['argv'][0].' <schema_file.sql>',PHP_EOL;
	echo '        schema_file.sql',PHP_EOL;
	echo '            Exported SQLite databases\'s structure with CREATE TABLE syntax.',PHP_EOL;
	echo '            The name of tables and fields shouldbe surrounded by \'[\' and \']\'', PHP_EOL;
	die( PHP_EOL );
}

$schema_file = $_SERVER['argv'][1];
if (!file_exists($schema_file)) {
	echo 'Error:  schema_file.sql "'.$schema_file.'" is nout found.',PHP_EOL;
	die( PHP_EOL );
}

$handle = fopen($schema_file, "r");
$state = 0;
$tables = array();
if ($handle) {
    while (($line = fgets($handle, 4096)) !== false) {
        //echo $buffer;
		if ($state == 0) {
			if (preg_match('/^\s*(CREATE TABLE.*?) \[(.+?)\] \(/im', $line, $matches)) {
				$state = 1;
				$table_name = $matches[2];
				$fields = array();
				$primaries = array();
				$autoinc = null;
				$creates = array();
				
				if (strpos($matches[1], 'EXISTS') === false) {
					//echo $matches[1];
					$matches[0] = $matches[1].' IF NOT EXISTS ['.$matches[2].'] (';
				}
				$creates[] = $matches[0];
			} else if (preg_match('/^\s*(CREATE .*?INDEX) (.*ON \[(.+?)\]) (.*);/im', $line, $matches)) {
				$table_name = $matches[3];
				
				if (strpos($matches[2], 'EXISTS') === false) {
					//echo $matches[1];
					$matches[0] = $matches[1].' IF NOT EXISTS '.$matches[2].' '.$matches[4].';';
				}
				$tables[$table_name]['creates'][] = $matches[0];
			}
		}
		else if ($state == 1) {
			$creates[] = chop($line);
			
			if (preg_match('/^(.+)(,|\);)\s*$/im', $line, $matches)) {
				$line = $matches[1];
				if ($matches[2] == ');') $state = 0;
			}
			
			//if (preg_match('/\);\s*$/im', $line)) $state = 0;
			
			if (preg_match('/^\s*\[(.+?)\] (.+)?/im', $line, $matches))
			{
				$field_name = $matches[1];
				$wd = explode(' ',$matches[2]);
				$len = 0;
				$i = 0;
				// detect CHAR(x) or TINYINT(y) or DECIMAL(z)
				if (preg_match('/([A-Z]+)\((.+)\)/im', $wd[$i], $matches)) {
					$wd[$i] = $matches[1];
					$len = $matches[2];
				}
				
				if ($wd[$i] == 'TEXT') {
					$type = 'string'; $i++;
				} else if ($wd[$i] == 'CHAR') {
					$type = 'string'; $i++;
				} else if ($wd[$i] == 'DATETIME' || $wd[$i] == 'DATE') {
					$type = 'datetime'; $i++;
				} else if ($wd[$i] == 'TINYINT') {
					if ($len == '1') $type = 'bool';
					else $type ='int';
					$i++;
				} else {
					$type = 'int';
					if ($wd[$i] == 'UNSIGNED') {
						$type = 'u'.$type; $i++;
					}
					if ($wd[$i] == 'BIG') {
						$type = $type.'64'; $i++;
					}
					if ($wd[$i] == 'INTEGER') {
						$i++;
					}
				}
				for(; $i<count($wd); ++$i) {
					if ($wd[$i] == 'PRIMARY') {
						$primaries[] = $field_name;
					}
					if ($wd[$i] == 'AUTOINCREMENT') {
						$autoinc = $field_name;
					}
				}
				$fields[$field_name] = $type;
				//echo '-- COL:'.$field_name,' TYPE:'.$type, PHP_EOL;
			} else if (preg_match('/CONSTRAINT .*(PRIMARY KEY) \((.+?)\)/im', $line, $matches))
			{
				$wd = explode(', ', $matches[2]);
				foreach($wd as $x)
					$primaries[] = trim($x, '[]');
				//echo '-- PRIMARY: '.$matches[2], PHP_EOL;
			}
			
			if ($state == 0) {
				$classname = 'C'.camel_case($table_name).'Base';
				$tables[$table_name] = array('fields'=>$fields, 
					'primaries'=> $primaries, 
					'creates'  => $creates,
					'classname'=> $classname,
					'autoinc'  => $autoinc);
				//echo 'STORE:'.$table_name,PHP_EOL;
			}
		}
		
    }
    if (!feof($handle)) {
        echo "Error: unexpected fgets() fail\n";
    }
    fclose($handle);
} 

ob_start();
?>/* File: DbSchema.hxx - Auto Generated */
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
<?php
$i = 0;
foreach ($tables as $table_name=>$table) {
	$i++; ?>
		<?php echo 'T'.camel_case($table_name).'	= '.$i ?>,
<?php } ?>
    } m_tablename;
    
protected:
    wxSQLite3Database* m_db;
};

///////////////////////////////////////////////////////////////////////////////
<?php
$header_contents = ob_get_contents();
ob_end_flush();
ob_start();
?>/* File: DbSchema.cxx - Auto Generated */

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

<?php foreach ($tables as $table_name=>$table) : 
$f_inits = array();
$f_sets = array();
$f_names = array();
$f_names2 = array();
$f_gets = array();
$f_gets2 = array();
$f_fmts = array();
$f_fmts2 = array();
$f_updates = array();
$f_wheres = array();
$f_members = array();
$f_params = array();

foreach ($table['fields'] as $field_name=>$field_type) {
	$f_inits[] = str_replace('?', $field_name, $typedef[$field_type]['init']).';';
	$f_sets[] = str_replace('?', $field_name, $typedef[$field_type]['set']).';';
	$fmt = $typedef[$field_type]['fmt'];
	$get = str_replace('?', $field_name, $typedef[$field_type]['get']);
	
	if ($field_name != $table['autoinc']) {
		$f_names2[] = '['.$field_name.']';
		$f_fmts2[] = $fmt;
		$f_gets2[] = $get;
	}
	$f_names[] = '['.$field_name.']';
	$f_fmts[] = $fmt;
	$f_gets[] = $get;
	
	$f_updates[$get] = '['.$field_name.'] = '.$fmt;
	
	$getpar = str_replace('?', $field_name.'_', $typedef[$field_type]['get']);
	if (in_array($field_name, $table['primaries'])) {
		$f_wheres[$get] = '['.$field_name.'] = '.$fmt;
		$f_params[$getpar] = $typedef[$field_type]['type'].'& '.$field_name.'_';
	}
		
	$f_members[] = $typedef[$field_type]['type'].'	'.$field_name;
	
}

$implode_members = implode(';'.PHP_EOL ."\t", $f_members);
$implode_params  = implode(", ", $f_params);

$header_contents.=<<<EOS
// Class $table[classname]
class $table[classname]: public CDbTableBase {
public:
	// members
	$implode_members;

	// virtuals
	virtual bool BeforeInsert()	{ return true; };
	virtual bool BeforeUpdate()	{ return true; };
	virtual bool AfterDelete()	{ return true; };
	
	// statics
	// create table
	static bool InitTable(wxSQLite3Database& _db);
	// select
	static bool Select($implode_params,
		$table[classname]& _ret, wxSQLite3Database& _db);
	
	// c-tors
	$table[classname](wxSQLite3Database *_db=NULL);

	int FetchResult(wxSQLite3ResultSet& _q, int _c=0);
	bool Insert(bool _autoinc=false);
	bool Update();
	bool Delete();
};


EOS;
?>
/******************************************************************************
 * Class <?php echo $table['classname']. PHP_EOL; ?>
 ******************************************************************************/

// (static) create table if not exists
bool <?php echo $table['classname']; ?>::InitTable(wxSQLite3Database& _db)
{
	wxString _sql = 
"<?php echo implode('\\'.PHP_EOL, $table['creates']); ?>";
	_db.ExecuteUpdate( _sql );
	return true;
}

// c-tors
<?php echo $table['classname']; ?>::<?php echo $table['classname']; ?>(wxSQLite3Database *_db):
		CDbTableBase(_db) 
{
	m_tablename = <?php echo 'T'.camel_case($table_name); ?>;
	<?php echo implode(PHP_EOL."\t", $f_inits); ?>

}

// fetch results
int <?php echo $table['classname']; ?>::FetchResult(wxSQLite3ResultSet& _q, int _c)
{
	<?php echo implode(PHP_EOL."\t", $f_sets); ?>
	return _c;
}
	
// inserts
bool <?php echo $table['classname']; ?>::Insert(bool _autoinc) 
{
	if (!BeforeInsert()) return false;
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
<?php if ($table['autoinc']): ?>
<?php $sql = "INSERT INTO [{$table_name}] (".implode(', ',$f_names).") \\\r\n\t\tVALUES (".implode(', ',$f_fmts).")"; ?>
	if (!_autoinc) {
		_sql.Format("<?php echo $sql; ?>",
			<?php echo implode(",".PHP_EOL."\t\t\t", $f_gets); ?>

		);
	} else {
<?php endif; ?>
<?php $sql2 = "INSERT INTO [{$table_name}] (".implode(', ',$f_names2).") \\\r\n\t\tVALUES (".implode(', ',$f_fmts2).")"; ?>
		_sql.Format("<?php echo $sql2; ?>",
			<?php echo implode(",".PHP_EOL."\t\t\t", $f_gets2); ?>

		);
<?php if ($table['autoinc']): ?>
	}
<?php endif; ?>

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - <?php echo $table['classname']; ?>::Insert()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}
	
	if (_affected > 0) {
<?php if ($table['autoinc']): ?>
		long _rowid = m_db->GetLastRowId().ToLong();
		if (_rowid != 0) <?php echo $table['autoinc']; ?> = _rowid;
<?php endif; ?>
		return true;
	}

	return false;
}

// updates
bool <?php echo $table['classname']; ?>::Update()
{
	if (!BeforeUpdate()) return false;
	
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
<?php $sql = "UPDATE {$table_name} SET ".implode(', ', array_values($f_updates))." \\\r\n\t\tWHERE ".implode(' AND ', array_values($f_wheres)); ?>
	_sql.Format("<?php echo $sql; ?>",
		<?php echo implode(','. PHP_EOL ."\t\t", array_merge(array_keys($f_updates), array_keys($f_wheres)) ); ?>

	);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - <?php echo $table['classname']; ?>::Update()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}

	if (_affected > 0) {
		return true;
	}

	return false;
}
	
// deletes
bool <?php echo $table['classname']; ?>::Delete()
{
	int _affected = 0;
	wxSQLite3StatementBuffer _sql;
<?php $sql = "DELETE FROM [{$table_name}] WHERE ".implode(' AND ', array_values($f_wheres)); ?>
	_sql.Format("<?php echo $sql; ?>",
		<?php echo implode(','. PHP_EOL ."\t\t\t", array_keys($f_wheres)); ?>

	);

	try {
		_affected = m_db->ExecuteUpdate( _sql );
	} catch (wxSQLite3Exception e) {
		wxMessageBox(e.GetMessage(), "SQLite3 Exception - <?php echo $table['classname']; ?>::Delete()", 
			wxCENTER|wxOK|wxICON_EXCLAMATION);
		return false;
	}

	if (_affected > 0)
		return AfterDelete();

	return false;
}
	
// (static) Selects one record
bool <?php echo $table['classname']; ?>::Select(<?php echo implode(", ", $f_params); ?>,
	<?php echo $table['classname']; ?>& _ret, wxSQLite3Database& _db)
{
	bool _retok = false;
	wxSQLite3StatementBuffer _sql;
<?php $sql = "SELECT * FROM [{$table_name}] WHERE ".implode(' AND ', array_values($f_wheres)); ?>
	_sql.Format("<?php echo $sql; ?>",
		<?php echo implode(','. PHP_EOL ."\t\t\t", array_keys($f_params)); ?>

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

<?php endforeach; ?>
<?php

file_put_contents("DbSchema.hxx", $header_contents);
file_put_contents("DbSchema.cxx", ob_get_contents());

ob_end_flush();
?>