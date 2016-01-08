// mainfrm.cpp : implementation of the PSMainFrame class
//

#include "stdafx.h"
#include "diplo.h"
#include "mainfrm.h"
#include "DiploView.h"
#include "TabView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern __declspec(dllimport) PSGame TG;

/////////////////////////////////////////////////////////////////////////////
// PSMainFrame

IMPLEMENT_DYNCREATE(PSMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(PSMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(PSMainFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_HISTORY_BACK, OnActionsBack)
	ON_COMMAND(ID_HISTORY_FORWARD, OnActionsForward)
	ON_COMMAND(ID_ANIMATE, OnAnimate)
	ON_COMMAND(ID_VIEW_HISTORYBAR, OnViewHistorybar)
	ON_COMMAND(IDM_ADJUDICATE, OnAdjudicate)
	ON_COMMAND(IDM_DELETE_MOVE, OnDeleteMove)
	ON_COMMAND(IDM_START_AGAIN, OnStartAgain)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectall)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CURRENTSITUATION, OnViewCurrentSituation)
	ON_COMMAND(ID_OPTIONS_FONTS, OnOptionsFonts)
	ON_UPDATE_COMMAND_UI(ID_ANIMATE, OnUpdateAnimate)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HISTORYBAR, OnUpdateViewHistorybar)
	ON_UPDATE_COMMAND_UI_RANGE(IDM_ADJUDICATE, IDM_START_AGAIN, OnUpdateAdjudicate)
	ON_UPDATE_COMMAND_UI(IDM_DELETE_MOVE, OnUpdateDeleteMove)
	ON_UPDATE_COMMAND_UI_RANGE(ID_EDIT_COPY, ID_EDIT_SELECT_ALL, OnUpdateEditMenu)
	ON_UPDATE_COMMAND_UI_RANGE(ID_HISTORY_BACK, ID_HISTORY_FORWARD, OnUpdateHistoryButtons)
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// arrays of IDs used to initialize control bars

// toolbar buttons - IDs are command buttons
static UINT BASED_CODE buttons[] =
{
	// same order as in the bitmap 'toolbar.bmp'
	ID_FILE_NEW,
	ID_FILE_OPEN,
	ID_FILE_BUILDGAME,
	ID_FILE_SAVE,
		ID_SEPARATOR,
	ID_EDIT_CURRENTSITUATION,
		ID_SEPARATOR,
	IDC_SUPPORTS,
	IDC_CONVOYS,
	IDC_OK,
	IDC_RESET,
		ID_SEPARATOR,
	IDC_ACTIONS_FASTENTRY,
		ID_SEPARATOR,
	IDM_DELETE_MOVE,
	IDM_ADJUDICATE,
		ID_SEPARATOR,
	ID_APP_ABOUT,
};


static UINT BASED_CODE indicators[] =
{
	ID_SEPARATOR,			// status line indicator
	ID_CURRENT_MOVE,
	ID_MAIN_PANE,           
	ID_CURRENT_TIME,
};

/////////////////////////////////////////////////////////////////////////////
// PSMainFrame construction/destruction

PSMainFrame::PSMainFrame()
{
	m_sizeUnit.cx = 30;// TODO: Hard code here.
	m_sizeUnit.cy = 20;// TODO: Hard code here.
	m_bAnimate = TRUE;
	m_bEnableHistoryButtons = TRUE;
}

PSMainFrame::~PSMainFrame()
{
}

int PSMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Add variant menus under New.
	AddMenus();

	if (!m_wndToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS) ||
		!m_wndToolBar.LoadBitmap(IDR_MAINFRAME) ||
		!m_wndToolBar.SetButtons(buttons, sizeof(buttons)/sizeof(UINT)))
	{
		TRACE("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	
	if (!m_wndHistoryBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS, 
		IDR_HISTORY_TOOLBAR))
	{
		TRACE("Failed to create HistoryBar\n");
		return -1;      // fail to create
	}

	EnableDocking(CBRS_ALIGN_ANY);

	m_wndToolBar.SetButtonStyle(10, TBBS_CHECKBOX);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar, AFX_IDW_DOCKBAR_TOP);
	m_wndHistoryBar.EnableDocking(CBRS_ALIGN_ANY);

	// Determine the position of the HistoryBar. It should be on the right of the ToolBar.
	CRect rectTool;
	RecalcLayout();
	m_wndToolBar.GetWindowRect(&rectTool);
	rectTool.OffsetRect(1, 0);
	DockControlBar(&m_wndHistoryBar, AFX_IDW_DOCKBAR_TOP, rectTool);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// Set the dimensions of the panes.
	CClientDC dc(this);
	TEXTMETRIC tm;
	BOOL bReturn = dc.GetTextMetrics(&tm);
	ASSERT(bReturn);
	int nAveWidth = tm.tmAveCharWidth;
	m_wndStatusBar.SetPaneInfo(1, ID_CURRENT_MOVE, SBPS_NORMAL, nAveWidth * 20);// TODO: Hard code here.
	m_wndStatusBar.SetPaneInfo(2, ID_MAIN_PANE, SBPS_NORMAL, nAveWidth * 6);
	m_wndStatusBar.SetPaneInfo(3, ID_CURRENT_TIME, SBPS_NORMAL, nAveWidth * 11);

    return 0;
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


BOOL PSMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	CRuntimeClass* pRuntime = RUNTIME_CLASS(PSTabView);
	CString strSection = "Settings";
	UINT uSplitter = AfxGetApp()->GetProfileInt(strSection, "Splitter", 180);
	if (!m_wndSplitter.CreateStatic(this, 1, 2) || 
		!m_wndSplitter.CreateView(0, 0, pRuntime, CSize(uSplitter, 0), pContext) || 
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(PSDiploView), CSize(0, 0), pContext))

		return FALSE;

	return TRUE;
}


void PSMainFrame::DisplayBuildBar(BOOL bShow)
{
	if (m_wndBuildBar.GetStyle() & WS_VISIBLE && !bShow || 
		!(m_wndBuildBar.GetStyle() & WS_VISIBLE) && bShow)
	{
		// Get the position of the main window.
		CRect pos;
		CWnd* pMainWnd = AfxGetMainWnd();
		pMainWnd->GetWindowRect(&pos);

		// Get the position of the build bar.
		CRect bar;
		m_wndBuildBar.GetWindowRect(&bar);

		// Show the build bar.
		m_pFloatingFrameClass = RUNTIME_CLASS(PSMiniFrame);
		FloatControlBar(&m_wndBuildBar, CPoint(pos.right - bar.Width(), pos.top));
		ShowControlBar(&m_wndBuildBar, bShow, FALSE);
		m_pFloatingFrameClass = RUNTIME_CLASS(CMiniDockFrameWnd);
	}
}


CView* PSMainFrame::GetDiploView()
{
	CView* pTabView = (CView*)GetDescendantWindow(AFX_IDW_PANE_FIRST);
	CSplitterWnd* pSplitter = (CSplitterWnd*)pTabView->GetParent();
	ASSERT(pSplitter);
	CView* pView = (CView*)pSplitter->GetPane(0, 1);
	ASSERT(pView);
	
	return pView;
}


void PSMainFrame::OnDestroy() 
{
	CString strSection = "Settings";
	int nSpeed = GetJump();
	AfxGetApp()->WriteProfileInt(strSection, "Animation Speed", nSpeed);
	CRect rect;
	GetWindowRect(&rect);
	AfxGetApp()->WriteProfileInt(strSection, "Window Width", rect.Width());
	AfxGetApp()->WriteProfileInt(strSection, "Window Height", rect.Height());

	int nCur, nMax;
	m_wndSplitter.GetColumnInfo(0, nCur, nMax);
	AfxGetApp()->WriteProfileInt(strSection, "Splitter", nCur);

	CFrameWnd::OnDestroy();
	
	// Destroy the history window.
	CWnd* pHistoryWnd = GetDescendantWindow(ID_HISTORY);
	ASSERT(pHistoryWnd);
	pHistoryWnd->DestroyWindow();
}


void PSMainFrame::OnActionsBack() 
{
	// Delete orders, if any.
	PSDiploView* pDiploView = (PSDiploView*)GetDiploView();
	pDiploView->StartAgain();
	PSOrdersTab* pOrdersTab = (PSOrdersTab*)GetDescendantWindow(ID_ORDERS);
	pOrdersTab->Clear();

	PSHistory* pHist = TG.GetHistory();
	ASSERT(pHist);
	PSStep* pPreviousStep = pHist->GetPreviousStep(TG.GetCurrentStep());
	if (!pPreviousStep)
	{
		MessageBox("No history recorded before this phase.", "Diplo", MB_ICONINFORMATION);
		return;
	}

	TG.SetCurrentStep(pPreviousStep);
	pDiploView->StartDiplo();
	CScrollView* pView = (CScrollView*)pDiploView;
	pView->Invalidate();
}


void PSMainFrame::OnActionsForward() 
{
	// See if there is a next step.
	PSHistory* pHist = TG.GetHistory();
	ASSERT(pHist);
	PSStep* pCurrentStep = TG.GetCurrentStep();
	PSStep* pNextStep = pHist->GetNextStep(pCurrentStep);
	if (!pNextStep)
	{
		MessageBox("No history recorded beyond this phase.", "Diplo", MB_ICONINFORMATION);
		return;
	}

	// Delete orders, if any.
	PSDiploView* pDiploView = (PSDiploView*) GetDiploView();
	pDiploView->StartAgain();
	PSOrdersTab* pOrdersTab = (PSOrdersTab*)GetDescendantWindow(ID_ORDERS);
	pOrdersTab->Clear();
	SetActiveView(pDiploView);

	// If this phase is ORDERS or RETREATS, mark the moving units and start the view's timer.
	PHASE CurrentPhase = pCurrentStep->GetPhase();
	if (CurrentPhase == ORDERS || CurrentPhase == RETREATS)
	{
		PSData* pNextData = pNextStep->GetData();
		for (int i = 0; i < pNextData->m_arStrMovingUnits.GetSize(); i++)
		{
			CString strOrder = pNextData->m_arStrMovingUnits[i];
			PSProvince* pLoc = TG.GetProvince(strOrder.Left(3));
			ASSERT(pLoc);
			PSProvince* pDest = TG.GetProvince(strOrder.Mid(6, 3));
			ASSERT(pDest);
			PSUnit* pUnit;
			if (CurrentPhase == ORDERS)
				pUnit = pLoc->GetUnit();
			else
				pUnit = pLoc->GetRetreatingUnit();
			ASSERT(pUnit);
			// Get the coast it is moving to. It is the one of the occupying unit in the next step.
			Units vUnits = pNextData->GetUnits(pUnit->GetOwner());
			int nSize = vUnits.size();
			for (int j = 0; j < nSize; j++)
			{
				PSUnit* pFutureUnit = vUnits[j];
				if (pFutureUnit->GetLocation() == pDest)
				{
					pUnit->SetDesiredCoast(pFutureUnit->GetCoast());
					break;
				}
			}
			ASSERT(j < nSize);
			pUnit->SetMoving(pDest);
		}
		TG.ClearRetreats();// So that, in case of retreats, the moving units will be drawn.
		if (CurrentPhase == RETREATS)
			TG.SetStatus(GETORDER);// To flag that the moving was triggeredd by browsing.
		UINT nTimer = pDiploView->SetTimer(ID_MOVE_TIMER, TIMER_PACE, NULL);
		ASSERT(nTimer);
	}
	else 
	{
		TG.SetCurrentStep(pNextStep);
		pDiploView->StartDiplo();
	}

}


void PSMainFrame::OnAnimate() 
{
	CMenu* pMenu = GetMenu();
	if (!m_bAnimate)
	{
		pMenu->CheckMenuItem(ID_ANIMATE, MF_CHECKED);
		m_bAnimate = TRUE;
	}
	else
	{
		pMenu->CheckMenuItem(ID_ANIMATE, MF_UNCHECKED);
		m_bAnimate = FALSE;
	}	
}


int PSMainFrame::GetJump()
{
	// Returns a value between 3 pixels and 30 pixels according to the position of the slider.
	return m_wndHistoryBar.GetSliderPosition();
}


void PSMainFrame::OnViewHistorybar() 
{
	ShowControlBar(&m_wndHistoryBar, !(m_wndHistoryBar.GetStyle() & WS_VISIBLE), FALSE);
}


void PSMainFrame::OnAdjudicate() 
{
	PSDiploView* pDiplo = (PSDiploView*) GetDiploView();
	pDiplo->Adjudicate();
}


void PSMainFrame::OnDeleteMove() 
{
	PSDiploView* pDiplo = (PSDiploView*) GetDiploView();
	PSOrdersTab* pOTab = pDiplo->m_pMoves;
	int nCurSel = pOTab->GetCurSel();
	ASSERT(nCurSel != LB_ERR);
	if (pOTab->GetCount() == 1)
	{
		OnStartAgain();
		return;
	}
	while (pOTab->GetCount() > nCurSel) 
		pOTab->DeleteString(nCurSel);

	pDiplo->DeleteOrder(nCurSel);
	pOTab->SetCurSel(nCurSel);
}


void PSMainFrame::OnStartAgain() 
{
	PSDiploView* pDiplo = (PSDiploView*) GetDiploView();
	pDiplo->StartAgain();
}


void PSMainFrame::OnEditCopy() 
{
	CEdit* pInfoTab = (CEdit*)GetDescendantWindow(ID_INFO);
	pInfoTab->SetFocus();
	pInfoTab->Copy();
}


void PSMainFrame::OnEditSelectall() 
{
	CEdit* pInfoTab = (CEdit*)GetDescendantWindow(ID_INFO);
	pInfoTab->SetFocus();
	pInfoTab->SetSel(0, -1);
}


void PSMainFrame::OnUpdateDeleteMove(CCmdUI* pCmdUI) 
{
	PSDiploView* pDiplo = (PSDiploView*) GetDiploView();
	PSOrdersTab* pOTab = pDiplo->m_pMoves;
	int nCurSel = pOTab->GetCurSel();
	if (nCurSel == LB_ERR)
		pCmdUI->Enable(FALSE);
	else 
		pCmdUI->Enable(TRUE);
}


void PSMainFrame::OnUpdateAdjudicate(CCmdUI* pCmdUI) 
{
	PSDiploView* pDiplo = (PSDiploView*) GetDiploView();
	PSOrdersTab* pOTab = pDiplo->m_pMoves;
	pCmdUI->Enable(pOTab->GetCount());
}


void PSMainFrame::OnUpdateEditMenu(CCmdUI* pCmdUI) 
{
	CWnd* pInfoTab = GetDescendantWindow(ID_INFO);
	if (pInfoTab->GetStyle() & WS_VISIBLE)
		pCmdUI->Enable(TRUE);
	else 
		pCmdUI->Enable(FALSE);
}


void PSMainFrame::OnUpdateViewHistorybar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_wndHistoryBar.GetStyle() & WS_VISIBLE);
}


void PSMainFrame::OnUpdateAnimate(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bAnimate);
}


void PSMainFrame::OnHelp() 
{
	CString strSection       = "Directories";
	CString strStringItem    = "Current";
	CString strValue = AfxGetApp()->GetProfileString(strSection, strStringItem);

	::WinHelp(NULL, strValue + "Diplo.hlp", HELP_INDEX, 0);
}


void PSMainFrame::OnUpdateHistoryButtons(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bEnableHistoryButtons && TG.GetHistory());
}


void PSMainFrame::OnViewCurrentSituation() 
{
	CEdit* pInfoTab = (CEdit*)GetDescendantWindow(ID_INFO);
	pInfoTab->SetWindowText(TG.GetAllInfo());
	CView* pDiplo = GetDiploView();
	pDiplo->Invalidate();
}


void PSMainFrame::CreateBuildBar()
{
	if (m_wndBuildBar.m_hWnd)
		return;

	if (!m_wndBuildBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOOLTIPS | CBRS_SIZE_FIXED, 
		IDR_MAIN_TOOLBAR))
	{
		TRACE("Failed to create BuildBar\n");
		return;      // fail to create
	}
}


void PSMainFrame::OnOptionsFonts() 
{
	PSInfoTab* pInfoTab = (PSInfoTab*)GetDescendantWindow(ID_INFO);
	CFont* pCurrentFont = pInfoTab->GetFont();
	LOGFONT lf;
	pCurrentFont->GetLogFont(&lf);
	CFontDialog dlg(&lf);
	dlg.DoModal();
	dlg.GetCurrentFont(&lf);
	pInfoTab->SetFontIndirect(&lf);

	CWinApp* pApp = AfxGetApp();
	BOOL bRet = pApp->WriteProfileString("Settings", "Font Name", dlg.GetFaceName());
	ASSERT(bRet);
	bRet = pApp->WriteProfileInt("Settings", "Font Size", dlg.GetSize() / 10);
	ASSERT(bRet);
}


void PSMainFrame::GetMessageString(UINT nID, CString& strMessage) const
{
	// Creates the string messages for Variants menu items and for the build bar buttons.

	// See if the ID is a build bar button.
	if (nID >= ID_FIRST_ARMY && nID < ID_FIRST_FLEET)
	{
		strMessage = "Army " + TG.GetPower(nID - ID_FIRST_ARMY + 1)->m_strName;
		return;
	}
	if (nID >= ID_FIRST_FLEET && nID < ID_FIRST_SC)
	{
		strMessage = "Fleet " + TG.GetPower(nID - ID_FIRST_FLEET + 1)->m_strName;
		return;
	}
	if (nID >= ID_FIRST_SC && nID < ID_SEASON_BUTTON)
	{
		strMessage = "SC " + TG.GetPower(nID - ID_FIRST_SC + 1)->m_strName;
		return;
	}

	// See if the ID is a new variant button.
	if (nID >= ID_FIRST_VARIANT && nID < ID_FIRST_VARIANT + MAX_VARIANTS)
	{
		CMenu* pMenu = GetMenu();
		CString strVariantName;
		pMenu->GetMenuString(nID, strVariantName, MF_BYCOMMAND);
		strMessage.Format("Start a new %s game", strVariantName);
		return;
	}

	CFrameWnd::GetMessageString(nID, strMessage);
}


BOOL PSMainFrame::OnToolTipText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
	// Creates the tooltips for the build bar.
    TOOLTIPTEXT* pTTT = (TOOLTIPTEXT*)pNMHDR;
    UINT nID = pNMHDR->idFrom;
	if (nID >= ID_FIRST_ARMY && nID < ID_SEASON_BUTTON)
	{
		CString strMessage;
		if (nID < ID_FIRST_FLEET)
			strMessage = "Army " + TG.GetPower(nID - ID_FIRST_ARMY + 1)->m_strName;
		if (nID >= ID_FIRST_FLEET && nID < ID_FIRST_SC)
			strMessage = "Fleet " + TG.GetPower(nID - ID_FIRST_FLEET + 1)->m_strName;
		if (nID >= ID_FIRST_SC)
			strMessage = "SC " + TG.GetPower(nID - ID_FIRST_SC + 1)->m_strName;

		char lpszTemp[50];
		strcpy(lpszTemp, (LPCTSTR)strMessage);
        pTTT->lpszText = lpszTemp;
		return TRUE;
	}
	
	return CFrameWnd::OnToolTipText(id, pNMHDR, pResult);
}


void PSMainFrame::AddMenus()
{
	CMenu* pMenu = GetMenu();
	CMenu* pFileMenu = pMenu->GetSubMenu(0);
	CMenu* pNewMenu = pFileMenu->GetSubMenu(0);
	// Remove the bogus menu
	pNewMenu->RemoveMenu(0, MF_BYPOSITION);
	// Append all the menus found in the registry under Variants key;
	CWinApp* pApp = AfxGetApp();
	CString strVariants;
	strVariants = pApp->GetProfileString("Settings", "Variants", "Standard");
	// strVariants contains all the names of the variants registered with Diplo, 
	// separated by a space. Parse strVariants to get the variants.
	int nIndex = 0;
	while (TRUE)
	{
		CString strVarFound;
		int nSpace = strVariants.Find(0x20);
		if (nSpace == -1)
		{
			pNewMenu->AppendMenu(MF_BYPOSITION, ID_FIRST_VARIANT + nIndex, strVariants);
			break;
		}

		else 
		{
			strVarFound = strVariants.Left(nSpace);
			pNewMenu->AppendMenu(MF_BYPOSITION, ID_FIRST_VARIANT + nIndex, strVarFound);
			nIndex++;
			strVariants = strVariants.Right(strVariants.GetLength() - nSpace - 1);
		}
	}
}


