// DVM.h : main header file for the DVM application
//

#if !defined(AFX_DVM_H__FA1CA866_9265_11D2_A96A_444553540000__INCLUDED_)
#define AFX_DVM_H__FA1CA866_9265_11D2_A96A_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "Game.h"

/////////////////////////////////////////////////////////////////////////////
// PSDVMApp:
// See DVM.cpp for the implementation of this class
//

class PSDVMApp : public CWinApp
{
public:
	PSDVMApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PSDVMApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

private:
	

// Implementation

	//{{AFX_MSG(PSDVMApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DVM_H__FA1CA866_9265_11D2_A96A_444553540000__INCLUDED_)
