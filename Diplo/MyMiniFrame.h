#if !defined(AFX_MYMINIFRAME_H__9389A0E2_D94B_11D1_A969_444553540000__INCLUDED_)
#define AFX_MYMINIFRAME_H__9389A0E2_D94B_11D1_A969_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MyMiniFrame.h : header file
//

#include "afxpriv.h"

/////////////////////////////////////////////////////////////////////////////
// PSMiniFrame frame

class PSMiniFrame : public CMiniDockFrameWnd
{
	DECLARE_DYNCREATE(PSMiniFrame)
protected:
	PSMiniFrame();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PSMiniFrame)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~PSMiniFrame();

	// Generated message map functions
	//{{AFX_MSG(PSMiniFrame)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYMINIFRAME_H__9389A0E2_D94B_11D1_A969_444553540000__INCLUDED_)
