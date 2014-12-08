// GiftsDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GiftsDialog.h"
#include "tinyxml/tinyxml.h"
#include <Shlwapi.h>

#define WM_FIRST_LOAD_COMPLETE     WM_USER+4509

// CGiftsDialog 对话框

IMPLEMENT_DYNAMIC(CGiftsDialog, CDialog)
CGiftsDialog::CGiftsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CGiftsDialog::IDD, pParent)
{
	m_nCurShowPage = -1;
	m_bHasFirstLoad = false;
	m_bHasSecondLoad= false;
	m_FirstLoadThread = NULL;
	m_FirstLoadThreadId=1;
}

CGiftsDialog::~CGiftsDialog()
{
}

void CGiftsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGiftsDialog, CDialog)
	ON_WM_SIZE()
	ON_MESSAGE(WM_FIRST_LOAD_COMPLETE,OnFirstLoadComplete)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnTcnSelchangeTab1)
	ON_MESSAGE(WM_UPDATETOOLTIPTEXT,OnUpdateToolTipText)
END_MESSAGE_MAP()


// CGiftsDialog 消息处理程序

void CGiftsDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CTabCtrl *pbt = (CTabCtrl*)GetDlgItem(IDC_TAB1);
	if(pbt && pbt->GetSafeHwnd())
	{
		CRect rtClient;
		GetClientRect(&rtClient);

		pbt->MoveWindow(0,0,rtClient.Width()-2,20);
		GetDlgItem(IDC_STATIC_GIFTCOUNT)->MoveWindow(6,24,36,20);
		GetDlgItem(IDC_CMB_COUNT)->MoveWindow(45,22,110,18);

		for (int i=0;i<MAX_LIST_GIFT_PAGES;i++)
		{
			m_listGiftDlg[i].MoveWindow(0,45,rtClient.Width(),250);
		}
	}
	
}

BOOL CGiftsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	LOGFONT font;
	GetFont()->GetLogFont(&font);
	font.lfHeight = 15;
	font.lfWeight = 500;
	m_headerFont.CreateFontIndirect(&font);
	// 标签控件
	CTabCtrl *pbt = (CTabCtrl*)GetDlgItem(IDC_TAB1);
	ASSERT( pbt );
	pbt->SetItemSize(CSize(46, 20));
	pbt->SetFont(&m_headerFont);

	for (int i=0;i<MAX_LIST_GIFT_PAGES;i++)
	{
		m_listGiftDlg[i].Create(IDD_DLG_GIFTLIST,this);
		m_listGiftDlg[i].ShowWindow(SW_HIDE);
	}

	m_toolTipDlg.Create(IDD_DLG_GIFT_TOOLTIP,this);
	m_toolTipDlg.MoveWindow(0,0,190,90);
	m_toolTipDlg.ShowWindow(SW_HIDE);

	return TRUE;
}

void CGiftsDialog::ShowPage(int nPage)
{
	if(nPage == m_nCurShowPage)
		return;

	if(nPage<0 || nPage>=MAX_LIST_GIFT_PAGES)
		return;

	if(m_nCurShowPage>=0)
		m_listGiftDlg[m_nCurShowPage].ShowWindow(SW_HIDE);

	m_nCurShowPage = nPage;
	m_listGiftDlg[m_nCurShowPage].ShowWindow(SW_SHOWNA);
}

LRESULT CGiftsDialog::OnUpdateToolTipText(WPARAM wParam,LPARAM lParam)
{
	ToolTipTextData* pTTTData = (ToolTipTextData*)wParam;
	if(!pTTTData)
		return 0L;

	if(m_toolTipDlg.GetSafeHwnd())
	{
		bool couldBuy = (pTTTData->nGoodValue<=100000);
		m_toolTipDlg.UpdateGiftToopTip(pTTTData->nID,pTTTData->nGoodValue,"K币",pTTTData->strGoodName,couldBuy,pTTTData->strGoodPath,pTTTData->rtItem, pTTTData->strGoodToolTip);
	}

	delete pTTTData;
	pTTTData=NULL;
	return 1;
}

void CGiftsDialog::StartFirstLoadThread()
{
	if( m_FirstLoadThread !=NULL)
	{
		WaitForSingleObject(m_FirstLoadThread,INFINITE);
		CloseHandle(m_FirstLoadThread);
		m_FirstLoadThread=NULL;
	}
	m_FirstLoadThread = CreateThread(NULL,0,FirstLoadThreadCB,this,0,&m_FirstLoadThreadId);
}

DWORD WINAPI CGiftsDialog::FirstLoadThreadCB(LPVOID lpParam)
{
	CGiftsDialog* self = (CGiftsDialog*) lpParam;
	self->FirstLoad();
	return 1;
}

LRESULT CGiftsDialog::OnFirstLoadComplete(WPARAM wParam,LPARAM lParam)
{

	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_TAB1);
	ASSERT(pTab);

	for(int i=0;i<m_vecTabsData.size() && i< MAX_LIST_GIFT_PAGES;i++)
	{
		TabsData configTabData = m_vecTabsData.at(i);
		pTab->InsertItem(i,configTabData.strTabName.c_str());
		m_listGiftDlg[i].SetLoadElementName(configTabData.strElementName.c_str());

		if(configTabData.nIsFirstLoad)
		{
			m_listGiftDlg[i].StartLoadGiftThread();
		}
	}

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_CMB_COUNT);
	if(m_vecCountData.size()>0)
	{
		for(int i=0;i<m_vecCountData.size();i++)
		{
			CountData configCountData = m_vecCountData.at(i);
			CString strName = configCountData.strName.c_str();
			CString strValue = configCountData.strValue.c_str();
			if(strName.Compare("")!=0)
				strValue=strValue+"("+strName+")";
			int index = pComboBox->AddString(strValue);
		}
		pComboBox->SetDroppedWidth(100);
		pComboBox->SetCurSel(0);
	}

	if(pTab->GetItemCount() > 0)
	{
		CRect rt;
		pTab->GetClientRect(&rt);
		pTab->SetItemSize(CSize((rt.Width())/pTab->GetItemCount(),20));
		ShowPage(0);	// 显示第一页：礼物
	}
	SecondLoad();
	return 1L;
}

bool CGiftsDialog::SecondLoad()
{
	if(!m_bHasFirstLoad)
		return false;
	if(m_bHasSecondLoad)
		return false;

	m_bHasSecondLoad=true;

	for(int i=0;i<m_vecTabsData.size() && i< MAX_LIST_GIFT_PAGES;i++)
	{
		TabsData configTabData = m_vecTabsData.at(i);
		if(!configTabData.nIsFirstLoad)
		{
			m_listGiftDlg[i].StartLoadGiftThread();
		}
	}

	return true;
}

bool CGiftsDialog::FirstLoad()
{
	if(m_bHasFirstLoad)
		return false;
	m_bHasFirstLoad=true;

	m_vecTabsData.clear();
	m_vecCountData.clear();

	// 读取XML数据
	bool bSucceeded = false;

	TCHAR fileName[MAX_PATH];
	if ( !GetModuleFileName(NULL, fileName, sizeof(fileName)) )
		return bSucceeded;

	TCHAR *p = strrchr(fileName, '\\');
	if (p)
		*(p + 1) = '\0';

	TCHAR goodspath[MAX_PATH];
	_tcscpy(goodspath, fileName);
	_tcscat(goodspath, _T("face\\AGuiGood.data"));

	if (!PathFileExists(goodspath))
		return bSucceeded;

	TiXmlDocument doc;
	CString path=goodspath;
	if( ! doc.LoadFile( path ) )
	{
		CString str=path+"error";
#ifdef _DEBUG
		MessageBox(str);
#endif
		return -1;
	}
	const TiXmlElement* root = doc.FirstChildElement();
	if( root )
	{
		bSucceeded = TRUE;
		int type = -1;	// 0为礼物分类,1为财富分类

		const TiXmlElement* it = NULL;
		const char* val1 = NULL;
		const char* val2 = NULL;
		const char* val3 = NULL;
		const char* val4 = NULL;
		const char* val5 = NULL;
		const char* val6 = NULL;
		const char* val7 = NULL;
		const char* val8 = NULL;
		const char* val9 = NULL;
		const char* val10 = NULL;

		const TiXmlElement* sub = root->FirstChildElement( "tabs" ); 
		if (sub)
		{
			it= sub->FirstChildElement();
			do{
				TabsData configTabData;
	
				val1 = it->Attribute("elementName");
				val2 = it->Attribute("tabName");
				val3 = it->Attribute("firstLoad");
				
				if(val1 && val2 && val3)
				{
					configTabData.strElementName = val1;
					configTabData.strTabName = val2;
					configTabData.nIsFirstLoad = atoi(val3);
					m_vecTabsData.push_back(configTabData);
				}
				
			}while((it=it->NextSiblingElement()));
		}
		
		sub = root->FirstChildElement( "count" );
		if (sub)
		{
			CString str,str1;
			it= sub->FirstChildElement();
			
			do{
				CountData configCountData;

				val1 = it->Attribute("name");
				val2 = it->Attribute("value");
				val3 = it->Attribute("showeffect");
				if(val1 && val2 && val3)
				{
					configCountData.strName = val1;
					configCountData.strValue = val2;
					configCountData.nIsShowEffect = atoi(val3);
					m_vecCountData.push_back(configCountData);
				}
			}while((it=it->NextSiblingElement()));
		}
	}

	PostMessage(WM_FIRST_LOAD_COMPLETE,NULL,NULL);
	return bSucceeded;
}


void CGiftsDialog::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_TAB1);
	ASSERT(pTab);
	ShowPage(pTab->GetCurSel());

	*pResult = 0;
}
