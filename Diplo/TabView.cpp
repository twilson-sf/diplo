// TabView.cpp : implementation file
//

#include "stdafx.h"
#include "diplo.h"
#include "TabView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PSTabView

IMPLEMENT_DYNCREATE(PSTabView, CFormView)

PSTabView::PSTabView()
	: CFormView(PSTabView::IDD)
{
	//{{AFX_DATA_INIT(PSTabView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

PSTabView::~PSTabView()
{
}

void PSTabView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PSTabView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PSTabView, CFormView)
	//{{AFX_MSG_MAP(PSTabView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PSTabView diagnostics

#ifdef _DEBUG
void PSTabView::AssertValid() const
{
	CFormView::AssertValid();
}

void PSTabView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// PSTabView message handlers

int PSTabView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_wndTab.Create(WS_CHILD | WS_VISIBLE | TCS_TABS | TCS_RIGHTJUSTIFY | TCS_FOCUSNEVER, 
		CRect(0, 0, 0, 0), this, ID_TAB);
	
	return 0;
}


void PSTabView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	m_wndTab.MoveWindow(0, 0, cx, cy);
}


void PSTabView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	SetScrollSizes(MM_TEXT, CSize(0, 0));
}
