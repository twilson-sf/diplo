// DVMDoc.h : interface of the PSDVMDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DVMDOC_H__FA1CA86C_9265_11D2_A96A_444553540000__INCLUDED_)
#define AFX_DVMDOC_H__FA1CA86C_9265_11D2_A96A_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class PSDVMDoc : public CDocument
{
protected: // create from serialization only
	PSDVMDoc();
	DECLARE_DYNCREATE(PSDVMDoc)

// Attributes
public:
protected:
	CString m_strMainMap;

// Operations
public:
	CString GetMainMap()								{return m_strMainMap;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PSDVMDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~PSDVMDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(PSDVMDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DVMDOC_H__FA1CA86C_9265_11D2_A96A_444553540000__INCLUDED_)
