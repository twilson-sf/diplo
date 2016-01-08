#if !defined(AFX_PASSWORDDLG_H__FA7DF342_5AE3_11D2_A969_444553540000__INCLUDED_)
#define AFX_PASSWORDDLG_H__FA7DF342_5AE3_11D2_A969_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PasswordDlg.h : header file
//
#include "HTMLinkStatic.h"

/////////////////////////////////////////////////////////////////////////////
// PSPasswordDlg dialog

class PSPasswordDlg : public CDialog
{
// Construction
public:
	PSPasswordDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(PSPasswordDlg)
	enum { IDD = IDD_PASSWORD };
	PSHTMLinkStatic	m_DiploLink;
	CString	m_strPassword;
	CString	m_strPasswordStatic;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PSPasswordDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL ValidatePassword();

protected:

private:
	int m_nRetries;

	// Generated message map functions
	//{{AFX_MSG(PSPasswordDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSWORDDLG_H__FA7DF342_5AE3_11D2_A969_444553540000__INCLUDED_)
