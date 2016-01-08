// ColorButton.cpp : implementation file
//
#include "stdafx.h"
#include "dvm.h"
#include "ColorButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PSColorButton

PSColorButton::PSColorButton()
{
	m_BckBrush.CreateSolidBrush(m_BackColor);
}

PSColorButton::~PSColorButton()
{
}

BEGIN_MESSAGE_MAP(PSColorButton, CButton)
	//{{AFX_MSG_MAP(PSColorButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PSColorButton message handlers

void PSColorButton::SetBckColor(COLORREF cr)
{
	m_BackColor = cr;
	m_BckBrush.Detach();
	m_BckBrush.CreateSolidBrush(cr);
	if (m_hWnd)
		RedrawWindow();
}//

void PSColorButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	UINT uStyle = DFCS_BUTTONPUSH;

	// This code only works with buttons.
	ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);

	// If drawing selected, add the pushed style to DrawFrameControl.
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
		uStyle |= DFCS_PUSHED;

	// Draw the button frame.
	::DrawFrameControl(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, DFC_BUTTON, uStyle);

	// Fill a small rectangle with the selected brush.
	CRect rect;
	GetClientRect(&rect);
	rect.DeflateRect(3, 3);
	::FillRect(lpDrawItemStruct->hDC, rect, m_BckBrush);

	// If it has the focus, draw a dotted line inside the main rect.
	if (this == GetFocus())
	{
		rect.DeflateRect(2, 2);
		::DrawFocusRect(lpDrawItemStruct->hDC, &rect);
	}
}
