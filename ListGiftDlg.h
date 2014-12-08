#pragma once
#include "GiftListCtrl.h"
#include "resource.h"
#include <string>
using namespace std;

typedef struct tagGiftData
{
	int  nID;
	UINT nType;				//ֻ�Զ���������Ч

	string strName;
	string imgPath;
	string strToolTip;

	UINT nValue;
	UINT nWidth;
	UINT nHeight;
	UINT nIsDynamicGift;	//�Ƿ񶯸�����
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

struct ToolTipTextData
{
	int     nID;
	CString strGoodName;
	CString strGoodPath;
	CString strGoodToolTip;
	UINT    nGoodValue;
	CRect   rtItem;

	ToolTipTextData()
	{
		nID=0;
		strGoodName="";
		strGoodPath="";
		strGoodToolTip = "";
		nGoodValue=0;
	}
};

// CListGiftDlg �Ի���

class CListGiftDlg : public CDialog
{
	DECLARE_DYNAMIC(CListGiftDlg)

public:
	CListGiftDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CListGiftDlg();

// �Ի�������
	enum { IDD = IDD_DLG_GIFTLIST };

private:
	CGiftListCtrl			m_listGiftCtrl;
	CImageList				m_imgListGift;	// ����ͼƬ�б�
	bool                    m_bHasLoadGiftImg;
	GiftData				m_curSelGift;
	CString                 m_strLoadElementName;

	HANDLE m_LoadGiftThread;
	DWORD  m_LoadGiftThreadId;

private:
	bool ParseDynamicGiftTypeAndId(CString strFileName,UINT& uType,int& uId);
	void AddGift(GiftData configGiftData);
	static DWORD WINAPI LoadGiftThreadCB(LPVOID lpParam);

public:
	bool LoadGiftImg();           //��������
	void ClearAllList();
	pGiftData GetGiftDataByGoodId(int goodid);
	pGiftData GetGiftDataByItem(int nItem);
	void StartLoadGiftThread();  //���̼߳�������
	void SetLoadElementName(CString strElementName);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	virtual afx_msg LRESULT OnLoadGiftImgComplete(WPARAM wParam,LPARAM lParam);
	virtual afx_msg LRESULT OnUpdateToolTipText(WPARAM wParam,LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMClickListGift(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListGift(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();

};