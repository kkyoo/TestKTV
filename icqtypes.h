/***************************************************************************
 *                                                                         *
 *   copyright            : (C) 2002 by EPH						            *
 ***************************************************************************/

#ifndef _ICQ_TYPES_H_
#define _ICQ_TYPES_H_

#include <string>
#include <list>
#include <vector>
#include <bitset>

using namespace std;

struct UseListInfo
{
	string strUin;
	int    nType;
};

typedef list<UseListInfo>	UseList;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;

typedef list<string> 	StrList;
typedef list<void *>	PtrList;
typedef list<uint32>	UinList;


#ifdef _WIN32
#define strncasecmp		_strnicmp
#define strcasecmp		_stricmp
#endif

struct ResultInfo
{
	CString host;
	CString ip;
	UINT port;
	UINT type;
	int  timeSpent;
	bool loginAddr;
};
typedef vector<ResultInfo> RESULT_VECTOT;

#endif
