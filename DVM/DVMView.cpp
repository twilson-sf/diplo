// DVMView.cpp : implementation of the PSDVMView class
//

#include "stdafx.h"
#include "DVM.h"
#include "DVMDoc.h"
#include "DVMView.h"
#include "Secjpeg.h"
#include "WelcomeProvEditor.h"
#include "WelcomePowEditor.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern __declspec(dllimport) PSGame TG;

/////////////////////////////////////////////////////////////////////////////
// PSDVMView

IMPLEMENT_DYNCREATE(PSDVMView, CScrollView)

BEGIN_MESSAGE_MAP(PSDVMView, CScrollView)
	//{{AFX_MSG_MAP(PSDVMView)
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_UPDATE_COMMAND_UI(ID_CURRENT_MOVE, OnUpdateCurrentPos)
	ON_COMMAND(ID_WIZARD_PROVINCES, OnWizardProvinces)
	ON_COMMAND(ID_PROV_CANCEL_BUTTON, OnProvCancel)
	ON_COMMAND(ID_PROV_OK_BUTTON, OnProvOK)
	ON_COMMAND(ID_COAST1_NEIGHBORS_BUTTON, OnCoast1NeighborsButton)
	ON_COMMAND(ID_COAST1_UNITS_BUTTON, OnCoast1UnitsButton)
	ON_COMMAND(ID_COAST2_NEIGHBORS_BUTTON, OnCoast2NeighborsButton)
	ON_COMMAND(ID_COAST2_UNITS_BUTTON, OnCoast2UnitsButton)
	ON_COMMAND(ID_PROV_BICOASTAL_BUTTON, OnProvBicoastalButton)
	ON_COMMAND(ID_PROV_BORDERS_BUTTON, OnProvBordersButton)
	ON_COMMAND_RANGE(ID_PROV_WATER_BUTTON, ID_PROV_DRY_BUTTON, OnProvTypeButtons)
	ON_COMMAND(ID_PROV_NEIGHBORS_BUTTON, OnProvNeighborsButton)
	ON_COMMAND(ID_PROV_SC_BUTTON, OnProvScButton)
	ON_COMMAND(ID_PROV_UNITS_BUTTON, OnProvUnitsButton)
	ON_UPDATE_COMMAND_UI(ID_PROV_BICOASTAL_BUTTON, OnUpdateProvBicoastalButton)
	ON_UPDATE_COMMAND_UI_RANGE(IDC_COAST1_NAME, IDC_COAST2_NAME, OnUpdateCoastNames)
	ON_UPDATE_COMMAND_UI_RANGE(ID_PROV_NEIGHBORS_BUTTON, ID_COAST2_UNITS_BUTTON, OnUpdateRadioButtons)
	ON_UPDATE_COMMAND_UI_RANGE(ID_PROV_WATER_BUTTON, ID_PROV_DRY_BUTTON, OnUpdateTypeButtons)
	ON_COMMAND(ID_WIZARD_POWER, OnWizardPowers)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PSDVMView construction/destruction

PSDVMView::PSDVMView() : 
m_pCurProv(NULL),
m_pMainMap(NULL),
m_Status(PROV_NONE)
{
}

PSDVMView::~PSDVMView()
{
	delete m_pMainMap;// This is new-ed by SECImage::MakeBitmap()
	delete m_pCurProv;// In case the user exits without closing the province bar.
}

BOOL PSDVMView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

void PSDVMView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// Set the cursor to normal.
	m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);

	// Load the main map.
	CString strMap = GetDocument()->GetMainMap();
	if (!strMap.IsEmpty())
	{
		SECJpeg img;
		img.LoadImage(strMap);
		CClientDC dc(this);
		m_pMainMap = img.MakeBitmap(&dc);
		if (!m_pMainMap)
			MessageBox("The " + strMap + " file was not found.\r\n"
				"Please make sure this file is located in the same folder as DVM.exe");
	}

	// Set the size of the view according to the dimensions of the main map.
	CSize sizeTotal;
	if (m_pMainMap)
	{
		BITMAP bmp;
		m_pMainMap->GetBitmap(&bmp);
		m_sizeMain.cx = sizeTotal.cx = bmp.bmWidth;
		m_sizeMain.cy = sizeTotal.cy = bmp.bmHeight;

		// Resize the main window.
		RECT rect;
		GetClientRect(&rect);
		int nXDif = sizeTotal.cx - rect.right + rect.left;
		int nYDif = sizeTotal.cy - rect.bottom + rect.top;
		CFrameWnd* pMainWnd = GetParentFrame();
		pMainWnd->GetWindowRect(&rect);
		pMainWnd->MoveWindow(rect.left, rect.top, 
			rect.right - rect.left + nXDif, rect.bottom - rect.top + nYDif);
	}
	else 
	{
		m_sizeMain.cx = sizeTotal.cx = 100;
		m_sizeMain.cy = sizeTotal.cy = 100;
	}

	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// PSDVMView drawing

void PSDVMView::OnDraw(CDC* pDC)
{
	PSDVMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!m_pMainMap)
		return;

	// Draw the main map.
	CDC* pM = new CDC;
	pM->CreateCompatibleDC(pDC);
	pM->SelectObject(*m_pMainMap);
	pDC->BitBlt(0, 0, m_sizeMain.cx, m_sizeMain.cy, pM, 0, 0, SRCCOPY);

	delete pM;

	// If the user is drawing borders, draw the polygon that defines the province.
	if (m_Status == PROV_BORDERS)
	{
		ASSERT(m_pCurProv);
		CPen pen;
		pen.CreatePen(PS_SOLID, 3, RGB(255, 0, 0));// Red
		CPen* pOldPen = pDC->SelectObject(&pen);
		if (!pOldPen)
			MessageBox("Error creating pen!");

		CPoint* pPt = (CPoint*)m_pCurProv->m_Vertexes.GetAt(0);
		pDC->MoveTo(*pPt);
		for (int j = 1; j < m_pCurProv->m_Vertexes.GetSize(); j++)
		{
			pDC->LineTo(*(pPt + j));
		}
		pDC->LineTo(*pPt);

		pen.DeleteObject();
		pDC->SelectObject(pOldPen);
	}
}

/////////////////////////////////////////////////////////////////////////////
// PSDVMView diagnostics

#ifdef _DEBUG
void PSDVMView::AssertValid() const
{
	CScrollView::AssertValid();
}

void PSDVMView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

PSDVMDoc* PSDVMView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(PSDVMDoc)));
	return (PSDVMDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// PSDVMView message handlers

BOOL PSDVMView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest == HTCLIENT)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}

	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

void PSDVMView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
	m_strMousePos = CheckPos(point);

	CScrollView::OnMouseMove(nFlags, point);
}

CString PSDVMView::CheckPos(CPoint point)
{
	for (int i = 1; i <= TG.GetNumberOfProvinces(); i++)
	{
		if (!(HRGN)TG.GetProvince(i)->m_poly)
			continue;

		if (TG.GetProvince(i)->m_poly.PtInRegion(point))
			return TG.GetProvince(i)->m_strName;
	}

	return "";
}

void PSDVMView::OnWizardProvinces() 
{
	PSWelcomeProvEditor dlgWelcome;
	if (dlgWelcome.DoModal() != IDOK)
		return;

	switch(dlgWelcome.m_nSelection)
	{
	case 0:// New
		m_pCurProv = new PSProvince;
		break;
	case 1:// Modify
		{
		PSProvince* pTempProv = TG.GetProvince(dlgWelcome.m_strSelectedProv);
		m_pCurProv = new PSProvince(pTempProv);
		break;
		}
	case 2:// Remove
		if (MessageBox("Are you sure you wish to delete it?", "DipSheet", MB_YESNO) == IDYES)
		{
			m_pCurProv = TG.GetProvince(dlgWelcome.m_strSelectedProv);
			TG.DeleteProvince(m_pCurProv);
		}
		return;
	default:
		ASSERT(0);
	}

	// Show the province bar.
	PSMainFrame* pFrame = (PSMainFrame*)GetParentFrame();
	ASSERT(pFrame);
	pFrame->DisplayBuildBar(TRUE);
}

void PSDVMView::OnWizardPowers() 
{
	PSWelcomePowEditor dlgWelcome;
	if (dlgWelcome.DoModal() == IDOK)
	{
	}
}

void PSDVMView::OnProvCancel() 
{
	// Close the province bar.
	PSMainFrame* pFrame = (PSMainFrame*)GetParentFrame();
	ASSERT(pFrame);
	pFrame->DisplayBuildBar(FALSE);

	m_Status = PROV_NONE;
	delete m_pCurProv;
	m_pCurProv = NULL;
	m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
}

void PSDVMView::OnProvOK() 
{
	// The user finished with the current province by pressing OK. Add m_pCurProv to TG
	// and if there already is a province with the same name, delete it.
	PSProvince* pTempProv = TG.GetProvince(m_pCurProv->m_strName);
	if (pTempProv)
		TG.DeleteProvince(pTempProv);
	TG.AddProvince(m_pCurProv);

	m_pCurProv = NULL;
	m_Status = PROV_NONE;

	// Close the province bar.
	PSMainFrame* pFrame = (PSMainFrame*)GetParentFrame();
	ASSERT(pFrame);
	pFrame->DisplayBuildBar(FALSE);
	OnWizardProvinces();// Show again the wizard.
}

void PSDVMView::OnCoast1NeighborsButton() 
{
	m_Status = CST1_NEIGHBORS;
}

void PSDVMView::OnCoast1UnitsButton() 
{
	m_Status = CST1_UNITS;
}

void PSDVMView::OnCoast2NeighborsButton() 
{
	m_Status = CST2_NEIGHBORS;
}

void PSDVMView::OnCoast2UnitsButton() 
{
	m_Status = CST2_UNITS;
}

void PSDVMView::OnProvBicoastalButton() 
{
	// If m_pCurProv is bicostal already, delete the two coasts. If it is not, 
	// add two new coasts.
	if (m_pCurProv->IsBicoastal())
	{
		m_pCurProv->RemoveCoasts();
		return;
	}

	PSCoast* pCst1 = new PSCoast(m_pCurProv);
	PSCoast* pCst2 = new PSCoast(m_pCurProv);
}

void PSDVMView::OnProvTypeButtons(UINT nID) 
{
	ASSERT(m_pCurProv);
	m_pCurProv->SetType(PROVINCE_TYPE(nID - ID_PROV_WATER_BUTTON));
}

void PSDVMView::OnProvBordersButton() 
{
	m_Status = PROV_BORDERS;
	// Redraw the province.
	CRect rect;
	m_pCurProv->m_poly.GetRgnBox(&rect);
	InvalidateRect(&rect);
	// Set the cursor to normal.
	m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_CROSS);
}

void PSDVMView::OnProvNeighborsButton() 
{
	m_Status = PROV_NEIGHBORS;
	// Redraw the province.
	CRect rect;
	m_pCurProv->m_poly.GetRgnBox(&rect);
	InvalidateRect(&rect);
	// Set the cursor to normal.
	m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_UPARROW);
}

void PSDVMView::OnProvUnitsButton() 
{
	m_Status = PROV_UNITS;
	// Redraw the province.
	CRect rect;
	m_pCurProv->m_poly.GetRgnBox(&rect);
	InvalidateRect(&rect);
	// Set the cursor to normal.
	m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZEALL);
}

void PSDVMView::OnProvScButton() 
{
	m_Status = PROV_SC;
	// Redraw the province.
	CRect rect;
	m_pCurProv->m_poly.GetRgnBox(&rect);
	InvalidateRect(&rect);
	// Set the cursor to normal.
	m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE);
}

/////////////////////////////////////////////////////////////////////////////
// Update Handlers
/////////////////////////////////////////////////////////////////////////////

void PSDVMView::OnUpdateProvBicoastalButton(CCmdUI* pCmdUI) 
{
	ASSERT(m_pCurProv);
	pCmdUI->SetCheck(m_pCurProv->IsBicoastal());
}

void PSDVMView::OnUpdateCoastNames(CCmdUI* pCmdUI) 
{
	ASSERT(m_pCurProv);
	pCmdUI->Enable(m_pCurProv->IsBicoastal());
}

void PSDVMView::OnUpdateRadioButtons(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(m_Status == Status(pCmdUI->m_nID - ID_PROV_NEIGHBORS_BUTTON + 1));
	if (pCmdUI->m_nID >= ID_COAST1_NEIGHBORS_BUTTON && 
		pCmdUI->m_nID <= ID_COAST2_UNITS_BUTTON)
		pCmdUI->Enable(m_pCurProv->IsBicoastal());
}

void PSDVMView::OnUpdateTypeButtons(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(m_pCurProv->GetType() == pCmdUI->m_nID - ID_PROV_WATER_BUTTON);
}

void PSDVMView::OnUpdateCurrentPos(CCmdUI* pCmdUI)
{
	pCmdUI->SetText(m_strMousePos);
}


PSProvince* PSDVMView::GetCurProv()
{
	return m_pCurProv;
}
