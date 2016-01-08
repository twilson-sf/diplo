// Diplo.h : main header file for the DIPLO application
//

#if !defined(AFX_DIPLO_H__37C86465_EB82_11D0_A968_985C06C10000__INCLUDED_)
#define AFX_DIPLO_H__37C86465_EB82_11D0_A968_985C06C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "Judge/resource.h"
#include "Game.h"
#include "vector"

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// PSDiploApp:
// See Diplo.cpp for the implementation of this class
//

class PSDiploApp : public CWinApp
{
public:
	PSDiploApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PSDiploApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(PSDiploApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	afx_msg void OnFileNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIPLO_H__37C86465_EB82_11D0_A968_985C06C10000__INCLUDED_)
