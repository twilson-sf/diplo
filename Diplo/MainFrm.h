// MainFrm.h : interface of the PSMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__37C86469_EB82_11D0_A968_985C06C10000__INCLUDED_)
#define AFX_MAINFRM_H__37C86469_EB82_11D0_A968_985C06C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define ID_MOVE_TIMER 1

#include "BuildBar.h"
#include "HistoryBar.h"
#include "MyMiniFrame.h"

class PSMainFrame : public CFrameWnd
{
protected: // create from serialization only
	PSMainFrame();
	DECLARE_DYNCREATE(PSMainFrame)
    BOOL PreCreateWindow(CREATESTRUCT& cs);

// Attributes
public:
protected:  // control bar embedded members
	CStatusBar		m_wndStatusBar;
	CToolBar		m_wndToolBar;
	PSBuildBar		m_wndBuildBar;
	PSHistoryBar	m_wndHistoryBar;
	CBitmap			m_bmpArrow;
	CSplitterWnd	m_wndSplitter;

private:
	BOOL m_bAnimate;
	SIZE m_sizeMain, m_sizeUnit;
	BOOL m_bEnableHistoryButtons;

// Operations
public:
	void DisplayBuildBar(BOOL);
	BOOL GetAnimate()									{return m_bAnimate;}
	CView* GetDiploView();
	int GetJump();
	void EnableHistoryButtons(BOOL bEnable)				{m_bEnableHistoryButtons = bEnable;}
	SIZE GetMainMapSize()								{return m_sizeMain;}
	SIZE GetUnitSize()									{return m_sizeUnit;}

// Implementation
public:
	virtual ~PSMainFrame();
	virtual void GetMessageString(UINT nID, CString& strMessage) const;
	void CreateBuildBar();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	void AddMenus();

	// Generated message map functions
protected:
	//{{AFX_MSG(PSMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	afx_msg void OnHelp();
	afx_msg void OnActionsBack();
	afx_msg void OnActionsForward();
	afx_msg void OnAnimate();
	afx_msg void OnViewHistorybar();
	afx_msg void OnAdjudicate();
	afx_msg void OnDeleteMove();
	afx_msg void OnStartAgain();
	afx_msg void OnEditSelectall();
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateAnimate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewHistorybar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAdjudicate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDeleteMove(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditMenu(CCmdUI* pCmdUI);
	afx_msg void OnUpdateHistoryButtons(CCmdUI* pCmdUI);
	afx_msg void OnViewCurrentSituation();
	afx_msg void OnOptionsFonts();
	afx_msg BOOL OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__37C86469_EB82_11D0_A968_985C06C10000__INCLUDED_)
