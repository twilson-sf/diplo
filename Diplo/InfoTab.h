#if !defined(AFX_CURRENTTAB_H__BA86BC03_1EE2_11D2_A969_444553540000__INCLUDED_)
#define AFX_CURRENTTAB_H__BA86BC03_1EE2_11D2_A969_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// PSInfoTab window

#define ID_INFO                         111

class PSInfoTab : public CEdit
{
// Construction
public:
	PSInfoTab();

// Attributes
public:
private:
	CFont m_Font;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PSInfoTab)
	//}}AFX_VIRTUAL

// Implementation
public:
	CFont* GetFont()						{return &m_Font;}
	void SetFontIndirect(LOGFONT* plf);
	virtual ~PSInfoTab();

	// Generated message map functions
protected:
	//{{AFX_MSG(PSInfoTab)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CURRENTTAB_H__BA86BC03_1EE2_11D2_A969_444553540000__INCLUDED_)
