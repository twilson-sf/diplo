
// DiploView.cpp : implementation of the PSDiploView class
//

#include "StdAfx.h"
#include "Diplo.h"
#include "DiploView.h"
#include "TabView.h"
#include "MainFrm.h"
#include "Whichcoast.h"
#include "Fleet.h"
#include "Army.h"
#include "Secjpeg.h"
#include "Secdib.h"

#include "Judge.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern __declspec(dllimport) PSGame TG;

/////////////////////////////////////////////////////////////////////////////
// PSDiploView

IMPLEMENT_DYNCREATE(PSDiploView, CScrollView)

BEGIN_MESSAGE_MAP(PSDiploView, CScrollView)
	//{{AFX_MSG_MAP(PSDiploView)
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_COMMAND(IDC_ACTIONS_FASTENTRY, OnActionsFastentry)
	ON_COMMAND(IDC_CONVOYS, OnConvoys)
	ON_COMMAND(ID_FILE_BUILDGAME, OnFileBuildgame)
	ON_COMMAND(ID_FILE_MODIFY, OnFileModify)
	ON_COMMAND(IDC_OK, OnOk)
	ON_COMMAND(IDC_RESET, OnReset)
	ON_COMMAND(IDC_SUPPORTS, OnSupports)
	ON_COMMAND(ID_OK_BUILD_BUTTON, OnOkBuildButton)
	ON_COMMAND(ID_FILE_SAVE_AS_BITMAP, OnFileSaveAsBitmap)
	ON_COMMAND_RANGE(ID_FIRST_ARMY, ID_FIRST_SC - 1, OnBuildUnit)
	ON_COMMAND_RANGE(ID_FIRST_SC, ID_SEASON_BUTTON - 1, OnBuildSC)
	ON_UPDATE_COMMAND_UI(IDC_ACTIONS_FASTENTRY, OnUpdateActionsFastentry)
	ON_UPDATE_COMMAND_UI(ID_CURRENT_MOVE, OnUpdateCurrentMove)
	ON_UPDATE_COMMAND_UI(ID_CURRENT_TIME, OnUpdateCurrentTime)
	ON_UPDATE_COMMAND_UI(ID_MAIN_PANE, OnUpdateMainPane)
	ON_UPDATE_COMMAND_UI(IDC_OK, OnUpdateOk)
	ON_UPDATE_COMMAND_UI(IDC_RESET, OnUpdateReset)
	ON_UPDATE_COMMAND_UI(IDC_CONVOYS, OnUpdateSupports)
	ON_UPDATE_COMMAND_UI(IDC_SUPPORTS, OnUpdateSupports)
	ON_UPDATE_COMMAND_UI_RANGE(ID_FILE_SAVE, ID_FILE_SAVE_AS, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI_RANGE(ID_FILE_BUILDGAME, ID_FILE_MODIFY, OnUpdateFileBuild)
	ON_UPDATE_COMMAND_UI_RANGE(ID_FIRST_ARMY, ID_SEASON_BUTTON - 1, OnUpdateBuildButtons)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PSDiploView construction/destruction

PSDiploView::PSDiploView() :
m_nFastEntry(TRUE),
#ifdef _DEBUG
m_pGame(&TG),
#endif // _DEBUG
m_pMoves(NULL),
m_bEnableSave(TRUE),
m_pMainMap(NULL)
{
	PSUnit::SetView(this);
}


PSDiploView::~PSDiploView()
{
	Orders::iterator pJ;
	for (pJ = m_Orders.begin(); pJ != m_Orders.end(); pJ++)
	{
		delete *pJ;
	}
	m_Orders.clear();
	delete m_pMainMap;// This is new-ed by SECImage::MakeBitmap()
}


BOOL PSDiploView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// PSDiploView printing

BOOL PSDiploView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}


void PSDiploView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void PSDiploView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// PSDiploView diagnostics

#ifdef _DEBUG
void PSDiploView::AssertValid() const
{
	CScrollView::AssertValid();
}


void PSDiploView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// PSDiploView drawing

void PSDiploView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	PSMainFrame* pFrame = (PSMainFrame*)GetParentFrame();
	m_sizeUnit.cx = pFrame->GetUnitSize().cx;
	m_sizeUnit.cy = pFrame->GetUnitSize().cy;
	PSUnit::SetSizes(m_sizeUnit.cx, m_sizeUnit.cy);
	m_pMoves = (PSOrdersTab*)pFrame->GetDescendantWindow(ID_ORDERS);

	// Create the Build Bar, now that I know what it should look like.
	pFrame->CreateBuildBar();

	hnewCursor = AfxGetApp()->LoadCursor(IDC_NEWCURSOR);
	hnormalCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);

	Orders::iterator pO;
	for (pO = m_Orders.begin(); pO != m_Orders.end(); pO++)
	{
		delete (*pO);
	}
	m_Orders.clear();

	m_nSelectedBuildButton = 0;
	m_nEnableDisable[RESET_BUTTON] = 0;
	m_nEnableDisable[OK_BUTTON] = 0;	
	LoadTheBitMaps();

	// Load the history of the game. If there isn't any, create a new one.
	if (!TG.LoadHistory())
	{
		PSHistory* pHist = new PSHistory;
		TG.SetHistory(pHist);
		PSStep* pNewStep = new PSStep(TG.GetData());
		TG.SetCurrentStep(pNewStep);
		pHist->AddStep(pNewStep);
	}
	LoadHistoryTab(TG.GetHistory());
	UpdateInfoTab();
	UpdateInfoTab(TG.GetAllInfo());
	StartDiplo();
}


void PSDiploView::StartDiplo()
{
	m_strCurrentOrder = "";
	int nRetreats = TG.GetNumberOfRetreats();
	int nDisbands = TG.GetNumberOfDisbands();
	int nBuilds = TG.GetNumberOfBuilds();
	if (nRetreats)
	{
		DisplayBuildBar(FALSE);
	    DoRetreats();
	}
	else 
	{
		if (nDisbands || nBuilds)
		{
			DisplayBuildBar(TRUE);
			Adjustments();
		}
		else
		{
			DisplayBuildBar(FALSE);
			TG.SetStatus(GETORDER);
		}
	}
	UpdateHistoryTab();
	PSMainFrame* pFrame = (PSMainFrame*)GetParentFrame();
	pFrame->SetActiveView(this);
}


void PSDiploView::LoadTheBitMaps()
{
	m_Units.Detach();
	m_UnitList.DeleteImageList();

	CWinApp* pApp = AfxGetApp();
	CString strValue;
	strValue = pApp->GetProfileString("Directories", "Current");

	// Load the main map.
	if (!m_pMainMap)
	{
		CString strMainMap = strValue + TG.GetType() + ".jpg";
		SECJpeg img;
		img.LoadImage(strMainMap);
		CClientDC dc(this);
		m_pMainMap = img.MakeBitmap(&dc);
		if (!m_pMainMap)
			ErrorMessage("The " + strMainMap + " file was not found.\r\n"
				"Please make sure this file is located in the same folder as Diplo.exe");
	}

	// Load the Units.
	HBITMAP hMap = (HBITMAP) ::LoadImage(AfxGetInstanceHandle(), 
		strValue + TG.GetType() + "Units.bmp", IMAGE_BITMAP, 
		0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (!hMap)
		ErrorMessage("The " + TG.GetType() + "Units.bmp file was not found.\r\n"
			"Please make sure this file is located in the same folder as Diplo.exe");
	if (!m_UnitList.Create(m_sizeUnit.cx, m_sizeUnit.cy, ILC_MASK | ILC_COLOR8, 0, 1))
	{
		ErrorMessage("Error creating the units.");
		return;
	}
	m_Units.Attach(hMap);
	m_UnitList.Add(&m_Units, RGB(0xC0, 0xC0, 0xC0));

	// Set the size of the view according to the dimensions of the main map.
	CSize sizeTotal;
	BITMAP bmp;
	m_pMainMap->GetBitmap(&bmp);
	m_sizeMain.cx = sizeTotal.cx = bmp.bmWidth;
	m_sizeMain.cy = sizeTotal.cy = bmp.bmHeight;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


void PSDiploView::OnDraw(CDC* pDC)
{
	// Draw the main map.
	CDC* pM = new CDC;
	pM->CreateCompatibleDC(pDC);
	pM->SelectObject(*m_pMainMap);
	pDC->BitBlt(0, 0, m_sizeMain.cx, m_sizeMain.cy, pM, 0, 0, SRCCOPY);

	PSData* pData = TG.GetData();

	// Fill the SC-s.
	SCs vSCs = pData->GetSCs();
	for (int i = 0; i < vSCs.size(); i++)
	{
		SC* pSC = vSCs[i];
		PSPower* pPow = pSC->m_pPower;
		PSProvince* pPrv = pSC->m_pProvince;
		CBrush SupplyBrush(pPow->GetColor());
		pDC->FillRect(&(pPrv->m_SupplySquare), &SupplyBrush);
	}

	// Draw the units.
	for (i = 0; i < pData->GetNumberOfUnits(); i++)
	{
		PSUnit* pUnit = pData->GetUnit(i);
		if (pUnit->IsRetreating())
			continue;
		PSPower* pPow = pUnit->GetOwner();
		int nTemp = pPow->GetIndex() + (TG.GetNumberOfPowers() * (pUnit->GetType() == FLEET));
		if (nTemp)
		{
			m_UnitList.Draw(pDC, nTemp - 1, pUnit->GetPosition(), ILD_TRANSPARENT);
		}
	}

	// Draw the order number on the units.
	int nIndex = 0;
	Orders::iterator pJ;
	for (pJ = m_Orders.begin(); pJ != m_Orders.end(); pJ++)
	{
		if (TG.GetStatus() == GETORDER)
		{
			nIndex++;
			PSProvince* pPrvThis = (*pJ)->GetLocation();
			DrawNumber(nIndex, pPrvThis);
		}
	}
	delete pM;
}


void PSDiploView::OnMouseMove(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
	m_strMousePos = CheckPos(point);

	CScrollView::OnMouseMove(nFlags, point);
}


void PSDiploView::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL bCtrl = nFlags & MK_CONTROL;
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
	if (TG.GetStatus() == BUILDGAME)
	{
		OnLBDBuildGame();
		CScrollView::OnLButtonDown(nFlags, point);
		return;
	}
	if (m_strMousePos == "")
	{
		// Mouse cursor is not in a hot spot.
		CScrollView::OnLButtonDown(nFlags, point);
		return;
	}
	if (TG.GetStatus() == ENDIPLO) 
		TG.SetStatus(GETORDER);
	if (TG.GetStatus() != GETORDER)
	{
		// Getting a Retreat, a Disband or a Build.
		// Make sure the province hasn't been already entered.
		if (m_strCurrentOrder.Right(1) == " ")
		{
			m_strCurrentOrder += m_strMousePos;
			m_nEnableDisable[OK_BUTTON] = 1;
			if (m_nFastEntry) 
				OnOk();
		}
		CScrollView::OnLButtonDown(nFlags, point);
		return;
	}

	m_nEnableDisable[RESET_BUTTON] = 1;
	if (m_strCurrentOrder == "")
	{
		// Beginning of an order.
		m_strCurrentOrder = m_strMousePos;
		CScrollView::OnLButtonDown(nFlags, point);
		return;
	}
	
	// From here on: m_strMousePos != "", m_strCurrentOrder != "", 
	// TG.GetStatus() == GETORDER.
	int nLen = m_strCurrentOrder.GetLength();

	if (m_strMousePos == m_strCurrentOrder)
	{
		// Hold order.
		m_strCurrentOrder += " H";
		m_nEnableDisable[OK_BUTTON] = 1;
		if (m_nFastEntry) 
			OnOk();
		CScrollView::OnLButtonDown(nFlags, point);
		return;
	}

	if (nLen == 3 && !bCtrl)
	{
		// Direct Move.
		m_strCurrentOrder += " - ";
		m_strCurrentOrder += m_strMousePos;
		m_nEnableDisable[OK_BUTTON] = 1;
		if (m_nFastEntry) 
			OnOk();
		CScrollView::OnLButtonDown(nFlags, point);
		return;
	}

	if (nLen == 6)
	{
		// Middle of a Convoy or of a Support.
		m_strCurrentOrder += m_strMousePos;
		CScrollView::OnLButtonDown(nFlags, point);
		return;
	}

	if (nLen == 9 && m_strCurrentOrder[4] == 'S')
	{
		// Support.
		if (m_strMousePos == m_strCurrentOrder.Right(3))
		{
			// Support of a Hold.
			m_strCurrentOrder += " H";
		}
		else
		{
			// Support of a Move.
			m_strCurrentOrder += " - ";
			m_strCurrentOrder += m_strMousePos;
		}
		m_nEnableDisable[OK_BUTTON] = 1;
		if (m_nFastEntry) 
			OnOk();
		CScrollView::OnLButtonDown(nFlags, point);
		return;
	}

	if (nLen == 9 && m_strCurrentOrder[4] == 'C')
	{
		// Convoy.
		m_strCurrentOrder += " - ";
		m_strCurrentOrder += m_strMousePos;
		m_nEnableDisable[OK_BUTTON] = 1;
		if (m_nFastEntry) 
			OnOk();
		CScrollView::OnLButtonDown(nFlags, point);
		return;
	}

	// Convoyed Move.
	if (m_strCurrentOrder.GetLength() > 4 && m_strCurrentOrder[4] != '-')
	{
		CScrollView::OnLButtonDown(nFlags, point);
		return;
	}
	m_strCurrentOrder += " - ";
	m_strCurrentOrder += m_strMousePos;
	m_nEnableDisable[OK_BUTTON] = 1;
	if (!bCtrl)
	{
		if (m_nFastEntry) 
			OnOk();
		CScrollView::OnLButtonDown(nFlags, point);
		return;
	}

	CScrollView::OnLButtonDown(nFlags, point);
}


void PSDiploView::OnLBDBuildGame()
{
	CString strRight = m_strCurrentOrder.Right(3);
	if (m_strMousePos == "") 
		return;
	if (m_buildStatus == SCS && strRight == "ns " || strRight == "n: ")
	{
		m_strCurrentOrder += m_strMousePos;
		m_nEnableDisable[OK_BUTTON] = 1;
		if (m_nFastEntry) 
			OnOk();
	}
}


void PSDiploView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
	if (m_strMousePos != "" && !m_nEnableDisable[OK_BUTTON] && 
		m_strCurrentOrder.Right(3) == m_strMousePos)
	{
		m_strCurrentOrder += " H";
		m_nEnableDisable[OK_BUTTON] = 1 ;
		if (m_nFastEntry) 
			OnOk();
	}
	
	CScrollView::OnLButtonDblClk(nFlags, point);
}


void PSDiploView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
	if (m_strMousePos.GetLength() == 3 && TG.GetStatus() != BUILDGAME)
	{
		PSProvince* pPrv = TG.GetProvince(m_strMousePos);
		if (pPrv->GetOccupant() && m_strCurrentOrder == "")
		{
			m_strCurrentOrder = m_strMousePos;
			m_nEnableDisable[OK_BUTTON] = 0;
			m_nEnableDisable[RESET_BUTTON] = 1;
		}
		return;
	}
	if (TG.GetStatus() == BUILDGAME)
	{
		PSProvince* pPrv = TG.GetProvince(m_strMousePos);
		if (pPrv && m_buildStatus == UNITS)
		{
			if (pPrv->GetOccupant())
			{
				PSUnit* pUnit = pPrv->GetUnit();
				pUnit->Destroy();
			}
		}
		if (pPrv && m_buildStatus == SCS)
		{
			PSPower* pPow = pPrv->GetOwner();
			if (pPow)
			{
				CBrush GreyBrush(RGB(192, 192, 192));
				pPow->Loses(pPrv);
				CClientDC dc(this);
				OnPrepareDC(&dc);
				dc.FillRect(&(pPrv->m_SupplySquare), &GreyBrush);
			}
		}
	}

	CScrollView::OnRButtonDown(nFlags, point);
}


void PSDiploView::OnFileBuildgame()
{
	// Clear current game and open a new one.
	PSDiploDoc* pDoc = (PSDiploDoc*) GetDocument();
	if (!pDoc->SaveModified())
		return;

	pDoc->SetModifiedFlag(FALSE);
	PSDiploApp* pApp = (PSDiploApp*)AfxGetApp();
	pApp->OnFileNew();

	// Start from ZERO
	m_nEnableDisable[RESET_BUTTON] = 0;
	m_nEnableDisable[OK_BUTTON] = 0;	
	m_nSelectedBuildButton = 0;
	m_strCurrentOrder = "";
	StartAgain();
	PSData* pData = TG.GetData();
	pData->ClearRetreats();
	pData->ClearDisbands();
	pData->ClearBuilds();
	int nUnits = pData->GetNumberOfUnits();
	for (int i = 0; i < nUnits; i++)
	{
		PSUnit* pUnit = pData->GetUnit(0);
		pUnit->Destroy();
	}
	for (i = 1; i <= TG.GetNumberOfPowers(); i++)
	{
		PSPower* pPow = TG.GetPower(i);
		pPow->Clear();
	}
	PSHistory* pHist = TG.GetHistory();
	delete pHist;
	TG.SetHistory(NULL);
	LoadHistoryTab(NULL);
	Invalidate();
	
	// Show the Build Bar
	DisplayBuildBar(TRUE);
	UpdateInfoTab();
	TG.SetStatus(BUILDGAME);

	// Do not allow to save.
	m_bEnableSave = FALSE;
}


void PSDiploView::OnFileModify() 
{
	PSDiploDoc* pDoc = (PSDiploDoc*) GetDocument();
	PSHistory* pHist = TG.GetHistory();
	ASSERT(pHist);
	if (pHist->GetNumberOfSteps() != 1)
	{
		// Warn the user that modifying a game will delete the entire history.
		if (AfxMessageBox(IDS_MODIFY_WARNING, MB_YESNO | MB_ICONQUESTION) != IDYES)
			return;

		// The user still wants to modify the game.
		// Remove the history file.
		CString strName = pDoc->GetPathName();
		ASSERT(strName != "");
		char* path_buffer = strName.GetBuffer(_MAX_PATH);
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		_splitpath( path_buffer, drive, dir, fname, ext );
		strName.Format("%s%s%s%s", drive, dir, fname, ".dph");
		TRY
		{
			CFile::Remove(strName);
		}
		CATCH(CFileException, fe)
		{
			ErrorMessage("The file " + strName + " couldn't be removed.");
		}
		END_CATCH
	}

	// Delete the game's history.
	delete pHist;
	TG.SetHistory(NULL);
	LoadHistoryTab(NULL);
	StartAgain();
	pDoc->SetModifiedFlag(FALSE);
	
	// Show the Build Bar
	DisplayBuildBar(TRUE);
	UpdateInfoTab();
	TG.SetStatus(BUILDGAME);
}


void PSDiploView::OnConvoys()
{
	m_strCurrentOrder += " C ";
}


void PSDiploView::OnSupports()
{
	m_strCurrentOrder += " S ";
}


void PSDiploView::OnReset()
{
	m_strCurrentOrder = "";
	m_nSelectedBuildButton = 0;
	m_nEnableDisable[RESET_BUTTON] = 0;
	m_nEnableDisable[OK_BUTTON] = 0;	
}


void PSDiploView::OnOk()
{
	if (TG.GetStatus() == GETORDER) OnOkGetOrder();
	else if (TG.GetStatus() == GETRETREAT) OnOkGetRetreat();
		else if (TG.GetStatus() == GETBUILD) OnOkGetBuild();
			else if (TG.GetStatus() == GETDISBAND) OnOkGetDisband();
				else if (TG.GetStatus() == BUILDGAME)
				{
					OnOkBuildGame();
					return;
				}
	m_nEnableDisable[RESET_BUTTON] = 0;		
}


CString PSDiploView::CheckPos(CPoint point)
{
	for (int i = 1; i <= TG.GetNumberOfProvinces(); i++)
		if ((TG.GetProvince(i)->m_poly).PtInRegion(point))
			return TG.GetProvince(i)->m_strName;

	return "";
}


void PSDiploView::Adjudicate()
{
	PSJudge theJudge;
	CString strVerdict = theJudge.Adjudicate(&m_Orders);

	m_nEnableDisable[RESET_BUTTON] = 0;
	m_nEnableDisable[OK_BUTTON] = 0;
	TG.SetStatus(NONE);
	
	// Mark the advancing units and save their orders in PSData::m_arStrMovingUnits.
	PSData* pData = TG.GetData();
	pData->m_arStrMovingUnits.RemoveAll();
	for (Orders::iterator pJ = m_Orders.begin(); pJ != m_Orders.end(); pJ++)
	{
		if ((*pJ)->GetType() == ADVANCING)
		{
			CString strMove = (*pJ)->m_strOutput.Left(3) + " - " + (*pJ)->GetTarget()->m_strName;
			pData->m_arStrMovingUnits.Add(strMove);
			PSUnit* pUnit = (*pJ)->GetUnit();
			pUnit->SetMoving((*pJ)->GetTarget());
		}
	}
	Invalidate();
	UpdateInfoTab();
	PSMainFrame* pFrame = (PSMainFrame*)GetParentFrame();
	UINT nTimer = SetTimer(ID_MOVE_TIMER, TIMER_PACE, NULL);
	ASSERT(nTimer);

	// Display the verdict and clean up.
	UpdateInfoTab(strVerdict);

	m_strCurrentOrder = "";
	TG.SetStatus(NONE);
	for (Orders::iterator pO = m_Orders.begin(); pO != m_Orders.end(); pO++)
	{
		delete *pO;
	}
	m_Orders.clear();
	m_pMoves->ResetContent();
	TransferDoc();
}


void PSDiploView::TransferDoc()
{
	PSDiploDoc* pDoc = (PSDiploDoc*) GetDocument();
	pDoc->SetModifiedFlag(TRUE);
}


void PSDiploView::OnOkGetOrder()
{
	m_nEnableDisable[OK_BUTTON] = 0;

	// Activate the orders tab.
	ActivateTab(ID_ORDERS);

	CString str;
	str = m_strCurrentOrder.Left(3);
	PSProvince* pPrv = TG.GetProvince(str);
	if (!pPrv->GetOccupant())
	{
		ErrorMessage("There is no unit in " + str + ".");
		return;
	}
	int nIndex = 1;
	Orders::iterator pJ;
	for (pJ = m_Orders.begin(); pJ != m_Orders.end(); pJ++)
	{
		nIndex++;
		if ((*pJ)->GetLocation() == pPrv)
		{
			m_pMoves->SelectString(nIndex - 2);
			ErrorMessage("Unit in " + str + " has already received an order.");
			return;
		}
	}
	PSOrder* pOrd = new PSOrder(m_strCurrentOrder);
	m_Orders.push_back(pOrd);

	CString strTemp;
	strTemp.Format("%d. " + m_strCurrentOrder, m_Orders.size());
	m_pMoves->AddThisString(strTemp);
	DrawNumber(nIndex, pOrd->GetLocation());
	m_strCurrentOrder = "";	
}


void PSDiploView::OnOkGetRetreat()
{
	PSProvince* pDest = TG.GetProvince(m_strCurrentOrder.Right(3));
	Retreat* pThisRet = TG.GetRetreat(TG.GetNumberOfRetreats() - 1);
	PSJudge theJudge;
	BOOL bLegal = theJudge.GetRetreat(pDest);

	if (bLegal)
	{
		m_bEnableSave = FALSE;
		PSUnit* pUnit = pThisRet->m_pUnit;

		// Put this retreat as a CString in m_arStrMovingUnits.
		CString strMove = pUnit->GetLocation()->m_strName;
		strMove += " - ";// It doesn't really matter what's here as long as it's 3 characters long.
		strMove += m_strCurrentOrder.Right(3);

		PSData* pData = TG.GetData();
		pData->m_arStrMovingUnits.Add(strMove);

		// Move the unit.
		pUnit->SetMoving(pDest);
		PSMainFrame* pFrame = (PSMainFrame*)GetParentFrame();
		UINT nTimer = SetTimer(ID_MOVE_TIMER, TIMER_PACE, NULL);
		ASSERT(nTimer);
		m_strCurrentOrder = "";
		TG.DeleteRetreat(pThisRet);
		return;
	}

	int nTemp = MessageBox("Illegal Entry. Is this a Disband ?", 
		"Diplo", MB_ICONEXCLAMATION | MB_YESNO);
	if (nTemp == IDYES)
	{
		PSUnit* pThisUnit = pThisRet->m_pUnit;
		pThisUnit->Destroy();
		TG.DeleteRetreat(pThisRet);
	}
	
	DoRetreats();
}


void PSDiploView::OnOkGetDisband()
{
	ASSERT(TG.GetNumberOfDisbands());
	Disband* pThisDisband = TG.GetDisband(TG.GetNumberOfDisbands() - 1);
	PSProvince* pPrv = TG.GetProvince(m_strCurrentOrder.Right(3));

	// Check for illegal entries.
	if (!pPrv->GetOccupant())
	{
		ErrorMessage("That province is empty. Enter again.");
		Adjustments();
		return;
	}
	if (pThisDisband->m_pPower != pPrv->GetOccupant())
	{
		ErrorMessage("That unit doesn't belong to " +
			pThisDisband->m_pPower->m_strName + 
			".\r\nEnter again.");
		Adjustments();
		return;
	}

	// The entry is legal.
	PSUnit* pUnit = pPrv->GetUnit();

	// Clear the unit.
	pUnit->Destroy();

	// Remove this disband from the vector of disbands.
	TG.DeleteDisband(pThisDisband);
	m_bEnableSave = FALSE;
	// TransferDoc();
	Adjustments();
}


void PSDiploView::OnOkGetBuild()
{
	Build* pThisBuild = TG.GetBuild(TG.GetNumberOfBuilds() - 1);

	// Create a unit and pass the build order to the Judge. If bLegal becomes FALSE, 
	// destroy the unit.
	PSProvince* pPrv = TG.GetProvince(m_strCurrentOrder.Right(3));
	ASSERT(pPrv);
	UNIT_TYPE uType;
	if (m_nSelectedBuildButton < ID_FIRST_FLEET)
		uType = ARMY;
	else
		uType = FLEET;
	PSJudge theJudge;
	PSUnit* pUnit = theJudge.GetBuild(uType, pPrv, pThisBuild->m_pPower);

	if (pUnit == NULL)
	{
		int nRes = MessageBox("Illegal Entry. Is this a 'No Build' ?", 
			"Diplo", MB_ICONEXCLAMATION | MB_YESNO);
		if (nRes == IDYES) 
		{
			TG.DeleteBuild(pThisBuild);
		}
		Adjustments();
		return;
	}

	// Build is legal.
	TG.GetData()->AddUnit(pUnit);
	pUnit->SetCoast(pUnit->GetDesiredCoast());
	pUnit->Draw();

	// Remove this build from vector of builds.
	TG.DeleteBuild(pThisBuild);
	m_bEnableSave = FALSE;
	Adjustments();		
}


void PSDiploView::OnOkBuildGame()
{
	PSProvince* pProvince = TG.GetProvince(m_strCurrentOrder.Right(3));
	ASSERT(pProvince);
	PSPower* pPower = NULL;
	if (m_nSelectedBuildButton < ID_FIRST_FLEET)
		pPower = TG.GetPower(m_nSelectedBuildButton - ID_FIRST_ARMY + 1);
	else if (m_nSelectedBuildButton < ID_FIRST_SC)
		pPower = TG.GetPower(m_nSelectedBuildButton - ID_FIRST_FLEET + 1);
	else 
		pPower = TG.GetPower(m_nSelectedBuildButton - ID_FIRST_SC + 1);
	ASSERT(pPower);
	if (m_buildStatus == SCS)
	{
		OnOkBuildSC(pPower);
		return;
	}

	// If the province is occupied delete the occupying unit.
	PSUnit* pUnit = pProvince->GetUnit();
	if (pUnit)
		pUnit->Destroy();

	// Create a unit and pass the build order to the Judge. If bLegal becomes FALSE, 
	// destroy the unit.
	if (m_nSelectedBuildButton < ID_FIRST_FLEET)
		pUnit = new PSArmy(pPower, pProvince);
	else 
		pUnit = new PSFleet(pPower, pProvince);

	// See if there are already 34 units on the map.
	if (TG.GetNumberOfUnits() == TG.GetNumberOfSCs())
	{
		CString strMessage;
		strMessage.Format("You cannot build more than %d units !", TG.GetNumberOfSCs());
		ErrorMessage(strMessage);
		m_strCurrentOrder = "";
		delete pUnit;
		return;
	}

	PSJudge theJudge;
	BOOL bLegal = theJudge.GetBuildGame(pUnit);

	if (!bLegal)
	{
		m_nEnableDisable[OK_BUTTON] = 0;
		m_strCurrentOrder = m_strCurrentOrder.Left(m_strCurrentOrder.GetLength() - 3);
		delete pUnit;
		return;
	}

	// Build is legal.
	TG.GetData()->AddUnit(pUnit);
	PSCoast* pDesiredCoast = pUnit->GetDesiredCoast();
	pUnit->SetCoast(pDesiredCoast);
	pUnit->SetPosition(pDesiredCoast->m_ptDrawUnit);
	pUnit->Draw();

	m_nEnableDisable[OK_BUTTON] = 0;
	m_strCurrentOrder = m_strCurrentOrder.Left(m_strCurrentOrder.GetLength() - 3);
}


void PSDiploView::OnOkBuildSC(PSPower* pPower)
{
	CString rig = m_strCurrentOrder.Right(3);
	PSProvince* pProvince = TG.GetProvince(rig);
	if (!pProvince->GetSC())
	{
		CString strTemp = m_strCurrentOrder;
		ErrorMessage(pProvince->m_strName + " is not a Supply Center.");
		m_strCurrentOrder = strTemp.Left(strTemp.GetLength() - 3);
		m_nEnableDisable[OK_BUTTON] = 0;
		return;
	}
	PSPower* pOwner = pProvince->GetOwner();
	if (pOwner)
	{
		pOwner->Loses(pProvince);
	}
	pPower->Gains(pProvince);
	CBrush SupplyBrush(pPower->GetColor());
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.FillRect(&(pProvince->m_SupplySquare), &SupplyBrush);
	m_strCurrentOrder = m_strCurrentOrder.Left(m_strCurrentOrder.GetLength() - 3);
	m_nEnableDisable[OK_BUTTON] = 0;	
}


void PSDiploView::StartAgain()
{
	if (TG.GetStatus() == BUILDGAME) 
		OnFileBuildgame();
	else
	{
		Orders::iterator pJ;
		for (pJ = m_Orders.begin(); pJ != m_Orders.end(); pJ++)
		{
			PSUnit* pUnit = (*pJ)->GetUnit();
			pUnit->Draw();
			delete *pJ;
		}
		m_Orders.clear();
		m_pMoves->ResetContent();
	}
	m_strCurrentOrder = "";
}


void PSDiploView::OnUpdateSupports(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_strCurrentOrder.GetLength() == 3);	
}


void PSDiploView::OnUpdateOk(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_nEnableDisable[OK_BUTTON]);	
}


void PSDiploView::OnUpdateReset(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_nEnableDisable[RESET_BUTTON]);	
}


void PSDiploView::OnUpdateFileBuild(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TG.GetStatus() != BUILDGAME);
}


void PSDiploView::LoadHistoryTab(PSHistory* pHist)
{
	CWnd* pFrame = GetParentFrame();
	PSHistoryTab* pHistTab = (PSHistoryTab*)pFrame->GetDescendantWindow(ID_HISTORY);
	ASSERT (pHistTab);
	pHistTab->Load(pHist);
}


void PSDiploView::UpdateHistoryTab()
{
	CWnd* pFrame = GetParentFrame();
	// Select the corresponding item in History tab.
	PSHistoryTab* pHistTab = (PSHistoryTab*)pFrame->GetDescendantWindow(ID_HISTORY);
	PSStep* pCurrentStep = TG.GetCurrentStep();
	// If the item is not in the tree, insert it.
	if (!pHistTab->GetItem(pCurrentStep))
		pHistTab->InsertStep(pCurrentStep);
	pHistTab->SelectItem(pHistTab->GetItem(pCurrentStep));
}


void PSDiploView::OnActionsFastentry() 
{
	CWnd* pFrame = GetParentFrame();
	CMenu* pMenu = pFrame->GetMenu();
	if (!m_nFastEntry)
	{
		pMenu->CheckMenuItem(IDC_ACTIONS_FASTENTRY, MF_CHECKED);
		m_nFastEntry = TRUE;
	}
	else
	{
		pMenu->CheckMenuItem(IDC_ACTIONS_FASTENTRY, MF_UNCHECKED);
		m_nFastEntry = FALSE;
	}	
}


void PSDiploView::OnUpdateActionsFastentry(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nFastEntry);
}


void PSDiploView::OnUpdateCurrentMove(CCmdUI* pCmdUI)
{
	pCmdUI->SetText(m_strCurrentOrder);
}


void PSDiploView::OnUpdateCurrentTime(CCmdUI* pCmdUI)
{
	CString strText;
	strText.Format((TG.GetSeason() == FALL) ? "Fall %d" : "Spring %d", TG.GetYear());
	pCmdUI->SetText(strText);
}


void PSDiploView::OnUpdateMainPane(CCmdUI* pCmdUI)
{
	pCmdUI->SetText(m_strMousePos);
}


void PSDiploView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if (TG.GetStatus() == BUILDGAME) 
		return;
	if (m_strMousePos.GetLength() == 3 && m_strCurrentOrder.GetLength() == 3)
	{
		PSProvince* pPrv = TG.GetProvince(m_strMousePos);
		if (pPrv->GetOccupant())
		{
			CMenu menu;
			menu.LoadMenu(IDR_CONTEXTMENU);
			CMenu* pContextMenu = menu.GetSubMenu(0);
			pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,
				point.x, point.y, this);
		}
		return;
	}	
}


void PSDiploView::DeleteOrder(int nCurSel)
{
	CString strTemp;
	PSOrder* pOrd = m_Orders[nCurSel];
	m_Orders.erase(m_Orders.begin() + nCurSel);
	PSUnit* pUnit = pOrd->GetUnit();
	pUnit->Draw();
	delete pOrd;
	
	// Clear the old order numbers and draw the new ones (stored in nIndex).
	m_pMoves->Clear();
	Orders::iterator pJ;
	int nIndex = 0;
	for (pJ = m_Orders.begin(); pJ != m_Orders.end(); pJ++)
	{
		nIndex++;
		pUnit = (*pJ)->GetUnit();
		pUnit->Draw();
		DrawNumber(nIndex, (*pJ)->GetLocation());

		// Add orders in the list box.
		strTemp.Format("%d. " + (*pJ)->m_strOutput, nIndex);
		m_pMoves->AddThisString(strTemp);
	}
}


BOOL PSDiploView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest == HTCLIENT)
	{
		if (m_strMousePos == "") 
			::SetCursor(hnormalCursor);
		else 
			::SetCursor(hnewCursor);
		return TRUE;
	}

	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}


void PSDiploView::DrawNumber(int nIndex, PSProvince* pPrv)
{
	// Draws the order number on the unit in pOrd->GetLocation()
	CClientDC dc(this);
	OnPrepareDC(&dc);
	CFont myFont;
	myFont.CreateFont(9,6,0,0,0,0,0,0,0,0,0,0,0,NULL);
	CFont* pFont = dc.SelectObject(&myFont);
	char buf[3];
	int len = (nIndex > 9) ? 2 : 1;
	_itoa(nIndex, buf, 10);
	PSCoast* pCoast = pPrv->GetUnit()->GetCoast();
	dc.TextOut(pCoast->m_ptDrawUnit.x + 1, pCoast->m_ptDrawUnit.y + 1, buf, len);
	dc.SelectObject(pFont);
}


void PSDiploView::OnBuildUnit(UINT nID)
{
	m_nSelectedBuildButton = nID;
	m_buildStatus = UNITS;
	if (TG.GetStatus() == BUILDGAME)
		m_nEnableDisable[RESET_BUTTON] = 1;
	if (nID < ID_FIRST_FLEET)
		m_strCurrentOrder = TG.GetPower(nID - ID_FIRST_ARMY + 1)->m_strName;
	else 
		m_strCurrentOrder = TG.GetPower(nID - ID_FIRST_FLEET + 1)->m_strName;
	m_strCurrentOrder += " builds one ";
	if (nID < ID_FIRST_FLEET)
		m_strCurrentOrder += "Army";
	else 
		m_strCurrentOrder += "Fleet";
	m_strCurrentOrder += " in: ";
}


void PSDiploView::OnBuildSC(UINT nID)
{
	m_nSelectedBuildButton = nID;
	m_buildStatus = SCS;
	m_nEnableDisable[RESET_BUTTON] = 1;
	m_strCurrentOrder = TG.GetPower(nID - ID_FIRST_SC + 1)->m_strName;
	m_strCurrentOrder += " owns ";
}


void PSDiploView::OnUpdateBuildButtons(CCmdUI* pCmdUI)
{
	if (TG.GetStatus() == BUILDGAME)
	{
		pCmdUI->Enable(TRUE);
		pCmdUI->SetRadio(pCmdUI->m_nID == m_nSelectedBuildButton);
		return;
	}

	if (TG.GetStatus() == GETBUILD)
	{
		if (m_nSelectedBuildButton < ID_FIRST_FLEET)
			pCmdUI->Enable(pCmdUI->m_nID == m_nSelectedBuildButton || 
				pCmdUI->m_nID - ID_FIRST_FLEET + ID_FIRST_ARMY == m_nSelectedBuildButton);
		else
			pCmdUI->Enable(pCmdUI->m_nID == m_nSelectedBuildButton || 
				pCmdUI->m_nID + ID_FIRST_FLEET - ID_FIRST_ARMY == m_nSelectedBuildButton);

		pCmdUI->SetRadio(pCmdUI->m_nID == m_nSelectedBuildButton);
		return;
	}
}


void PSDiploView::DisplayBuildBar(BOOL bShow)
{
	PSMainFrame* pFrame = (PSMainFrame*)GetParentFrame();
	pFrame->DisplayBuildBar(bShow);
}


void PSDiploView::SetYear(int nYear) 
{
	TG.SetYear(nYear);	
}


void PSDiploView::EndDiplo()
{
	for (int i = 1; i <= TG.GetNumberOfProvinces(); i++) 
	{
		TG.GetProvince(i)->SetStandoff(FALSE);
		TG.GetProvince(i)->SetBattleground(FALSE);
	}
	if (TG.GetSeason() == FALL) 
	{
		TG.SetYear(TG.GetYear() + 1);
		TG.SetSeason(SPRING);
	}
	else 
		TG.SetSeason(FALL);
	TransferDoc();
	m_bEnableSave = TRUE;
	m_strCurrentOrder = "";
	TG.SetStatus(ENDIPLO);
	m_nEnableDisable[OK_BUTTON] = 0;
	m_nEnableDisable[RESET_BUTTON] = 0;
	Orders::iterator pJ;
	for (pJ = m_Orders.begin(); pJ != m_Orders.end(); pJ++)
	{
		delete (*pJ);
	}
	m_Orders.clear();

	// Add this step to the history.
	PSStep* pStep = new PSStep(TG.GetData());
	PSHistory* pHist = TG.GetHistory();
	ASSERT(pHist);
	pHist->AddStep(pStep, TG.GetCurrentStep());
	LoadHistoryTab(pHist);
	TG.SetCurrentStep(pStep);
	UpdateHistoryTab();
}


void PSDiploView::DoRetreats(BOOL bAddToHistory)
{
	m_nEnableDisable[OK_BUTTON] = 0;
	m_nEnableDisable[RESET_BUTTON] = 0;

	int nRet = TG.GetNumberOfRetreats();
	if (nRet)
	{
		TG.SetStatus(GETRETREAT);
		if (bAddToHistory)
		{
			PSStep* pStep = new PSStep(TG.GetData());
			PSHistory* pHist = TG.GetHistory();
			ASSERT(pHist);
			pHist->AddStep(pStep, TG.GetCurrentStep());
			LoadHistoryTab(pHist);
			TG.SetCurrentStep(pStep);
			UpdateHistoryTab();
		}
		
		Retreat* pRet = TG.GetRetreat(nRet - 1);
		ASSERT (!pRet->m_Possib.empty());
		m_strCurrentOrder = ((pRet->m_pUnit->GetType() == ARMY) ? "Army in " : "Fleet in ") + 
			pRet->m_pUnit->GetLocation()->m_strName + " retreats to ";
		return;
	}

	if (TG.GetSeason() == FALL)
	{
		PSJudge TheJudge;
		CString strAdj = TheJudge.GetAdjustments();
		if (strAdj != "")
		{
			UpdateInfoTab(strAdj);
			ActivateTab(ID_INFO);
		}
	}

	m_strCurrentOrder = "";

	m_nSelectedBuildButton = 0;
	Adjustments(TRUE);
}


void PSDiploView::Adjustments(BOOL bAddToHistory)
{
	int nBuilds = TG.GetNumberOfBuilds();
	int nDisbands = TG.GetNumberOfDisbands();
	if (nBuilds || nDisbands)
	{
		TG.SetStatus(GETBUILD);
		if (bAddToHistory)
		{
			Invalidate();
			PSStep* pStep = new PSStep(TG.GetData());
			PSHistory* pHist = TG.GetHistory();
			ASSERT(pHist);
			pHist->AddStep(pStep, TG.GetCurrentStep());
			LoadHistoryTab(pHist);
			TG.SetCurrentStep(pStep);
			UpdateHistoryTab();
		}
	}

	if (nBuilds)
	{
		TG.SetStatus(GETBUILD);
		m_nEnableDisable[OK_BUTTON] = 0;
		m_nEnableDisable[RESET_BUTTON] = 0;
		DisplayBuildBar(TRUE);
		Build* pBui = TG.GetBuild(nBuilds - 1);
		ASSERT(pBui->m_pPower->GetNumberOfSCs() > pBui->m_pPower->GetNumberOfUnits());
		int nPowers = TG.GetNumberOfPowers();
		int nIndex = m_nSelectedBuildButton + nPowers - 2;
		if (m_nSelectedBuildButton >= ID_FIRST_FLEET)
			nIndex -= 100;
		if (!m_nSelectedBuildButton || TG.GetPower(nIndex % nPowers + 1) != pBui->m_pPower)
			m_nSelectedBuildButton = ID_FIRST_ARMY + pBui->m_pPower->GetIndex() - 1;
		m_strCurrentOrder.Format(pBui->m_pPower->m_strName + " builds one ");
		if (m_nSelectedBuildButton < ID_FIRST_FLEET)
			m_strCurrentOrder += "Army in ";
		else 
			m_strCurrentOrder += "Fleet in ";

		return;
	}

	DisplayBuildBar(FALSE);

	if (nDisbands)
	{
		TG.SetStatus(GETDISBAND);
		m_nEnableDisable[OK_BUTTON] = 0;
		m_nEnableDisable[RESET_BUTTON] = 0;
		Disband* pDis = TG.GetDisband(nDisbands - 1);
		ASSERT(pDis->m_pPower->GetNumberOfUnits() > pDis->m_pPower->GetNumberOfSCs());
		m_strCurrentOrder.Format(pDis->m_pPower->m_strName + " disbands the unit in ");
		return;
	}

	EndDiplo();	
	UpdateHistoryTab();
}


void PSDiploView::ErrorMessage(CString str)
{
	MessageBox(str, "Diplo", MB_ICONEXCLAMATION);
	m_strCurrentOrder = "";			
}


void PSDiploView::OnOkBuildButton() 
{
	// This is the handler for the Close button in the Build Bar.
	m_strCurrentOrder = "";
	TransferDoc();
	TG.SetStatus(GETORDER);
	m_nEnableDisable[RESET_BUTTON] = 0;
	m_nEnableDisable[OK_BUTTON] = 0;
	m_nSelectedBuildButton = 0;
	// Create a new history and load it in the tab.
	PSHistory* pHist = new PSHistory;
	TG.SetHistory(pHist);
	PSStep* pNewStep = new PSStep(TG.GetData());
	TG.SetCurrentStep(pNewStep);
	pHist->AddStep(pNewStep);
	LoadHistoryTab(pHist);
	DisplayBuildBar(FALSE);
	UpdateInfoTab(TG.GetAllInfo());

	// Allow game to be saved.
	m_bEnableSave = TRUE;
}


void PSDiploView::ActivateTab(UINT uID)
{
	CWnd* pFrame = GetParentFrame();
	PSDiploTabCtrl* pTabCtrl= (PSDiploTabCtrl*)pFrame->GetDescendantWindow(ID_TAB);
	ASSERT(pTabCtrl);
	int nSel = 0;
	switch (uID)
	{
	case ID_INFO:
		nSel = 0;
		break;
	case ID_HISTORY:
		nSel = 1;
		break;
	case ID_ORDERS:
		nSel = 2;
		break;
	default:
		ASSERT(0);
		break;
	}

	// Select the tab only if it is not the current selection already.
	if (pTabCtrl->GetCurSel() != nSel)
		pTabCtrl->SetCurSel(nSel);
}


void PSDiploView::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
	case ID_MOVE_TIMER:
		MoveUnits();
		break;
	default:
		ASSERT(0);
	}

	CScrollView::OnTimer(nIDEvent);
}


void PSDiploView::MoveUnits() 
{
	PSMainFrame* pFrame = (PSMainFrame*)GetParentFrame();
	pFrame->EnableHistoryButtons(FALSE);
	BOOL bMovingUnits = FALSE;
	for (int i = 0; i < TG.GetNumberOfUnits(); i++)
	{
		PSUnit* pUnit = TG.GetUnit(i);
		ASSERT(pUnit);
		if (pUnit->IsMoving())
		{
			bMovingUnits = TRUE;
			pUnit->Move(pFrame->GetJump(), pFrame->GetAnimate());
		}
	}
	if (!bMovingUnits)
	{
		pFrame->SetActiveView(this);
		KillTimer(ID_MOVE_TIMER);
		pFrame->EnableHistoryButtons(TRUE);

		switch (TG.GetStatus())
		{
		case NONE:{
			// Comes from Adjudicate().
			PSJudge J;
			CString strRetreats = J.GetRetreats();
			UpdateInfoTab(strRetreats);
			DoRetreats(TRUE);
			break; }
		case GETRETREAT:
			// Comes from getting a Retreat.
			DoRetreats();
			break;
		default: {
			// Comes from browsing forward through the history.
			PSHistory* pHist = TG.GetHistory();
			ASSERT(pHist);
			PSStep* pNextStep = pHist->GetNextStep(TG.GetCurrentStep());
			ASSERT(pNextStep);
			TG.SetCurrentStep(pNextStep);
			StartDiplo();
			break; }
		}
		pFrame->SetActiveView(this);
	}
}


void PSDiploView::OnFileSaveAsBitmap() 
{
	// Create a CBitmap from the view.
	CClientDC dc(this);
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;

	if (!bitmap.CreateCompatibleBitmap(&dc, m_sizeMain.cx, m_sizeMain.cy))
	{
		ErrorMessage(_T("Couldn't create bitmap."));
		return;
	}

	CBitmap* pOldBmp = MemDC.SelectObject(&bitmap);
	OnDraw(&MemDC);
	
	// Get the name of the file to save to.
	LPSTR lpszFilter = "Images (*.jpg)|*.jpg|Bitmaps(*.bmp)|*.bmp||";
	CString strName = GetDocument()->GetTitle();
	char* path_buffer = strName.GetBuffer(_MAX_PATH);
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath(path_buffer, drive, dir, fname, ext);
	CFileDialog dlg(FALSE, "jpg", fname, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER, lpszFilter);
	CString strPath;
	if (dlg.DoModal() != IDOK)
		return;

	strPath = dlg.GetPathName();
	CString strExt = dlg.GetFileExt();

	// Save the image.
	if (strExt == "bmp")
	{
		SECDib img;
		img.CreateFromBitmap(&dc, &bitmap);
		img.SaveImage(strPath);
	}
	else 
	{
		if (strExt != "jpg")
			ErrorMessage("Error while saving image: " + strExt + ": Unknown format.");

		else 
		{
			SECJpeg img;
			img.CreateFromBitmap(&dc, &bitmap);
			img.SaveImage(strPath);
		}
	}
}


void PSDiploView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	CScrollView::OnPrepareDC(pDC, pInfo);

	if (pDC->IsPrinting())
	{
		pDC->SetMapMode(MM_ISOTROPIC);
		long xExt = (long)m_sizeMain.cx * pDC->GetDeviceCaps(LOGPIXELSX) / 100;
		int nZoom = xExt / m_sizeMain.cx;

		pDC->SetWindowExt(m_sizeMain.cx / nZoom, m_sizeMain.cy / nZoom);
		pDC->SetViewportExt(m_sizeMain.cx, m_sizeMain.cy);
	}
}


void PSDiploView::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	// These should be disabled while entering the RDB-s.
	pCmdUI->Enable(m_bEnableSave);
}


void PSDiploView::UpdateInfoTab(CString strText)
{
	CWnd* pFrame = GetParentFrame();
	PSInfoTab* pInfoTab = (PSInfoTab*)pFrame->GetDescendantWindow(ID_INFO);
	if (strText == "")
	{
		pInfoTab->SetWindowText("");
		return;
	}
	// Position the caret at the end of the text and add strAdj to it.
	int nTextLength = pInfoTab->GetWindowTextLength();
	pInfoTab->SetSel(nTextLength, nTextLength);
	pInfoTab->ReplaceSel(strText);
	ActivateTab(ID_INFO);
}


void PSDiploView::Invalidate()
{
	for (int i = 1; i <= TG.GetNumberOfPowers(); i++)
	{
		PSPower* pPow = TG.GetPower(i);
		SCs vSCs = TG.GetSCs(pPow);
		for (int j = 0; j < vSCs.size(); j++)
		{
			SC* pSC = vSCs[j];
			PSProvince* pProv = pSC->m_pProvince;
			InvalidateRect(&(pProv->m_SupplySquare));
		}
	}

	for (i = 0; i < TG.GetNumberOfUnits(); i++)
	{
		PSUnit* pUnit = TG.GetUnit(i);
		pUnit->Draw();
	}
}
