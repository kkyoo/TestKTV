#pragma once
#include "GiftListCtrl.h"
#include "resource.h"
#include <string>
using namespace std;

typedef struct tagGiftData
{
	int  nID;
	UINT nType;				//只对动感礼物有效

	string strName;
	string imgPath;
	string strToolTip;

	UINT nValue;
	UINT nWidth;
	UINT nHeight;
	UINT nIsDynamicGift;	//是否动感礼物
	UINT nIsShowEffect;
	UINT nIsSpecialGift;
	UINT nIsFire;
	UINT nIsDoll;
	UINT nIsProp;

	tagGiftData()
	{
		memset(this, 0, sizeof(tagGiftData));
	}
}GiftData, *pGiftData;

// CListGiftDlg 对话框

class CListGiftDlg : public CDialog
{
	DECLARE_DYNAMIC(CListGiftDlg)

public:
	CListGiftDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CListGiftDlg();

// 对话框数据
	enum { IDD = IDD_DLG_GIFTLIST };

private:
	CGiftListCtrl			m_listGift;
	CImageList				m_imglstGift;	// 礼物图片列表
	bool                    m_bHasLoadGiftImg;
	GiftData				m_curSelGift;

private:
	bool ParseDynamicGiftTypeAndId(CString strFileName,UINT& uType,int& uId);
	void AddGift(GiftData configGiftData);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	virtual afx_msg LRESULT OnLoadGiftImgComplete(WPARAM wParam,LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMClickListGift(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListGift(NMHDR *pNMHDR, LRESULT *pResult);

public:
	bool LoadGiftImg();
	void ClearAllList();
};
