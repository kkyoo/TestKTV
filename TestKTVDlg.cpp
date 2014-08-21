// TestKTVDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestKTV.h"
#include "TestKTVDlg.h"
#include ".\testktvdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_TAB_WND					100					//属性页 ID

#define IDC_OUTBAR				1001

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTestKTVDlg 对话框



CTestKTVDlg::CTestKTVDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestKTVDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	InitStruct();
}

void CTestKTVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestKTVDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_PHOTO, OnBnClickedBtnPhoto)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_SPECIALGIFT, OnBnClickedBtnSpecialgift)
END_MESSAGE_MAP()


// CTestKTVDlg 消息处理程序

BOOL CTestKTVDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_PhotoGraphMgr=CreatePhotographMgr(NULL);
	m_NPGuideMgr=CreateNPGuideMgr();
	CRect rtBtnNext,rtBtnEnd;
	rtBtnNext.left = 691;
	rtBtnNext.top = 571;
	rtBtnNext.right = rtBtnNext.left+126;
	rtBtnNext.bottom = rtBtnNext.top+50;

	rtBtnEnd.left = 464;
	rtBtnEnd.top = 609;
	rtBtnEnd.right = rtBtnNext.left+169;
	rtBtnEnd.bottom = rtBtnNext.top+101;

	int nClientType = 2;
	int nVersion = 0;
	int nStepNum = 2;
	
	//m_NPGuideMgr->StartNewProductGuide(GetRootDir(),nClientType,nVersion,nStepNum,rtBtnNext,rtBtnEnd);

	m_pSkinConfContext=new CSkinConfContext();
	m_GiftSpecial=new CExpensiveGiftCtrl;
	m_GiftSpecial->SubclassWindow(GetDlgItem(IDC_STATIC_SPECIALGIFT)->GetSafeHwnd());
	m_GiftSpecial->SetSkinConfContext(m_pSkinConfContext);
	m_GiftSpecial->SetWindowPos(NULL,0,130,1050,60,SWP_NOZORDER);

	//建立tab控件
	m_cTab.CreateTabWnd(this,EPHDLG_CLASSNAME,ID_TAB_WND);
	m_cTab.ShowCopyRight(true);

	outbarCtrl.CreateEx(NULL,/*WS_EX_STATICEDGE,*//*WS_EX_CLIENTEDGE,*/ NULL, NULL, WS_VISIBLE | WS_CHILD,
		CRect(0,0,10,10), &m_cTab, IDC_OUTBAR);	


	CBitmap bitmap,bitmap1;
	bitmap.LoadBitmap(IDB_TABLOGO);
	bitmap1.LoadBitmap(IDB_TABLOGO_1);
	m_cTab.AddTabPage(&outbarCtrl,TEXT("好友管理"),&bitmap,RGB(192,192,192),false);

	m_cTab.AddTabPage(&outbarCtrl,TEXT("KTV房间"),&bitmap1,RGB(192,192,192),false);

	//装载插件
	//LoadEphPlugins();					//mainly ktvplugins.Comment the statement,we can't see the ktv desktop in the list

	//设置初始tab页面
	int pos = 1;
	m_cTab.SetActivePageByExtIndex(pos);	

	m_cTab.MoveWindow(0,200,500,150);
	m_cTab.ShowWindow(SW_SHOW);
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CTestKTVDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestKTVDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CTestKTVDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestKTVDlg::InitStruct()
{
	m_basePath = _T("");
	m_PhotoGraphMgr = NULL;
	m_NPGuideMgr = NULL;
	m_pSkinConfContext=NULL;
	m_GiftSpecial=NULL;
	m_tabImageList.Create(25,25,ILC_COLOR24|ILC_MASK,0, 8);

	

	m_funcImageList.Create(16, 16, ILC_COLOR24|ILC_MASK,2,2);

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_FUNCVIDEO);
	m_funcImageList.Add(&bitmap,RGB(192,192,192));

	outbarCtrl.setListener(this);
	outbarCtrl.setImageList(&AFCResource::getInstance().getLargeImageList(), &AFCResource::getInstance().getSmallImageList(), &m_funcImageList);
}
void CTestKTVDlg::SetRootDir(CString strBasePath)
{
	m_basePath = strBasePath;
}
CString CTestKTVDlg::GetRootDir()
{ 
	return m_basePath;
}

void CTestKTVDlg::OnBnClickedBtnPhoto()
{
	if(m_PhotoGraphMgr)
	{
		CRect rt;
		GetClientRect(&rt);
		ClientToScreen(&rt);
		CTime time = CTime::GetCurrentTime();
		CString strFilePath="",strFileName="";
		strFilePath.Format("%sImgClip\\Photo\\",GetRootDir());
		strFileName.Format("img_%04d%02d%02d_%02d%02d%02d",time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());
		m_PhotoGraphMgr->ClipRect(rt,strFilePath,strFileName);
		/*CRect rtClip(rt);
		CDC* pDeskDC = GetDC();
		m_PhotoGraphMgr->ClipRect(rtClip,this,strFilePath,strFileName,rt,pDeskDC);*/
	}
}

void CTestKTVDlg::OnDestroy()
{
	if(m_PhotoGraphMgr)
	{
		m_PhotoGraphMgr->DestroyPhotograph();
		delete m_PhotoGraphMgr;
		m_PhotoGraphMgr=NULL;
	}
	if(m_NPGuideMgr)
	{
		delete m_NPGuideMgr;
		m_NPGuideMgr=NULL;
	}
	if(m_pSkinConfContext)
	{
		delete m_pSkinConfContext;
		m_pSkinConfContext=NULL;
	}
	if(m_GiftSpecial)
	{
		delete m_GiftSpecial;
		m_GiftSpecial=NULL;
	}
	
	CDialog::OnDestroy();
}

void CTestKTVDlg::OnBnClickedBtnSpecialgift()
{
	STBigItemHead* sInfo =new STBigItemHead;
	sInfo->nFromUserID="13146149";
	sInfo->sFromUserName="49";
	sInfo->nToUserID="13146149";
	sInfo->sToUserName="49";
	sInfo->sUnitName="个";
	sInfo->sItemName="硕果累累";
	sInfo->sSendTime="03-25 14:07";
	sInfo->sPicName="1.gif";
	sInfo->sTip="";
	sInfo->nType=1;
	sInfo->nItemNum=1;
	static int nItemNum=0;
	nItemNum++;
	m_GiftSpecial->OnDisplayBroadCastItem(sInfo,1);
	//m_GiftSpecial->Write(sInfo);
}
void CTestKTVDlg::LoadEphPlugins()
{

	// 初始化插件 [7/27/2003 15:58 肖猛]
	EphBase::SetEphInstance(this);

#ifdef _DEBUG
	CString tmp = GetRootDir() + "plugins\\ded";
#else
	CString tmp = GetRootDir() + "plugins\\ed";
#endif

	//BOOL bLoadAllPlugs = AfxGetApp()->GetProfileInt("", "bAllPlug", 0);

	char buf[20] = {0};
	strcpy(buf, "*.dll");
	CString dir;

	CFileFind finder;
	dir.Format("%s%s", tmp, buf);
	BOOL cont = finder.FindFile(dir);

	int count=0;
	while (cont) 
	{
		cont = finder.FindNextFile();			
		HINSTANCE module = AfxLoadLibrary(finder.GetFilePath());
		TRACE("load library : %s \n", finder.GetFilePath());
		if (!module)
		{
#ifdef _DEBUG
			LPVOID lpMsgBuf;
			FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				GetLastError(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL 
				);
			tmp.Format("[%s]%s",finder.GetFilePath(),(LPCTSTR)lpMsgBuf);
			MessageBox(tmp, "Error", MB_OK | MB_ICONINFORMATION );
			LocalFree( lpMsgBuf );
#endif
			continue;
		}			


		GetPlugin getPlugin = (GetPlugin) GetProcAddress(module, "getPlugin");


		if (!getPlugin)
		{
			FreeLibrary(module);
			{	
				continue;
			}
		}
		EphPlugin* pPplugin = getPlugin();								


		//pPplugin->Destroy();

		pPplugin->SetModulHandle(module);
		pPplugin->Init();
		m_plugins.Add(pPplugin);
	}
	finder.Close();
}

void CTestKTVDlg::AddTab(char* pstrName,char* pstrToolTip,CBitmap *pbmImage,COLORREF crMask,CWnd *pWnd, WORD width, WORD height)
{
	if(pbmImage)
		m_tabImageList.Add(pbmImage,crMask);
	else
	{
		CBitmap bit;
		bit.LoadBitmap(IDB_PLGUICON);
		m_tabImageList.Add(&bit,RGB(255,255,255));
	}
	//CImageList* plist = m_tabCtrl.SetImageList(&m_tabImageList);
	//!!!从插件传递过来的pbmImage位图，背景色都变成RGB(0,0,0)？？？
	if (!m_cTab.AddTabPage(pWnd, pstrName,pbmImage,RGB(0,0,0),false))	
	{
		TRACE1("Failed to add %s.\n",pstrName);
		return;
	}
}

void CTestKTVDlg::itemDragged(int item, int toFolder)
{
	
	
}
void CTestKTVDlg::itemClicked(int item)
{
	CRect clientrc;
	GetWindowRect(clientrc);

	CPoint pt;
	::GetCursorPos(&pt);
}
