/***************************************************************************
 *                                                                         *
 *   copyright            : (C) 2002 by EPH                                *
 ***************************************************************************/

// EphCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "EphCtrl.h"
#include "libim/icqlink.h"
#include "libim/icqclient.h"
#include "face/AFCResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class ContactItem : public OutBarItem {
public:
	ContactItem(IcqContact *c)
		:OutBarItem(c->nick.c_str(), AFCResource::getInstance().getFaceIndex(c->face, c->status),c->video?0:-1),
		contact(c) {}

	int compare(ContactItem *item);
	
	IcqContact *contact;
};

/*
 * Compare two items based on the contact's nick name and current status
 */
int ContactItem::compare(ContactItem *item)
{
	int s1 = contact->status;
	int s2 = item->contact->status;
	if (s1 != s2) {
		switch (s1) {
		case STATUS_ONLINE:
			return -1;
		case STATUS_OFFLINE:
			return 1;
		case STATUS_AWAY:
			return (s2 == STATUS_ONLINE ? 1 : -1);
		}
	}

	//code by lxw 2003-7-24 »áÔ±ÅÅÐò
	if(contact->level != item->contact->level)
	{
		if(contact->level > item->contact->level)
			return -1;
		else if(contact->level < item->contact->level)
			return 1;
	}

	int ret = text.CollateNoCase(item->text);
	if (ret != 0)
		return ret;

	return (contact->uin > item->contact->uin ? 1 : -1);
}


// The colors that will be shown circly when a contact is online
static COLORREF blinkColors[] = {
	RGB(255, 0, 0),
	RGB(255, 255, 0),
	RGB(0, 255, 255),
	RGB(0, 255, 0),
	RGB(0, 0, 255),
};
#define NR_BLINK_COLORS	(sizeof(blinkColors) / sizeof(*blinkColors))

// The offsets that the icon will swing circly when a message comes.
#define BLINK_OFFSET	3
static CPoint blinkOffsets[] = {
	CPoint(0, 0),
	CPoint(BLINK_OFFSET, BLINK_OFFSET),
	CPoint(0, 0),
	CPoint(-BLINK_OFFSET, BLINK_OFFSET),
};
#define NR_BLINK_OFFSETS	(sizeof(blinkOffsets) / sizeof(*blinkOffsets))

#define IDT_BLINK_TEXT		2001
#define IDT_BLINK_IMAGE		2002

/////////////////////////////////////////////////////////////////////////////
// EphCtrl

EphCtrl::EphCtrl()
{
	onlineOnly = FALSE;
	m_hSkin = NULL;

	memberColor = RGB(255,0,0);
}

EphCtrl::~EphCtrl()
{
}

/*
 * Return Contact at folder and item
 */
IcqContact *EphCtrl::contact(int folder, int item)
{
	return ((ContactItem *) folders[folder]->items[item])->contact;
}

void EphCtrl::setShowOnlineOnly(BOOL b)
{
	onlineOnly = b;
	if (b) {
		for (int i = getFolderCount(); --i >= 0;)
			folders[i]->scrollPos = 0;
	}
	repaintInsideRect();
}


/*
 * Reorder and redraw the corresponding item
 */
void EphCtrl::update(uint32 uin, BOOL move)
{
	int folder, item;
	if (findItem(uin, folder, item)) {
		vector<OutBarItem *> &items = folders[folder]->items;
		ContactItem *p = (ContactItem *) items[item];
		IcqContact *c = p->contact;

		if (move) {
			items.erase(items.begin() + item);
			addContact(folder, c);
			delete p;
		} else {
			p->image = AFCResource::getInstance().getFaceIndex(c->face, c->status);
			p->text = c->nick.c_str();
		}

		if (folder == selFolder)
			repaintInsideRect();
	}
}

void EphCtrl::update(const uint32 uin, const int folder, BOOL move)
{
	int item;	
	if (findC(uin, folder, item)) {
		vector<OutBarItem *> &items = folders[folder]->items;
		ContactItem *p = (ContactItem *) items[item];
		IcqContact *c = p->contact;

		if (move) {
			items.erase(items.begin() + item);
			addContact(folder, c);
			delete p;
		} else {
			p->image = AFCResource::getInstance().getFaceIndex(c->face, c->status);
			p->text = c->nick.c_str();
		}

		if (folder == selFolder)
			repaintInsideRect();
	}

}

void EphCtrl::addContact(int folder, IcqContact *c)
{
	if (folder < 0 || folder >= getFolderCount())
		return;

	ContactItem *p = new ContactItem(c);
	vector<OutBarItem *> &items = folders[folder]->items;

	// Binary insert
	int low = 0, high = items.size() - 1;	
	while (low <= high) {
		int mid = (low + high) / 2;
		if (p->compare((ContactItem *) items[mid]) > 0)
			low = mid + 1;
		else
			high = mid - 1;
	}
	insertItem(folder, low, p);
}

void EphCtrl::insertContactFront(int folder, IcqContact *c)
{
	if (folder < 0 || folder >= getFolderCount())
		return;

	ContactItem *p = new ContactItem(c);
	vector<OutBarItem *> &items = folders[folder]->items;

	insertItem(folder, 0, p);
}

void EphCtrl::insertContactBack(int folder, IcqContact *c)
{
	if (folder < 0 || folder >= getFolderCount())
		return;

	ContactItem *p = new ContactItem(c);
	vector<OutBarItem *> &items = folders[folder]->items;
	int high = items.size();

	insertItem(folder, high, p);
}

void EphCtrl::removeContact(uint32 uin)
{
	int folder, item;
	if (findItem(uin, folder, item))
		removeItem(folder, item);
}

void EphCtrl::removeContact(uint32 uin, const int folderfrom, const int folderto)
{
	int folder, item;
	if (findItem(uin, folder, item))
	{
		if(folder>=folderfrom && folder<=folderto)
			removeItem(folder, item);
	}		
}
/*
 * Find an item's position by uin
 */
BOOL EphCtrl::findItem(uint32 uin, int &folder, int &item)
{
	for (int i = 0; i < (int) folders.size(); i++) {
		vector<OutBarItem *> &items = folders[i]->items;
		for (int j = 0; j < (int) items.size(); j++) {
			if (((ContactItem *)items[j])->contact->uin == uin) {
				folder = i;
				item = j;
				return TRUE;
				
			}
		}
	}
	
	return FALSE;
}

/*
 * Find an item's contract by uin
 */
//add by dl ,for charroom send txt
IcqContact* EphCtrl::findC(uint32 uin, int &folder, int &item)
{
	for (int i = 0; i < (int) folders.size(); i++) {
		vector<OutBarItem *> &items = folders[i]->items;
		for (int j = 0; j < (int) items.size(); j++) {
			if (((ContactItem *)items[j])->contact->uin == uin) {
				folder = i;
				item = j;
				return ((ContactItem *)items[j])->contact;
			}
		}
	}
	return NULL;
}

IcqContact* EphCtrl::findC(uint32 uin, const int folder, int &item)
{
	vector<OutBarItem *> &items = folders[folder]->items;
	for (int j = 0; j < (int) items.size(); j++) {
		if (((ContactItem *)items[j])->contact->uin == uin) {
			item = j;
			return ((ContactItem *)items[j])->contact;
		}
	}
    return NULL;	
}

IcqContact* EphCtrl::findC(uint32 uin, const int folderfrom, const int folderto, int &folder, int &item)
{
	if(folderfrom < folderto)
	{
		for (int i = folderfrom; i < folderto; i++) {
			vector<OutBarItem *> &items = folders[i]->items;
			for (int j = 0; j < (int) items.size(); j++) {
				if (((ContactItem *)items[j])->contact->uin == uin) {
					folder = i;
					item = j;
					return ((ContactItem *)items[j])->contact;
				}
			}
		}	
	}
	else if(folderfrom == folderto)
	{
		return findC(uin, folderfrom, item);
	}
	else
	{
	}	
	return NULL;
}

IcqContact* EphCtrl::getC(const int folder, const int item)
{
	vector<OutBarItem *> &items = folders[folder]->items;
	return ((ContactItem *)items[item])->contact;
}
//end dl
/*
 * Blink text when a user is online
 */
void EphCtrl::doBlinkText()
{
	CClientDC dc(this);
	CRect rc;
	getInsideRect(rc);
	CRgn rgn;
	rgn.CreateRectRgnIndirect(rc);
	dc.SelectClipRgn(&rgn);
	
	BlinkList::iterator it, next;
	for (it = blinkTextList.begin(); it != blinkTextList.end(); it = next) {
		next = it;
		++next;

		BlinkInfo *p = &(*it);
		int folder, item;
		if (findItem(p->uin, folder, item)) {
			p->frame--;
			if (folder != selFolder || item > getMaxVisibleItem())
				continue;
			if (p->frame >= 0) {
				drawItemText(&dc, item, blinkColors[p->frame % NR_BLINK_COLORS]);
				continue;
			}
			drawItemText(&dc, item, fontColor);
		}
		blinkTextList.erase(it);
	}
	if (blinkTextList.empty())
		KillTimer(IDT_BLINK_TEXT);
}

/*
 * Blink icon when new message comes
 */
void EphCtrl::doBlinkImage()
{
	if (blinkImageList.empty()) {
		KillTimer(IDT_BLINK_IMAGE);
		return;
	}

	CClientDC dc(this);
	CRect rc;
	getInsideRect(rc);
	CRgn rgn;
	rgn.CreateRectRgnIndirect(rc);

	BlinkList::iterator it;
	for (it = blinkImageList.begin(); it != blinkImageList.end(); ++it) {
		BlinkInfo *p = &(*it);
		int folder, item;
		if (!findItem(p->uin, folder, item))
			continue;

		p->frame++;

		// Draw small icon on the folder
		getFolderRect(rc, folder);
		rc.left += 2;
		rc.right = rc.left + SMALL_ICON_W;
		rc.top += (FOLDER_HEIGHT - SMALL_ICON_H) / 2;
		rc.bottom = rc.top + SMALL_ICON_H;
		drawFolder(&dc, folder, FALSE, FALSE);
		if (p->frame & 1) {
			int img = contact(folder, item)->face;
			smallIcons->Draw(&dc, img, rc.TopLeft(), ILD_TRANSPARENT);
		}

		if (p->frame >= NR_BLINK_OFFSETS)
			p->frame = 0;
		if (folder != selFolder || item > getMaxVisibleItem())
			continue;

		dc.SelectClipRgn(&rgn);

		CRect rcIcon;
		getIconRect(rc, item);
		rcIcon = rc;
		rcIcon.InflateRect(BLINK_OFFSET, BLINK_OFFSET);
		drawBackground(&dc, rcIcon);
		CPoint &pt = blinkOffsets[p->frame];
		rc.OffsetRect(pt.x, pt.y);
		drawItemImage(&dc, item, rc);

		dc.SelectClipRgn(NULL);
	}
}

void EphCtrl::blinkText(uint32 uin)
{
	int folder, item;
	if (!findItem(uin, folder, item))
		return;
		
	SetTimer(IDT_BLINK_TEXT, 300, NULL);
	blinkTextList.push_back(BlinkInfo(uin, 5 * NR_BLINK_COLORS));
}

void EphCtrl::blinkImage(uint32 uin, BOOL blink)
{
	if (blink) {
		int folder, item;
		if (!findItem(uin, folder, item))
			return;
		
		SetTimer(IDT_BLINK_IMAGE, 400, NULL);
		blinkImageList.push_back(BlinkInfo(uin, 0));
	} else {
		BlinkList::iterator it = blinkImageList.begin();
		while (it != blinkImageList.end()) {
			if ((*it).uin == uin)
				it = blinkImageList.erase(it);
			else
				++it;
		}
		if (blinkImageList.empty())
			KillTimer(IDT_BLINK_IMAGE);
		Invalidate();
	}
}

/*
 * Serialize state
 */
void EphCtrl::save(DBOutStream &out)
{
	out << bgColor << fontColor;
	out << (uint8) largeIconView;

	uint16 nrFolders = folders.size();
	out << nrFolders;
	for (int i = 0; i < nrFolders; i++) {
		OutBarFolder *f = folders[i];
		out << f->text;

		uint16 nrItems = 0;
		//if (i != nrFolders - 2)
			nrItems = f->items.size();
		out << nrItems;
		for (int j = 0; j < nrItems; j++)
			out << ((ContactItem *) f->items[j])->contact->uin;
		
	}

	out << memberColor;
}

void EphCtrl::load(DBInStream &in)
{
	int i;
	for (i = folders.size() - 1; i >= 0; i--)
		delete folders[i];
	folders.clear();

	in >> bgColor >> fontColor;
	uint8 b;
	in >> b;
	largeIconView = b;

	uint16 nrFolders;
	in >> nrFolders;
	for (i = 0; i < nrFolders; i++) {
		string name;
		in >> name;
		addFolder(name.c_str());		

		uint16 nrItems;
		in >> nrItems;
		for (int j = 0; j < nrItems; j++) {
			uint32 uin;
			in >> uin;
			IcqContact *c = icqLink->findContact(uin);
			if(!c)
			{
				c=new IcqContact;
				c->uin=uin;
				CString nickname;
				nickname.Format("%lu",uin);
				c->nick=nickname;
				icqLink->contactList.push_back(c);
			}
			if (c) {
				c->type = (i < nrFolders - 1 ? CONTACT_FRIEND : CONTACT_IGNORE);
				if(i == nrFolders - 2)
				{
					insertContactBack(i, c);;
				}
				else
				{
					addContact(i, c);
				}				
			}
		}
	}

	in >> memberColor;
}

int EphCtrl::getMaxItem()
{
	int max = OutBarCtrl::getMaxItem();
	if (onlineOnly) {
		while (--max >= 0) {
			if (contact(max)->status != STATUS_OFFLINE) {
				++max;
				break;
			}
		}
	}
	return max;
}

void EphCtrl::drawBackground(CDC *pDC, CRect &rc)
{
//	OutBarCtrl::drawBackground(pDC, rc);

	DrawSkinImageRect( m_hSkin, "BackImg" , pDC->m_hDC , &rc );

/*	CGameImageHelper SelectHandle(&CGameImageLink::m_OutBarResource.m_BackImage);
	SelectHandle.StretchBlt(pDC->m_hDC,rc.left,rc.top,rc.Width(),rc.Height(),
								0,0,
								CGameImageLink::m_OutBarResource.m_BackImage.m_SelectRect.Width(),
								CGameImageLink::m_OutBarResource.m_BackImage.m_SelectRect.Height(),
								SRCCOPY);
*/	
}

void EphCtrl::drawItemText(CDC *pDC, int i, COLORREF color)
{
	// Show nick name of contacts that are in away status in different color
//	IcqContact *c = contact(i);
//	if(c)
//	{
//	OutBarCtrl::drawItemText(pDC, i,
//		c->status == STATUS_AWAY ? RGB(0, 0, 255) : color);
//	}
	IcqContact *c = contact(i);
	if(c)
	{
	if(c->level > 0)
		OutBarCtrl::drawItemText(pDC, i,
			c->status == STATUS_AWAY ? RGB(0, 0, 255) : memberColor);
	else
		OutBarCtrl::drawItemText(pDC, i,
			c->status == STATUS_AWAY ? RGB(0, 0, 255) : color);
	}
}

void EphCtrl::drawFolder(CDC *pDC, int i, BOOL pressed, BOOL hilight)
{
//	OutBarCtrl::drawFolder(pDC, i, pressed, hilight);

	CRect rc;
	getFolderRect(rc, i);
	
	if(hilight)
	{
		DrawSkinImageSection( m_hSkin, "SelectedFolder" , pDC->m_hDC , &rc );
	}
	else
	{
		DrawSkinImageSection( m_hSkin, "NormalFolder" , pDC->m_hDC , &rc );
	}
	
	drawFolderText(pDC, i, rc);
}

void EphCtrl::drawScroll(CDC *pDC, int i, BOOL pressed)
{
//	OutBarCtrl::drawScroll(pDC, i, pressed);

	CRect rc;
	getScrollRect(rc, i);
	if (i == SCROLL_DIR_UP)
	{
		if(pressed)
			DrawSkinImageSection( m_hSkin, "PressedUpArrowButton" , pDC->m_hDC , &rc );
		else
			DrawSkinImageSection( m_hSkin, "UpArrowButton" , pDC->m_hDC , &rc );
	}
	else
	{
		if(pressed)
			DrawSkinImageSection( m_hSkin, "PressedDownArrowButton" , pDC->m_hDC , &rc );
		else
			DrawSkinImageSection( m_hSkin, "DownArrowButton" , pDC->m_hDC , &rc );
	}

}

BEGIN_MESSAGE_MAP(EphCtrl, OutBarCtrl)
	//{{AFX_MSG_MAP(EphCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
//	ON_MESSAGE(WM_SKINDATACHANGED , OnSkinDataChanged )
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// EphCtrl message handlers
//===========================================================================================
//Edit by Ðí±ù<xubing@et66.com> for SkinMagic at 2003-5-31 13:37:44
//===========================================================================================
int EphCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//register to skinmagic system
	/*if(bSkin)
		RegisterSkinWindow( m_hWnd , 0 );*/
	
	m_hSkin = OpenSkinData("SkinOutlookBar");
	return 0;
}

void EphCtrl::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_hSkin)
		CloseSkinData( m_hSkin );
	/*if(bSkin)
	{
		UnregisterSkinWindow( m_hWnd );
	}*/
}

void EphCtrl::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == IDT_BLINK_TEXT)
		doBlinkText();
	else if (nIDEvent == IDT_BLINK_IMAGE)
		doBlinkImage();
	else
		OutBarCtrl::OnTimer(nIDEvent);
}
void EphCtrl::GetFriendInfo(int groupIndex,int itemIndex,UINT& uin,UINT &pic,CString& nickName,bool& isOnline)
{
	IcqContact* pContact =  contact( groupIndex,  itemIndex);
	uin = pContact->uin;
	pic = pContact->face;
	nickName= pContact->nick.c_str();
	isOnline = (pContact->status != STATUS_OFFLINE);
}

