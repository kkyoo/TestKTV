// GiftListCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GiftListCtrl.h"

// CGiftListCtrl

IMPLEMENT_DYNAMIC(CGiftListCtrl, CListCtrl)
CGiftListCtrl::CGiftListCtrl()
{
}

CGiftListCtrl::~CGiftListCtrl()
{
}


BEGIN_MESSAGE_MAP(CGiftListCtrl, CListCtrl)
	ON_WM_MOUSEMOVE()
	ON_WM_MEASUREITEM()
END_MESSAGE_MAP()


// CGiftListCtrl ��Ϣ�������


BOOL CGiftListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	m_toolTip.RelayEvent(pMsg);

	return CListCtrl::PreTranslateMessage(pMsg);
}

void CGiftListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	LVHITTESTINFO info;
	info.flags = LVHT_ONITEMLABEL;
	info.pt = point;
	int subitem = SubItemHitTest(&info);
	if (info.iItem >=0 && info.iItem < GetItemCount())
	{
		if ( subitem != -1) 
		{
			CRect rt;
			GetItemRect(info.iItem,&rt,LVIR_ICON);
			if(!rt.PtInRect(point))
			{
				int belowItem = GetNextItem(info.iItem,LVNI_BELOW);
				if(belowItem>0 && belowItem<GetItemCount())
				{
					::SendMessage(GetParent()->GetSafeHwnd(),WM_UPDATETOOLTIPTEXT,(WPARAM)belowItem,(LPARAM)GetSafeHwnd());
					m_toolTip.Activate(TRUE);
				}
			}
			else
			{				
				::SendMessage(GetParent()->GetSafeHwnd(),WM_UPDATETOOLTIPTEXT,(WPARAM)info.iItem,(LPARAM)GetSafeHwnd());
				m_toolTip.Activate(TRUE);
			}
		}
		else
			m_toolTip.Activate(FALSE);
	}
	else
		m_toolTip.Activate(FALSE);

	CListCtrl::OnMouseMove(nFlags, point);
}

void CGiftListCtrl::SetToolTipText(CString strToolTip)
{
	if(m_toolTip.GetSafeHwnd())
	{
		m_toolTip.UpdateTipText(strToolTip,this);
	}
}

void CGiftListCtrl::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	if(!m_toolTip.m_hWnd)
	{
		m_toolTip.Create(this);
		m_toolTip.Activate(FALSE);
		m_toolTip.AddTool(this, _T(""));
		m_toolTip.SetMaxTipWidth(260); // Set the max tip width to 260 characters
		m_toolTip.SetDelayTime(TTDT_AUTOPOP, 3000); // Set auto pop delay time to 3s
		m_toolTip.SetDelayTime(TTDT_INITIAL, 100);
		m_toolTip.SetDelayTime(TTDT_RESHOW, 100);
	}

	CListCtrl::PreSubclassWindow();
}
void CGiftListCtrl::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//lpMeasureItemStruct->itemHeight = 100;
	CListCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}
