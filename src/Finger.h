#ifndef  __FINGER_H
#define  __FINGER_H

#include <iostream>
/*!
\brief 指纹模块错误码
*/
typedef enum enum_FingerErr
{
    ERR_FINGER_OK = 0,				/*!< 成功*/
	ERR_PACKET,			/*!< 包错误*/
	ERR_NO_FINGER,		/*!< 传感器上无手指*/
	ERR_GET_IMG,		/*!< 图像录入不成功*/

	//////////////////////////////////////////
	ERR_IMG_NOT_CLEAN=6,  /*!<采集的图像不清楚 */
	ERR_IMG_NOT_ENOUGH=7,  /*!<采集的图像的特征点不足 */
	ERR_NOT_MATCH=8,		/*!<指纹不匹配 */
	ERR_SEARCH=9,			/*!<没有搜索到指纹 */

	//////////////////////////////////////////////
	ERR_GEN_CHAR_FAIL=0xa,  /*!<生成特征失败,将char1和char2中的特征文件合成模板失败 */
	ERR_PAGE_RANGE=0xb,			/*!<表示PageID 超出指纹库范围 */
	ERR_RD_TEMPLATE=0xc,	/*!<读出的模板数据有错或者无效 */
	ERR_UP_FEATURE=0xd, /*!<上传特征失败 */
	ERR_CONTINUE_RECV=0xe,  /*!<不能继续接收后续的包 */
	ERR_UP_IMG=0xf,			/*!<上传图像失败 */
	ERR_DEL_TEMPLATE=0x10,	/*!<删除模板失败 */
	ERR_CLEAR_FINGER_DB, /*!<清空指纹库失败 */

	//////////////////////////////////////////
	ERR_PASSWORD=0x13,		/*!<口令不正确 */
	///////////////////////////////////////////
	ERR_NO_IMG=0x15,			/*!<图像缓冲区中没有图像 */
	///////////////////////////////////////////
	ERR_RW_FLASH=0x18,		/*!<读写FLASH 出错 */
	///////////////////////////////////////////////////
	ERR_REG_INDEX=0x1a,		/*!<寄存器序号错误 */
	//////////////////////////////////////////////////
	ERR_ADDRESS=0x20, /*!<地址码错误 */
	ERR_AUTH,		/*!<必须验证口令 */
	////////////////////////////////////////////////////
	ERR_TIMEOUT,		/*!<接收到的数据超时获取长度不对 */
	ERR_RS232,			/*!<接口通信故障 */
	ERR_GEN_TEMP_FAIL,  /*!<生成模板失败 */
    ERR_INVALID_VAL	/*!<参数错误 */

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
	unsigned short  vid; //系统识别码
	unsigned short  num_fingers; //指纹容量
	unsigned int    baud_rate;
	unsigned short  security_level;
	unsigned short  packet_size;
	unsigned int    dev_addr;
}TDevParam;


/*!
\class CFinger
\brief 指纹识别模块类
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
	\brief 读取模块指纹模板索引表，且每次最多读取256 个指纹模板的索引表。
	*/
	ERR_FINGER read_conlist(unsigned char index,unsigned int &conlist);
/*!
	\brief 读模块内已存储的指纹模板个数	
*/
	ERR_FINGER read_finger_num(unsigned short &finger_num);
/*!
\brief
	探测手指，探测到后录入指纹图像存于ImageBuffer，并返回录入成功确认码；
	若探测不到手指，直接返回无手指确认码(模块对于每一条指令都快速反应，因
	此如连续探测，需进行循环处理，可限定循环的次数或总时间)。	

*/
	ERR_FINGER gen_image(void);

/*!
\brief 将模块图像缓冲区ImageBuffer 中的数据上传给上位机
*/
	ERR_FINGER upload_image(void);

/*! \brief 上位机下载图像数据到模块图像缓冲区ImageBuffer 

  \param img_size 图片的大小必须大于256*288

*/

	ERR_FINGER download_image(unsigned char* img_buf, int img_size,int pkt_size);
/*!
\brief 将ImageBuffer 中的原始图像生成指纹特征,特征存储于CharBuffer1 或
		CharBuffer2。
\param  BufferID(特征缓冲区号)
\return 确认字
	*/
	ERR_FINGER img2tz(int buf_id);

	/*!
\brief 将CharBuffer1 与CharBuffer2 中的特征文件合并生成模板，模板存于
		CharBuffer1 与CharBuffer2(两者内容相同)。
\param  none
\return 确认字
*/
	ERR_FINGER reg_model(void);


	/*!
\brief 删除模块指纹库中指定的一段（指定ID 号开始的N 个指纹模板）模板。
\param  PageID(指纹库模板号) + N 删除的模板个数。
\return 确认字
*/
	ERR_FINGER del_char(unsigned short start_page, int num);

/*!
\brief  删除模块中指纹库内所有指纹模板。
\param  none
\return 确认字
*/
	ERR_FINGER empty(void);

	
	/*!
\brief  令模块生成一个随机数并返回给上位机（参见4.7 随机数产生器）。
\param none
\return 确认字
*/
	ERR_FINGER get_random_code(unsigned int &random);

	/// 密码验证，串口模块需要进行密码验证，USB模块不需要
	ERR_FINGER authen(unsigned int addr,unsigned int password);
	/// 波特率设置
	ERR_FINGER set_bps(int bps);
	/// 通信包大小设置
    ERR_FINGER set_packet_size(unsigned short pkt_size);

	/// 读取指定页的记事本内容
	/// 页码数 16 pages
	/// 页大小 32 bytes
	ERR_FINGER read_note_page(unsigned char page,unsigned char *buff, int len);

	/// 写入内容到指定页
	/// 页码数 16 pages
	/// 页大小 32 bytes
	ERR_FINGER write_note_page(unsigned char page,unsigned char *buff, int len);

	
	/*!
		\brief  下载指纹模板到模块的一个特征缓冲区中
		\param  buf_id	特征缓冲区ID,取值范围0-1
		\param  buff	指纹模板数据区
		\param  len		模板模板长度
		\return 返回下载结果
	*/
	ERR_FINGER download_to_buffer(int buf_id,unsigned char *buff, int buff_len,int pkt_size);


    ERR_FINGER upload_to_feature_buffer(int buf_id,int page);
	ERR_FINGER upload_to_buffer(int buf_id,unsigned char *buff, int buff_len,int pkt_size);


		/*!
		\brief  保存指定特征缓冲区中指纹模板到指纹模板数据库中
		\param  buf_id	特征缓冲区ID,取值范围0-1
		\param  page	指纹模板数据库区的页编号(0-999)
		\return 返回存储结果
	*/
	ERR_FINGER store_finger(int buf_id,int page);
	/*!
		\brief  读取图像数据
		\param  buf_id	特征缓冲区ID,取值范围0-1
		\param  page	指纹模板数据库区的页编号(0-999)
		\return 返回存储结果
	*/
	ERR_FINGER read_image(int buf_id,int page);

	/*!
		\brief  录入指纹
		\return 返回记录下的指纹所在的序号
		\retval >=0 成功
		\retval <0  失败
	*/

	int record_finger(int page);

	/*!
		\brief  根据采集的图像生成特征数据,并且存放在指定buf_id中(0,1)
		\return 返回记录下的指纹所在的序号
		\retval >=0 成功
		\retval <0  失败
	*/
	ERR_FINGER gen_featrue_by_image(int buf_id);


	/*!
		\brief  匹配特征缓冲区1和特征缓冲区2中的指纹特征是否一致。
		\return 匹配结果
		\retval ERR_OK 匹配
		\retval 其他   失败
	*/
	ERR_FINGER match_finger();


	/*!
		\brief  自动录入指纹,存入指定指纹库位置并返回结果。
		\param	录入指纹所保存的位置
		\return 返回录入指纹的结果
		\retval ERR_OK 匹配
		\retval 其他   失败
	*/
	ERR_FINGER auto_record_finger(int page);

	/*!
		\brief  搜素指纹数据。
		\param  start_page 指纹库起始编号
		\param  num		搜索的指纹个数
		\return 匹配结果,指纹的编号
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
