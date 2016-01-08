#if !defined(AFX_DIPLOTABCTRL_H__B0F223CB_1E0E_11D2_A969_444553540000__INCLUDED_)
#define AFX_DIPLOTABCTRL_H__B0F223CB_1E0E_11D2_A969_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DiploTabCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PSDiploTabCtrl window

#include "InfoTab.h"
#include "HistoryTab.h"
#include "OrdersTab.h"

#define ID_TAB                          107

class PSDiploTabCtrl : public CTabCtrl
{
// Construction
public:
	PSDiploTabCtrl();

// Attributes
public:

private:
	PSHistoryTab m_History;
	PSOrdersTab m_Orders;
	PSInfoTab m_Info;
	CFont m_Font;

// Operations
public:
	int SetCurSel(int);

private:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PSDiploTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~PSDiploTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(PSDiploTabCtrl)
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIPLOTABCTRL_H__B0F223CB_1E0E_11D2_A969_444553540000__INCLUDED_)
