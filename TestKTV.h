// TestKTV.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CTestKTVApp:
// �йش����ʵ�֣������ TestKTV.cpp
//

class CTestKTVApp : public CWinApp
{
public:
	CTestKTVApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestKTVApp theApp;