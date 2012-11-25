#ifndef  RS232_H
#define  RS232_H 


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
	int m_port;
};
#endif