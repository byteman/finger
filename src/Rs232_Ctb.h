#ifndef  RS232_H
#define  RS232_H 
#include "wx/ctb/getopt.h"
#if ( GPIB )
# include "wx/ctb/gpib.h"
#endif
#include "wx/ctb/iobase.h"
#include "wx/ctb/serport.h"
#include "wx/ctb/timer.h"

class RS232Interface
{
public:
	RS232Interface();
	bool SetBaud(int bps);
	bool Open(const char* devname);
	int  Write(unsigned char* buff, int len);
	int  Recv(unsigned char* buff, int len,int timeout);
	int  FlushIn(void);
	int  FlushOut(void);
	size_t  InQueSize(void);
	size_t  OutQueSize(void);
private:
	wxIOBase* m_port;
};
#endif