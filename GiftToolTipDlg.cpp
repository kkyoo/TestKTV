// GiftToolTipDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GiftToolTipDlg.h"

#define  ID_TIMER_HIDE_DLG  10086
// CGiftToolTipDlg 对话框

IMPLEMENT_DYNAMIC(CGiftToolTipDlg, CDialog)
CGiftToolTipDlg::CGiftToolTipDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGiftToolTipDlg::IDD, pParent)
{
	m_curGoodId=-1;
	m_bTimeOn = false;
	m_ptLast = CPoint(0,0);
	m_curItemRect = CRect(0,0,0,0);
}

CGiftToolTipDlg::~CGiftToolTipDlg()
{
}

void CGiftToolTipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GIF, m_PicGif);
}


BEGIN_MESSAGE_MAP(CGiftToolTipDlg, CDialog)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CGiftToolTipDlg 消息处理程序

BOOL CGiftToolTipDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_staticGiftName.SubclassDlgItem(IDC_STATIC_GIFTNAME,this);
	m_staticGiftValue.SubclassDlgItem(IDC_STATIC_GIFT_VALUE,this);
	m_staticCouldBuy.SubclassDlgItem(IDC_STATIC_COULD_BUY,this);
	m_staticDesc.SubclassDlgItem(IDC_STATIC_DESC,this);
	m_staticDesc.SetTextColor(RGB(255,100,3));
	m_staticDesc.SetTextAlient(ALIAN_CENTER);
	m_staticDesc.SetWindowText("");
	m_staticDesc.SetVCenter(TRUE);
	m_staticGiftValue.SetTextColor(RGB(0,0,0));
	m_staticGiftName.SetTextColor(RGB(0,0,0));
	m_staticCouldBuy.SetTextColor(RGB(255,100,3));

	return TRUE; 
}

void CGiftToolTipDlg::UpdateGiftToopTip(int goodid,UINT goodValue,CString strValue,CString strGoodName,bool couldBuy,CString strGoodPath,CRect rt,CString desc)
{
	if(goodid!=m_curGoodId)
	{
		m_curGoodId = goodid;
		m_curItemRect = rt;
		int index=strGoodPath.ReverseFind('.');
		if(index>=0)
		{
			strGoodPath=strGoodPath.Left(index);
		}
		strGoodPath+=".gif";
		m_PicGif.UnLoad();
		m_PicGif.Load(strGoodPath);
		m_PicGif.ReSizePic(CSize(79,59));
		m_PicGif.Draw();

		CString str;
		str.Format("名称:%s",strGoodName);
		m_staticGiftName.SetWindowText(str);

		str.Format("价格:%u%s",goodValue,strValue);
		m_staticGiftValue.SetWindowText(str);

		if(couldBuy)
		{
			int num = 100000/goodValue;//CKTVDlg::getInstance().m_nMyMoney/goodValue;
			str.Format("够买%d个",num);
		}
		else
			str="买不起";
		m_staticCouldBuy.SetWindowText(str);
		m_staticDesc.SetWindowText(desc);

		CPoint pt;
		GetCursorPos(&pt);

		if(m_curItemRect.PtInRect(pt))
		{
			int posX,posY;
			posX = m_curItemRect.left;
			posY =m_curItemRect.bottom-20;
			CRect rcWnd;
			SystemParametersInfo(SPI_GETWORKAREA,0,(PVOID)&rcWnd,0);
			if(posX>rcWnd.right-190)
				posX=rcWnd.right-190;
			SetWindowPos(NULL,posX,posY,0,0,SWP_NOSIZE|SWP_NOZORDER);
			ShowWindow(SW_SHOWNA);
			Invalidate();
			KillTimer(ID_TIMER_HIDE_DLG);
			SetTimer(ID_TIMER_HIDE_DLG,100,NULL);
			m_bTimeOn = true;
		}
	}
}

void CGiftToolTipDlg::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == ID_TIMER_HIDE_DLG)
	{
		CPoint pt;
		GetCursorPos(&pt);

		if(!m_curItemRect.PtInRect(pt))
		{
			KillTimer(ID_TIMER_HIDE_DLG);
			m_bTimeOn = false;
			ShowWindow(SW_HIDE);
			m_curGoodId = -1;
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CGiftToolTipDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}

void CGiftToolTipDlg::OnDestroy()
{
	m_staticGiftName.UnsubclassWindow();
	m_staticGiftValue.UnsubclassWindow();
	m_staticCouldBuy.UnsubclassWindow();
	m_staticDesc.UnsubclassWindow();

	CDialog::OnDestroy();
}

void CGiftToolTipDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(GetDlgItem(IDC_STATIC_GIFTNAME)->GetSafeHwnd())
	{
		CRect rt;
		GetClientRect(&rt);
		//ClientToScreen(&rt);
		GetDlgItem(IDC_STATIC_GIFTNAME)->MoveWindow(rt.left+82,0,rt.right-82,15);
		GetDlgItem(IDC_STATIC_GIFT_VALUE)->MoveWindow(rt.left+82,18,rt.right-82,15);
		GetDlgItem(IDC_STATIC_COULD_BUY)->MoveWindow(rt.left+82,36,rt.right-82,15);
		GetDlgItem(IDC_STATIC_DESC)->MoveWindow(rt.left+5,rt.bottom-15,rt.right-10,15);
		GetDlgItem(IDC_STATIC_GIF)->MoveWindow(rt.left,rt.top,80,60);
	}
	
}
int CGiftToolTipDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;


	return 0;
}
