// Moves.cpp : implementation file
//

#include "stdafx.h"
#include "diplo.h"
#include "OrdersTab.h"
#include "DiploView.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PSOrdersTab dialog


PSOrdersTab::PSOrdersTab()
{
}


PSOrdersTab::~PSOrdersTab()
{
}


BEGIN_MESSAGE_MAP(PSOrdersTab, CListBox)
	//{{AFX_MSG_MAP(PSOrdersTab)
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PSOrdersTab message handlers
void PSOrdersTab::AddThisString(CString str)
{
	AddString(str);
	int nrItems = GetCount();
	SelectString(nrItems - 1);
}


void PSOrdersTab::SelectString(int nCount)
{
	int rez = SetCurSel(nCount);
	if (rez == LB_ERR) 
		MessageBox(L"No Selection", L"Diplo", MB_ICONEXCLAMATION);
}


void PSOrdersTab::Clear()
{
	ResetContent();
}


int PSOrdersTab::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_Font.CreatePointFont(80, L"Tahoma");
	SetFont(&m_Font);

	return 0;
}


BOOL PSOrdersTab::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// This is necessary for routing the menu commands to the MainFrame;
	CWnd* pWnd = AfxGetMainWnd();
	ASSERT(pWnd);
	if (pWnd->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	return CListBox::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void PSOrdersTab::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_DELETE)
	{
		CWnd* pWnd = AfxGetMainWnd();
		int nCurSel = GetCurSel();
		if (nCurSel != LB_ERR)
			pWnd->PostMessage(WM_COMMAND, IDM_DELETE_MOVE, NULL);
	}

	CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
}


void PSOrdersTab::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CListBox::OnShowWindow(bShow, nStatus);
	if (bShow)
		SetFocus();
}
