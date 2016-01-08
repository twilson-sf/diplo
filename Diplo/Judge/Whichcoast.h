// whichcoast.h : header file
//
#ifndef _WHICHCOAST_H_
#define _WHICHCOAST_H_
/////////////////////////////////////////////////////////////////////////////
// PSWhichCoast dialog

#ifdef EXPORTING
class __declspec(dllexport) PSWhichCoast : public CDialog
#else
class __declspec(dllimport) PSWhichCoast : public CDialog
#endif
{
// Construction
public:
	PSWhichCoast(PSProvince*, CWnd* pParent = NULL);
	PSProvince* m_pPrv;

// Dialog Data
	//{{AFX_DATA(PSWhichCoast)
	enum { IDD = IDD_WHICH_COAST };
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(PSWhichCoast)
	virtual BOOL OnInitDialog();
	afx_msg void OnFirstCoast();
	afx_msg void OnSecondCoast();
	afx_msg void OnNocoast();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // _WHICHCOAST_H_
