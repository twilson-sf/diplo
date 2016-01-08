// MainFrm.h : interface of the PSMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__FA1CA86A_9265_11D2_A96A_444553540000__INCLUDED_)
#define AFX_MAINFRM_H__FA1CA86A_9265_11D2_A96A_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ProvBar.h"

class PSMainFrame : public CFrameWnd
{
protected: // create from serialization only
	PSMainFrame();
	DECLARE_DYNCREATE(PSMainFrame)

// Attributes
public:

// Operations
public:
	void DisplayBuildBar(BOOL bShow);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PSMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~PSMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar	m_wndStatusBar;
	CToolBar	m_wndToolBar;
	PSProvBar	m_wndProvBar;

// Generated message map functions
protected:
	//{{AFX_MSG(PSMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__FA1CA86A_9265_11D2_A96A_444553540000__INCLUDED_)
