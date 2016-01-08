#if !defined(AFX_HISTORYBAR_H__BEE94DE2_2F6C_11D2_A969_444553540000__INCLUDED_)
#define AFX_HISTORYBAR_H__BEE94DE2_2F6C_11D2_A969_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HistoryBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PSHistoryBar window

class PSHistoryBar : public CToolBar
{
// Construction
public:
	PSHistoryBar();

// Attributes
public:
private:
	CSliderCtrl m_Slider;

// Operations
public:
	int GetSliderPosition();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PSHistoryBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~PSHistoryBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(PSHistoryBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTORYBAR_H__BEE94DE2_2F6C_11D2_A969_444553540000__INCLUDED_)
