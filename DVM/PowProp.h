#if !defined(AFX_POWPROP_H__96DF8962_DED8_11D2_A96A_444553540000__INCLUDED_)
#define AFX_POWPROP_H__96DF8962_DED8_11D2_A96A_444553540000__INCLUDED_

#include "ColorButton.h"	// Added by ClassView
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PowProp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PSPowProp dialog

class PSPowProp : public CDialog
{
// Construction
public:
	void SetTitle(CString strTitle)						{m_strTitle = strTitle;}
	CString GetTitle()									{return m_strTitle;}
	void SetColor(COLORREF cr)							{m_ColorButton.SetBckColor(cr);}
	COLORREF GetColor()									{return m_ColorButton.GetBckColor();}
	PSPowProp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(PSPowProp)
	enum { IDD = IDD_POW_PROP };
	CString	m_strName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PSPowProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_strTitle;
	PSColorButton m_ColorButton;

	// Generated message map functions
	//{{AFX_MSG(PSPowProp)
	afx_msg void OnColorButton();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POWPROP_H__96DF8962_DED8_11D2_A96A_444553540000__INCLUDED_)
