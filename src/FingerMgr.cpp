#include <cstdio>
#include "FingerMgr.h"

#define  DEF_ADDR 0xFFFFFFFF
#define  DEF_PWD  0x00000000
#define  BUF_ID		0
#define  PAGE_BEGIN 0
#define  PAGE_END	20


bool CFingerMgr::has_finger(void)
{
	m_ret = m_finger_dev.gen_image();

    return (ERR_FINGER_OK == m_ret);

}
int CFingerMgr::login()
{
    int page = -1;
	
	if(!has_finger()) return -1;
	m_ret = m_finger_dev.search_finger(PAGE_BEGIN,PAGE_END,page);
	
    if(m_ret != ERR_FINGER_OK)
	{
		return -2;
	}
	if(page == -1) return -3;

	int id = m_db.LookForIdByPage(page);

	if(id == -1) return -4;

	return id;
}
int CFingerMgr::clear_fingers(void)
{
	
	m_ret = m_finger_dev.empty();
    if (ERR_FINGER_OK != m_ret)
	{
		return -1;
	}
	if(!m_db.ClearDB()) return -2;

	m_db.UnloadDB();

	if(!m_db.LoadDb()) return -3;

	return 0;
}
int CFingerMgr::upload_info(int id, unsigned char* finger_data, int len, int &page,int &version)
{
	
	if(m_init_ok)
	{
		
        int buff_id = 1;
        int npage = m_db.LookForPageById(id);
        if(npage == -1)
		{
			printf("can not find id[%d]\n",id);
			return -1;
		}
        page = npage;
        m_ret = m_finger_dev.upload_to_feature_buffer(buff_id,npage);
        if(ERR_FINGER_OK != m_ret)
		{
            printf("err:%s\n",m_finger_dev.format_error (m_ret));
			return -2;
		}
        m_ret =m_finger_dev.upload_to_buffer(buff_id,finger_data,len,m_param.packet_size);
        if(ERR_FINGER_OK != m_ret)
		{
			printf("err:%s\n",m_finger_dev.format_error (m_ret));
			return -3;
		}
		return 0;
	}
	return -4;
}
int CFingerMgr::download_info(int id, int page,unsigned char* finger_data, int len, int version)
{
	if(m_init_ok){

        int buff_id = 1;

        m_ret = m_finger_dev.download_to_buffer(buff_id,finger_data,len,m_param.packet_size);

        if(m_ret != ERR_FINGER_OK) return -1;


		if( ( page < 0 )  || ( page >= m_param.num_fingers ) )
		{
			printf("page[%d] error\n",page);
			return -2;
        }

		m_ret = m_finger_dev.store_finger(buff_id,page);

        if(m_ret != ERR_FINGER_OK)
			return -3;

		if(m_db.SaveWorker(page,id,version)== false) 
			return -4;

		return 0;

	}
	return -5;
}
unsigned short CFingerMgr::get_finger_version(int id)
{
	return 0;
}

bool CFingerMgr::get_worker_by_id(int id,CWorker& worker)
{
    return m_db.GetWokerById(id,worker);
}
bool CFingerMgr::get_worker_by_index(int index,CWorker& worker)
{
    return m_db.GetWokerByIndex(index,worker);
}
int CFingerMgr::get_worker_num(void)
{
	return m_db.GetNum();
}

int CFingerMgr::get_finger_num(void)
{
	unsigned short num = 0;

	m_ret = m_finger_dev.read_finger_num(num);
    if(m_ret == ERR_FINGER_OK)
	{
		return num;
	}
	return 0;
}

void CFingerMgr::dump_param(TDevParam &param)
{
		
	printf("*************device param**************\n");
	printf("reg=%d\n",param.status.status_value);
	printf("vid=%d\n",param.vid);
	printf("finger_num=%d\n",param.num_fingers);
	printf("security level=%d\n",param.security_level);
	printf("dev_addr=0x%X\n",param.dev_addr);
	printf("packet size=%d\n",param.packet_size);
	printf("baud=%d\n",param.baud_rate);
	printf("***********************************\r\n");
	
	
}
const char* CFingerMgr::get_last_error(void)
{
	return CFinger::format_error(m_ret);
}
int CFingerMgr::init(const char* inf_dev, int baud)
{

	m_ret = m_finger_dev.OpenInterface(inf_dev,baud);
    if(m_ret != ERR_FINGER_OK)
	{
		return m_ret;
	}
	m_ret = m_finger_dev.authen(DEF_ADDR,DEF_PWD);
    if(ERR_FINGER_OK != m_ret)
	{
		return m_ret;
	}
	if(load_worker_info() == false ) 
	{
		m_ret = ERR_INVALID_VAL;
		return -4;
	}
	m_ret = m_finger_dev.get_dev_pararm(m_param);

    if(ERR_FINGER_OK != m_ret)
	{
		printf("load device param failed\n");
		return -3;
	}
	m_init_ok = true;
	return 0;
}


bool	 CFingerMgr::load_worker_info(void)
{
	if(m_db.LoadDb() == false) return false;

	return true;
}

int		 CFingerMgr::lookfor_page_by_id(int id)
{
	int page = m_db.LookForPageById(id);

	if(page == -1)
	{
		page = m_db.GetNum();
	}

	return page;
}


//指纹的页数和note的页码数是一致的
int CFingerMgr::record_finger(int page,int id,int version)
{
	//根据ID查找指纹所在的页码
	//int finger_page = lookfor_page_by_id(id);
	//单次录入指纹，并将指纹保存到指定的页码中
    m_ret = m_finger_dev.auto_record_finger(page);
    if(ERR_FINGER_OK != m_ret)
	{
		return -2;
	}
    if(m_db.SaveWorker(page,id,version)== false) 
	{
		m_ret = ERR_INVALID_VAL;
		return -1;
	}

	return 0;
}

int  CFingerMgr::gen_feature_up(unsigned char* buff, int buff_len)
{

	m_ret = m_finger_dev.gen_feature_up(buff,buff_len,m_param.packet_size);

    if(ERR_FINGER_OK != m_ret)
	{
		return -1;
	}
	return 0;

}
