#include <cstdio>
#include "FingerDb.h"
#include "CppSQLite3.h"

#define  DB_DBG(str) 

static CppSQLite3DB m_DB;
static CppSQLite3Buffer m_lastSql;

FingerDB::FingerDB()
{
	
}
bool FingerDB::CreateTable(const std::string& table)
{
    
	try{
		m_lastSql.format("CREATE TABLE  if not exists %s (finger_id TEXT NOT NULL,person_id TEXT NOT NULL, version TEXT NOT NULL);",table.c_str());
		int rows = m_DB.execDML(m_lastSql);
		return true;
	}
	catch (CppSQLite3Exception& e) {
		printf("db err[%s]\n",e.errorCodeAsString(e.errorCode()));
		return false;
	}
	return true;
}
bool FingerDB::Open(std::string path)
{
    try{
        m_DB.open(path.c_str());
		
    }catch(CppSQLite3Exception& e){
        //DB_DBG("open %s failed\n",path.c_str());
        return false;
    }
    return true;
}

bool FingerDB::GetWokerByIndex(int index,CWorker& worker)
{
    TWorkerList list;
    if(ListAllWorkers(list) == false) return false;

    if(index >= list.size ()) return false;

    worker = list.at (index);
    return true;
}
bool FingerDB::GetWokerById(int id,CWorker& worker)
{
    try
    {
        m_lastSql.format("select * from finger_table");
        CppSQLite3Query qry = m_DB.execQuery(m_lastSql);

        if (!qry.eof())
        {
            worker.m_finger_index   = qry.getIntField("finger_id");
            worker.m_finger_version = qry.getIntField("version");
            worker.m_id				= qry.getIntField("person_id");
        }
        qry.finalize();
        return true;
    }
    catch (CppSQLite3Exception& e) {
        return false;
    }
    return true;
}
bool FingerDB::ListAllWorkers(TWorkerList& list)
{
    try
    {
        m_lastSql.format("select * from finger_table");
        CppSQLite3Query qry = m_DB.execQuery(m_lastSql);

        while (!qry.eof())
        {
            CWorker worker;
            worker.m_finger_index   = qry.getIntField("finger_id");
            worker.m_finger_version = qry.getIntField("version");
            worker.m_id				= qry.getIntField("person_id");
            list.push_back(worker);
            qry.nextRow();

        }
        qry.finalize();
        return true;
    }
    catch (CppSQLite3Exception& e) {
        return false;
    }
    return true;
}
bool     FingerDB::LoadPerson()
{
	return true;
}
bool FingerDB::LoadDb()
{
	if(Open("./finger.db") == false) 
	{
		printf("open finger.db failed\n");
		return false;
	}

	if(CreateTable("finger_table") == false) 
	{		
		printf("create table failed\n");
		return false;
	}
	
	
	return true;
}

bool FingerDB::AddWorker(int page, int id, int version)
{
    bool ret = false;
	
    m_lastSql.format("Insert or rollback into finger_table (finger_id,person_id,version) values(%d,%d,%d);", \
		page,id,version);

    try{
        int rows = m_DB.execDML(m_lastSql);
        if (rows) {
			ret = true;
        }
    }
    catch (CppSQLite3Exception& e) {
		ret = false;
    }
    
    return ret;
	
}
bool FingerDB::UpdateWorker(int page, int id, int version)
{
	bool ret = false;
	
	m_lastSql.format("update finger_table set person_id=%d,version=%d where finger_id=%d;", id,version,page);

    try{
        int rows = m_DB.execDML(m_lastSql);
        if (rows) {
			ret = true;
        }
    }
    catch (CppSQLite3Exception& e) {
		ret = false;
		
    }
    
    return ret;
}
bool FingerDB::ExistFinger(int page)
{
	bool find = false;
	try{
		
		m_lastSql.format("select * from finger_table where finger_id=%d",page);
		
		CppSQLite3Query qry = m_DB.execQuery(m_lastSql);
		if (!qry.eof()) {
			find = true;
		}
		qry.finalize();
	}catch (CppSQLite3Exception& e) {
		
	}
	return find;
}

bool	FingerDB::ExistWorkerId(int worker_id)
{
	return true;
}
bool FingerDB::SaveWorker(int page, int id,int version)
{
	if(ExistFinger(page))
	{
        return UpdateWorker(page,id,version);
	}else{
        return AddWorker(page,id,version);
	}

}
int  FingerDB::GetNum(void)
{
	int num = 0;
	try{
		CppSQLite3Table tb = m_DB.getTable ("select * from finger_table");
		num = tb.numRows ();
	}
	catch(...)
	{
		
	}
	return num;
}

int  FingerDB::LookForIdByPage(int page)
{
	int id = -1;
	try
	{
		m_lastSql.format("select * from finger_table where finger_id=%d;",page);
		CppSQLite3Query qry = m_DB.execQuery(m_lastSql);
		
		if (!qry.eof()) 
		{
			
			id  = qry.getIntField("person_id");
			
		}
		qry.finalize();
	}
	catch (CppSQLite3Exception& e) {
		
	}
	return id;
}
int  FingerDB::LookForPageById(int id)
{
	int page = -1;
	try
	{
        m_lastSql.format("select * from finger_table where person_id=%d;",id);
		CppSQLite3Query qry = m_DB.execQuery(m_lastSql);

		if (!qry.eof()) 
		{

			page  = qry.getIntField("finger_id");
			
		}
		qry.finalize();
	}
	catch (CppSQLite3Exception& e) {
		
	}
	return page;
}

bool FingerDB::UnloadDB()
{
	m_DB.close();
	return true;
}
bool FingerDB::ClearDB(void)
{
	bool ret = false;
	
    m_lastSql.format("drop table finger_table");
	
    try{
        int rows = m_DB.execDML(m_lastSql);
        if (rows) {
			//ret = true;
        }
		ret = true;
    }
    catch (CppSQLite3Exception& e) {
		ret = false;
    }
    
    return ret;
}