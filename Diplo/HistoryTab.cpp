// HistoryTab.cpp : implementation file
//

#include "stdafx.h"
#include "diplo.h"
#include "HistoryTab.h"
#include "OrdersTab.h"
#include "MainFrm.h"
#include "DiploView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern __declspec(dllimport) PSGame TG;

/////////////////////////////////////////////////////////////////////////////
// PSHistoryTab

PSHistoryTab::PSHistoryTab() : 
m_pHistory(NULL)
{
}

PSHistoryTab::~PSHistoryTab()
{
	for (int i = 0; i < m_Steps.GetSize(); i++)
	{
		delete m_Steps[i];
	}
}


BEGIN_MESSAGE_MAP(PSHistoryTab, CTreeCtrl)
	//{{AFX_MSG_MAP(PSHistoryTab)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_SETFOCUS()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PSHistoryTab message handlers

int PSHistoryTab::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_Font.CreatePointFont(80, L"Tahoma");// Hard code here.
	SetFont(&m_Font);

	// Initialize m_Phases.
	m_Phases.Add("Orders");
	m_Phases.Add("Retreats");
	m_Phases.Add("Adjustments");

	return 0;
}


void PSHistoryTab::Load(PSHistory* pHist)
{
	// Delete m_Steps and the entire tree;
	for (int i = 0; i < m_Steps.GetSize(); i++)
	{
		delete m_Steps[i];
	}
	m_Steps.RemoveAll();
	DeleteAllItems();

	if (pHist)
	{
		m_pHistory = pHist;

		// Get the document's name and set it as the root.
		PSMainFrame* pFrame = (PSMainFrame*)GetParentFrame();
		ASSERT(pFrame);
		CDocument* pDiploDoc = pFrame->GetActiveDocument();
		CString strTitle = pDiploDoc->GetTitle();
		m_hRoot = InsertItem(strTitle);

		// Add the steps.
		for (int i = 0; i < pHist->GetNumberOfSteps(); i++)
			InsertStep(pHist->GetStep(i));
	}
}


void PSHistoryTab::InsertStep(PSStep* pStep)
{
	long lYear = pStep->GetYear();
	InsertYear(pStep);
}


void PSHistoryTab::InsertYear(PSStep* pStep)
{
	// Years can be inserted only in the root, in sorted fashion.
	CString strYear;
	strYear.Format(L"%d", (int)pStep->GetYear());
	// See if the year is already in the tree.
	HTREEITEM hYear = GetChildItem(m_hRoot);
	while (GetItemText(hYear) != strYear && hYear != NULL)
	{
		hYear = GetNextSiblingItem(hYear);
	}
	// hYear is NULL if there are no more children.
	if (hYear == NULL)
		hYear = InsertItem(strYear, m_hRoot);

	// Remove all items below the inserted year.
	HTREEITEM hNext;
	while (hNext = GetNextSiblingItem(hYear))
	{
		DeleteItem(hNext);
	}

	InsertSeason(hYear, pStep);

}


void PSHistoryTab::InsertSeason(HTREEITEM hYear, PSStep* pStep)
{
	// hYear represents the year item the season will be attached to. I rely on the fact that 
	// both Spring and Fall must exist and in this particular order.
	CString strSeason;
	SEASON Season = pStep->GetSeason();
	strSeason = (Season == FALL) ? "Fall" : "Spring";
	// See if the season is already in the tree.
	HTREEITEM hSeason = GetChildItem(hYear);
	while (GetItemText(hSeason) != strSeason && hSeason != NULL)
	{
		hSeason = GetNextSiblingItem(hSeason);
	}
	// hSeason is NULL if there are no more children.
	if (hSeason == NULL)
		hSeason = InsertItem(strSeason, hYear);

	// Remove all items below the inserted season.
	HTREEITEM hNext;
	while (hNext = GetNextSiblingItem(hSeason))
	{
		DeleteItem(hNext);
	}

	InsertPhase(hSeason, pStep);
}


void PSHistoryTab::InsertPhase(HTREEITEM hSeason, PSStep* pStep)
{
	// hSeason represents the year item the season will be attached to. I rely on the fact that 
	// Orders phase must exist and is always the first, Adjustments is always last.
	CString strPhase;
	PHASE Phase = pStep->GetPhase();
	strPhase = m_Phases[Phase];

	// See if the phase is already in the tree.
	HTREEITEM hPhase = GetChildItem(hSeason);
	while (GetItemText(hPhase) != strPhase && hPhase != NULL)
	{
		hPhase = GetNextSiblingItem(hPhase);
	}
	// hPhase is NULL if there are no more children.
	if (hPhase == NULL)
	{
		switch (Phase)
		{
		case ORDERS:
			hPhase = InsertItem(strPhase, hSeason, TVI_FIRST);
			break;
		case RETREATS:
			{
				HTREEITEM hOrders = GetChildItem(hSeason);
				hPhase = InsertItem(strPhase, hSeason, hOrders);
				break;
			}
		case ADJUSTMENTS:
			hPhase = InsertItem(strPhase, hSeason, TVI_LAST);
			break;
		}
	}

	// Remove all items below the inserted phase.
	HTREEITEM hNext;
	while (hNext = GetNextSiblingItem(hPhase))
	{
		DeleteItem(hNext);
	}

	HTREEITEM* phPhase = new HTREEITEM;
	*phPhase = hPhase;
	m_Steps.InsertAt(m_Steps.GetSize(), phPhase);
}


HTREEITEM PSHistoryTab::GetItem(PSStep* pStep)
{
	// Returns the item that has the features above.
	long lYear = pStep->GetYear();
	SEASON Season = pStep->GetSeason();
	PHASE Phase	= pStep->GetPhase();
	HTREEITEM hReturnItem = NULL;

	for (int i = 0; i < m_Steps.GetSize(); i++)
	{
		HTREEITEM* phItem = (HTREEITEM*)m_Steps[i];
		hReturnItem = *phItem;
		CString strName;
		strName = m_Phases[Phase];
		if (strName == GetItemText(hReturnItem))
		{
			strName = (pStep->GetSeason() == FALL) ? "Fall" : "Spring";
			HTREEITEM hParent = GetParentItem(hReturnItem);
			if (strName == GetItemText(hParent))
			{
				strName.Format("%d", (int)lYear);
				hParent = GetParentItem(hParent);
				if (strName == GetItemText(hParent))
				{
					return hReturnItem;
				}
			}
		}
	}

	return NULL;
}


void PSHistoryTab::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	// If the tree control doesn't have the focus, do nothing.
    if (GetFocus() != this)
        return;

	PSMainFrame* pFrame = (PSMainFrame*)GetParentFrame();
	ASSERT(pFrame);
	PSDiploView* pDiploView = (PSDiploView*)pFrame->GetDiploView();

	// Delete orders, if any.
	if (TG.GetStatus() == GETORDER)
	{
		pDiploView->StartAgain();
		PSOrdersTab* pOrdersTab = (PSOrdersTab*)pFrame->GetDescendantWindow(ID_ORDERS);
		pOrdersTab->Clear();
	}

	// If the item is a step, set that step as the current step for the game.
	HTREEITEM hSelItem = GetSelectedItem();
	for (int i = 0; i < m_Steps.GetSize(); i++)
	{
		if (hSelItem == *((HTREEITEM*)m_Steps[i]))
		{
			PSHistory* pHist = TG.GetHistory();
			ASSERT(pHist);
			PSStep* pStep = pHist->GetStep(i);
			TG.SetCurrentStep(pStep);
			
			// Refresh DiploView and start the game.
			pDiploView->StartDiplo();
			CScrollView* pView = (CScrollView*)pDiploView;
			pView->Invalidate();
			return;
		}
	}

	// If the item is not a step, expand it.
	Expand(hSelItem, TVE_EXPAND);

	*pResult = 0;
}


void PSHistoryTab::OnSetFocus(CWnd* pOldWnd) 
{
	CTreeCtrl::OnSetFocus(pOldWnd);
	HTREEITEM item = GetSelectedItem();
	SelectItem(item);
}


void PSHistoryTab::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CTreeCtrl::OnShowWindow(bShow, nStatus);
		if (bShow)
		SetFocus();
}
