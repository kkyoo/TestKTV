// TestKTVDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CTestKTVDlg �Ի���



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


// CTestKTVDlg ��Ϣ�������

BOOL CTestKTVDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

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
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTestKTVDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
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
		m_cmbUser.SetWindowText(_T("�������½ID"));
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
		m_cmbUser.SetWindowText(_T("�������½ID"));
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
	sInfo->sUnitName="��";
	sInfo->sItemName="˶������";
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
