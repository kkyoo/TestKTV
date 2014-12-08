// ListGiftDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ListGiftDlg.h"
#include "tinyxml/tinyxml.h"
#include <Shlwapi.h>

#define  GIFT_PIC_W  40
#define  GIFT_PIC_H  40

#define WM_LOAD_GIFT_IMG_COMPLETE     WM_USER+4509

// CListGiftDlg 对话框

IMPLEMENT_DYNAMIC(CListGiftDlg, CDialog)
CListGiftDlg::CListGiftDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CListGiftDlg::IDD, pParent)
{
	m_bHasLoadGiftImg = false;
	m_LoadGiftThread = NULL;
	m_LoadGiftThreadId=1;
	m_strLoadElementName="";
}

CListGiftDlg::~CListGiftDlg()
{
}

void CListGiftDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_GIFT, m_listGiftCtrl);
}


BEGIN_MESSAGE_MAP(CListGiftDlg, CDialog)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_GIFT, OnNMClickListGift)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_GIFT, OnNMDblclkListGift)
	ON_MESSAGE(WM_LOAD_GIFT_IMG_COMPLETE,OnLoadGiftImgComplete)
	ON_MESSAGE(WM_UPDATETOOLTIPTEXT,OnUpdateToolTipText)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CListGiftDlg 消息处理程序

BOOL CListGiftDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	LONG style = GetWindowLong(m_listGiftCtrl, GWL_STYLE);
	style|= LVS_AUTOARRANGE;
	SetWindowLong(m_listGiftCtrl, GWL_STYLE, style);

	//m_listGift.SubclassDlgItem(IDC_LIST_GIFT, this);     //图片直接的间隔距离会出错,DoDataExchange继承控件即可
	m_listGiftCtrl.SetIconSpacing(50, 50);

	m_imgListGift.DeleteImageList();
	BOOL bResult = m_imgListGift.Create(GIFT_PIC_W, GIFT_PIC_H, ILC_COLOR16| ILC_MASK, 20, 10);
	ASSERT( bResult );
	m_listGiftCtrl.SetImageList( &m_imgListGift, LVSIL_NORMAL);

	return TRUE; 
}

void CListGiftDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(m_listGiftCtrl.GetSafeHwnd())
	{
		CRect rtClient,rtListGift;
		GetClientRect(&rtClient);
		m_listGiftCtrl.MoveWindow(&rtClient);
	}
}


void CListGiftDlg::OnNMClickListGift(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		pGiftData pData = ( pGiftData )m_listGiftCtrl.GetItemData( pNMListView->iItem );
		ASSERT( pData );
		CString s;
		m_curSelGift = *pData;

		CString strMsg="";
		strMsg.Format("nID=%u,nType=%u,strName=%s,strToolTip=%s,nValue=%u,nWidth=%u,nHeight=%u,nIsDynamicGift=%u,nIsShowEffect=%u,nIsSpecialGift=%u,nIsFire=%u,nIsDoll=%u,nIsProp=%u",\
			m_curSelGift.nID,m_curSelGift.nType,m_curSelGift.strName.c_str(),m_curSelGift.strToolTip.c_str(),m_curSelGift.nValue,m_curSelGift.nWidth,m_curSelGift.nHeight,\
			m_curSelGift.nIsDynamicGift,m_curSelGift.nIsShowEffect,m_curSelGift.nIsSpecialGift,m_curSelGift.nIsFire,m_curSelGift.nIsDoll,m_curSelGift.nIsProp);
		MessageBox(strMsg);
	}

	*pResult = 0;
}

void CListGiftDlg::OnNMDblclkListGift(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		pGiftData pData = ( pGiftData )m_listGiftCtrl.GetItemData( pNMListView->iItem );
		ASSERT( pData );
		CString s;
		m_curSelGift = *pData;

		CString strMsg="";
		strMsg.Format("nID=%u,nType=%u,strName=%s,strToolTip=%s,nValue=%u,nWidth=%u,nHeight=%u,nIsDynamicGift=%u,nIsShowEffect=%u,nIsSpecialGift=%u,nIsFire=%u,nIsDoll=%u,nIsProp=%u",\
			m_curSelGift.nID,m_curSelGift.nType,m_curSelGift.strName.c_str(),m_curSelGift.strToolTip.c_str(),m_curSelGift.nValue,m_curSelGift.nWidth,m_curSelGift.nHeight,\
			m_curSelGift.nIsDynamicGift,m_curSelGift.nIsShowEffect,m_curSelGift.nIsSpecialGift,m_curSelGift.nIsFire,m_curSelGift.nIsDoll,m_curSelGift.nIsProp);
		MessageBox(strMsg);
	}

	*pResult = 0;
}

void CListGiftDlg::ClearAllList()
{
	for(int i=0; i<m_listGiftCtrl.GetItemCount(); i++)
	{
		pGiftData pData = ( pGiftData )m_listGiftCtrl.GetItemData(i);
		ASSERT(pData);
		if ( pData)
			delete pData;
		pData = NULL; 
	}
	m_listGiftCtrl.DeleteAllItems();
}

pGiftData CListGiftDlg::GetGiftDataByGoodId(int goodid)
{
	for(int i=0; i<m_listGiftCtrl.GetItemCount(); i++)
	{
		pGiftData pData = ( pGiftData )m_listGiftCtrl.GetItemData(i);
		ASSERT(pData);
		if (pData &&  pData->nID == goodid)
			return pData;
	}
	return NULL;
}

pGiftData CListGiftDlg::GetGiftDataByItem(int nItem)
{
	if(nItem<m_listGiftCtrl.GetItemCount())
	{
		pGiftData pData = ( pGiftData )m_listGiftCtrl.GetItemData(nItem);
		ASSERT(pData);
		return pData;
	}
	return NULL;
}

void CListGiftDlg::SetLoadElementName(CString strElementName)
{
	m_strLoadElementName = strElementName;
}

bool CListGiftDlg::ParseDynamicGiftTypeAndId(CString strFileName,UINT& uType,int& uId)
{
	//格式：flash002005
	if(strFileName.GetLength() < 6)
		return false;

	CString str = strFileName.Right(3);
	uId = atoi(str);

	strFileName = strFileName.Mid(0,strFileName.GetLength() - 3);
	str = strFileName.Right(3);
	uType = atoi(str);

	return true;
}

LRESULT CListGiftDlg::OnLoadGiftImgComplete(WPARAM wParam,LPARAM lParam)
{
	
	return 1L;
}

LRESULT CListGiftDlg::OnUpdateToolTipText(WPARAM wParam,LPARAM lParam)
{
	int nItem = (int)wParam;
	HWND ListHwnd = (HWND)lParam;

	if(m_listGiftCtrl.GetSafeHwnd() == ListHwnd)
	{
		pGiftData info = GetGiftDataByItem(nItem);
		if(info && GetParent() && GetParent()->GetSafeHwnd())
		{
			ToolTipTextData* pTTTData = new ToolTipTextData;

			pTTTData->nID = info->nID;
			pTTTData->strGoodName = info->strName.c_str(); 
			pTTTData->strGoodPath = info->imgPath.c_str();
			pTTTData->nGoodValue = info->nValue;
			pTTTData->strGoodToolTip = info->strToolTip.c_str();
			CRect rt;
			m_listGiftCtrl.GetItemRect(nItem,rt,LVIR_BOUNDS);
			m_listGiftCtrl.ClientToScreen(rt);
			pTTTData->rtItem = rt;

			::SendMessage(GetParent()->GetSafeHwnd(),WM_UPDATETOOLTIPTEXT,(WPARAM)pTTTData,NULL);
			//CopyRect

			/*CString strMsg="";
			strMsg.Format("nID=%u,nType=%u,strName=%s,strToolTip=%s,nValue=%u,nWidth=%u,nHeight=%u,nIsDynamicGift=%u,nIsShowEffect=%u,nIsSpecialGift=%u,nIsFire=%u,nIsDoll=%u,nIsProp=%u",\
				info->nID,info->nType,info->strName.c_str(),info->strToolTip.c_str(),info->nValue,info->nWidth,info->nHeight,\
				info->nIsDynamicGift,info->nIsShowEffect,info->nIsSpecialGift,info->nIsFire,info->nIsDoll,info->nIsProp);
			MessageBox(strMsg);*/
		}
	}
	
	return 1;
}

void CListGiftDlg::StartLoadGiftThread()
{
	if( m_LoadGiftThread !=NULL)
	{
		WaitForSingleObject(m_LoadGiftThread,INFINITE);
		CloseHandle(m_LoadGiftThread);
		m_LoadGiftThread=NULL;
	}
	m_LoadGiftThread = CreateThread(NULL,0,LoadGiftThreadCB,this,0,&m_LoadGiftThreadId);
}

DWORD WINAPI CListGiftDlg::LoadGiftThreadCB(LPVOID lpParam)
{
	CListGiftDlg* self = (CListGiftDlg*) lpParam;
	self->LoadGiftImg();
	return 1;
}

void CListGiftDlg::AddGift(GiftData configGiftData)
{
	IPicture	*pPic;
	IStream		*pStm;
	HANDLE		hFile = NULL;
	DWORD		dwFileSize, dwByteRead;
	HGLOBAL		hGlobal;
	LPVOID		pvData;
	OLE_XSIZE_HIMETRIC hmWidth;		// 图片的真实宽度 
	OLE_YSIZE_HIMETRIC hmHeight;	// 图片的真实高度
	int			index;
	HDC			hTmpDC = ::CreateCompatibleDC( ::GetDC( NULL ) );
	ASSERT(hTmpDC);
	HBITMAP		hTmpBmp = ::CreateCompatibleBitmap(::GetDC(NULL), GIFT_PIC_W,GIFT_PIC_H);
	ASSERT(hTmpBmp);
	HBITMAP		hOldBmp = ( HBITMAP )::SelectObject(hTmpDC, hTmpBmp);
	HBITMAP		hbm;
	HBRUSH		hbr = ::CreateSolidBrush(RGB(255,255,255));
	bool		bResult = false;

	TCHAR drv[32], dir[MAX_PATH], fname[_MAX_FNAME], ext[32];
	_splitpath(configGiftData.imgPath.c_str(), drv, dir, fname, ext);
	//打开硬盘中的图形文件 
	hFile = CreateFile(configGiftData.imgPath.c_str(), GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); 

	if (hFile != INVALID_HANDLE_VALUE) 
	{ 
		dwFileSize = GetFileSize(hFile, NULL);	//获取文件字节数 

		if( dwFileSize != 0xFFFFFFFF )
		{

			// 分配全局存储空间 
			hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize); 
			pvData = NULL; 

			if ( hGlobal )
			{
				if ( ( pvData = GlobalLock(hGlobal) ) != NULL)	// 锁定分配内存块
				{
					ReadFile(hFile, pvData, dwFileSize, &dwByteRead, NULL);	//把文件读入内存缓冲区
					GlobalUnlock( hGlobal ); 

					CreateStreamOnHGlobal(hGlobal, TRUE, &pStm); 

					// 装入图形文件 
					bResult = BOOL(OleLoadPicture(pStm, dwFileSize, TRUE, IID_IPicture, (LPVOID*)&pPic)); 
					if( SUCCEEDED(bResult)  && pPic)
					{

						pPic->get_Width( &hmWidth ); 
						pPic->get_Height( &hmHeight );  

						::FillRect(hTmpDC, CRect(0, 0, GIFT_PIC_W,GIFT_PIC_H), hbr);
						pPic->Render(hTmpDC, 0, 0, GIFT_PIC_W,GIFT_PIC_H, 0, hmHeight, hmWidth, -hmHeight, NULL);

						hbm = (HBITMAP)CopyImage(hTmpBmp, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION| LR_COPYRETURNORG);
						ASSERT(hbm);

						pPic->Release(); 

						index = m_imgListGift.Add(CBitmap::FromHandle(hbm), RGB(255,255,255));
						index = m_listGiftCtrl.InsertItem(m_listGiftCtrl.GetItemCount(), "", index);
						pGiftData pData = new GiftData;
						if(configGiftData.nIsDynamicGift)
						{
							ParseDynamicGiftTypeAndId(fname,pData->nType,pData->nID);
						}
						else
						{
							pData->nID = atoi(fname+4);
						}
						pData->nType = configGiftData.nType;

						pData->strName = configGiftData.strName;
						pData->imgPath = configGiftData.imgPath;
						pData->strToolTip = configGiftData.strToolTip;

						pData->nValue = configGiftData.nValue;
						pData->nIsDynamicGift = configGiftData.nIsDynamicGift;
						pData->nIsShowEffect = configGiftData.nIsShowEffect;
						pData->nIsSpecialGift = configGiftData.nIsSpecialGift;
						pData->nIsFire = configGiftData.nIsFire;
						pData->nIsDoll = configGiftData.nIsDoll;
						pData->nIsProp = configGiftData.nIsProp;
						
						m_listGiftCtrl.SetItemData( index, (DWORD_PTR)pData );

						hmWidth	=	MulDiv(hmWidth,	96, 2540);  
						hmHeight=	MulDiv(hmHeight, 96, 2540);  
						
						pData->nWidth = hmWidth;
						pData->nHeight = hmHeight;
					}
				}
				GlobalFree( hGlobal );
			}
		}
		CloseHandle(hFile);//关闭打开的文件 
	}

	::SelectObject(hTmpDC, ( HGDIOBJ ) hOldBmp);
	::DeleteObject( hTmpBmp );
	::DeleteObject( hbr );
	::DeleteDC( hTmpDC );
}

bool CListGiftDlg::LoadGiftImg()
{
	if(m_strLoadElementName.Compare("")==0)
		return false;

	if(m_bHasLoadGiftImg)
		return false;

	m_bHasLoadGiftImg=true;
	ClearAllList();
	
	// 读取XML数据
	bool bSucceeded = false;
	TCHAR fileName[MAX_PATH];
	if ( !GetModuleFileName(NULL, fileName, sizeof(fileName)) )
		return bSucceeded;
	TCHAR *p = strrchr(fileName, '\\');
	if (p)
		*(p + 1) = '\0';
	TCHAR goodspath[MAX_PATH];
	_tcscpy(goodspath, fileName);
	_tcscat(goodspath, _T("face\\AGuiGood.data"));

	if (!PathFileExists(goodspath))
		return bSucceeded;

	TiXmlDocument doc;
	CString path=goodspath;
	if( ! doc.LoadFile( path ) )
	{
		CString str=path+"error";
#ifdef _DEBUG
		MessageBox(str);
#endif
		return -1;
	}
	const TiXmlElement* root = doc.FirstChildElement();
	if( root )
	{
		bSucceeded = TRUE;
		
		const TiXmlElement* it = NULL;
		const char* val1 = NULL;
		const char* val2 = NULL;
		const char* val3 = NULL;
		const char* val4 = NULL;
		const char* val5 = NULL;
		const char* val6 = NULL;
		const char* val7 = NULL;
		const char* val8 = NULL;
		const char* val9 = NULL;
		const char* val10 = NULL;

		const TiXmlElement* sub = root->FirstChildElement(m_strLoadElementName); 
		
		if (sub)
		{
			it= sub->FirstChildElement();
			do{
				GiftData configGiftData;
				CString strToolTip="";

				val1 = it->Attribute("name");
				val2 = it->Attribute("value");
				val3 = it->Attribute("img");
				val4 = it->Attribute("tooltip");
				val5 = it->Attribute("dynamicgift");
				val6 = it->Attribute("showeffect");
				val7 = it->Attribute("bFire");
				val8 = it->Attribute("bspecialgift");
				val9 = it->Attribute("bVodDoll");
				val10 = it->Attribute("bPropGift");
				if(val1)
					configGiftData.strName = val1;
				if(val2)
					configGiftData.nValue = atoi(val2);
				if(val3)
					configGiftData.imgPath = val3;
				if(val4)
					strToolTip = val4;
				if(val5)
					configGiftData.nIsDynamicGift = atoi(val5);
				if(val6)
					configGiftData.nIsShowEffect = atoi(val6);
				if (val7)
					configGiftData.nIsFire = atoi(val7);
				if(val8)
					configGiftData.nIsSpecialGift = atoi(val8);
				if(val9)
					configGiftData.nIsDoll = atoi(val9);
				if(val10)
					configGiftData.nIsProp = atoi(val10);

				CString sPath;
				sPath.Format( _T("%s%s"), fileName, configGiftData.imgPath.c_str() );
				configGiftData.imgPath = sPath;
				strToolTip.Replace("\\n","\r\n");
				configGiftData.strToolTip=strToolTip;
				
				AddGift(configGiftData);

			}while((it=it->NextSiblingElement()));
		}
		
		
	}
	PostMessage(WM_LOAD_GIFT_IMG_COMPLETE,NULL,NULL);
	return bSucceeded;
}

void CListGiftDlg::OnDestroy()
{
	CDialog::OnDestroy();

	ClearAllList();
}
