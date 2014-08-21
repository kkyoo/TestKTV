// TestKTV.cpp : 定义应用程序的类行为。
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


// CTestKTVApp 构造

CTestKTVApp::CTestKTVApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CTestKTVApp 对象

CTestKTVApp theApp;


// CTestKTVApp 初始化

BOOL CTestKTVApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CString m_dir = GetModuleDir();

	AFCResource::getInstance().Init(m_dir,360);
	CTestKTVDlg dlg;
	dlg.SetRootDir(m_dir);
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用“确定”来关闭
		//对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用“取消”来关闭
		//对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	// 而不是启动应用程序的消息泵。
	return FALSE;
}

CString GetModuleDir()
{
	CString strPath;
	TCHAR szFull[_MAX_PATH];       //全路径 "e:\myProject\vc\bin\debug\TestMFC.exe"
	TCHAR szDrive[_MAX_DRIVE];     //硬盘 "e:"
	TCHAR szDir[_MAX_DIR];         //目录 "\myProject\vc\bin\debug\"
	TCHAR szFileName[_MAX_FNAME];  //文件名 "TestMFC"
	TCHAR szExt[_MAX_EXT];         //文件格式 ".exe"
	::GetModuleFileName(NULL, szFull, sizeof(szFull)/sizeof(TCHAR));
	_tsplitpath(szFull, szDrive, szDir, szFileName, szExt);
	_tcscpy(szFull, szDrive);
	_tcscat(szFull, szDir);
	strPath = CString(szFull);
	return strPath;
}
