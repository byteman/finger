#ifndef FINGER_DB_H
#define FINGER_DB_H

#include <iostream>
#include <vector>


typedef struct _tag_WorkerInfo 
{
	
	int m_id;		/*!< ��Ա��ID��� */
	int m_finger_index; /*!< ָ��ģ�����ڵ���� */
	unsigned short m_finger_version; /*!< ָ��ģ��İ汾 */
	unsigned short m_crc; /*!< ��ǰ�����ݵ�У��� */
}WorkerInfo;

class CWorker
{
public:
	CWorker(){}
	CWorker(int id);
	CWorker(WorkerInfo* info)
	{
		if(info)
		{
			m_id = info->m_id;
			m_finger_index = info->m_finger_index;
			m_finger_version= info->m_finger_version;
		}
		else
		{
			m_id = 0;
			m_finger_index=0;
			m_finger_version=0;
		}
		
	}
public:
	int m_id;		/*!< ��Ա��ID��� */
	int m_finger_index; /*!< ָ��ģ�����ڵ���� */
	unsigned short m_finger_version; /*!< ָ��ģ��İ汾 */
	
};

typedef std::vector<CWorker> TWorkerList;

class FingerDB
{
public:
	FingerDB();
	bool LoadDb();
	bool UnloadDB();
	int  LookForPageById(int id);
	int  LookForIdByPage(int page);
    int  LookForVersionById(int id);
	bool SaveWorker(int page, int id,int version);
	int  GetNum(void);	
    bool UpdateValue(int page, int id, int version);
    bool GetWokerByIndex(int index,CWorker& worker);
    bool GetWokerById(int id,CWorker& worker);
    bool ListAllWorkers(TWorkerList& list);
	bool ClearDB(void);
private:
	bool	 Open(std::string path);
	bool     LoadPerson(void);
	bool	 CreateTable(const std::string& table);
    bool	 AddWorker(int page, int id ,int version);
    bool     UpdateWorker(int page, int id, int version);
	bool	 ExistFinger(int fg_page);
	bool	 ExistWorkerId(int worker_id);

};
#endif
