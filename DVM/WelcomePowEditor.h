#if !defined(AFX_WELCOMEPOWEDITOR_H__96DF8961_DED8_11D2_A96A_444553540000__INCLUDED_)
#define AFX_WELCOMEPOWEDITOR_H__96DF8961_DED8_11D2_A96A_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// WelcomePowEditor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PSWelcomePowEditor dialog

class PSWelcomePowEditor : public CDialog
{
// Construction
public:
	PSWelcomePowEditor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(PSWelcomePowEditor)
	enum { IDD = IDD_WELCOME_POW_EDITOR };
	int		m_nSelection;
	CString	m_strSelectedPow;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PSWelcomePowEditor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(PSWelcomePowEditor)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WELCOMEPOWEDITOR_H__96DF8961_DED8_11D2_A96A_444553540000__INCLUDED_)
