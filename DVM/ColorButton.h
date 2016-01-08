#if !defined(AFX_COLORBUTTON_H__A88538C1_DF70_11D2_A96A_444553540000__INCLUDED_)
#define AFX_COLORBUTTON_H__A88538C1_DF70_11D2_A96A_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ColorButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PSColorButton window

class PSColorButton : public CButton
{
// Construction
public:
	PSColorButton();
	virtual ~PSColorButton();

// Attributes
public:
protected:
	COLORREF m_BackColor;
	CBrush m_BckBrush;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PSColorButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetBckColor(COLORREF cr);
	COLORREF GetBckColor()								{return m_BackColor;}

	// Generated message map functions
protected:
	//{{AFX_MSG(PSColorButton)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBUTTON_H__A88538C1_DF70_11D2_A96A_444553540000__INCLUDED_)
