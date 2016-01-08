// Game.cpp: implementation of the PSGame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Game.h"
#include "Judge.h"

#define SQUAREDIM 7

// Global Object:
__declspec(dllexport) PSGame TG;
#pragma warning(disable:4786)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PSGame::PSGame() : 
m_strType(""),
m_pCurrentStep(NULL),
m_pHistory(NULL)
{
}


PSGame::~PSGame()
{
	// Clean up.
	Clear();
}


void PSGame::SetCurrentStep(PSStep* pStep)
{
	m_pCurrentStep = pStep;
	PSData* pData = pStep->GetData();
	int nPowers = GetNumberOfPowers();
	for (int i = 1; i <= nPowers; i++)
	{
		PSPower* pPow = GetPower(i);
		pPow->Clear();
	}
	m_Data.Clear();
	m_Data = *pData;
	m_Data.m_arStrMovingUnits.RemoveAll();
	const SCs vSCs = pData->GetSCs();
	for (i = 0; i < vSCs.size(); i++)
	{
		SC* pSC = vSCs[i];
		pSC->m_pPower->Gains(pSC->m_pProvince, FALSE);
	}
}


void PSGame::Clear()
{
	m_Data.Clear();
	delete m_pHistory;
	m_pHistory = NULL;

	for (Powers::iterator pPow = m_Powers.begin(); pPow < m_Powers.end(); pPow++)
		delete *pPow;
	m_Powers.clear();

	for (Provinces::iterator pPrv = m_Provinces.begin(); pPrv < m_Provinces.end(); pPrv++)
		delete *pPrv;
	m_Provinces.clear();
}


int PSGame::GetNumberOfSCs()
{
	int nSCs = 0;
	for (int i = 0; i < m_Provinces.size(); i++)
	{
		if (GetProvince(i)->GetSC())
			nSCs++;
	}

	return nSCs;
}


PSProvince* PSGame::GetProvince(int i)
{
	ASSERT(i < m_Provinces.size());
	return m_Provinces[i];
}


PSPower* PSGame::GetPower(int i)
{
	ASSERT(i < m_Powers.size());
	return m_Powers[i];
}


PSPower* PSGame::GetPower(CString strName)
{
	for (int i = 0; i < m_Powers.size(); i++)
	{
		if (m_Powers[i]->m_strName == strName)
		{
			return m_Powers[i];
		}
	}

	return NULL;
}


CString PSGame::GetAllInfo()
{
	CString strInfo = "";
	for (int i = 1; i <= TG.GetNumberOfPowers(); i++)
	{
		PSPower* pPow = TG.GetPower(i);
		strInfo += pPow->GetAllInfo();
	}

	strInfo += "\r\n";
	
	return strInfo;
}


PSProvince* PSGame::GetProvince(CString strName)
{   
	// Takes the Name of the Province and returns a pointer to it (NULL if none found)
	if (strName == "")
		return NULL;

	for (int i = 1; i <= GetNumberOfProvinces(); i++)
	{
		if (m_Provinces[i]->m_strName == strName)
		{
			return m_Provinces[i];
		}
	}

	return NULL;
}


void PSGame::DeleteProvince(PSProvince* pPrv)
{
	for (Provinces::iterator pIt = m_Provinces.begin(); pIt < m_Provinces.end(); pIt++)
	{
		if (*pIt == pPrv)
		{
			delete *pIt;
			m_Provinces.erase(pIt);
		}
	}
}


void PSGame::DeletePower(PSPower* pPow)
{
	for (Powers::iterator pIt = m_Powers.begin(); pIt < m_Powers.end(); pIt++)
	{
		if (*pIt == pPow)
		{
			delete *pIt;
			m_Powers.erase(pIt);
		}
	}
}


BOOL PSGame::Load(CString strType)
{
	PSIOFile f;
	CFileException e;	

	// If the whole path is in strType, load the game. If not, build the path from the registry.
	char* path_buffer = strType.GetBuffer(_MAX_PATH);
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath( path_buffer, drive, dir, fname, ext );
	if (!strcmp(ext, ""))
	{
		CString strSection       = "Directories";
		CString strStringItem    = "Current";

		CWinApp* pApp = AfxGetApp();

		CString strValue;
		strValue = pApp->GetProfileString(strSection, strStringItem);
		strValue += strType + "._ps";
		strType = strValue;
	}
	f.Open(strType, CFile::modeRead, &e);
	if (e.m_cause)
	{
		CString strMessage;
		strMessage.Format("%s%s%s", "The file ", strType, " wasn't found.");
		AfxMessageBox(strMessage, MB_ICONEXCLAMATION);
		return FALSE;
	}
	CString strIn;

	f.ReadString(strIn);
	f.ReadString(strIn);
	int nNrOfPowers = f.ReadInt();
	// Bogus Power
	PSPower* pPower = new PSPower;
	TG.AddPower(pPower);

	for (int i = 0; i < nNrOfPowers; i++)
	{
		pPower = new PSPower;
		TG.AddPower(pPower);
		f.ReadString(pPower->m_strName);
		pPower->SetColor((COLORREF)(f.ReadInt()));
	}

	f.ReadString(strIn);
	if (strIn != "")
	{
		f.Close();
		return FALSE;
	}
	const int nNrOfProvinces = f.ReadInt();
	// Bogus Province
	PSProvince* pProv = new PSProvince;
	TG.AddProvince(pProv);

	for (i = 0; i < nNrOfProvinces; i++)
	{
		f.ReadString(strIn);
		if (strIn.GetLength() != 3)
		{
			f.Close();
			return FALSE;
		}
		pProv = TG.GetProvince(strIn);
		if (!pProv)
		{
			pProv = new PSProvince;
			TG.AddProvince(pProv);
			pProv->m_strName = strIn;
		}
		pProv->SetType((PROVINCE_TYPE)f.ReadInt());
		int nSC = f.ReadInt();
		pProv->SetFlags(nSC);
		if (nSC)
		{
			int nXPos = f.ReadInt();
			pProv->m_SupplySquare.left = nXPos;
			pProv->m_SupplySquare.right = nXPos + SQUAREDIM;
			int nYPos = f.ReadInt();
			pProv->m_SupplySquare.top = nYPos;
			pProv->m_SupplySquare.bottom = nYPos + SQUAREDIM;
		}
		PSCoast* pCoast = new PSCoast(pProv);
		pCoast->m_strName = "Central";
		int nNeighbors = f.ReadInt();
		for (int j = 0; j < nNeighbors; j++)
		{
			f.ReadString(strIn);
			if (strIn.GetLength() != 3)
			{
				f.Close();
				return FALSE;
			}
			PSProvince* pNeigh = TG.GetProvince(strIn);
			if (!pNeigh)
			{
				pNeigh = new PSProvince;
				TG.AddProvince(pNeigh);
				pNeigh->m_strName = strIn;
			}
			pCoast->AddNeighbor(pNeigh);
		}
		pCoast->m_ptDrawUnit.x = f.ReadInt();
		pCoast->m_ptDrawUnit.y = f.ReadInt();

		// Set the polygon.
		int nVertexes = f.ReadInt();
		CPoint* pPt = new CPoint[nVertexes];
		for (j = 0; j < nVertexes; j++)
		{
			(pPt + j)->x = f.ReadInt();
			(pPt + j)->y = f.ReadInt();
			pProv->m_Vertexes.Add(pPt + j);
		}
		pProv->m_poly.CreatePolygonRgn(pPt, nVertexes, ALTERNATE);

		int nBicoastal = f.ReadInt();
		if (nBicoastal)
		{
			for (j = 0; j < 2; j++)
			{
				PSCoast* pCoast = new PSCoast(pProv);
				f.ReadString(pCoast->m_strName);
				int nNeighbors = f.ReadInt();
				for (int k = 0; k < nNeighbors; k++)
				{
					f.ReadString(strIn);
					PSProvince* pNeigh = TG.GetProvince(strIn);
					if (!pNeigh)
					{
						pNeigh = new PSProvince;
						TG.AddProvince(pNeigh);
						pNeigh->m_strName = strIn;
					}
					pCoast->AddNeighbor(pNeigh);
				}
				pCoast->m_ptDrawUnit.x = f.ReadInt();
				pCoast->m_ptDrawUnit.y = f.ReadInt();
			}
		}

		f.ReadString(strIn);
		if (strIn != "")
		{
			f.Close();
			return FALSE;
		}
	}

	// Load first and last year.
	f.ReadString(strIn);
	if (strIn != "Years")
	{
		f.Close();
		return FALSE;
	}
	int nFirst = f.ReadInt();
	int nLast = f.ReadInt();
	if (nLast <= nFirst)
	{
		f.Close();
		return FALSE;
	}
	TG.SetFirstYear(nFirst);
	TG.SetLastYear(nLast);

	f.Close();
	return TRUE;
}


BOOL PSGame::LoadHistory()
{
	// Get the name of the game.
	CFrameWnd* pFrame = (CFrameWnd*) AfxGetMainWnd();
	CDocument* pDoc = pFrame->GetActiveDocument();
	CString strName = pDoc->GetPathName();
	if (strName == "")
		return FALSE;

	// Open the history file.
	char* path_buffer = strName.GetBuffer(_MAX_PATH);
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath( path_buffer, drive, dir, fname, ext );
	strName.Format("%s%s%s%s", drive, dir, fname, ".dph");

	CFile f;
	CFileException fe;
	f.Open(strName, CFile::modeRead, &fe);
	if (fe.m_cause)
		return FALSE;

	// Load the history.
	PSHistory* pHist = new PSHistory;
	CArchive arh(&f, CArchive::load);
	pHist->Serialize(arh);
	arh.Close();
	f.Close();

	// If the data in the last step of the history is the same as the game's data, copy the history
	// into the game's history. If not, disregard the loaded history.
	PSStep* pLastStep = pHist->GetLastStep();
	PSData* pLastData = pLastStep->GetData();
	if (!(*pLastData == m_Data))// Operator != not defined.
	{
		AfxMessageBox(IDS_BAD_HISTORY);
		delete pHist;
		return FALSE;
	}

	SetHistory(pHist);
	SetCurrentStep(pLastStep);
	return TRUE;
}


BOOL PSGame::SaveHistory(LPCTSTR lpszPathName)
{
	CString strName(lpszPathName);

	// Open the history file or create a new one.
	CFile f;
	CFileException e;
	strName = strName.Left(strName.GetLength() - 3);
	strName += "dph";
	f.Open(strName, CFile::modeCreate | CFile::modeWrite, &e);
	if (e.m_cause)
		return FALSE;

	// Save the history.
	PSHistory* pHist = GetHistory();
	ASSERT(pHist);
	CArchive arh(&f, CArchive::store);
	pHist->Serialize(arh);
	arh.Close();
	f.Close();

	return TRUE;
}

