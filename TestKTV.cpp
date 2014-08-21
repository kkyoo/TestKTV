// TestKTV.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "TestKTV.h"
#include "TestKTVDlg.h"
#include "face/AFCResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CString GetModuleDir();

// CTestKTVApp

BEGIN_MESSAGE_MAP(CTestKTVApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTestKTVApp ����

CTestKTVApp::CTestKTVApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CTestKTVApp ����

CTestKTVApp theApp;


// CTestKTVApp ��ʼ��

BOOL CTestKTVApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CString m_dir = GetModuleDir();

	AFCResource::getInstance().Init(m_dir,360);
	CTestKTVDlg dlg;
	dlg.SetRootDir(m_dir);
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȷ�������ر�
		//�Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//�Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	// ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

CString GetModuleDir()
{
	CString strPath;
	TCHAR szFull[_MAX_PATH];       //ȫ·�� "e:\myProject\vc\bin\debug\TestMFC.exe"
	TCHAR szDrive[_MAX_DRIVE];     //Ӳ�� "e:"
	TCHAR szDir[_MAX_DIR];         //Ŀ¼ "\myProject\vc\bin\debug\"
	TCHAR szFileName[_MAX_FNAME];  //�ļ��� "TestMFC"
	TCHAR szExt[_MAX_EXT];         //�ļ���ʽ ".exe"
	::GetModuleFileName(NULL, szFull, sizeof(szFull)/sizeof(TCHAR));
	_tsplitpath(szFull, szDrive, szDir, szFileName, szExt);
	_tcscpy(szFull, szDrive);
	_tcscat(szFull, szDir);
	strPath = CString(szFull);
	return strPath;
}
