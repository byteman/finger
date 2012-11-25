#include "Rs232_Ctb.h"


RS232Interface::RS232Interface()
{
	m_port =  new wxSerialPort();
}

bool RS232Interface::SetBaud(int bps)
{
	wxBaud baudrate = wxBAUD_9600;
	switch (bps)
	{
		case 9600:
			baudrate = wxBAUD_115200;
			break;
		case 115200:
			baudrate = wxBAUD_115200;
			break;
		default:
			baudrate = wxBAUD_9600;
	}

	int ret = ((wxSerialPort*)m_port)->SetBaudRate(baudrate);
	return true;
}
bool RS232Interface::Open(const char* devname)
{
	if(m_port->Open(devname) < 0) 
	{
		return false;
	}
	return true;
}

int  RS232Interface::Write(unsigned char* buff, int len)
{

	return m_port->Write((char*)buff,len);
}

int  RS232Interface::Recv(unsigned char* buff, int len,int timeout)
{
	return m_port->Readv((char*)buff,len,timeout);
}
int  RS232Interface::FlushIn(void)
{
    wxSerialPort* sp = (wxSerialPort*)m_port;
    sp->FlushIn ();
    return 0;
}
int  RS232Interface::FlushOut(void)
{
    wxSerialPort* sp = (wxSerialPort*)m_port;
    sp->FlushOut ();
    return 0;
}
size_t  RS232Interface::InQueSize(void)
{
    wxSerialPort* sp = (wxSerialPort*)m_port;
    return sp->InQueueSize();
}
size_t  RS232Interface::OutQueSize(void)
{
    wxSerialPort* sp = (wxSerialPort*)m_port;
    return sp->InQueueSize();
}
