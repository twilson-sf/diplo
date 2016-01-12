// DiploTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Diplo.h"
#include "DiploTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PSDiploTabCtrl

PSDiploTabCtrl::PSDiploTabCtrl()
{
}

PSDiploTabCtrl::~PSDiploTabCtrl()
{
}


BEGIN_MESSAGE_MAP(PSDiploTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(PSDiploTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PSDiploTabCtrl message handlers

void PSDiploTabCtrl::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_Info.ShowWindow(SW_HIDE);
	m_History.ShowWindow(SW_HIDE);
	m_Orders.ShowWindow(SW_HIDE);

	int nIndex = GetCurFocus();
	switch(nIndex)
	{
	case 0:
		m_Info.ShowWindow(SW_SHOW);
		break;
	case 1:
		m_History.ShowWindow(SW_SHOW);
		break;
	case 2:
		m_Orders.ShowWindow(SW_SHOW);
		break;
	default:
		ASSERT(0);
		break;
	}

	*pResult = 0;
}


int PSDiploTabCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CClientDC dc(this);
	int nHeight = -((dc.GetDeviceCaps(LOGPIXELSY) * 8) / 72);
	m_Font.CreateFont(nHeight, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, L"Tahoma");// TODO: Hard code.
	SetFont(&m_Font);

	TC_ITEM item;
	item.mask = TCIF_TEXT;
	item.pszText = L"Info";
	InsertItem(0, &item);
	item.pszText = L"History";
	InsertItem(1, &item);
	item.pszText = L"Orders";
	InsertItem(2, &item);

	m_Info.Create(WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | 
		ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL, 
		CRect(0, 0, 0, 0), this, ID_INFO);
	m_Info.ShowWindow(SW_SHOW);
	m_History.Create(WS_VISIBLE | WS_CHILD | WS_BORDER | 
		TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS, 
		CRect(0, 0, 0, 0), this, ID_HISTORY);
	m_History.ShowWindow(SW_HIDE);
	m_Orders.Create(WS_VISIBLE | WS_CHILD | WS_BORDER, 
		CRect(0, 0, 0, 0), this, ID_ORDERS);
	m_Orders.ShowWindow(SW_HIDE);

	return 0;
}


void PSDiploTabCtrl::OnSize(UINT nType, int cx, int cy) 
{
	const int nInset = 3;
	CTabCtrl::OnSize(nType, cx, cy);
	CRect rect;
	GetItemRect(0, &rect);
	
	m_Info.MoveWindow(nInset, rect.bottom + nInset, cx - 2 * nInset, cy - rect.bottom - 2 * nInset);
	m_History.MoveWindow(nInset, rect.bottom + nInset, cx - 2 * nInset, cy - rect.bottom - 2 * nInset);
	m_Orders.MoveWindow(nInset, rect.bottom + nInset, cx - 2 * nInset, cy - rect.bottom - 2 * nInset);
}


int PSDiploTabCtrl::SetCurSel(int nItem)
{
	int nReturn = CTabCtrl::SetCurSel(nItem);

	NMHDR nmhdr;
	nmhdr.hwndFrom = m_hWnd;
	nmhdr.idFrom = ID_TAB;
	nmhdr.code = TCN_SELCHANGE;
	SendMessage(WM_NOTIFY, NULL, (long) &nmhdr);

	return nReturn;
}

