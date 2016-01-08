// DiploDoc.h : interface of the PSDiploDoc class
//
/////////////////////////////////////////////////////////////////////////////
#include "mystruct.h"

#if !defined(AFX_DIPLODOC_H__37C8646B_EB82_11D0_A968_985C06C10000__INCLUDED_)
#define AFX_DIPLODOC_H__37C8646B_EB82_11D0_A968_985C06C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#define MAX_VARIANTS 25

class PSDiploDoc : public CDocument
{
protected: // create from serialization only
	PSDiploDoc();
	DECLARE_DYNCREATE(PSDiploDoc)

// Attributes
public:

// Operations
public:

private:
	BOOL LoadOldFormat(CArchive& arh);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PSDiploDoc)
	public:
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnNewDocument();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~PSDiploDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(PSDiploDoc)
	afx_msg void OnNewGame(UINT);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIPLODOC_H__37C8646B_EB82_11D0_A968_985C06C10000__INCLUDED_)
