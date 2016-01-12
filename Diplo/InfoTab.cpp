// CurrentTab.cpp : implementation file
//

#include "stdafx.h"
#include "diplo.h"
#include "InfoTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PSInfoTab

PSInfoTab::PSInfoTab()
{
}

PSInfoTab::~PSInfoTab()
{
}


BEGIN_MESSAGE_MAP(PSInfoTab, CEdit)
	//{{AFX_MSG_MAP(PSInfoTab)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PSInfoTab message handlers

int PSInfoTab::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	CClientDC dc(this);
	CWinApp* pApp = AfxGetApp();
	CString strName = pApp->GetProfileString(L"Settings", L"Font Name", L"Tahoma");
	int nSize = pApp->GetProfileInt(L"Settings", L"Font Size", 8);
	int nHeight = -((dc.GetDeviceCaps(LOGPIXELSY) * nSize) / 72);
	m_Font.CreateFont(nHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, strName);
	SetFont(&m_Font);

	return 0;
}


void PSInfoTab::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CEdit::OnShowWindow(bShow, nStatus);
	if (bShow)
		SetFocus();
}


void PSInfoTab::SetFontIndirect(LOGFONT* plf)
{
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect(plf);
	SetFont(&m_Font);
}

