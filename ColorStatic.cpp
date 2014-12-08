// ColorStatic.cpp : 实现文件
//

#include "stdafx.h"
#include "ColorStatic.h"
#include ".\colorstatic.h"


// CColorStatic

IMPLEMENT_DYNAMIC(CColorStatic, CStatic)
CColorStatic::CColorStatic()
{
	m_bUseSysFont = true;
	m_bUseSysTextColor = true;
	m_bVCenter = false;
	m_nHAlient = ALIAN_LEFT;
	m_szText = CSize(0,0);
	m_bAdjustTextSize = FALSE;
}

CColorStatic::~CColorStatic()
{
}


BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CColorStatic 消息处理程序


void CColorStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()

	CRect rt;
	GetClientRect(&rt);

	dc.SetBkMode(TRANSPARENT);

	//设置字体
	CFont* pCurrentFont;
	if(m_bUseSysFont)
		pCurrentFont = GetFont(); 
	else
		pCurrentFont = &m_fontUserDefine;

	CFont *pOldFont = dc.SelectObject(pCurrentFont);

	//设置文本颜色
	if(!m_bUseSysTextColor)
		dc.SetTextColor(m_clrTextColor);

	CString str;
	GetWindowText(str);

	CSize  sizeText = dc.GetTextExtent(str);
	if(m_bAdjustTextSize)
	{
		SetWindowPos(NULL,0,0,sizeText.cx,sizeText.cy,SWP_NOMOVE|SWP_NOZORDER);
		GetClientRect(&rt);
	}

	CRgn clipRgn;
	clipRgn.CreateRectRgnIndirect(&rt);
	dc.SelectClipRgn(&clipRgn);

	int nY;
	if(m_bVCenter)
		nY = (rt.Height() - sizeText.cy)/2;
	else
		nY = rt.Height() - sizeText.cy;

	int nX = 0;
	if(m_nHAlient == ALIAN_CENTER)
		nX = (rt.Width() - sizeText.cx)/2;
	else if(m_nHAlient == ALIAN_RIGHT)
		nX = rt.Width() - sizeText.cx;
	
	dc.TextOut(nX,nY,str);

	dc.SelectObject(pOldFont);
}

void CColorStatic::SetTextFont(LOGFONT*  lf)
{
	m_fontUserDefine.DeleteObject();
	m_fontUserDefine.CreateFontIndirect(lf);
	m_bUseSysFont = false;

	Invalidate();
}

void CColorStatic::SetTextColor(COLORREF clrTextColor)
{
	m_clrTextColor = clrTextColor;
	m_bUseSysTextColor = false;

	Invalidate();
}

void CColorStatic::SetWindowText(LPCTSTR lpszString)
{
	CWnd::SetWindowText(lpszString);
	Invalidate();
}
void CColorStatic::SetVCenter(bool bVCenter)
{
	m_bVCenter = bVCenter;
	Invalidate();
}
void CColorStatic::SetTextAlient(ALIAN_TYPE hAlient)
{
	m_nHAlient = hAlient;
	Invalidate();
}

void CColorStatic::SetAdjustTextSize(BOOL bAdjust)
{
	m_bAdjustTextSize = bAdjust;
	Invalidate();
}
CSize CColorStatic::GetTextSize()
{
	CPaintDC dc(this); 
	dc.SetBkMode(TRANSPARENT);

	//设置字体
	CFont* pCurrentFont;
	if(m_bUseSysFont)
		pCurrentFont = GetFont(); 
	else
		pCurrentFont = &m_fontUserDefine;
	CFont *pOldFont = dc.SelectObject(pCurrentFont);

	CString str;
	GetWindowText(str);

	m_szText = dc.GetTextExtent(str);
	return m_szText;
}