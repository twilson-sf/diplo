#ifndef _MOVES_H_
#define _MOVES_H_

/////////////////////////////////////////////////////////////////////////////
// PSOrdersTab dialog

#define ID_ORDERS                       106

class PSOrdersTab : public CListBox
{
// Construction
public:
	PSOrdersTab();
	virtual ~PSOrdersTab();

// Attributes:
public:
	void AddThisString(CString);
	void SelectString(int);
	void Clear(void);

private:
	CFont m_Font;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PSOrdersTab)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(PSOrdersTab)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif
