#ifndef WHG_DATAPACKET_H
#define WHG_DATAPACKET_H

#include <string>
#include <winsock.h>

using namespace std;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
#ifdef WIN32
typedef unsigned __int64 uint64;
#else
typedef unsigned long long uint64;
#endif

#define MAX_PACKET_SIZE		4096

class CDataPacket
{
public:
	virtual const char *getData() = 0;
	virtual int getSize() = 0;
};

class OutDataPacket : public CDataPacket 
{
public:
	virtual OutDataPacket &operator <<(uint8 b) = 0;
	virtual OutDataPacket &operator <<(uint16 w) = 0;
	virtual OutDataPacket &operator <<(uint32 dw) = 0;
	virtual OutDataPacket &operator <<(uint64 dw) = 0;
	virtual OutDataPacket &operator <<(const char *str) = 0;
	virtual OutDataPacket &operator <<(string &s) = 0;
	virtual void writeData(const char *buf, int n) = 0;
};

class InDataPacket : public CDataPacket 
{
public:
	virtual InDataPacket &operator >>(uint8 &b) = 0;
	virtual InDataPacket &operator >>(uint16 &w) = 0;
	virtual InDataPacket &operator >>(uint32 &dw) = 0;
	virtual InDataPacket &operator >>(uint64 &dw) = 0;
	virtual InDataPacket &operator >>(const char *&str) = 0;
	virtual InDataPacket &operator >>(string &s) = 0;
	virtual InDataPacket &operator >>(CString &str)=0;
	virtual uint16 getCmd(){return 0;};
	virtual const char *readData(int &n) = 0;
};

class COutDataPacket : public OutDataPacket 
{
public:
	COutDataPacket() 
	{
		cursor = data;
	}
	const char *getData() 
	{
		return data;
	}
	int getSize() 
	{
		return (cursor - data);
	}

	virtual OutDataPacket &operator <<(string &str);
	virtual OutDataPacket &operator <<(uint8 b);
	virtual OutDataPacket &operator <<(uint16 w);
	virtual OutDataPacket &operator <<(uint32 dw);
	virtual OutDataPacket &operator <<(uint64 dw);
	virtual OutDataPacket &operator <<(const char *str);
	void writeData(const char *buf, int n);
	int setCursor(int off);

protected:
	char data[MAX_PACKET_SIZE];
	char *cursor;
};

class CInDataPacket : public InDataPacket 
{
public:
	CInDataPacket(const char *d, int len) 
	{
		cursor = data = d;
		datalen = len;
	}
	virtual const char *getData() 
	{
		return data;
	}
	virtual int getSize() 
	{
		return datalen;
	}
	
	virtual InDataPacket &operator >>(uint8 &b);
	virtual InDataPacket &operator >>(uint16 &w);
	virtual InDataPacket &operator >>(uint32 &dw);
	virtual InDataPacket &operator >>(uint64 &dw);
	virtual InDataPacket &operator >>(const char *&str);
	virtual InDataPacket &operator >>(string &str);
	virtual InDataPacket &operator >>(CString &str);
	const char *readData(int &n);
	int setCursor(int pos);

protected:
	const char *data;
	int datalen;
	const char *cursor;
};

class CDataPacketTrans
{
public:
	static CInDataPacket* OutToIn(COutDataPacket* out);

};

#endif