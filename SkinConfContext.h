#pragma once
#include <map>
#include "PictureExEx.h"
class CSkinConfContext
{
public:
	CSkinConfContext(void);
	~CSkinConfContext(void);

protected:
	std::map<CString,LPVOID> m_MapPictureEx;

public:
	LPVOID GetPictureExEx(LPCTSTR lpName,int nConsume=-1);

};
