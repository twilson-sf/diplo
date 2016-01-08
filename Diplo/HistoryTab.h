#if !defined(AFX_HISTORYTAB_H__BA86BC04_1EE2_11D2_A969_444553540000__INCLUDED_)
#define AFX_HISTORYTAB_H__BA86BC04_1EE2_11D2_A969_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HistoryTab.h : header file
//

#include "History.h"
#define ID_HISTORY                      112

/////////////////////////////////////////////////////////////////////////////
// PSHistoryTab window

class PSHistoryTab : public CTreeCtrl
{
// Construction
public:
	PSHistoryTab();

// Attributes
public:
private:
	PSHistory* m_pHistory;
	CFont m_Font;
	HTREEITEM m_hRoot;
	CPtrArray m_Steps;
	CStringArray m_Phases;

// Operations
public:
	void Load(PSHistory*);
	void InsertStep(PSStep*);
	HTREEITEM GetItem(PSStep*);

private:
	void InsertYear(PSStep*);
	void InsertSeason(HTREEITEM, PSStep*);
	void InsertPhase(HTREEITEM, PSStep*);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PSHistoryTab)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~PSHistoryTab();

	// Generated message map functions
protected:
	//{{AFX_MSG(PSHistoryTab)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTORYTAB_H__BA86BC04_1EE2_11D2_A969_444553540000__INCLUDED_)
