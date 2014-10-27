/***************************************************************************
 *																		   *
 *   copyright            : (C) 2002 by EPH			                       *
 ***************************************************************************/

#ifndef _ICQ_CONFIG_H
#define _ICQ_CONFIG_H

#include "icqtypes.h"

/*
 * Simple utility class that manages the eph.cfg file
 */
class /*AFX_EXT_CLASS*/ IcqConfig {
public:
	static void setDir(const char *dir) {
		configDir = dir;
	}
	static void addUser(uint32 uin);
	static void delUser(uint32 uin);
	static void getAllUsers(UinList &l);

	static void addUser(CString uin,int nType=0);
	static void delUser(CString uin,int nType=0);
	static void getAllUsers(UseList &l);
	
private:
	static string configDir;
};

#endif
