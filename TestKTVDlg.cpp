// TestKTVDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestKTV.h"
#include "TestKTVDlg.h"
#include ".\testktvdlg.h"
#include "icqconfig.h"
#include "icqtypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
	DDX_Control(pDX, IDC_COMBO_UINLIST, m_cmbUser);
	DDX_Control(pDX, IDC_EDIT_USE, m_editUse);
}

BEGIN_MESSAGE_MAP(CTestKTVDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_PHOTO, OnBnClickedBtnPhoto)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_SPECIALGIFT, OnBnClickedBtnSpecialgift)
	ON_BN_CLICKED(IDC_BTN_ADD_USE, OnBnClickedBtnAddUse)
	ON_BN_CLICKED(IDC_BTN_DEL_USE, OnBnClickedBtnDelUse)
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


	m_cmbUser.LimitText(31);
	CString dir = GetRootDir() + "aguiConfig\\";
	IcqConfig::setDir(dir);

	UpdateUseList();
	
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

void CTestKTVDlg::UpdateUseList()
{
	m_cmbUser.ResetContent();
	UseList l;
	IcqConfig::getAllUsers(l);
	if (l.size() <= 0)
	{
		m_cmbUser.SetWindowText(_T("请输入登陆ID"));
	}
	else 
	{
		UseList::iterator it;
		for (it = l.begin(); it != l.end(); ++it)
		{
			CString str;
			str.Format("%s", (*it).strUin.c_str());
			m_cmbUser.AddString(str);
		}
		m_cmbUser.SetCurSel(0);
	}

	/*m_cmbUser.ResetContent();
	UinList l;
	IcqConfig::getAllUsers(l);
	if (l.size() <= 0)
	{
		m_cmbUser.SetWindowText(_T("请输入登陆ID"));
	}
	else 
	{
		UinList::iterator it;
		for (it = l.begin(); it != l.end(); ++it)
		{
			CString str;
			str.Format("%lu", *it);
			m_cmbUser.AddString(str);
		}
		m_cmbUser.SetCurSel(0);
	}*/
}

void CTestKTVDlg::InitStruct()
{
	m_basePath = _T("");
	m_PhotoGraphMgr = NULL;
	m_NPGuideMgr = NULL;
	m_pSkinConfContext=NULL;
	m_GiftSpecial=NULL;
	
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
void CTestKTVDlg::OnBnClickedBtnAddUse()
{
	CString strUse ="";
	m_editUse.GetWindowText(strUse);

	/*uint32 uin = atoi(strUse);
	if (uin>0)
	{
		IcqConfig::addUser(uin);
		UpdateUseList();
	}*/

	if(strUse.Compare("")!=0)
	{
		IcqConfig::addUser(strUse);
		UpdateUseList();
	}
}

void CTestKTVDlg::OnBnClickedBtnDelUse()
{
	CString strUse ="";
	m_editUse.GetWindowText(strUse);

	/*uint32 uin = atoi(strUse);
	if (uin>0)
	{
		IcqConfig::delUser(uin);
		UpdateUseList();
	}*/

	if(strUse.Compare("")!=0)
	{
		IcqConfig::delUser(strUse);
		UpdateUseList();
	}
}
