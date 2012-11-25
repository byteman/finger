include "Rs232.h"
#include <pcomm.h>
#include <stdio.h>
#include <assert.h>

RS232Interface::RS232Interface()
{
	
}

bool RS232Interface::SetBaud(int bps)
{
	return (SIO_OK==sio_baud(m_port,bps));
}
int  RS232Interface::FlushIn(void)
{
	return sio_flush(m_port,0);

}
int  RS232Interface::FlushOut(void)
{
	return sio_flush(m_port,1);
}
bool RS232Interface::Open(const char* devname)
{
	int pos = strlen(devname);
	m_port = 1;
	return	(SIO_OK==sio_open(m_port));
}

int  RS232Interface::Write(unsigned char* buff, int len)
{
	return sio_write(m_port,(char*)buff,len);
}

size_t  RS232Interface::InQueSize(void)
{
	return sio_iqueue(m_port);
}
size_t  RS232Interface::OutQueSize(void)
{
	return sio_oqueue(m_port);
}
int  RS232Interface::Recv(unsigned char* buff, int len,int timeout)
{
	//sio_flush(m_port,2);
	sio_SetReadTimeouts(m_port,timeout,10);
	int size = sio_read(m_port,(char*)buff,len);
	assert(InQueSize()==0);
	return size;
}
