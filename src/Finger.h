#ifndef  __FINGER_H
#define  __FINGER_H

#include <iostream>
/*!
\brief ָ��ģ�������
*/
typedef enum enum_FingerErr
{
    ERR_FINGER_OK = 0,				/*!< �ɹ�*/
	ERR_PACKET,			/*!< ������*/
	ERR_NO_FINGER,		/*!< ������������ָ*/
	ERR_GET_IMG,		/*!< ͼ��¼�벻�ɹ�*/

	//////////////////////////////////////////
	ERR_IMG_NOT_CLEAN=6,  /*!<�ɼ���ͼ����� */
	ERR_IMG_NOT_ENOUGH=7,  /*!<�ɼ���ͼ��������㲻�� */
	ERR_NOT_MATCH=8,		/*!<ָ�Ʋ�ƥ�� */
	ERR_SEARCH=9,			/*!<û��������ָ�� */

	//////////////////////////////////////////////
	ERR_GEN_CHAR_FAIL=0xa,  /*!<��������ʧ��,��char1��char2�е������ļ��ϳ�ģ��ʧ�� */
	ERR_PAGE_RANGE=0xb,			/*!<��ʾPageID ����ָ�ƿⷶΧ */
	ERR_RD_TEMPLATE=0xc,	/*!<������ģ�������д������Ч */
	ERR_UP_FEATURE=0xd, /*!<�ϴ�����ʧ�� */
	ERR_CONTINUE_RECV=0xe,  /*!<���ܼ������պ����İ� */
	ERR_UP_IMG=0xf,			/*!<�ϴ�ͼ��ʧ�� */
	ERR_DEL_TEMPLATE=0x10,	/*!<ɾ��ģ��ʧ�� */
	ERR_CLEAR_FINGER_DB, /*!<���ָ�ƿ�ʧ�� */

	//////////////////////////////////////////
	ERR_PASSWORD=0x13,		/*!<�����ȷ */
	///////////////////////////////////////////
	ERR_NO_IMG=0x15,			/*!<ͼ�񻺳�����û��ͼ�� */
	///////////////////////////////////////////
	ERR_RW_FLASH=0x18,		/*!<��дFLASH ���� */
	///////////////////////////////////////////////////
	ERR_REG_INDEX=0x1a,		/*!<�Ĵ�����Ŵ��� */
	//////////////////////////////////////////////////
	ERR_ADDRESS=0x20, /*!<��ַ����� */
	ERR_AUTH,		/*!<������֤���� */
	////////////////////////////////////////////////////
	ERR_TIMEOUT,		/*!<���յ������ݳ�ʱ��ȡ���Ȳ��� */
	ERR_RS232,			/*!<�ӿ�ͨ�Ź��� */
	ERR_GEN_TEMP_FAIL,  /*!<����ģ��ʧ�� */
    ERR_INVALID_VAL	/*!<�������� */

}ERR_FINGER;


typedef struct tag_DevStatus{
	unsigned int busy:1;
	unsigned int FingerPass:1;
	unsigned int PwdPass:1;
	unsigned int ImgBufStat:1;
}TDevStatus;

typedef union{
	TDevStatus status_bits;
	unsigned short status_value;
}TStatus;
typedef struct tag_DevParam{
	TStatus status;
	unsigned short  vid; //ϵͳʶ����
	unsigned short  num_fingers; //ָ������
	unsigned int    baud_rate;
	unsigned short  security_level;
	unsigned short  packet_size;
	unsigned int    dev_addr;
}TDevParam;


/*!
\class CFinger
\brief ָ��ʶ��ģ����
*/
class CFinger
{
public:
	CFinger();
	ERR_FINGER OpenInterface(const char* path,int baud);

	ERR_FINGER get_dev_pararm(TDevParam& param);
	ERR_FINGER set_dev_pararm(TDevParam& param);

	ERR_FINGER set_password(unsigned int password);
	ERR_FINGER set_address(unsigned int address);
/*!
	\brief ��ȡģ��ָ��ģ����������ÿ������ȡ256 ��ָ��ģ���������
	*/
	ERR_FINGER read_conlist(unsigned char index,unsigned int &conlist);
/*!
	\brief ��ģ�����Ѵ洢��ָ��ģ�����	
*/
	ERR_FINGER read_finger_num(unsigned short &finger_num);
/*!
\brief
	̽����ָ��̽�⵽��¼��ָ��ͼ�����ImageBuffer��������¼��ɹ�ȷ���룻
	��̽�ⲻ����ָ��ֱ�ӷ�������ָȷ����(ģ�����ÿһ��ָ����ٷ�Ӧ����
	��������̽�⣬�����ѭ���������޶�ѭ���Ĵ�������ʱ��)��	

*/
	ERR_FINGER gen_image(void);

/*!
\brief ��ģ��ͼ�񻺳���ImageBuffer �е������ϴ�����λ��
*/
	ERR_FINGER upload_image(void);

/*! \brief ��λ������ͼ�����ݵ�ģ��ͼ�񻺳���ImageBuffer 

  \param img_size ͼƬ�Ĵ�С�������256*288

*/

	ERR_FINGER download_image(unsigned char* img_buf, int img_size,int pkt_size);
/*!
\brief ��ImageBuffer �е�ԭʼͼ������ָ������,�����洢��CharBuffer1 ��
		CharBuffer2��
\param  BufferID(������������)
\return ȷ����
	*/
	ERR_FINGER img2tz(int buf_id);

	/*!
\brief ��CharBuffer1 ��CharBuffer2 �е������ļ��ϲ�����ģ�壬ģ�����
		CharBuffer1 ��CharBuffer2(����������ͬ)��
\param  none
\return ȷ����
*/
	ERR_FINGER reg_model(void);


	/*!
\brief ɾ��ģ��ָ�ƿ���ָ����һ�Σ�ָ��ID �ſ�ʼ��N ��ָ��ģ�壩ģ�塣
\param  PageID(ָ�ƿ�ģ���) + N ɾ����ģ�������
\return ȷ����
*/
	ERR_FINGER del_char(unsigned short start_page, int num);

/*!
\brief  ɾ��ģ����ָ�ƿ�������ָ��ģ�塣
\param  none
\return ȷ����
*/
	ERR_FINGER empty(void);

	
	/*!
\brief  ��ģ������һ������������ظ���λ�����μ�4.7 ���������������
\param none
\return ȷ����
*/
	ERR_FINGER get_random_code(unsigned int &random);

	/// ������֤������ģ����Ҫ����������֤��USBģ�鲻��Ҫ
	ERR_FINGER authen(unsigned int addr,unsigned int password);
	/// ����������
	ERR_FINGER set_bps(int bps);
	/// ͨ�Ű���С����
    ERR_FINGER set_packet_size(unsigned short pkt_size);

	/// ��ȡָ��ҳ�ļ��±�����
	/// ҳ���� 16 pages
	/// ҳ��С 32 bytes
	ERR_FINGER read_note_page(unsigned char page,unsigned char *buff, int len);

	/// д�����ݵ�ָ��ҳ
	/// ҳ���� 16 pages
	/// ҳ��С 32 bytes
	ERR_FINGER write_note_page(unsigned char page,unsigned char *buff, int len);

	
	/*!
		\brief  ����ָ��ģ�嵽ģ���һ��������������
		\param  buf_id	����������ID,ȡֵ��Χ0-1
		\param  buff	ָ��ģ��������
		\param  len		ģ��ģ�峤��
		\return �������ؽ��
	*/
	ERR_FINGER download_to_buffer(int buf_id,unsigned char *buff, int buff_len,int pkt_size);


    ERR_FINGER upload_to_feature_buffer(int buf_id,int page);
	ERR_FINGER upload_to_buffer(int buf_id,unsigned char *buff, int buff_len,int pkt_size);


		/*!
		\brief  ����ָ��������������ָ��ģ�嵽ָ��ģ�����ݿ���
		\param  buf_id	����������ID,ȡֵ��Χ0-1
		\param  page	ָ��ģ�����ݿ�����ҳ���(0-999)
		\return ���ش洢���
	*/
	ERR_FINGER store_finger(int buf_id,int page);
	/*!
		\brief  ��ȡͼ������
		\param  buf_id	����������ID,ȡֵ��Χ0-1
		\param  page	ָ��ģ�����ݿ�����ҳ���(0-999)
		\return ���ش洢���
	*/
	ERR_FINGER read_image(int buf_id,int page);

	/*!
		\brief  ¼��ָ��
		\return ���ؼ�¼�µ�ָ�����ڵ����
		\retval >=0 �ɹ�
		\retval <0  ʧ��
	*/

	int record_finger(int page);

	/*!
		\brief  ���ݲɼ���ͼ��������������,���Ҵ����ָ��buf_id��(0,1)
		\return ���ؼ�¼�µ�ָ�����ڵ����
		\retval >=0 �ɹ�
		\retval <0  ʧ��
	*/
	ERR_FINGER gen_featrue_by_image(int buf_id);


	/*!
		\brief  ƥ������������1������������2�е�ָ�������Ƿ�һ�¡�
		\return ƥ����
		\retval ERR_OK ƥ��
		\retval ����   ʧ��
	*/
	ERR_FINGER match_finger();


	/*!
		\brief  �Զ�¼��ָ��,����ָ��ָ�ƿ�λ�ò����ؽ����
		\param	¼��ָ���������λ��
		\return ����¼��ָ�ƵĽ��
		\retval ERR_OK ƥ��
		\retval ����   ʧ��
	*/
	ERR_FINGER auto_record_finger(int page);

	/*!
		\brief  ����ָ�����ݡ�
		\param  start_page ָ�ƿ���ʼ���
		\param  num		������ָ�Ƹ���
		\return ƥ����,ָ�Ƶı��
	*/
    ERR_FINGER search_finger(unsigned short start_page, unsigned short num,int &page);


	ERR_FINGER  request_gen_feature_up();
	ERR_FINGER  gen_feature_up_data(unsigned char* buff,int pkt_size,bool &end);
	ERR_FINGER  gen_feature_up(unsigned char* buff, int buff_len,int pkt_size);

	static const char* format_error(ERR_FINGER err);
private:

	int  SendPacket(unsigned char type,unsigned char* context, int len);
	bool RecvPacket(unsigned char* context,int len,int timeout);


	ERR_FINGER request_download_to_buffer(int buf_id);

	ERR_FINGER download_packet_to_buffer(unsigned char* pkt_buff, int pkt_len,bool end);


	ERR_FINGER requst_upload_to_buffer(int buf_id);
	ERR_FINGER upload_packet_to_buffer(unsigned char *buff,int pkt_size,bool &end);
    void dump_ack_buf(unsigned char *buff,int buff_size);
	unsigned int  m_dev_addr;
	unsigned int  m_dev_password;
	unsigned char m_sendbuf[1024];
	unsigned char m_recvbuf[1024];
};

#endif
