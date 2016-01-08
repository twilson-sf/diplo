#if !defined(AFX_WELCOMEPROVEDITOR_H__F951DAE1_C645_11D2_A96A_444553540000__INCLUDED_)
#define AFX_WELCOMEPROVEDITOR_H__F951DAE1_C645_11D2_A96A_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// WelcomeProvEditor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PSWelcomeProvEditor dialog

class PSWelcomeProvEditor : public CDialog
{
// Construction
public:
	PSWelcomeProvEditor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(PSWelcomeProvEditor)
	enum { IDD = IDD_WELCOME_PROV_EDITOR };
	int		m_nSelection;
	CString	m_strSelectedProv;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PSWelcomeProvEditor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(PSWelcomeProvEditor)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WELCOMEPROVEDITOR_H__F951DAE1_C645_11D2_A96A_444553540000__INCLUDED_)
