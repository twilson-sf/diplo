// DVMView.h : interface of the PSDVMView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DVMVIEW_H__FA1CA86E_9265_11D2_A96A_444553540000__INCLUDED_)
#define AFX_DVMVIEW_H__FA1CA86E_9265_11D2_A96A_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

enum Status {PROV_NONE, PROV_NEIGHBORS, PROV_UNITS, PROV_BORDERS, PROV_SC, 
	CST1_NEIGHBORS, CST1_UNITS, CST2_NEIGHBORS, CST2_UNITS};

#include "DVMDoc.h"

class PSDVMView : public CScrollView
{
protected: // create from serialization only
	PSDVMView();
	DECLARE_DYNCREATE(PSDVMView)

// Attributes
public:
	PSDVMDoc* GetDocument();
private:
	CString m_strMousePos;
	Status m_Status;
	HCURSOR m_hCursor;

// Operations
public:
	Status GetStatus()									{return m_Status;}
	void SetStatus(Status stat)							{m_Status = stat;}

protected:
	CString CheckPos(CPoint);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PSDVMView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	PSProvince* GetCurProv();
	virtual ~PSDVMView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	PSProvince* m_pCurProv;
	CBitmap* m_pMainMap;
	SIZE m_sizeMain;
	//{{AFX_MSG(PSDVMView)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnUpdateCurrentPos(CCmdUI* pCmdUI);
	afx_msg void OnWizardProvinces();
	afx_msg void OnProvCancel();
	afx_msg void OnProvOK();
	afx_msg void OnCoast1NeighborsButton();
	afx_msg void OnCoast1UnitsButton();
	afx_msg void OnCoast2NeighborsButton();
	afx_msg void OnCoast2UnitsButton();
	afx_msg void OnProvBicoastalButton();
	afx_msg void OnProvBordersButton();
	afx_msg void OnProvTypeButtons(UINT);
	afx_msg void OnProvNeighborsButton();
	afx_msg void OnProvScButton();
	afx_msg void OnProvUnitsButton();
	afx_msg void OnUpdateProvBicoastalButton(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCoastNames(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTypeButtons(CCmdUI*);
	afx_msg void OnUpdateRadioButtons(CCmdUI*);
	afx_msg void OnWizardPowers();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DVMView.cpp
inline PSDVMDoc* PSDVMView::GetDocument()
   { return (PSDVMDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DVMVIEW_H__FA1CA86E_9265_11D2_A96A_444553540000__INCLUDED_)
