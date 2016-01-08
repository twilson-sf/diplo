// BuildBar.h : header file
//

#ifndef _BUILDBAR_H_
#define _BUILDBAR_H_
/////////////////////////////////////////////////////////////////////////////
// PSBuildBar window

class PSBuildBar : public CToolBar
{
// Construction
public:
	PSBuildBar();

// Attributes
public:
	CComboBox m_comboYear;
	CButton	m_SpringButton;
	CButton	m_FallButton;
	CButton	m_CloseButton;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PSBuildBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~PSBuildBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(PSBuildBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnYearCombo();
	afx_msg void OnUpdateYearCombo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOKBuildButton(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSeasonButton(CCmdUI* pCmdUI);
	afx_msg void OnSeasonButton();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif //_BUILDBAR_H_
/////////////////////////////////////////////////////////////////////////////
