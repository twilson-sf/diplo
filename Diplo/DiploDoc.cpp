// diplodoc.cpp : implementation of the PSDiploDoc class
//

#include "stdafx.h"
#include "diplo.h"
#include "diplodoc.h"
#include "Power.h"
#include "Army.h"
#include "Fleet.h"
#include "HistoryTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern __declspec(dllimport) PSGame TG;

/////////////////////////////////////////////////////////////////////////////
// PSDiploDoc

IMPLEMENT_DYNCREATE(PSDiploDoc, CDocument)

BEGIN_MESSAGE_MAP(PSDiploDoc, CDocument)
	//{{AFX_MSG_MAP(PSDiploDoc)
	ON_COMMAND_RANGE(ID_FIRST_VARIANT, ID_FIRST_VARIANT + MAX_VARIANTS, OnNewGame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PSDiploDoc construction/destruction

PSDiploDoc::PSDiploDoc()
{
}


PSDiploDoc::~PSDiploDoc()
{
}


void PSDiploDoc::OnNewGame(UINT nID)
{
	if (!CDocument::OnNewDocument())
		return;
	
	// Get the string of the menu item that was selected.
	CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
	CMenu* pMainMenu = pFrame->GetMenu();
	CString strVariantName;
	pMainMenu->GetMenuString(nID, strVariantName, MF_BYCOMMAND);

	CFile f;
	CFileException e;
	
	CWinApp* pApp = AfxGetApp();

	CString strValue;
	strValue = pApp->GetProfileString("Directories", "Current");
	strValue += strVariantName + ".dip";
	f.Open(strValue, CFile::modeRead, &e);
	if (e.m_cause)
	{
		AfxMessageBox("The " + strVariantName + ".dip file was not found.\r\n"
			"Please make sure this file is located in the same folder as Diplo.exe");
		return;
	}
	CArchive arh(&f, CArchive::load);
	Serialize(arh);
	arh.Close();
	f.Close();

	return;
}


BOOL PSDiploDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	// Start with a Standard game.
	CFile f;
	CFileException e;
	
	CWinApp* pApp = AfxGetApp();

	CString strVariantName = "Standard";
	CString strValue;
	strValue = pApp->GetProfileString("Directories", "Current");
	strValue += strVariantName + ".dip";
	f.Open(strValue, CFile::modeRead, &e);
	if (e.m_cause)
	{
		AfxMessageBox("The " + strVariantName + ".dip file was not found.\r\n"
			"Please make sure this file is located in the same folder as Diplo.exe");
		return FALSE;
	}
	CArchive arh(&f, CArchive::load);
	Serialize(arh);
	arh.Close();
	f.Close();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// PSDiploDoc serialization
void PSDiploDoc::Serialize(CArchive& arh)
{
	if (arh.IsStoring())
	{
		PSData* pData = TG.GetHistory()->GetLastStep()->GetData();
		pData->Serialize(arh);
	}
	else
	{
		TG.Clear();
		CString strIn;
		arh >> strIn;

		// See if it's old format.
		BOOL bOldFormat = FALSE;
		if (strIn == "Syr")
		{
			strIn = "Standard";
			bOldFormat = TRUE;
		}

		// Get the information from the PS file.
		TG.SetType(strIn);
		if (!TG.Load(strIn))
			return;

		// Build redundant information.
		for (int i = 1; i <= TG.GetNumberOfProvinces(); i++)
		{
			PSProvince* pPrv = TG.GetProvince(i);
			PSPower* pPow = pPrv->GetHSC();
			if (pPow)
				pPow->AddHSC(pPrv);
		}

		if (!bOldFormat)
			TG.GetData()->Serialize(arh);
		else 
			LoadOldFormat(arh);
	}
}


/////////////////////////////////////////////////////////////////////////////
// PSDiploDoc diagnostics

#ifdef _DEBUG
void PSDiploDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void PSDiploDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


BOOL PSDiploDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnSaveDocument(lpszPathName))
		return FALSE;

	// Set the name of the root in the history tree to the name of the game.
	CWnd* pMainWnd = AfxGetMainWnd();
	PSHistoryTab* pHistory = (PSHistoryTab*)pMainWnd->GetDescendantWindow(ID_HISTORY);
	HTREEITEM hRoot = pHistory->GetRootItem();
	
	CFileException fe;
	CFile* pFile = GetFile(lpszPathName, CFile::modeRead | CFile::shareDenyNone, &fe);
	if (!pFile)
		return FALSE;

	pHistory->SetItemText(hRoot, pFile->GetFileName());
	ReleaseFile(pFile, TRUE);

	// Save the history.
	TG.SaveHistory(lpszPathName);

	return TRUE;
}


BOOL PSDiploDoc::LoadOldFormat(CArchive& arh)
{
	PSData* pData = TG.GetData();
	CString strName; 
	long lBogus; // This variable will not be used.
	// These three are needed for retreats, damn it!
	CStringArray arStrNames;
	int nRetArray[20][15];
	int nRetArrayIndex = 0;
	for (int i = 1; i <= 75; i++)
	{
		if (i != 1) // The first string ("Syr") has been already read.
			arh >> strName;
		else 
			strName = "Syr";
		arStrNames.Add(strName);
		arh >> lBogus; arh >> lBogus;
		long lOccupant, lBelongs, lArmy, lNn;
		arh >> lOccupant; arh >> lBelongs;
		arh >> lNn; arh >> lBogus;
		arh >> lArmy;
		for (int k = 0; k < lNn; k++)
			arh >> lBogus;
		if (lOccupant)
		{
			ASSERT(lArmy);
			PSProvince* pLoc = TG.GetProvince(strName);
			ASSERT(pLoc);
			PSUnit* pUnit = NULL;
			PSPower* pPow = TG.GetPower(lOccupant);
			if (lArmy == 1)
				pUnit = new PSArmy(pPow, pLoc);
			else 
				pUnit = new PSFleet(pPow, pLoc);
			pData->AddUnit(pUnit);
			
			// There is no way I can tell the coast it is on. This is an incompatibility with 
			// old versions. For bicoastal provinces, set the fleet's coast to the first.
			if (lArmy == 2 && pLoc->IsBicoastal())
			{
				PSCoast* pCst = pLoc->GetCoast(1);
				ASSERT(pCst);
				pUnit->SetCoast(pCst);
				pUnit->SetPosition(pCst->m_ptDrawUnit);
			}
		}

		if (lBelongs)
		{
			SC* pSC = new SC;
			PSProvince* pPrv = TG.GetProvince(strName);
			PSPower* pPow = TG.GetPower(lBelongs);
			ASSERT(pPrv);
			pSC->m_pProvince = pPrv;
			pSC->m_pPower = pPow;
			pData->AddSC(pSC);
		}
	}

	CString strBogus;
	for (i = 1; i <= 7; i++)
	{
		arh >> strBogus; arh >> lBogus;	arh >> lBogus;
	}

	long lIn, lRetindex, lBuiindex, lDisindex;
	arh >> lIn; pData->SetSeason((SEASON)lIn);
	arh >> lIn; pData->SetYear(lIn);
	arh >> lRetindex >> lBuiindex >> lDisindex >> lBogus;

	long lPower, lArmy, lLocal, lNp;
	for (i = 1; i <= lRetindex; i++)
	{
		arh >> lPower;
		arh >> lArmy;
		arh >> lLocal;
		arh >> lBogus;
		arh >> lNp;
		if (!lNp)
			// This is a disband, not a retreat.
			continue;

		// Create a new unit. (Repeat steps above)
		PSUnit* pUnit = NULL;
		PSPower* pPow = TG.GetPower(lPower);
		PSProvince* pLoc = TG.GetProvince(strName);
		ASSERT(pLoc);
		if (lArmy == 1)
			pUnit = new PSArmy(pPow, pLoc);
		else 
			pUnit = new PSFleet(pPow, pLoc);
		pData->AddUnit(pUnit);
		
		if (lArmy == 2 && pLoc->IsBicoastal())
		{
			PSCoast* pCst = pLoc->GetCoast(1);
			ASSERT(pCst);
			pUnit->SetCoast(pCst);
			pUnit->SetPosition(pCst->m_ptDrawUnit);
		}

		// Make the unit to be retreating.
		Retreat* pRet = new Retreat;
		pRet->m_pUnit = pUnit;
		pRet->m_pOrig = NULL;
		long lPossibIndex;
		for (int k = 0; k < lNp; k++)
		{
			arh >> lPossibIndex;
			nRetArray[nRetArrayIndex][k] = lPossibIndex;
		}
		nRetArray[nRetArrayIndex][k + 1] = 0; // Marks the end.
		nRetArrayIndex++;
		TG.AddRetreat(pRet);
	}

	TG.SetStatus(GETORDER);
	if (lDisindex)
		TG.SetStatus(GETDISBAND);
	if (lBuiindex)
		TG.SetStatus(GETBUILD);
	if (TG.GetNumberOfRetreats())
		TG.SetStatus(GETRETREAT);

	// Build all the pRet->m_Possib;
	for (i = 0; i < nRetArrayIndex; i++)
	{
		Retreat* pRet = TG.GetRetreat(i);
		for (int j = 0; nRetArray[i][j] != 0; j++)
		{
			int nValue = nRetArray[i][j];
			CString strProv = arStrNames[nValue];
			pRet->m_Possib.push_back(TG.GetProvince(strProv));
		}
	}

	// Builds and Disbands.
	for (i = 1; i <= lBuiindex; i++)
	{
		arh >> lPower;
		Build* pBui = new Build;
		pBui->m_pPower = TG.GetPower(lPower);
		TG.AddBuild(pBui);
	}
	for (i = 1; i <= lDisindex; i++)
	{
		arh >> lPower;
		Disband* pDis = new Disband;
		pDis->m_pPower = TG.GetPower(lPower);
		TG.AddDisband(pDis);
	}

	return TRUE;
}

