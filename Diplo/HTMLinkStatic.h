#if !defined(AFX_HTMLINKSTATIC_H__F3B1D023_55F9_11D2_A969_444553540000__INCLUDED_)
#define AFX_HTMLINKSTATIC_H__F3B1D023_55F9_11D2_A969_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HTMLinkStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PSHTMLinkStatic window

class PSHTMLinkStatic : public CStatic
{
// Construction
public:
	PSHTMLinkStatic();

// Attributes
public:
private:
	HCURSOR m_hFinger;
	CBitmap m_bmpLink;
	HBITMAP m_hbmp;
	CString m_strLink;
	CBrush m_Brush;
	COLORREF m_BkColor;
	CFont m_Font;

// Operations
public:
	void SetLink(CString strLink);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PSHTMLinkStatic)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~PSHTMLinkStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(PSHTMLinkStatic)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTMLINKSTATIC_H__F3B1D023_55F9_11D2_A969_444553540000__INCLUDED_)
