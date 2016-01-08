#if !defined(AFX_TABVIEW_H__BA86BC05_1EE2_11D2_A969_444553540000__INCLUDED_)
#define AFX_TABVIEW_H__BA86BC05_1EE2_11D2_A969_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TabView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PSTabView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "DiploTabCtrl.h"

class PSTabView : public CFormView
{
protected:
	PSTabView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(PSTabView)

// Form Data
public:
	//{{AFX_DATA(PSTabView)
	enum { IDD = IDD_TAB_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
private:
	PSDiploTabCtrl m_wndTab;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PSTabView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~PSTabView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(PSTabView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABVIEW_H__BA86BC05_1EE2_11D2_A969_444553540000__INCLUDED_)
