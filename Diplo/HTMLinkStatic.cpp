// HTMLinkStatic.cpp : implementation file
//

#include "stdafx.h"
#include "diplo.h"
#include "HTMLinkStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PSHTMLinkStatic

PSHTMLinkStatic::PSHTMLinkStatic() : 
m_strLink("")
{
}


PSHTMLinkStatic::~PSHTMLinkStatic()
{
}


BEGIN_MESSAGE_MAP(PSHTMLinkStatic, CStatic)
	//{{AFX_MSG_MAP(PSHTMLinkStatic)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PSHTMLinkStatic message handlers

BOOL PSHTMLinkStatic::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest == HTCLIENT)
	{
		::SetCursor(m_hFinger);
		return TRUE;
	}
	
	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}


void PSHTMLinkStatic::PreSubclassWindow() 
{
	CStatic::PreSubclassWindow();
	m_hFinger = AfxGetApp()->LoadCursor(IDC_FINGER_CURSOR);
	CFont* pFont = GetFont();
	LOGFONT lf;
	pFont->GetLogFont(&lf);
	lf.lfUnderline = TRUE;
	m_Font.CreateFontIndirect(&lf);
	SetFont(&m_Font);
	SetWindowText(m_strLink);
	m_BkColor = GetSysColor(COLOR_3DFACE);
	m_Brush.CreateSolidBrush(m_BkColor);
}


void PSHTMLinkStatic::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CString strSection       = "Directories";
	CString strStringItem    = "Current";
	CString strDirectory = AfxGetApp()->GetProfileString(strSection, strStringItem);

	HWND hWnd = GetSafeHwnd();
	HINSTANCE hInst = ShellExecute(hWnd, "open", m_strLink, NULL, strDirectory, 0);
	if (hInst <= (HINSTANCE)32)
	{
		TRACE (_T("Couldn't open PlanetSoft home page.\r"));
	}

	CStatic::OnLButtonDown(nFlags, point);
}


void PSHTMLinkStatic::SetLink(CString strLink)
{
	m_strLink = strLink;
}


HBRUSH PSHTMLinkStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	pDC->SetBkColor(m_BkColor);
	pDC->SetTextColor(RGB(0, 0, 255));
	return m_Brush;
}

