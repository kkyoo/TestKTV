// TestKTVDlg.h : 头文件
//

#pragma once
#include "IKtvLib.h"
#include "ExpensiveGiftCtrl.h"
#include "SkinConfContext.h"
#include "afxwin.h"
#include "resource.h"
#include "GiftsDialog.h"
#include "IAGuiSocketCore.h"

// CTestKTVDlg 对话框
class CTestKTVDlg : public CDialog,public IAGuiSocketCoreListen
{
// 构造
public:
	CTestKTVDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTKTV_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	CString              m_basePath;
	IPhotograph*         m_PhotoGraphMgr;
	INPGuide*            m_NPGuideMgr;

	CExpensiveGiftCtrl*   m_GiftSpecial;
	CSkinConfContext *   m_pSkinConfContext;

	//CListGiftDlg         m_listGiftDlg;
	CGiftsDialog         m_GiftsDlg;


private:
	void InitStruct();
	
public:
	CString GetRootDir();
	void SetRootDir(CString strBasePath);

	void UpdateUseList();

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnPhoto();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnSpecialgift();
	CComboBox m_cmbUser;
	CEdit m_editUse;
	afx_msg void OnBnClickedBtnAddUse();
	afx_msg void OnBnClickedBtnDelUse();
	afx_msg void OnBnClickedBtnListgift();
};
