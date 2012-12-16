#include "Rs2323.h"


RS232Interface::RS232Interface()
{
	
}

bool RS232Interface::SetBaud(int bps)
{
    m_port.setBaudrate (bps);
	return true;
}
bool RS232Interface::Open(const char* devname)
{
	m_port.setPort(devname);
    m_port.open();
	return true;
}

int  RS232Interface::Write(unsigned char* buff, int len)
{

	return m_port.write(buff,len);
}

int  RS232Interface::Recv(unsigned char* buff, int len,int timeout)
{
    m_port.setReadTimeout (timeout);
	return m_port.read(buff,len);
}
int  RS232Interface::FlushIn(void)
{
    m_port.flushInput ();
    return 0;
}
int  RS232Interface::FlushOut(void)
{
    m_port.flushOutput ();
    return 0;
}
size_t  RS232Interface::InQueSize(void)
{
    
    return m_port.available ();
}
size_t  RS232Interface::OutQueSize(void)
{
    return 0;
}
