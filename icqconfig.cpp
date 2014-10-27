/***************************************************************************
 *																		   *
 *   copyright            : (C) 2002 by EPH			                       *
 ***************************************************************************/

#include "stdafx.h"
#include <stdio.h>
#include "icqconfig.h"

#define CONFIG_FILE		"wawa.cfg"
#define CONFIG_FILE2	"wawa2.cfg"

string IcqConfig::configDir;


void IcqConfig::getAllUsers(UinList &l)
{
	string pathName = configDir + CONFIG_FILE;
	FILE *file = fopen(pathName.c_str(), "r");
	char line[11];
	
	if (file) {
		while (fgets(line, sizeof(line), file))
		{
			uint32 uin=strtoul(line, NULL, 10);
			if(uin>0)
				l.push_back(strtoul(line, NULL, 10));
		}
		
		fclose(file);
	}
}

void IcqConfig::addUser(uint32 uin)
{
	UinList l;
	bool newUser = true;
	string pathName = configDir + CONFIG_FILE;
	FILE *file = fopen(pathName.c_str(), "r");
	
	if (file) {
		char line[11];	
		while (fgets(line, sizeof(line), file)) {
			uint32 d = strtoul(line, NULL, 10);
			if (d == uin) {
				newUser = false;
				l.push_front(d);
			} else
				l.push_back(d);
		}
		fclose(file);
	}
	if (newUser)
		l.push_front(uin);

	file = fopen(pathName.c_str(), "w");
	if (file) {
		UinList::iterator it;
		for (it = l.begin(); it != l.end(); ++it)
			fprintf(file, "%lu\n", *it);
		fclose(file);
	}
}

void IcqConfig::delUser(uint32 uin)
{
	UinList l;
	string pathName = configDir + CONFIG_FILE;
	FILE *file = fopen(pathName.c_str(), "r");
	
	if (file) {
		char line[11];	
		while (fgets(line, sizeof(line), file)) {
			uint32 d = strtoul(line, NULL, 10);
			if (d != uin)
				l.push_back(d);
		}
		fclose(file);
	}

	file = fopen(pathName.c_str(), "w");
	if (file) {
		UinList::iterator it;
		for (it = l.begin(); it != l.end(); ++it)
			fprintf(file, "%lu\n", *it);
		fclose(file);
	}
}


void IcqConfig::getAllUsers(UseList &l)
{
	string pathName = configDir + CONFIG_FILE2;
	FILE *file = fopen(pathName.c_str(), "r");
	char line[35];

	if (file) 
	{
		while (fgets(line, sizeof(line), file))
		{
			CString d;
			d.Format("%s",line);
			d.Replace("\n","");
			int nIndex = d.Find("_");
			if(nIndex>0 && (nIndex<d.GetLength()-1))
			{
				UseListInfo uli;
				CString strType="",strUse="";
				strType = d.Left(nIndex);
				strUse = d.Right(d.GetLength()-nIndex-1);
				uli.nType = atoi(strType);
				uli.strUin = strUse.GetString();
				l.push_back(uli);
			}
		}

		fclose(file);
	}
}

void IcqConfig::addUser(CString uin,int nType)
{
	StrList l;
	bool newUser = true;

	CString strUse="";
	strUse.Format("%d_%s",nType,uin);

	string pathName = configDir + CONFIG_FILE2;
	FILE *file = fopen(pathName.c_str(), "r");

	if (file) 
	{
		char line[35];	
		while (fgets(line, sizeof(line), file)) 
		{
			CString d;
			d.Format("%s",line);
			d.Replace("\n","");
			if (d.Compare(strUse)==0) 
			{
				newUser = false;
				l.push_front(d.GetString());
			} else if (d.Compare("")!=0)
				l.push_back(d.GetString());
		}
		fclose(file);
	}
	if (newUser)
		l.push_front(strUse.GetString());

	file = fopen(pathName.c_str(), "w");
	if (file) 
	{
		StrList::iterator it;
		for (it = l.begin(); it != l.end(); ++it)
			fprintf(file, "%s\n", (*it).c_str());
		fclose(file);
	}
}

void IcqConfig::delUser(CString uin,int nType)
{
	StrList l;

	CString strUse="";
	strUse.Format("%d_%s",nType,uin);

	string pathName = configDir + CONFIG_FILE2;
	FILE *file = fopen(pathName.c_str(), "r");

	if (file) 
	{
		char line[35];	
		while (fgets(line, sizeof(line), file)) 
		{
			CString d;
			d.Format("%s",line);
			d.Replace("\n","");
			if (d.Compare(strUse)!=0)
				l.push_back(d.GetString());
		}
		fclose(file);
	}

	file = fopen(pathName.c_str(), "w");
	if (file) 
	{
		StrList::iterator it;
		for (it = l.begin(); it != l.end(); ++it)
			fprintf(file, "%s\n", (*it).c_str());
		fclose(file);
	}
}