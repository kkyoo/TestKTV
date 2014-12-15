#include "StdAfx.h"
#include ".\datapacket.h"
#include <string.h>

int COutDataPacket::setCursor(int pos) {
	int old = cursor - data;
	cursor = data + pos;
	return old;
}

OutDataPacket &COutDataPacket::operator <<(uint8 b)
{
	if (cursor <= data + MAX_PACKET_SIZE - sizeof(b)) {
		*(uint8 *) cursor = b;
		cursor += sizeof(b);
	}
	return (*this);
}

OutDataPacket &COutDataPacket::operator <<(uint16 w)
{
	if (cursor <= data + MAX_PACKET_SIZE - sizeof(w)) {
		*(uint16 *) cursor = htons(w);
		cursor += sizeof(w);
	}
	return (*this);
}

OutDataPacket &COutDataPacket::operator <<(uint32 dw)
{
	if (cursor <= data + MAX_PACKET_SIZE - sizeof(dw)) {
		*(uint32 *) cursor = htonl(dw);
		cursor += sizeof(dw);
	}
	return (*this);
}

OutDataPacket &COutDataPacket::operator <<(uint64 dw)
{
	if (cursor <= data + MAX_PACKET_SIZE - sizeof(dw)) {
		*(uint64 *) cursor = dw;
		cursor += sizeof(dw);
	}
	return (*this);
}

OutDataPacket &COutDataPacket::operator <<(const char *str)
{
	uint16 len = strlen(str) + 1;
	if (cursor <= data + MAX_PACKET_SIZE - sizeof(len) - len) {
		operator <<(len);
		strcpy(cursor, str);
		cursor += len;
	}
	return (*this);
}

OutDataPacket &COutDataPacket::operator <<(string &str) 
{
	return operator <<(str.c_str());
}

void COutDataPacket::writeData(const char *buf, int n)
{
	if (cursor <= data + MAX_PACKET_SIZE - n) {
		memcpy(cursor, buf, n);
		cursor += n;
	}
}

InDataPacket &CInDataPacket::operator >>(uint8 &b)
{
	if (cursor <= data + datalen - sizeof(b)) {
		b = *(uint8 *) cursor;
		cursor += sizeof(b);
	} else
		b = 0;
	return (*this);
}

InDataPacket &CInDataPacket::operator >>(uint16 &w)
{
	if (cursor <= data + datalen - sizeof(w)) {
		w = ntohs(*(uint16 *) cursor);
		cursor += sizeof(w);
	} else
		w = 0;
	return (*this);
}

InDataPacket &CInDataPacket::operator >>(uint32 &dw)
{
	if (cursor <= data + datalen - sizeof(dw)) {
		dw = ntohl(*(uint32 *) cursor);
		cursor += sizeof(dw);
	} else
		dw = 0;
	return (*this);

}

InDataPacket &CInDataPacket::operator >>(uint64 &dw)
{
	if (cursor <= data + datalen - sizeof(dw)) {
		dw = *(uint64 *) cursor;
		cursor += sizeof(dw);
	} else
		dw = 0;
	return (*this);

}

InDataPacket &CInDataPacket::operator >>(const char *&str)
{
	uint16 len;
	operator >>(len);

	if (len&&cursor <= data + datalen - len && !cursor[len - 1]) {
		str = cursor;
		cursor += len;
	} else
		str = "";
	return (*this);
}

InDataPacket &CInDataPacket::operator >>(string &str)
{
	const char *p;
	operator >>(p);
	str = p;
	return (*this);
}

InDataPacket &CInDataPacket::operator >>(CString &str) 
{
	string stri;
	operator >>(stri);
	str= stri.c_str();
	return (*this);
}

const char *CInDataPacket::readData(int &n)
{
	n = datalen - (cursor - data);
	return cursor;
}

int CInDataPacket::setCursor(int pos) 
{
	int old = cursor - data;
	cursor = data + pos;
	return old;
}

CInDataPacket* CDataPacketTrans::OutToIn(COutDataPacket* out)
{
	if(out)
	{
		char buf[MAX_PACKET_SIZE];
		char *p = buf;
		int n = out->getSize();
		memcpy(p, out->getData(), n);
		CInDataPacket *in = new CInDataPacket(p,n);
		return in;
	}
	return NULL;
}
