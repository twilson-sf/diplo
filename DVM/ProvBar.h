#if !defined(AFX_PROVBAR_H__D0EB4B22_C6BF_11D2_A96A_444553540000__INCLUDED_)
#define AFX_PROVBAR_H__D0EB4B22_C6BF_11D2_A96A_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ProvBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PSProvBar window

class PSProvBar : public CToolBar
{
// Construction
public:
	PSProvBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PSProvBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	CEdit m_NameProvEdit;
	CEdit m_NameCoast1Edit;
	CEdit m_NameCoast2Edit;
	CButton m_OKButton;
	CButton m_CancelButton;

	virtual ~PSProvBar();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(PSProvBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKillfocusProvName();
	afx_msg void OnKillfocusCoast1Name();
	afx_msg void OnKillfocusCoast2Name();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROVBAR_H__D0EB4B22_C6BF_11D2_A96A_444553540000__INCLUDED_)
