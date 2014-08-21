/***************************************************************************
 *                                                                         *
 *   copyright            : (C) 2002 by EPH                                *
 ***************************************************************************/
#if !defined(AFX_EPHCTRL_H__EB8DBF55_4F3E_41D2_8726_515944D25531__INCLUDED_)
#define AFX_EPHCTRL_H__EB8DBF55_4F3E_41D2_8726_515944D25531__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EphCtrl.h : header file
//

#include "face/OutBarCtrl.h"
#include "libim/icqdbserialize.h"
#include "skin/SkinMagicLib.h"

class IcqContact;
class DBInStream;
class DBOutStream;
//class IcqSkin;

/////////////////////////////////////////////////////////////////////////////
// EphCtrl window

class EphCtrl : public OutBarCtrl, public DBSerialize {

// Construction
public:
	EphCtrl();

// Attributes
public:
	BOOL showOnlineOnly() {
		return onlineOnly;
	}
	void setShowOnlineOnly(BOOL b);
	IcqContact *contact(int item) {
		return contact(selFolder, item);
	}
	IcqContact *contact(int folder, int item);
	
	void GetFriendInfo(int groupIndex,int itemIndex,UINT& uin,UINT &pic,CString& nickName,bool& isOnline);

	//void setSkin(LPCTSTR skinName,BOOL flag = TRUE);
	
	void setMemberColor(COLORREF color)
	{
		memberColor = color;
		repaintInsideRect();
	}
	COLORREF getMemberColor() {return memberColor;}
// Operations
public:
	void update(uint32 uin, BOOL move = FALSE);
	void update(const uint32 uin, const int folder, BOOL move);
	void addContact(int folder, IcqContact *c);
	void insertContactFront(int folder, IcqContact *c);
	void insertContactBack(int folder, IcqContact *c);
	void removeContact(uint32 uin);
	void blinkText(uint32 uin);
	void blinkImage(uint32 uin, BOOL blink = TRUE);

	void load(DBInStream &in);
	void save(DBOutStream &out);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EphCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~EphCtrl();	
	IcqContact* findC(uint32 uin, const int folder, int &item);
	IcqContact* findC(uint32 uin, const int folderfrom, const int folderto, int &folder, int &item);
	IcqContact* getC(const int folder, const int item);
	void removeContact(uint32 uin, const int folderfrom, const int folderto);	

private:
	IcqContact* findC(uint32 uin, int &folder, int &item);
	
private:
	class BlinkInfo {
	public:
		BlinkInfo(uint32 uin, int frame) {
			this->uin = uin;
			this->frame = frame;
		}
		uint32 uin;
		int frame;
	};
	typedef list<BlinkInfo> BlinkList;

	virtual int getMaxItem();
	virtual void drawBackground(CDC *pDC, CRect &rc);
	virtual void drawItemText(CDC *pDC, int i, COLORREF color);
	virtual void drawFolder(CDC *pDC, int i, BOOL pressed, BOOL hilight);
	virtual void drawScroll(CDC *pDC, int i, BOOL pressed);
	
	BOOL findItem(uint32 uin, int &folder, int &item);
	void doBlinkText();
	void doBlinkImage();
	
	BOOL onlineOnly;
	BlinkList blinkTextList;
	BlinkList blinkImageList;
	//BOOL bSkin;
	//CString m_strSkinName;
	COLORREF	memberColor;
	
	HSKIN		m_hSkin;
	// Generated message map functions
protected:
	//{{AFX_MSG(EphCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	//afx_msg LRESULT OnSkinDataChanged(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EPHCTRL_H__EB8DBF55_4F3E_41D2_8726_515944D25531__INCLUDED_)
