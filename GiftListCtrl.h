#pragma once

#define WM_UPDATETOOLTIPTEXT WM_USER+4500

class CGiftListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CGiftListCtrl)

	CToolTipCtrl	m_toolTip;
public:
	CGiftListCtrl();
	virtual ~CGiftListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	void SetToolTipText(CString strToolTip);
protected:
	virtual void PreSubclassWindow();

public:
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
};
