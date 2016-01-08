// DiploView.h : interface of the PSDiploView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIPLOVIEW_H__37C8646D_EB82_11D0_A968_985C06C10000__INCLUDED_)
#define AFX_DIPLOVIEW_H__37C8646D_EB82_11D0_A968_985C06C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "mystruct.h"
#include "Order.h"
#include "DiploDoc.h"
#include "OrdersTab.h"

enum BUILD_STATUS {UNITS, SCS};
#define OK_BUTTON		0
#define RESET_BUTTON	1
#define TIMER_PACE		3 // 3 ms

class PSDiploView : public CScrollView
{
protected: // create from serialization only
	PSDiploView();
	virtual ~PSDiploView();
	DECLARE_DYNCREATE(PSDiploView)

// Attributes
public:
	UINT m_nSelectedBuildButton;
	PSOrdersTab* m_pMoves;

private:
	HCURSOR hnewCursor, hnormalCursor;
	CBitmap* m_pMainMap;
	CBitmap m_Units;
	CImageList m_UnitList;
	Orders m_Orders;
	CString m_strCurrentOrder, m_strMousePos;
	BUILD_STATUS m_buildStatus;
	int m_nEnableDisable[2];
	SIZE m_sizeMain, m_sizeUnit;
	BOOL m_nFastEntry;
	BOOL m_bEnableSave;
#ifdef _DEBUG
	PSGame* m_pGame;
#endif // _DEBUG

// Operations
public:
	void SetYear(int);
	void StartAgain();
	void Adjudicate();
	void DeleteOrder(int);
	void UpdateHistoryTab();
	void UpdateInfoTab(CString strText = "");
	void StartDiplo();
	PSDiploDoc* GetDocument()							{return (PSDiploDoc*)m_pDocument;}

private:
	CString CheckPos(CPoint);
	void DoRetreats(BOOL = FALSE);
	void Adjustments(BOOL bAddToHistory = FALSE);
	void LoadTheBitMaps();
	void ErrorMessage(CString);
	void TransferDoc();
	void DisplayToDo();
	void OnOkGetOrder();
	void OnOkGetRetreat();
	void OnOkGetDisband();
	void OnOkGetBuild();
	void OnOkBuildSC(PSPower*);
	void OnOkBuildGame();
	void OnLBDBuildGame();
	void EndDiplo();
	void DrawNumber(int, PSProvince*);
	void DisplayBuildBar(BOOL);
	void LoadHistoryTab(PSHistory*);
	void ActivateTab(UINT);
	void MoveUnits();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PSDiploView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void Invalidate();

// Generated message map functions
protected:
	//{{AFX_MSG(PSDiploView)
	afx_msg void OnActionsFastentry();
	afx_msg void OnBuildSC(UINT);
	afx_msg void OnBuildUnit(UINT);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnConvoys();
	afx_msg void OnFileBuildgame();
	afx_msg void OnFileModify();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnOk();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnReset();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSupports();
	afx_msg void OnUpdateActionsFastentry(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBuildButtons(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCurrentMove(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCurrentTime(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMainPane(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOk(CCmdUI* pCmdUI);
	afx_msg void OnUpdateReset(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileBuild(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSupports(CCmdUI* pCmdUI);
	afx_msg void OnOkBuildButton();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnFileSaveAsBitmap();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIPLOVIEW_H__37C8646D_EB82_11D0_A968_985C06C10000__INCLUDED_)
