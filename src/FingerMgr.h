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
	\brief		��������ʼ��
	\return		���س�ʼ���Ľ��
	\retval		0  �ɹ�
	\retval		-1 ʧ��

	*/
	int init(const char* inf_dev, int baud);
	/*!
	\brief		�û���¼
	\return		���ص�¼�Ľ��
	\retval >=0	��¼�ɹ�,����Ա��ID
	\retval -1	û�м�⵽��ָ
	\retval -2	û�и���ָ��ָ����Ϣ
	\retval -3	�豸����
	*/
	int login();

	/*!
		���ָ��ģ�����Ƿ�����ָ
	*/
	bool has_finger(void);


	int clear_fingers(void);
	/*! 
	\brief				����ָ��ID��Ա��ָ��ģ��
	\param id			��ԱID
	\param finger_data	ָ��ģ������
	\param len			ָ��ģ�峤��
	\param version		ָ��ģ��汾
	\return				���ز������
	\retval			0 	�ɹ�
	\retval			-1  ʧ��
	*/
	int download_info(int id, int page,unsigned char* finger_data, int len, int version);
///�ϴ�ָ��ģ����ָ��id��ŵ���Ա��ָ�����ݡ�
	int upload_info(int id, unsigned char* finger_data, int len, int &page,int &version);


	/*! 
	\brief				����ָ��ID��Ա��ָ��ģ��İ汾
	\param id			��ԱID
	\param len			ָ��ģ�峤��
	\param version		ָ��ģ��汾
	\return			    ����ָ�ư汾
	\retval				>0 ��ȡ����ȷ�İ汾��
	\retval				0  �Ҳ�����ID
	\retval				<0 δ֪����
	*/
	unsigned short get_finger_version(int id);


		/*!
	\brief		��ȡ��Ա�ĸ���
	\return		������Ա�ĸ���
	*/

	int get_worker_num(void);

	/*!
	\brief		������Ż�ȡ������Ա��Ϣ
	\return		������Ա��Ϣ
	\retval		NULL  ʧ��
	\retval     !NULL �ɹ�
	*/
    bool get_worker_by_index(int index,CWorker& worker);


		/*!
	\brief		���ݱ�Ż�ȡ������Ա��Ϣ
	\return		������Ա��Ϣ
	\retval		NULL  ʧ��
	\retval     !NULL �ɹ�
	*/
    bool get_worker_by_id(int id,CWorker& worker);

	/*!
	\brief		¼��ָ�Ʋ�����ID
	\return		¼����
	\retval		0  �ɹ�
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

