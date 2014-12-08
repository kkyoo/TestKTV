#pragma once

enum ALIAN_TYPE
{
	ALIAN_LEFT=0,
	ALIAN_CENTER,
	ALIAN_RIGHT,
};
// CColorStatic

class CColorStatic : public CStatic
{
	DECLARE_DYNAMIC(CColorStatic)

protected:
	bool		m_bUseSysFont;
	CFont		m_fontUserDefine;

	bool		m_bUseSysTextColor;
	COLORREF	m_clrTextColor;

	bool		m_bVCenter;				//是否垂直居中对齐
	ALIAN_TYPE     m_nHAlient;

	CSize       m_szText;
	BOOL        m_bAdjustTextSize;

public:
	CColorStatic();
	virtual ~CColorStatic();

	void SetTextFont(LOGFONT*  lf);
	void SetTextColor(COLORREF clrTextColor);
	void SetWindowText(LPCTSTR lpszString);
	void SetVCenter(bool bVCenter);
	void SetTextAlient(ALIAN_TYPE hAlient);

	void SetAdjustTextSize(BOOL bAdjust);
	CSize GetTextSize();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


