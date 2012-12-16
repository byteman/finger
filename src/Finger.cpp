#include <string.h>
#include <cstdio>
#include "Finger.h"
#include "Rs2323.h"
#include <assert.h>
RS232Interface rs232;

#define  CMD_PACKET  1
#define  DATA_PACKET 2
#define  ACK_PACKET  7
#define  END_PACKET  8

#define CHECK_BUF_ID(buf_id)  (assert( (buf_id==1) || (buf_id==2)))
CFinger::CFinger()
{
	m_dev_password  = 0;
	m_dev_addr		= 0xFFFFFFFF;


}
unsigned short CheckSum(unsigned char* context, int len)
{
	unsigned short sum = 0;

	for (int i = 0; i < len; i++)
	{
		sum += context[i];
	}

	return sum;
}

bool CFinger::RecvPacket(unsigned char* context, int len,int timeout)
{
	memset(context,0,len);
	int t_len = rs232.Recv(context,len,timeout);
    //printf("recv len = %d\n",t_len);
	return (t_len==len);
}

int CFinger::SendPacket(unsigned char type,unsigned char* context, int len)
{
	int index = 0;
	int t_len = 0;
	m_sendbuf[0] = 0xEF;
	m_sendbuf[1] = 0x01;

	m_sendbuf[2] = (m_dev_addr>>24)&0xFF;
	m_sendbuf[3] = (m_dev_addr>>16)&0xFF;
	m_sendbuf[4] = (m_dev_addr>>8)&0xFF;
	m_sendbuf[5] = (m_dev_addr>>0)&0xFF;

	m_sendbuf[6] = type;

	t_len = len + 2;
	m_sendbuf[7] = (t_len>>8)&0xFF;
	m_sendbuf[8] = (t_len&0xFF);
	
	memcpy(m_sendbuf+9,context,len);

	index = 9 + len;

	unsigned short sum = CheckSum(m_sendbuf+6,len+3);

	m_sendbuf[index]   = (sum>>8)&0xFF;
	m_sendbuf[index+1] = sum&0xFF;

	rs232.FlushIn(); //这里很关键，有可能在发送前，接收缓冲中就有数据了，这个时候再去接收就要出错。

	return rs232.Write(m_sendbuf,index+2);

}
ERR_FINGER CFinger::OpenInterface(const char* path,int baud)
{
	if(rs232.Open(path) == false)	 return ERR_RS232;
	if(rs232.SetBaud(baud) == false) return ERR_RS232;

    return ERR_FINGER_OK;
}
 
ERR_FINGER CFinger::set_password(unsigned int password)
{
	unsigned char context[5];
    ERR_FINGER ret = ERR_FINGER_OK;
	context[0] = 0x09;

	context[1] = (password>>24)&0xFF;
	context[2] = (password>>16)&0xFF;
	context[3] = (password>>8)&0xFF;
	context[4] = (password>>0)&0xFF;

	SendPacket(CMD_PACKET,context,5);
	
	if(RecvPacket(m_recvbuf,12,1000))
	{
		ret = (ERR_FINGER)m_recvbuf[9];
	}
	else
	{
		ret = ERR_TIMEOUT;
	}
	
	
	return ret;

}
ERR_FINGER CFinger::read_conlist(unsigned char index,unsigned int &conlist)
{
	unsigned char context[5];
    ERR_FINGER ret = ERR_FINGER_OK;

	if( index > 3) return ERR_INVALID_VAL;

	context[0] = 0x1F;
	
	context[1] = index;

	
	SendPacket(CMD_PACKET,context,2);
	
	if(RecvPacket(m_recvbuf,12,1000))
	{
		ret = (ERR_FINGER)m_recvbuf[9];
        if(ret== ERR_FINGER_OK)
		{
			conlist = (m_recvbuf[10] << 24) + (m_recvbuf[11] << 16) + (m_recvbuf[12] << 8)+(m_recvbuf[13]);
		}
	}
	else
	{
		ret = ERR_TIMEOUT;
	}
	
	
	return ret;
}

ERR_FINGER CFinger::read_finger_num(unsigned short &finger_num)
{
	unsigned char context[5];
    ERR_FINGER ret = ERR_FINGER_OK;
		
	context[0] = 0x1D;
		
	SendPacket(CMD_PACKET,context,1);
	
	if(RecvPacket(m_recvbuf,14,1000))
	{
		ret = (ERR_FINGER)m_recvbuf[9];
        if(ret== ERR_FINGER_OK)
		{
			finger_num =  (m_recvbuf[10] << 8)+(m_recvbuf[11]);
		}
	}
	else
	{
		ret = ERR_TIMEOUT;
	}
	
	
	return ret;
}
ERR_FINGER CFinger::set_address(unsigned int address)
{
	unsigned char context[5];
    ERR_FINGER ret = ERR_FINGER_OK;
	context[0] = 0x09;
	
	context[1] = (address>>24)&0xFF;
	context[2] = (address>>16)&0xFF;
	context[3] = (address>>8)&0xFF;
	context[4] = (address>>0)&0xFF;
	
	SendPacket(CMD_PACKET,context,5);
	
	if(RecvPacket(m_recvbuf,12,1000))
	{
		ret = (ERR_FINGER)m_recvbuf[9];
	}
	else
	{
		ret = ERR_TIMEOUT;
	}
	
	
	return ret;
}
ERR_FINGER CFinger::search_finger(unsigned short start_page, unsigned short num,int &page)
{
	unsigned char context[5];

    ERR_FINGER ret = ERR_FINGER_OK;


	context[0] = 0x20;
	context[1] = (start_page>>8)&0xFF;
	context[2] = (start_page>>0)&0xFF;
	context[3] = (num>>8)&0xFF;
	context[4] = (num>>0)&0xFF;

	SendPacket(CMD_PACKET,context,5);

	if(RecvPacket(m_recvbuf,16,1000))
	{
		ret = (ERR_FINGER)m_recvbuf[9] ;
        if(ret == ERR_FINGER_OK)
		{
			page = ((m_recvbuf[10]<<8) + m_recvbuf[11]);
		}
	}
	else
	{
		page = -1;
		ret = ERR_TIMEOUT;
	}

	return ret;

}

ERR_FINGER CFinger::request_download_to_buffer(int buf_id)
{
	unsigned char context[5];
    ERR_FINGER ret = ERR_FINGER_OK;

    CHECK_BUF_ID(buf_id);
	context[0] = 0x09;
	context[1] = (buf_id)&0xFF;
	
	SendPacket(CMD_PACKET,context,2);
	
	if(RecvPacket(m_recvbuf,12,1000))
	{
		ret = (ERR_FINGER)m_recvbuf[9];
	}
	else
	{
		ret = ERR_TIMEOUT;
	}
	
	
	return ret;


}
ERR_FINGER CFinger::store_finger(int buf_id,int page)
{
	unsigned char context[5];
    ERR_FINGER ret = ERR_FINGER_OK;

    CHECK_BUF_ID(buf_id);
	context[0] = 0x06;
	context[1] = (buf_id)&0xFF;
	context[2] = (page>>8)&0xFF;
	context[3] = (page)&0xFF;

	SendPacket(CMD_PACKET,context,4);
	
    if(RecvPacket(m_recvbuf,12,1000))
	{
		ret = (ERR_FINGER)m_recvbuf[9];
	}
	else
	{
		ret = ERR_TIMEOUT;
	}
	
	return ret;
}

ERR_FINGER CFinger::requst_upload_to_buffer(int buf_id)
{
	unsigned char context[5];
    ERR_FINGER ret = ERR_FINGER_OK;
    CHECK_BUF_ID(buf_id);
	context[0] = 0x08;
	context[1] = (buf_id)&0xFF;
	
	SendPacket(CMD_PACKET,context,2);
	
	if(RecvPacket(m_recvbuf,12,1000))
	{
		ret = (ERR_FINGER)m_recvbuf[9];
	}
	else
	{
		ret = ERR_TIMEOUT;
	}
	
	return ret;
}
ERR_FINGER CFinger::upload_packet_to_buffer(unsigned char *buff,int pkt_size,bool &end)
{
    ERR_FINGER ret = ERR_FINGER_OK;
	
	int ack_len = pkt_size+11;

	if(RecvPacket(m_recvbuf,ack_len,1000))
	{
		if (m_recvbuf[6] == 0x2)
		{
			memcpy(buff,m_recvbuf+9,pkt_size);
			end = false;
		}
		else
		{
			if(m_recvbuf[6] == 0x8)
			{
				memcpy(buff,m_recvbuf+9,pkt_size);
				end = true;
			}
			else
			{
				end = true;
				ret = ERR_INVALID_VAL;
			}
		}
	}
	else
	{
		end = true;
		ret = ERR_TIMEOUT;
	}
	return ret;
}

ERR_FINGER CFinger::upload_to_buffer(int buf_id,unsigned char *buff, int buff_len,int pkt_size)
{

    ERR_FINGER ret = ERR_FINGER_OK;
    CHECK_BUF_ID(buf_id);
	if (buff_len < 512) ///模板文件长度不能大于512字节
	{
		
		return ERR_INVALID_VAL;
	}
	
	ret = requst_upload_to_buffer(buf_id);

    if( ret != ERR_FINGER_OK) return ret;
	
	int index = 0;

	bool end = false;

	while( (!end) && (index < buff_len))
	{
		ret = upload_packet_to_buffer(buff+index,pkt_size,end);
		
        if(ERR_FINGER_OK != ret)
		{
			break;
		}

		index	 += pkt_size;
		
	}
	
	
	return ret;
}


ERR_FINGER CFinger::upload_to_feature_buffer(int buf_id,int page)
{
	unsigned char context[5];
    ERR_FINGER ret = ERR_FINGER_OK;
    CHECK_BUF_ID(buf_id);
	context[0] = 0x07;
	context[1] = (buf_id)&0xFF;
	context[2] = (page>>8)&0xFF;
	context[3] = (page)&0xFF;
	
	SendPacket(CMD_PACKET,context,4);
	
	if(RecvPacket(m_recvbuf,12,1000))
	{
		ret =(ERR_FINGER) m_recvbuf[9];
	}
	else
	{
		ret = ERR_TIMEOUT;
	}
	
	return ret;
}
ERR_FINGER CFinger::download_packet_to_buffer(unsigned char* pkt_buff, int pkt_len,bool end)
{

	unsigned char context[512];

    ERR_FINGER ret = ERR_FINGER_OK;
	unsigned char type = end?END_PACKET:DATA_PACKET;

	memcpy(context,pkt_buff,pkt_len);

	SendPacket(type,context,pkt_len);

	return ret;

}
//每个buf的长度是512字节 ,而模板文件会小于 buf的长度
ERR_FINGER CFinger::download_to_buffer(int buf_id,unsigned char *buff, int buff_len,int pkt_size)
{
	int left_len = buff_len;
    ERR_FINGER ret = ERR_FINGER_OK;
    CHECK_BUF_ID(buf_id);
	if (buff_len > 512) ///模板文件长度不能大于512字节
	{
		
		return ERR_INVALID_VAL;
	}

	ret = request_download_to_buffer(buf_id);
    if( ret != ERR_FINGER_OK)
    {
        printf("request_download_to_buffer failed\n");
        return ret;
    }
#if 1
	int index = 0;
	while(left_len > 0)
	{
		int send_len = (left_len >= pkt_size) ? pkt_size: left_len;

		bool end = (left_len <= pkt_size) ? true : false;

		ret = download_packet_to_buffer(buff+index,send_len,end);
        //这里最好做一个延时操作,否则有可能后面的store_finger命令不会返回数据.

        usleep (10000);
        if(ret != ERR_FINGER_OK)
		{
            printf("download_packet_to_buffer failed\n");
			break;
		}
		left_len -= send_len;
		index	 += send_len;
		
	}
#endif
	return ret;
}
ERR_FINGER CFinger::authen(unsigned int addr,unsigned int password)
{
	unsigned char context[5];
    ERR_FINGER ret = ERR_FINGER_OK;
	m_dev_password = password;
	m_dev_addr	   = addr;
	
	context[0] = 0x13;

	context[1] = (m_dev_password>>24)&0xFF;;
	context[2] = (m_dev_password>>16)&0xFF;;
	context[3] = (m_dev_password>>8)&0xFF;;
	context[4] = (m_dev_password>>0)&0xFF;;


	SendPacket(CMD_PACKET,context,5);
	
	if(RecvPacket(m_recvbuf,12,1000))
	{
		ret = (ERR_FINGER)m_recvbuf[9];
	}
	else
	{
		ret = ERR_TIMEOUT;
	}
	

	return ret;
}
ERR_FINGER CFinger::gen_image(void)
{
	unsigned char context[5];
    ERR_FINGER ret = ERR_FINGER_OK;
	context[0] = 0x1;

	SendPacket(CMD_PACKET,context,1);
	
	if(RecvPacket(m_recvbuf,12,1000))
	{
		ret = (ERR_FINGER)m_recvbuf[9];
	}
	else
	{
		ret = ERR_TIMEOUT;
	}
	
	
	return ret;

}
ERR_FINGER CFinger::upload_image(void)
{
	unsigned char context[5];
    ERR_FINGER ret = ERR_FINGER_OK;
	context[0] = 0xa;
	
	SendPacket(CMD_PACKET,context,1);
	
	if(RecvPacket(m_recvbuf,12,1000))
	{
		ret = (ERR_FINGER)m_recvbuf[9];
	}
	else
	{
		ret = ERR_TIMEOUT;
	}
	
	
	return ret;

}
ERR_FINGER CFinger::download_image(unsigned char* img_buf, int img_size,int pkt_size)
{
	unsigned char context[5];
    ERR_FINGER ret = ERR_FINGER_OK;
	context[0] = 0xb;
	
	SendPacket(CMD_PACKET,context,1);
	
	if(RecvPacket(m_recvbuf,12,1000))
	{
		ret = (ERR_FINGER)m_recvbuf[9];
	}
	else
	{
		ret = ERR_TIMEOUT;
	}
	
    if(ret != ERR_FINGER_OK) 	return ret;

	int index = 0;
	int left_len = img_size;
	while(left_len > 0)
	{
		int send_len = (left_len >= pkt_size) ? pkt_size: left_len;
		
		bool end = (left_len <= pkt_size) ? true : false;
		
		ret = download_packet_to_buffer(img_buf+index,send_len,end);
        if(ret != ERR_FINGER_OK)
		{
			break;
		}		
		left_len -= send_len;
		index	 += send_len;

	}
	return ret;
}
ERR_FINGER CFinger::img2tz(int buf_id)
{
	unsigned char context[5];
    ERR_FINGER ret = ERR_FINGER_OK;

    CHECK_BUF_ID(buf_id);
	context[0] = 0xb;
	context[1] = (buf_id&0xFF);

	SendPacket(CMD_PACKET,context,2);
	
	if(RecvPacket(m_recvbuf,12,1000))
	{
		ret = (ERR_FINGER)m_recvbuf[9];
	}
	else
	{
		ret = ERR_TIMEOUT;
	}
	
	return ret;
}
ERR_FINGER CFinger::reg_model(void)
{
	unsigned char context[5];
    ERR_FINGER ret = ERR_FINGER_OK;
	context[0] = 0x5;

	
	SendPacket(CMD_PACKET,context,1);
	
	if(RecvPacket(m_recvbuf,12,1000))
	{
		ret = (ERR_FINGER)m_recvbuf[9];
	}
	else
	{
		ret = ERR_TIMEOUT;
	}
	
	return ret;
}
ERR_FINGER CFinger::get_random_code(unsigned int &random)
{
	unsigned char context[5];
    ERR_FINGER ret = ERR_FINGER_OK;
	
	context[0] = 0x14;
	
	SendPacket(CMD_PACKET,context,1);
	
	if(RecvPacket(m_recvbuf,12,1000))
	{
		ret = (ERR_FINGER)m_recvbuf[9];
        if(ret== ERR_FINGER_OK)
		{
			random =  (m_recvbuf[10] << 24)+(m_recvbuf[11] << 16)+(m_recvbuf[12] << 8)+(m_recvbuf[13]);
		}
	}
	else
	{
		ret = ERR_TIMEOUT;
	}
	
	
	return ret;
}
ERR_FINGER CFinger::empty(void)
{
	unsigned char context[5];
    ERR_FINGER ret = ERR_FINGER_OK;
	context[0] = 0xd;
	
	
	SendPacket(CMD_PACKET,context,1);
	
	if(RecvPacket(m_recvbuf,12,1000))
	{
		ret = (ERR_FINGER)m_recvbuf[9];
	}
	else
	{
		ret = ERR_TIMEOUT;
	}
	
	return ret;
}
ERR_FINGER CFinger::del_char(unsigned short start_page, int num)
{
	unsigned char context[5];
    ERR_FINGER ret = ERR_FINGER_OK;
	context[0] = 0xC;
	context[1] = (start_page>>8)&0xFF;
	context[2] = (start_page)&0xFF;
	context[3] = (num>>8)&0xFF;
	context[4] = (num)&0xFF;

	
	SendPacket(CMD_PACKET,context,5);
	
	if(RecvPacket(m_recvbuf,12,1000))
	{
		ret = (ERR_FINGER)m_recvbuf[9];
	}
	else
	{
		ret = ERR_TIMEOUT;
	}
	
	return ret;
}
ERR_FINGER CFinger::read_note_page(unsigned char page,unsigned char *buff, int len)
{

	unsigned char context[33];
    ERR_FINGER ret = ERR_FINGER_OK;
	
	if(len <  32)  return ERR_INVALID_VAL;
	if(page > 0xe) return ERR_INVALID_VAL;
	
	
	context[0] = 0x19;
	context[1] = page;
	
	SendPacket(CMD_PACKET,context,2);
	
	if(RecvPacket(m_recvbuf,44,1000))
	{
		ret = (ERR_FINGER)m_recvbuf[9];
        if(ERR_FINGER_OK == ret)
		{
			memcpy(buff,m_recvbuf+10,32);
		}
	}
	else
	{
		ret = ERR_TIMEOUT;
	}
	
	return ret;
}

int CFinger::record_finger(int page)
{

	return -1;
}
void CFinger::dump_ack_buf(unsigned char *buff,int buff_size)
{
    printf("********************dump ack data******************\n");
    for(int i = 0; i < buff_size; i++)
    {
        printf("%02x ",buff[i]);
    }
    printf("\n");
}
ERR_FINGER CFinger::auto_record_finger(int page)
{
	unsigned char context[5];
    ERR_FINGER ret = ERR_FINGER_OK;

	
	context[0] = 0x21;
	
	context[1] = (page>>8)&0xFF;
	context[2] = (page>>0)&0xFF;

	
	SendPacket(CMD_PACKET,context,3);
//当无手指的时候，模块回应的无手指的时间比较长，所以这里的超时时间设置5s
    if(RecvPacket(m_recvbuf,12,5000))
	{
        dump_ack_buf(m_recvbuf,12);
		ret = (ERR_FINGER)m_recvbuf[9];
	}
	else
	{
		ret = ERR_TIMEOUT;
	}

	return ret;
}

ERR_FINGER CFinger::gen_featrue_by_image(int buf_id)
{
    CHECK_BUF_ID(buf_id);
    return ERR_FINGER_OK;
}


ERR_FINGER CFinger::write_note_page(unsigned char page,unsigned char *buff, int len)
{

	unsigned char context[33];
    ERR_FINGER ret = ERR_FINGER_OK;
	
	if(len >  32)  return ERR_INVALID_VAL;
	if(page > 0xe) return ERR_INVALID_VAL;

	memset(context,0,33);
	context[0] = 0x18;
	context[1] = page;

	memcpy(context+2,buff,len);
	
	SendPacket(CMD_PACKET,context,34);
	
	if(RecvPacket(m_recvbuf,12,1000))
	{
		ret = (ERR_FINGER)m_recvbuf[9];
	}
	else
	{
		ret = ERR_TIMEOUT;
	}
	
	return ret;
}

ERR_FINGER CFinger::get_dev_pararm(TDevParam& param)
{

	unsigned char context[5];
    ERR_FINGER ret = ERR_FINGER_OK;
	unsigned short pkt_list[] = {32,64,128,256};
	
	context[0] = 0xF;
	
	SendPacket(CMD_PACKET,context,1);
	
	if(RecvPacket(m_recvbuf,28,1000))
	{
		ret = (ERR_FINGER)m_recvbuf[9];
        if(ERR_FINGER_OK == ret )
		{
				param.status.status_value = ((m_recvbuf[10]<<8)+m_recvbuf[11]);
				param.vid = ((m_recvbuf[12]<<8)+m_recvbuf[13]);
				param.num_fingers = ((m_recvbuf[14]<<8)+m_recvbuf[15]);
				param.security_level = ((m_recvbuf[16]<<8)+m_recvbuf[17]);
				param.dev_addr = (m_recvbuf[18]<<24)+(m_recvbuf[19]<<16)+(m_recvbuf[20]<<8)+m_recvbuf[21];
				int idx =  ((m_recvbuf[22]<<8)+m_recvbuf[23]);
				param.packet_size= pkt_list[idx%4];
				idx = (m_recvbuf[24]<<8)+m_recvbuf[25];
				param.baud_rate = idx * 9600;
		}
	}
	else
	{
		ret = ERR_TIMEOUT;
	}
	
	return ret;
}
ERR_FINGER CFinger::set_dev_pararm(TDevParam& param)
{
	
	return ERR_INVALID_VAL;
}

ERR_FINGER CFinger::set_bps(int bps)
{
	unsigned char context[5];
    ERR_FINGER ret = ERR_FINGER_OK;
	
	if( (bps < 1) || (bps > 12) ) return ERR_INVALID_VAL;
	
	
	context[0] = 0xE;
	context[1] = 0x4;
	context[2] = bps&0xFF;
	SendPacket(CMD_PACKET,context,3);
	
	
	if(RecvPacket(m_recvbuf,12,1000))
	{
		ret = (ERR_FINGER)m_recvbuf[9];
	}
	else
	{
		ret = ERR_TIMEOUT;
	}

	return ret;
}
ERR_FINGER CFinger::set_packet_size(unsigned short pkt_size)
{
	unsigned char context[5];
    ERR_FINGER ret = ERR_FINGER_OK;
	
	if(pkt_size!=32)  return ERR_INVALID_VAL;
	if(pkt_size!=64)  return ERR_INVALID_VAL;
	if(pkt_size!=128) return ERR_INVALID_VAL;
	if(pkt_size!=256) return ERR_INVALID_VAL;
	

	context[0] = 0xE;
	context[1] = 0x6;
    unsigned char value = 0;
    switch(pkt_size)
    {
        case 32:
            value=0;
            break;
        case 64:
            value=1;
            break;
        case 128:
            value=2;
            break;
        case 256:
            value=3;
            break;
        default:
            value = 2;
            break;

    }
    context[2] = value;
	SendPacket(CMD_PACKET,context,3);


	if(RecvPacket(m_recvbuf,12,1000))
	{
		ret = (ERR_FINGER)m_recvbuf[9];
	}
	else
	{
		ret = ERR_TIMEOUT;
	}

	return ret;
}

ERR_FINGER  CFinger::request_gen_feature_up()
{
	unsigned char context[5];
    ERR_FINGER ret = ERR_FINGER_OK;

	
	context[0] = 0x31;

	SendPacket(CMD_PACKET,context,1);
	
	
	if(RecvPacket(m_recvbuf,12,1000))
	{
		ret = (ERR_FINGER)m_recvbuf[9];
	}
	else
	{
		ret = ERR_TIMEOUT;
	}
	
	return ret;
}
ERR_FINGER  CFinger::gen_feature_up_data(unsigned char* buff,int pkt_size,bool &end)
{
    ERR_FINGER ret = ERR_FINGER_OK;
	
    if(RecvPacket(buff,pkt_size+11,1000))
	{
		if (buff[6] == 0x2)
		{
			end = false;
		}
		else
		{
			
			if(buff[6] == 0x8)
			{
				end = true;
			}
			else
			{
				end = true;
				ret = ERR_INVALID_VAL;
			}
		}
	}
	else
	{
		end = true;
		ret = ERR_TIMEOUT;
	}
	return ret;
	

}
ERR_FINGER  CFinger::gen_feature_up(unsigned char* buff, int buff_len,int pkt_size)
{
    ERR_FINGER ret = ERR_FINGER_OK;

	ret = request_gen_feature_up() ;
    if( ret != ERR_FINGER_OK)
	{
		return ret;
	}
	int index = 0;
	
	bool end = false;
	
	while( (!end) && (index < buff_len))
	{
		ret = gen_feature_up_data(buff+index,pkt_size,end);
		
        if(ERR_FINGER_OK != ret)
		{
			break;
		}
		
		index	 += pkt_size;
		
	}
	
	return ret;
}

typedef struct tag_errItem{
	ERR_FINGER  err_code;
	const char* err_string;
}ErrItem;
static ErrItem err_list[] = {

    {ERR_FINGER_OK,				"成功"},
	{ERR_PACKET,			"包错误"},
	{ERR_NO_FINGER,			"传感器上无手指"},
	{ERR_GET_IMG,			"图像录入不成功"},
	{ERR_IMG_NOT_CLEAN,		"采集的图像不清楚"},
	{ERR_IMG_NOT_ENOUGH,	"采集的图像的特征点不足"},
	{ERR_NOT_MATCH,			"指纹不匹配"},
	{ERR_SEARCH,			"没有搜索到指纹"},
	{ERR_GEN_CHAR_FAIL,		"生成特征失败,将char1和char2中的特征文件合成模板失败"},
	{ERR_PAGE_RANGE,		"表示PageID 超出指纹库范围"},
	{ERR_RD_TEMPLATE,		"读出的模板数据有错或者无效"},
	{ERR_UP_FEATURE,		"上传特征失败"},
	{ERR_CONTINUE_RECV,		"不能继续接收后续的包"},
	{ERR_UP_IMG,			"上传图像失败"},
	{ERR_DEL_TEMPLATE,		"删除模板失败"},
	{ERR_CLEAR_FINGER_DB,	"清空指纹库失败"},
	{ERR_PASSWORD,			"口令不正确"},
	{ERR_NO_IMG,			"图像缓冲区中没有图像"},
	{ERR_RW_FLASH,			"读写FLASH 出错"},
	{ERR_REG_INDEX,			"寄存器序号错误"},
	{ERR_ADDRESS,			"地址码错误"},
	{ERR_AUTH,				"必须验证口令"},
	{ERR_TIMEOUT,			"接收到的数据超时获取长度不对"},
	{ERR_RS232,				"接口通信故障"},
	{ERR_GEN_TEMP_FAIL,		"生成模板失败"},
	{ERR_INVALID_VAL,		"参数错误"},

};

const char* CFinger::format_error(ERR_FINGER err)
{
	int i = 0;
	int max_cnt = (sizeof(err_list) / sizeof(ErrItem));
	for(i = 0; i < max_cnt; i++)
	{
		if(err == err_list[i].err_code)
			break; 
	}
	if(i < max_cnt) return err_list[i].err_string;

	return "unknown";
}
