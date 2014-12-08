#pragma once
#include "resource.h"
#include "ColorStatic.h"
#include "PictureEx.h"



class CGiftToolTipDlg : public CDialog
{
	DECLARE_DYNAMIC(CGiftToolTipDlg)

public:
	CGiftToolTipDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGiftToolTipDlg();


// 对话框数据
	enum { IDD = IDD_DLG_GIFT_TOOLTIP };

private:
	CColorStatic	m_staticGiftName;
	CColorStatic	m_staticGiftValue;
	CColorStatic	m_staticCouldBuy;
	CColorStatic    m_staticDesc;

	int             m_curGoodId;
	bool            m_bTimeOn;
	CPoint          m_ptLast;
	CPictureEx      m_PicGif;
	CRect           m_curItemRect;
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	void UpdateGiftToopTip(int goodid,UINT goodValue,CString strValue,CString strGoodName,bool couldBuy,CString strGoodPath,CRect rt,CString desc="");

public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
