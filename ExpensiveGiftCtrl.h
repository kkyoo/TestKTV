// ExpensiveGiftCtrl.h: interface for the CExpensiveGiftCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXPENSIVEGIFTCTRL_H__1881DD50_152A_4CEF_8794_F239105CBF65__INCLUDED_)
#define AFX_EXPENSIVEGIFTCTRL_H__1881DD50_152A_4CEF_8794_F239105CBF65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../MVUILib/PictureExEx.h"
/*
struct tgGifInfo
{
	CPictureExEx *pPicture;
	int nThreadCurrent;   //线程当前
	int nMAx;
	int nNum;
	CRect Rect;
	HDC hMenDC;
	HBITMAP  hMemBM;
	HBITMAP  hOldBM;
};
*/
#include "PictureExEx.h"
#include "SkinConfContext.h"

struct STBigItemHead
{
	CString nFromUserID;
	CString sFromUserName;
	CString nToUserID;
	CString sToUserName;
	CString sUnitName;
	CString sItemName;
	CString sSendTime;
	CString sPicName;
	CString sTip;
	int nType;
	int nItemNum;
};
struct TRoadItemInfo
{
	int nItemNumber;
	STBigItemHead* pContent;
};
class  CExpensiveGiftCtrl  : public CWnd
{
public:
	CExpensiveGiftCtrl();
	virtual ~CExpensiveGiftCtrl();

protected:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();	
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();

private:
	bool m_bTrackLeave; // 鼠标离开标识

	BOOL m_bWaitRoadItem,m_bUpRoadItem,m_bFirstUpRoadItem;
	BOOL OnUpNewRoadItem();
	BOOL OnWaitNewRoadItem(BOOL bUpNow);
	CPtrList       *m_pRoadItem;	//跑道道具链表
	STBigItemHead* m_pNowInfo;
	int  m_nPos,m_nStep;
	CFont m_font[2];
public:
	CRect m_rect;
	void CallJScript(CString param1,CString param2);	//外调接口：加速
	void Clear();										//清空
	void Write(STBigItemHead* sInfo);					//写入
    void SetSkinConfContext(CSkinConfContext *pSkinConfContext);
	void OnDisplayBroadCastItem(STBigItemHead* pContent,int nItemNumber);	//新奢侈道具
	
	DECLARE_MESSAGE_MAP()
private:
	CToolTipCtrl m_toolTip;
	CString sNowInfo;
	int m_nAnimate;
	BOOL m_bAddSpeed;
	BOOL m_bExitThread;
	BOOL m_bPicIsOK;
	int m_nItemNumber;
	void Animate();
	void  InsertGif( LPCTSTR szFilename,INT nNum);
	CString m_sLocalPath;
	bool m_invalid;
	int m_cx;				//图片的长
	int m_cy;				//图片的高

	CPictureExEx *m_pPicture;
	CSkinConfContext *m_pSkinConfContext;

	int m_nThreadCurrent;   //当前帧数
	HDC m_hMenDC;           //当前图片设备
	HDC m_hPaintMenDC;      //
	HDC m_hFontDC;
	HBITMAP m_hPaintBM;
	HBITMAP  m_hMemBM;      //一张动态GIF 
	bool DrawGif(CDC *pMenDC,int &nSize,int &nPos);         //绘图
	bool DrawTextEx(CDC *pMenDC,CDC *pFontDC,CString sContent,CFont *font,COLORREF fontRGB,int nHeight,int &nSize,int &nPos);  //nHeight字体位置
	
};

#endif // !defined(AFX_EXPENSIVEGIFTCTRL_H__1881DD50_152A_4CEF_8794_F239105CBF65__INCLUDED_)
