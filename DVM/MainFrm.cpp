// MainFrm.cpp : implementation of the PSMainFrame class
//

#include "stdafx.h"
#include "DVM.h"
#include "MainFrm.h"
#include "MyMiniFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PSMainFrame

IMPLEMENT_DYNCREATE(PSMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(PSMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(PSMainFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_CURRENT_MOVE,
};

/////////////////////////////////////////////////////////////////////////////
// PSMainFrame construction/destruction

PSMainFrame::PSMainFrame()
{
	// TODO: add member initialization code here
	
}

PSMainFrame::~PSMainFrame()
{
}

int PSMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndProvBar.Create(this, WS_CHILD))
	{
		TRACE0("Failed to create provbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	m_wndProvBar.EnableDocking(0);
	m_pFloatingFrameClass = RUNTIME_CLASS(PSMiniFrame);
	FloatControlBar(&m_wndProvBar, CPoint(0, 0));
	m_pFloatingFrameClass = RUNTIME_CLASS(CMiniDockFrameWnd);

	return 0;
}

BOOL PSMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	CFrameWnd::PreCreateWindow(cs);

	CString strSection = "Settings";
	UINT uWidth = AfxGetApp()->GetProfileInt(strSection, "Window Width", 870);
	UINT uHeight = AfxGetApp()->GetProfileInt(strSection, "Window Height", 715);
	cs.cx = uWidth;
	cs.cy = uHeight;

	return TRUE;
}

void PSMainFrame::DisplayBuildBar(BOOL bShow)
{
	if (m_wndProvBar.GetStyle() & WS_VISIBLE && !bShow || 
		!(m_wndProvBar.GetStyle() & WS_VISIBLE) && bShow)
	{
		// Get the position of the main window.
		CRect pos;
		CWnd* pMainWnd = AfxGetMainWnd();
		pMainWnd->GetWindowRect(&pos);

		// Get the position of the build bar.
		CRect bar;
		m_wndProvBar.GetWindowRect(&bar);

		// Show the build bar.
		m_pFloatingFrameClass = RUNTIME_CLASS(PSMiniFrame);
		FloatControlBar(&m_wndProvBar, CPoint(pos.right - bar.Width(), pos.top));
		ShowControlBar(&m_wndProvBar, bShow, FALSE);
		m_pFloatingFrameClass = RUNTIME_CLASS(CMiniDockFrameWnd);
	}
}


/////////////////////////////////////////////////////////////////////////////
// PSMainFrame diagnostics

#ifdef _DEBUG
void PSMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void PSMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// PSMainFrame message handlers

void PSMainFrame::OnDestroy() 
{
	CString strSection = "Settings";
	CRect rect;
	GetWindowRect(&rect);
	AfxGetApp()->WriteProfileInt(strSection, "Window Width", rect.Width());
	AfxGetApp()->WriteProfileInt(strSection, "Window Height", rect.Height());

	CFrameWnd::OnDestroy();
}
