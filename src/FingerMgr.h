#ifndef __FINGER_MGR_H
#define __FINGER_MGR_H  

#include <iostream>
#include <vector>
#include "Finger.h"
#include "FingerDb.h"


class CFingerMgr
{
public:

	CFingerMgr(){m_init_ok = false;}
	/*!
	\brief		管理器初始化
	\return		返回初始化的结果
	\retval		0  成功
	\retval		-1 失败

	*/
	int init(const char* inf_dev, int baud);
	/*!
	\brief		用户登录
	\return		返回登录的结果
	\retval >=0	登录成功,该人员的ID
	\retval -1	没有检测到手指
	\retval -2	没有该手指的指纹信息
	\retval -3	设备故障
	*/
	int login();

	/*!
		检查指纹模块上是否有手指
	*/
	bool has_finger(void);


	int clear_fingers(void);
	/*! 
	\brief				更新指定ID人员的指纹模板
	\param id			人员ID
	\param finger_data	指纹模板数据
	\param len			指纹模板长度
	\param version		指纹模板版本
	\return				返回操作结果
	\retval			0 	成功
	\retval			-1  失败
	*/
	int download_info(int id, int page,unsigned char* finger_data, int len, int version);
///上传指纹模块中指定id编号的人员的指纹数据。
	int upload_info(int id, unsigned char* finger_data, int len, int &page,int &version);


	/*! 
	\brief				返回指定ID人员的指纹模板的版本
	\param id			人员ID
	\param len			指纹模板长度
	\param version		指纹模板版本
	\return			    返回指纹版本
	\retval				>0 获取到正确的版本号
	\retval				0  找不到该ID
	\retval				<0 未知错误
	*/
	unsigned short get_finger_version(int id);


		/*!
	\brief		获取人员的个数
	\return		返回人员的个数
	*/

	int get_worker_num(void);

	/*!
	\brief		根据序号获取操作人员信息
	\return		操作人员信息
	\retval		NULL  失败
	\retval     !NULL 成功
	*/
    bool get_worker_by_index(int index,CWorker& worker);


		/*!
	\brief		根据编号获取操作人员信息
	\return		操作人员信息
	\retval		NULL  失败
	\retval     !NULL 成功
	*/
    bool get_worker_by_id(int id,CWorker& worker);

	/*!
	\brief		录入指纹并分配ID
	\return		录入结果
	\retval		0  成功
  */
    int record_finger(int page,int id,int version);

	int  get_packet_size(void);
	int  set_packet_size(void); 


	int  gen_feature_up(unsigned char* buff, int buff_len);


	static void dump_param(TDevParam &param);
	const char* get_last_error(void);
	int get_finger_num(void);

	CFinger		m_finger_dev;

private:
	int		 lookfor_page_by_id(int id);
	void	 reset_default_value(void);
	bool	 load_worker_info(void);
	TDevParam	m_param;
	FingerDB	m_db;
    bool        m_init_ok;
    ERR_FINGER  m_ret;

};
#endif

