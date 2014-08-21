// TestKTVDlg.h : 头文件
//

#pragma once
#include "IKtvLib.h"
#include "ExpensiveGiftCtrl.h"
#include "SkinConfContext.h"
#include "pluginbase/EphBase.h"
#include "pluginbase/EphPlugin.h"
#include <afxtempl.h>
#include "face/AFCTabCtrl.h"
#include "EphCtrl.h"
#include "face/AFCResource.h"

#define EPHDLG_CLASSNAME "EPHDLG_CLASS"

typedef CTypedPtrArray<CPtrArray,EphPlugin*> EphPluginArray;
// CTestKTVDlg 对话框
class CTestKTVDlg : public CDialog
	,public EphBase
	,public OutBarListener
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
	EphPluginArray m_plugins;

private:
	void InitStruct();
	
public:
	CString GetRootDir();
	void SetRootDir(CString strBasePath);

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
	void LoadEphPlugins();



	 CImageList m_funcImageList;

	CImageList		m_tabImageList;
	CNewGameClass	m_cTab;
	EphCtrl outbarCtrl;
	//ephbase 插件虚函数

	void virtual AddTab(char* pstrName,char* pstrToolTip,CBitmap *icon,COLORREF crMask,CWnd *pWnd, WORD width = 0, WORD height = 0);
	void virtual AddMainMenuItem(char* pstrMenuText,EphPlugin* pPlugin,CBitmap *icon,COLORREF crMask){};
	void virtual AddFriendRightMenuItem(char* pstrMenuText,EphPlugin* pPlugin,bool needFriendOnline,CBitmap *icon,COLORREF crMask){};
	//void virtual AddFriendLeftMenuItem(char* pstrMenuText,EphPlugin* pPlugin,bool needFriendOnline,CBitmap *icon,COLORREF crMask){};
	void virtual sendPluginMessage(EphPlugin* pPlugin,UINT destUin,const char* pText){};

	//属性设置和获取接口
	virtual BOOL GetProperty(const char* type,CString& val){return FALSE;};
	virtual BOOL SetProperty(const char* type,CString val){return FALSE;};

	//执行主程序命令接口
	virtual BOOL Exec(const char* cmd,const char* param){return FALSE;};

	virtual EphPlugin* FindEphPlugin(const char* UID){return NULL;};
	virtual CWnd*  GetEphTab(){return NULL;};
	virtual int GetUIConfig(int nPlugin){return 0;};

	void SendMessageToPlugins(UINT message,WPARAM wParam,LPARAM lParam){};


	virtual void renamed(int obj, int index, const char *text){};
	virtual void itemDragged(int item, int toFolder);
	virtual void selFolderChanged(int folder){};
	virtual void itemClicked(int item);
	virtual void itemDoubleClicked(int item){} ;
	virtual void funcClicked(int item,int no){} ;
	virtual void funcDoubleClicked(int item,int no){} ;
	virtual void rightButtonDown(int obj, int index) {};
	virtual void itemOver(int item) {};
};
