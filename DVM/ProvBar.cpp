// ProvBar.cpp : implementation file
//

#include "stdafx.h"
#include "dvm.h"
#include "ProvBar.h"
#include "DVMView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PSProvBar

PSProvBar::PSProvBar()
{
}

PSProvBar::~PSProvBar()
{
}


BEGIN_MESSAGE_MAP(PSProvBar, CToolBar)
	//{{AFX_MSG_MAP(PSProvBar)
	ON_WM_CREATE()
	ON_EN_KILLFOCUS(IDC_PROV_NAME, OnKillfocusProvName)
	ON_EN_KILLFOCUS(IDC_COAST1_NAME, OnKillfocusCoast1Name)
	ON_EN_KILLFOCUS(IDC_COAST2_NAME, OnKillfocusCoast2Name)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PSProvBar message handlers

int PSProvBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!LoadToolBar(IDR_PROV_TOOLBAR))
		return 0;
	
	SetBarStyle(GetBarStyle() |	CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED);
	CRect rect;
	GetItemRect(0, rect);
	// Make the edit boxes and the Cancel button 2 times wider than a button.
	int nWidth = rect.Width() * 2;
	SetButtonInfo(1, IDC_PROV_NAME, TBBS_SEPARATOR, nWidth);
	SetButtonInfo(9, IDC_COAST1_NAME, TBBS_SEPARATOR, nWidth);
	SetButtonInfo(13, IDC_COAST2_NAME, TBBS_SEPARATOR, nWidth);
	SetButtonInfo(11, ID_PROV_CANCEL_BUTTON, TBBS_SEPARATOR, nWidth);
	
	// Wrap the ends of the rows.
	UINT nStyle = GetButtonStyle(0);
	SetButtonStyle(7, nStyle | TBBS_WRAPPED);// After the SC button.
	SetButtonStyle(11, nStyle | TBBS_WRAPPED);// After the Cancel button.
	
	// Create the Name Edit Boxes.
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_BORDER;
	GetItemRect(1, rect);
	rect.right = rect.left + nWidth;
	m_NameProvEdit.Create(dwStyle, rect, this, IDC_PROV_NAME);

	GetItemRect(9, rect);
	rect.right = rect.left + nWidth;
	m_NameCoast1Edit.Create(dwStyle, rect, this, IDC_COAST1_NAME);

	GetItemRect(13, rect);
	rect.right = rect.left + nWidth;
	m_NameCoast2Edit.Create(dwStyle, rect, this, IDC_COAST2_NAME);

	// Create the OK and Cancel buttons.
	dwStyle = WS_CHILD | WS_VISIBLE;
	GetItemRect(11, rect);
	rect.right = rect.left + nWidth;
	m_CancelButton.Create(L"Cancel", dwStyle, rect, this, ID_PROV_CANCEL_BUTTON);

	GetItemRect(14, rect);
	rect.left += nWidth / 2;
	rect.right = rect.left + nWidth;
	m_OKButton.Create(L"OK", dwStyle, rect, this, ID_PROV_OK_BUTTON);
	return 0;
}

void PSProvBar::OnKillfocusProvName()
{
	PSDVMView* pView = (PSDVMView*)((CFrameWnd*)AfxGetMainWnd())->GetActiveView();
	ASSERT(pView);

	PSProvince* pCurProv = pView->GetCurProv();
	m_NameProvEdit.GetWindowText(pCurProv->m_strName);
}

void PSProvBar::OnKillfocusCoast1Name()
{
}

void PSProvBar::OnKillfocusCoast2Name()
{
}
