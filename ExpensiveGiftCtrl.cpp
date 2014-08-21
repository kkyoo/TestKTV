// ExpensiveGiftCtrl.cpp: implementation of the CExpensiveGiftCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExpensiveGiftCtrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define ERRORRETURN(rpc,result)	if(!rpc) goto result;



CExpensiveGiftCtrl::CExpensiveGiftCtrl()
{
    m_pSkinConfContext=NULL;
	m_pRoadItem = new CPtrList;
	m_bWaitRoadItem=FALSE;
	m_bUpRoadItem=FALSE;
	m_bFirstUpRoadItem=FALSE;

	char path[500] = {0};
	::GetModuleFileName(NULL, path, 499);
	CString sTmpPath = path;
	m_sLocalPath = sTmpPath.Mid(0, sTmpPath.ReverseFind('\\') + 1);

	m_pNowInfo=NULL;
	m_nStep=5;
	m_nItemNumber=0;
    m_pPicture=NULL;
    m_hMenDC=NULL;           //��ǰͼƬ�豸
	m_hMemBM=NULL;          
	m_nPos=0;
	m_bAddSpeed=FALSE;		//�Ǽ���״̬	
	m_bPicIsOK=FALSE;		//Ĭ��ûͼƬ
	m_nAnimate=0;


	m_bTrackLeave = false;
	m_font[0].CreateFont(40, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, 
			GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("����"));
	m_font[1].CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, 
			GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("����"));

} 

CExpensiveGiftCtrl::~CExpensiveGiftCtrl()
{
    m_font[0].DeleteObject();
	m_font[1].DeleteObject();
	Clear();
	if(m_pRoadItem != NULL)
	{
		while(m_pRoadItem->GetCount() > 0)
		{
             TRoadItemInfo *pInfo=(TRoadItemInfo *)m_pRoadItem->RemoveHead();
			 if(pInfo != NULL)
			 {
				 delete pInfo->pContent;
				 delete pInfo;
				 pInfo=NULL;
			 }
		}
		delete m_pRoadItem;
		m_pRoadItem = NULL;
	}
	if(m_pNowInfo != NULL)
	{
		delete m_pNowInfo;
		m_pNowInfo=NULL;
	}
}
BEGIN_MESSAGE_MAP(CExpensiveGiftCtrl, CWnd)
//{{AFX_MSG_MAP(CEasyChatGuestDlg)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CExpensiveGiftCtrl::PreTranslateMessage(MSG* pMsg)   
{
	if (m_toolTip.m_hWnd != NULL)
		m_toolTip.RelayEvent(pMsg);
	return CWnd::PreTranslateMessage(pMsg);
}

void CExpensiveGiftCtrl::SetSkinConfContext(CSkinConfContext *pSkinConfContext)
{
   m_pSkinConfContext=pSkinConfContext;
}
void CExpensiveGiftCtrl::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 1)
	{
		if(!m_bTrackLeave)
		   m_nPos+=m_nStep;                //��ʼ���Ӧ����
       	CRect ClientRect;
    	GetClientRect(&ClientRect);
		if(m_nPos < ClientRect.Width()+m_cx*m_nItemNumber)
		{
			if(m_nAnimate==2)
			{
			   m_nAnimate=0;
			   Animate();
			}
			else
				m_nAnimate++;
		}
		Invalidate();                   //ȫ����ˢ
	}
	if (nIDEvent == 1300)
	{
		KillTimer(1300);
		OnUpNewRoadItem();
	}
	if (nIDEvent == 1301)
	{
		KillTimer(1301);
		m_bWaitRoadItem=FALSE;
		if(m_pRoadItem->GetCount()!=0)	//�����ж���,ֱ������
			OnWaitNewRoadItem(TRUE);
	}
	CWnd::OnTimer(nIDEvent);

}


void CExpensiveGiftCtrl::OnDisplayBroadCastItem(STBigItemHead* pContent,int nItemNumber)
{
//	return;
	TRoadItemInfo *pInfo = new TRoadItemInfo;
	pInfo->nItemNumber = nItemNumber;
	pInfo->pContent = pContent;
	m_pRoadItem->AddTail(pInfo);
	
	if(nItemNumber>m_nItemNumber)		//���ϱ���
		OnWaitNewRoadItem(TRUE);
	else								
		OnWaitNewRoadItem(FALSE);
}

BOOL CExpensiveGiftCtrl::OnWaitNewRoadItem(BOOL bUpNow)		    //׼������һ���ܵ�����
{
	if(!m_bFirstUpRoadItem)				//��һ��ֱ����ʾ
	{
		OnUpNewRoadItem();
		return 1;
	}
	if(bUpNow || !m_bWaitRoadItem)		//ֱ�Ӽ���
	{		
		//	m_browserPubic.Write("333ֱ�Ӽ���<br>");
		CallJScript("AddSpead","");
		
		/*
		if(m_nItemNumber < 5)
			SetTimer(1300,2000,NULL);
		if(m_nItemNumber < 20 && m_nItemNumber>=5)
			SetTimer(1300,4000,NULL);	
		if(m_nItemNumber < 40 && m_nItemNumber>=20)
			SetTimer(1300,6000,NULL);
		if(m_nItemNumber >= 40)
			SetTimer(1300,8000,NULL);
		*/
	}
	else								//�ȴ�ǰһ��ˢ��
	{
		//��ʱ�����滻
		m_bWaitRoadItem=TRUE;	
		
		if(m_nItemNumber < 5)
			SetTimer(1301,5000,NULL);		
		if(m_nItemNumber < 20 && m_nItemNumber>=5)
			SetTimer(1301,10000,NULL);		
		if(m_nItemNumber < 40 && m_nItemNumber>=20)
			SetTimer(1301,15000,NULL);
		if(m_nItemNumber >= 40)
			SetTimer(1301,30000,NULL);
	}
	return TRUE;
}

BOOL CExpensiveGiftCtrl::OnUpNewRoadItem()		    //����һ���ܵ�����
{

	m_bFirstUpRoadItem=TRUE;
	STBigItemHead* pContent;
    TRoadItemInfo *pInfo=NULL;
	if(m_pRoadItem->GetCount() > 0)
		pInfo = (TRoadItemInfo *)m_pRoadItem->RemoveHead();
	if(pInfo != NULL)			
	{
		m_nItemNumber=pInfo->nItemNumber;
		pContent=pInfo->pContent;
		if(pContent==NULL)
			return 0;
		delete pInfo;
		pInfo = NULL;
	}
	else
		return 0; 
	

	Write(pContent);
	
	
	//��ʱ�����滻  
	m_bWaitRoadItem=TRUE;	
	if(m_nItemNumber < 5)
		SetTimer(1301,5000,NULL);		
	if(m_nItemNumber < 20 && m_nItemNumber>=5)
		SetTimer(1301,10000,NULL);		
	if(m_nItemNumber < 40 && m_nItemNumber>=20)
		SetTimer(1301,15000,NULL);
	if(m_nItemNumber >= 40)
		SetTimer(1301,30000,NULL);
	
	return TRUE;
}

void CExpensiveGiftCtrl::Clear()
{

	KillTimer(1);
	if(m_hMenDC != NULL)  //�ͷ�DC�豸
	{
		DeleteObject(m_hMemBM);
		DeleteDC(m_hMenDC);
		m_hMenDC=NULL;           //��ǰͼƬ�豸
		m_hMemBM=NULL;          
	}
}


void CExpensiveGiftCtrl::Write(STBigItemHead* sInfo)
{
	if(m_pNowInfo!=NULL)
	{
		delete m_pNowInfo;
		m_pNowInfo=NULL;
	}
	if(sInfo!=NULL)
	{
		m_nPos=0;  //��ͷ��ʼ
		m_nStep=5;
		m_pNowInfo=sInfo;
	}	
	CString sTmp;
	if (sInfo->nType == 1) // ���԰���
		sTmp.Format("itembox\\%s",sInfo->sPicName);
	else
		sTmp.Format("ITEM\\%s",sInfo->sPicName);
	CString sLocalPath =m_sLocalPath  + sTmp;
	m_nItemNumber = sInfo->nItemNum;
	InsertGif(sLocalPath,1);

	sNowInfo="";
	/*
	for(int i=0;i<nItemNumber;i++)
	{
		sNowInfo+="<img  align=absmiddle src='"+m_sAppPath+"\\item\\"+pSItem->sBigPicName+"' height='57'> ";
	}
	*/
	sTmp = "";
	CString sTemp = "���ĵ��͸�";
	if (m_pNowInfo->nFromUserID != "0")
	{
		sTmp.Format("(%s)",m_pNowInfo->nFromUserID);
		sTemp = "�͸�";
	}
	sNowInfo.Format("%s%s %s%s(%s)\r\n %d%s%s (%s)",//%s  
	m_pNowInfo->sFromUserName,sTmp,sTemp,m_pNowInfo->sToUserName,m_pNowInfo->nToUserID,m_pNowInfo->nItemNum,m_pNowInfo->sUnitName,m_pNowInfo->sItemName,m_pNowInfo->sSendTime);

}


void CExpensiveGiftCtrl::CallJScript(CString param1,CString param2)	//����ӿڣ�����
{
	//AfxMessageBox("����");
	m_nStep=20;
	m_bAddSpeed=TRUE;
}


void CExpensiveGiftCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{ 
	if ( !m_bTrackLeave) {
		//Track the mouse leave event
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = GetSafeHwnd();
		tme.dwFlags = TME_LEAVE;
		_TrackMouseEvent(&tme);
		m_bTrackLeave = true;

		if (m_toolTip.m_hWnd == NULL)
		{
			m_toolTip.Create(this, TTS_ALWAYSTIP);
			m_toolTip.AddTool(this, sNowInfo);
			m_toolTip.SetMaxTipWidth(1000);
			//m_toolTip.Activate(FALSE);
		}
		else
		m_toolTip.UpdateTipText(sNowInfo,this);
		//::PostMessage(GetParent()->GetSafeHwnd(),WM_SCROLL_TIP,1,(LPARAM)(LPCTSTR)sNowInfo);

	}
	
	CWnd::OnMouseMove(nFlags, point);
}

LRESULT CExpensiveGiftCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	//::PostMessage(GetParent()->GetSafeHwnd(),WM_SCROLL_TIP,0,0);
	m_bTrackLeave = false;
	return 0;
}

void CExpensiveGiftCtrl::PreSubclassWindow() 
{ 
	ModifyStyle(0, SS_NOTIFY, TRUE);
	CWnd::PreSubclassWindow();
}

void CExpensiveGiftCtrl::OnPaint() 
{
	CPaintDC dc(this);

	CRect ClientRect;
	GetClientRect(&ClientRect);
	
	CDC MemDC;
	CBitmap MemBitMap;
	BOOL  bOverOut = FALSE;
	
	//����һ����dc���ݵ��ڴ��ڴ��豸����
	MemDC.CreateCompatibleDC(&dc);
	//����һ����dc���ݵ�λͼ����СΪ�����ͻ���
	MemBitMap.CreateCompatibleBitmap(&dc,ClientRect.Width(),ClientRect.Height());
	CBitmap *pOldBitMap = MemDC.SelectObject(&MemBitMap);
	MemDC.FillSolidRect(&ClientRect,RGB(232,243,255));

	CDC FontMenDC;  //����������
	CBitmap FontMenBM;
	FontMenDC.CreateCompatibleDC(&MemDC);
	FontMenBM.CreateCompatibleBitmap(&MemDC,ClientRect.Width(),ClientRect.Height());
	CBitmap *pOldFontBM=FontMenDC.SelectObject(&FontMenBM);
	FontMenDC.FillSolidRect(&ClientRect,RGB(232,243,255));
	if(m_pNowInfo!=NULL)
	{
		CString sTmp;
		int nSize=0;   //���㳤��
		int nPos=m_nPos; //���������㳤��
		///////////////////////////////////////////��ͼ/////////////////////////////////
	    ERRORRETURN(DrawGif(&MemDC,nSize,nPos),Result);

		
		ERRORRETURN(DrawTextEx(&MemDC,&FontMenDC,m_pNowInfo->sFromUserName,&m_font[0],RGB(232,7,108),10,nSize,nPos),Result);
		if (m_pNowInfo->nFromUserID != "0")	
		{
			sTmp.Format("(%s)",m_pNowInfo->nFromUserID);		
			ERRORRETURN(DrawTextEx(&MemDC,&FontMenDC,sTmp,&m_font[1],RGB(80,0 ,36),26,nSize,nPos),Result);
			sTmp="�͸�";
		}
		else
		{
			ERRORRETURN(DrawTextEx(&MemDC,&FontMenDC,sTmp,&m_font[1],RGB(80,0 ,36),26,nSize,nPos),Result);
			sTmp="���ĵ��͸�";
		}
		ERRORRETURN(DrawTextEx(&MemDC,&FontMenDC,sTmp,&m_font[0],RGB(80,0 ,36),10,nSize,nPos),Result);
		ERRORRETURN(DrawTextEx(&MemDC,&FontMenDC,m_pNowInfo->sToUserName,&m_font[0],RGB(232,7,108),10,nSize,nPos),Result);
		sTmp.Format("(%s)",m_pNowInfo->nToUserID);
		ERRORRETURN(DrawTextEx(&MemDC,&FontMenDC,sTmp,&m_font[1],RGB(80,0,36),26,nSize,nPos),Result);
		sTmp.Format("%d",m_pNowInfo->nItemNum);
		ERRORRETURN(DrawTextEx(&MemDC,&FontMenDC,sTmp,&m_font[0],RGB(80,0,36),10,nSize,nPos),Result);
		sTmp.Format("%s",m_pNowInfo->sUnitName);
		ERRORRETURN(DrawTextEx(&MemDC,&FontMenDC,sTmp,&m_font[0],RGB(80,0,36),10,nSize,nPos),Result);
		sTmp.Format("%s",m_pNowInfo->sItemName);
		ERRORRETURN(DrawTextEx(&MemDC,&FontMenDC,sTmp,&m_font[0],RGB(255,0,0),10,nSize,nPos),Result);
		sTmp.Format("%s",m_pNowInfo->sTip); // ����
		ERRORRETURN(DrawTextEx(&MemDC,&FontMenDC,sTmp,&m_font[1],RGB(255,0,0),26,nSize,nPos),Result);
		sTmp.Format(" (%s)",m_pNowInfo->sSendTime);
		ERRORRETURN(DrawTextEx(&MemDC,&FontMenDC,sTmp,&m_font[1],RGB(80,0,36),26,nSize,nPos),Result);
		if(nPos > ClientRect.Width())
		{
			m_nPos=0;
			bOverOut=TRUE;
		}
	}
Result:
#ifdef KTV
	   MemDC.Draw3dRect(ClientRect,RGB(185,213,255), RGB(185,213,255));
#else
	   MemDC.Draw3dRect(ClientRect,RGB(95,154,214), RGB(95,154,214));
#endif
	dc.BitBlt(0,0,ClientRect.Width(),ClientRect.Height(),&MemDC, 0, 0,SRCCOPY);

	FontMenDC.SelectObject(pOldFontBM);
   	FontMenBM.DeleteObject();
   	FontMenDC.DeleteDC();


	MemDC.SelectObject(pOldBitMap);
	MemBitMap.DeleteObject();
	MemDC.DeleteDC(); 

	
	if(m_bAddSpeed && bOverOut)
		OnUpNewRoadItem();	
}
bool CExpensiveGiftCtrl::DrawGif(CDC *pMenDC,int &nSize,int &nPos)         //��ͼ
{
	if(!m_bPicIsOK)
		return FALSE;

    CRect ClientRect;
	GetClientRect(ClientRect);
	int nHeight=(ClientRect.Height()-m_cy)/2;
   	if(m_nPos < ClientRect.Width()+m_cx*m_nItemNumber)  //��ͼ��Ҫ����
		{
			//������Ҫ�漸��ͼ
			int nLeftLen=ClientRect.Width()-m_nPos;
			if(nLeftLen>=0)  //ͼƬ��û����
			{
				nSize=nLeftLen;
				for(int i=0;i<m_nItemNumber;i++)
				{
					
					::StretchBlt(pMenDC->m_hDC, nSize,nHeight,m_cx, m_cy,m_hMenDC, 0, 0,m_pPicture->m_PictureSize.cx, m_pPicture->m_PictureSize.cy,SRCCOPY);
					//nSize=m_cx;
					nSize=nSize+m_cx+2;
					if(nSize >= ClientRect.Width())  //ͼ̫���� ��ʣ�ಿ��
					{
						int nCX=nSize-ClientRect.Width();   //ʣ��ͼ�ĳ���
						if(nCX > 0)
							::StretchBlt(pMenDC->m_hDC, nSize,nHeight,nCX, m_cy,m_hMenDC, 0, 0,nCX, m_pPicture->m_PictureSize.cy,SRCCOPY);
						return false;
						
					}
				}
			}
			else  //ͼƬ��һЩ��û�������򶼻�û����
			{
				int nResultLen=m_cx*m_nItemNumber + nLeftLen;
				if(nResultLen > 0)  //����ͼ��Ҫ�ػ�
				{
					int nCX = nResultLen%m_cx;  //���µ�ͼƬ����
					if(nCX > 0)
					{
						::StretchBlt(pMenDC->m_hDC, nSize,nHeight,nCX, m_cy,m_hMenDC,m_cx-nCX, 0,nCX, m_pPicture->m_PictureSize.cy,SRCCOPY);
						nSize=nSize+nCX+2;
					}
					for(int i=0;i<(nResultLen/m_cx);i++)  //
					{
		
						::StretchBlt(pMenDC->m_hDC, nSize,nHeight,m_cx, m_cy,m_hMenDC, 0, 0,m_pPicture->m_PictureSize.cx, m_pPicture->m_PictureSize.cy,SRCCOPY);
					//	nSize+=m_cx;
						nSize=nSize+m_cx+2;
						if(nSize >= ClientRect.Width())  //ͼ̫���� ��ʣ�ಿ��
						{
							int nCX=nSize-ClientRect.Width();   //ʣ��ͼ�ĳ���
							if(nCX > 0)
								::StretchBlt(pMenDC->m_hDC, nSize,nHeight,nCX, m_cy,m_hMenDC, 0, 0,nCX, m_pPicture->m_PictureSize.cy,SRCCOPY);
							return false;
							
						}
					}
				}
			}
		}
		nPos=nPos-m_cx*m_nItemNumber;  
		if( nPos <= 0)
			return false; 
        return true;
}
bool CExpensiveGiftCtrl::DrawTextEx(CDC *pMenDC,CDC *pFontDC,CString sContent,CFont *font,COLORREF fontRGB,int nHeight,int &nSize,int &nPos)  //nHeight����λ��
{
		CRect ClientRect;
		GetClientRect(ClientRect);
		pFontDC->FillSolidRect(&ClientRect,RGB(232,243,255));
		CFont *pOldFont=pFontDC->SelectObject(font); 
		CSize sz;		
		pFontDC->SetTextColor(fontRGB);		
		GetTextExtentPoint32(pFontDC->m_hDC,sContent,sContent.GetLength(),&sz);
		pFontDC->DrawText(sContent, CRect(0,0,sz.cx,sz.cy), DT_LEFT | DT_WORDBREAK | DT_BOTTOM | DT_SINGLELINE);
		pFontDC->SelectObject(pOldFont);
        int nResultlength=0;
		nPos=nPos-sz.cx;
		if(nSize==0)
		{
			nResultlength=nPos-ClientRect.Width()+sz.cx;
			sz.cx=sz.cx-nResultlength;
		}
		if(sz.cx > 0)
		{
			BitBlt(pMenDC->m_hDC,nSize,nHeight,sz.cx,sz.cy,pFontDC->m_hDC,nResultlength,0,SRCCOPY);
			nSize+=sz.cx;
		}
		if(nSize >= ClientRect.Width())
			return false;
		return true;
	
}

void CExpensiveGiftCtrl::InsertGif(LPCTSTR szFilename,INT nNum)
{
		CString sTmp;
		CPictureExEx* pPicture=(CPictureExEx *)m_pSkinConfContext->GetPictureExEx(szFilename);
		if(pPicture==NULL)
		{
			m_bPicIsOK=FALSE;
			return;
		}
		else
			m_bPicIsOK=TRUE;

		m_cx=pPicture->m_PictureSize.cx;
		m_cy=pPicture->m_PictureSize.cy;
		if (m_cy>61)
			m_cy = 58;
		m_pPicture=pPicture;
        m_nThreadCurrent=0;
	    Clear();
	  	HDC hWinDC =::GetDC(NULL);
		//////////////����һ��GIF���豸/////////////////////
	 	m_hMenDC=CreateCompatibleDC(hWinDC);
		m_hMemBM=CreateCompatibleBitmap(hWinDC,m_pPicture->m_PictureSize.cx,m_pPicture->m_PictureSize.cy);
        SelectObject(m_hMenDC,m_hMemBM);
       	HBRUSH hBrush = CreateSolidBrush(RGB(232,243,255));
	    RECT rect = {0,0,m_pPicture->m_PictureSize.cx,m_pPicture->m_PictureSize.cy};
		FillRect(m_hMenDC,&rect,hBrush);
		Animate();
		::ReleaseDC(NULL,hWinDC);
		::DeleteObject(hBrush);
		SetTimer(1,70,NULL);
	
	
}
void CExpensiveGiftCtrl::Animate()
{
	if(m_hMenDC==NULL || m_pPicture == NULL)
		return;
   	if (m_pPicture->m_arrFrames[m_nThreadCurrent].m_pPicture)
	{
		
	
		long hmWidth;
		long hmHeight;
		m_pPicture->m_arrFrames[m_nThreadCurrent].m_pPicture->get_Width(&hmWidth);
		m_pPicture->m_arrFrames[m_nThreadCurrent].m_pPicture->get_Height(&hmHeight);
		
		m_pPicture->m_arrFrames[m_nThreadCurrent].m_pPicture->Render(m_hMenDC, 
			m_pPicture->m_arrFrames[m_nThreadCurrent].m_frameOffset.cx, 
			m_pPicture->m_arrFrames[m_nThreadCurrent].m_frameOffset.cy, 
			m_pPicture->m_arrFrames[m_nThreadCurrent].m_frameSize.cx, 
			m_pPicture->m_arrFrames[m_nThreadCurrent].m_frameSize.cy, 
			0, hmHeight, hmWidth,-hmHeight, NULL);
	
		m_nThreadCurrent++;
		if(m_nThreadCurrent==m_pPicture->m_arrFrames.size())
			m_nThreadCurrent=0;

	}
	
	
}
