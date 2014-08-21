#include "StdAfx.h"
#include ".\skinconfcontext.h"

CSkinConfContext::CSkinConfContext(void)
{
}

CSkinConfContext::~CSkinConfContext(void)
{
}
//sj t4.2 消费等级
LPVOID CSkinConfContext::GetPictureExEx(LPCTSTR lpName,int nConsume)
{
	CString sName;
	sName.Format("%s_%d",lpName,nConsume);
	if(nConsume==-1)
	{
		CPictureExEx *pInfo=NULL;
		std::map<CString,LPVOID>::iterator iter;
		iter=m_MapPictureEx.find(sName);
		if(iter != m_MapPictureEx.end())
		{
			OutputDebugString("GetPictureExEx old");
			pInfo=(CPictureExEx *)iter->second;
		}
		else
		{
			OutputDebugString("GetPictureExEx new");
			pInfo=new CPictureExEx();
			if(!pInfo->Load(lpName)) 
			{
				delete pInfo;
				return NULL;
			}
			m_MapPictureEx[sName]=(LPVOID)pInfo;

		}
		return pInfo;
	}
	else
	{
		/*CMyPictureEx *pInfo=NULL;
		std::map<CString,LPVOID>::iterator iter;
		iter=m_MapMyPictureEx.find(sName);
		if(iter != m_MapMyPictureEx.end())
		{
			OutputDebugString("GetMyPictureEx old");
			pInfo=(CMyPictureEx *)iter->second;
		}
		else
		{
			OutputDebugString("GetMyPictureEx new");
			pInfo=new CMyPictureEx();
			if(!pInfo->Load(lpName,nConsume)) 
			{
				delete pInfo;
				return NULL;
			}
			m_MapMyPictureEx[sName]=(LPVOID)pInfo;

		}
		return pInfo;*/
	}
	return NULL;
}