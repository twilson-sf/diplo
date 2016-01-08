// DVMDoc.cpp : implementation of the PSDVMDoc class
//

#include "stdafx.h"
#include "DVM.h"

#include "DVMDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern __declspec(dllimport) PSGame TG;

/////////////////////////////////////////////////////////////////////////////
// PSDVMDoc

IMPLEMENT_DYNCREATE(PSDVMDoc, CDocument)

BEGIN_MESSAGE_MAP(PSDVMDoc, CDocument)
	//{{AFX_MSG_MAP(PSDVMDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PSDVMDoc construction/destruction

PSDVMDoc::PSDVMDoc()
{
	m_strMainMap = "";
}

PSDVMDoc::~PSDVMDoc()
{
}

BOOL PSDVMDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_strMainMap = "";

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// PSDVMDoc serialization

void PSDVMDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// PSDVMDoc diagnostics

#ifdef _DEBUG
void PSDVMDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void PSDVMDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// PSDVMDoc commands

BOOL PSDVMDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath(lpszPathName, drive, dir, fname, ext);
	m_strMainMap = lpszPathName;
	if (!TG.Load((CString)drive + dir + fname + "._ps"))
		return FALSE;

	return TRUE;
}

BOOL PSDVMDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDocument::CanCloseFrame(pFrame);
}
