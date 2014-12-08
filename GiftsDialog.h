#pragma once
#include "ListGiftDlg.h"
#include "GiftToolTipDlg.h"
#include <vector>
using namespace std;

#define  MAX_LIST_GIFT_PAGES  10

struct TabsData
{
	string strElementName;
	string strTabName;
	UINT   nIsFirstLoad;

	TabsData()
	{
		memset(this, 0, sizeof(TabsData));
	}
};

struct CountData
{
	string  strName;
	string  strValue;
	UINT    nIsShowEffect;

	CountData()
	{
		memset(this, 0, sizeof(CountData));
	}
};

// CGiftsDialog 对话框

class CGiftsDialog : public CDialog
{
	DECLARE_DYNAMIC(CGiftsDialog)

public:
	CGiftsDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGiftsDialog();

// 对话框数据
	enum { IDD = IDD_GIFTS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	virtual afx_msg LRESULT OnFirstLoadComplete(WPARAM wParam,LPARAM lParam);
	virtual afx_msg LRESULT OnUpdateToolTipText(WPARAM wParam,LPARAM lParam);

private:
	static DWORD WINAPI FirstLoadThreadCB(LPVOID lpParam);

private:
	CListGiftDlg  m_listGiftDlg[MAX_LIST_GIFT_PAGES];
	CFont         m_headerFont;		// 列表标题字体
	int           m_nCurShowPage;
	bool          m_bHasFirstLoad;
	bool          m_bHasSecondLoad;
	vector<TabsData>   m_vecTabsData;
	vector<CountData>  m_vecCountData;
	CGiftToolTipDlg    m_toolTipDlg;

	HANDLE m_FirstLoadThread;
	DWORD  m_FirstLoadThreadId;


public:
	void ShowPage(int nPage);
	bool FirstLoad();
	bool SecondLoad();
	void StartFirstLoadThread();

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
