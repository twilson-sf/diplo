// cpp: implementation of the PSData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Judge.h"
#include "Data.h"
#include "Army.h"
#include "Fleet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern PSGame TG;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PSData::PSData() : 
m_nStatus(GETORDER),
m_lYear(1901),
m_Season(SPRING)
{
}


PSData::PSData(PSData& Data)
{
	m_nStatus = Data.m_nStatus;
	m_lYear = Data.m_lYear;
	m_Season = Data.m_Season;
	for (int i = 0; i < Data.GetNumberOfUnits(); i++)
	{
		PSUnit* pOldUnit = Data.GetUnit(i);
		PSUnit* pNewUnit = pOldUnit->Clone();
		m_Units.push_back(pNewUnit);
		Retreat* pOldRet = Data.GetRetreat(pOldUnit);
		if (pOldRet)
		{
			Retreat* pNewRet = new Retreat;
			pNewRet->m_pUnit = pNewUnit;
			pNewRet->m_pOrig = pOldRet->m_pOrig;
			for (int i = 0; i < pOldRet->m_Possib.size(); i++)
			{
				pNewRet->m_Possib.push_back(pOldRet->m_Possib[i]);
			}
			m_Retreats.push_back(pNewRet);
		}
	}
	ASSERT(m_Retreats.size() == Data.m_Retreats.size());
	for (Disbands::iterator pDis = Data.m_Disbands.begin(); pDis < Data.m_Disbands.end(); pDis++)
	{
		Disband* pNewDis = new Disband;
		pNewDis->m_pPower = (*pDis)->m_pPower;
		m_Disbands.push_back(pNewDis);
	}

	for (Builds::iterator pBui = Data.m_Builds.begin(); pBui < Data.m_Builds.end(); pBui++)
	{
		Build* pNewBui = new Build;
		pNewBui->m_pPower = (*pBui)->m_pPower;
		m_Builds.push_back(pNewBui);
	}

	SCs vSCs = Data.GetSCs();
	for (i = 0; i < vSCs.size(); i++)
	{
		SC* pSC = new SC;
		pSC->m_pProvince = Data.GetSC(i)->m_pProvince;
		pSC->m_pPower = Data.GetSC(i)->m_pPower;
		m_SCs.push_back(pSC);
	}
	m_arStrMovingUnits.RemoveAll();
	for (i = 0; i < Data.m_arStrMovingUnits.GetSize(); i++)
	{
		CString strMovingUnit = Data.m_arStrMovingUnits[i];
		m_arStrMovingUnits.Add(strMovingUnit);
	}
}


PSData::~PSData()
{
	// Clean up.
	Clear();
}


PSData& PSData::operator=(PSData& Data)
{
	if (this != &Data)
	{
		m_nStatus = Data.m_nStatus;
		m_lYear = Data.m_lYear;
		m_Season = Data.m_Season;
		for (int i = 0; i < Data.GetNumberOfUnits(); i++)
		{
			PSUnit* pOldUnit = Data.GetUnit(i);
			PSUnit* pNewUnit = pOldUnit->Clone();
			m_Units.push_back(pNewUnit);
			Retreat* pOldRet = Data.GetRetreat(pOldUnit);
			if (pOldRet)
			{
				Retreat* pNewRet = new Retreat;
				pNewRet->m_pUnit = pNewUnit;
				pNewRet->m_pOrig = pOldRet->m_pOrig;
				for (int i = 0; i < pOldRet->m_Possib.size(); i++)
				{
					pNewRet->m_Possib.push_back(pOldRet->m_Possib[i]);
				}
				m_Retreats.push_back(pNewRet);
			}
		}
		ASSERT(m_Retreats.size() == Data.m_Retreats.size());
		for (Disbands::iterator pDis = Data.m_Disbands.begin(); pDis < Data.m_Disbands.end(); pDis++)
		{
			Disband* pNewDis = new Disband;
			pNewDis->m_pPower = (*pDis)->m_pPower;
			m_Disbands.push_back(pNewDis);
		}
		for (Builds::iterator pBui = Data.m_Builds.begin(); pBui < Data.m_Builds.end(); pBui++)
		{
			Build* pNewBui = new Build;
			pNewBui->m_pPower = (*pBui)->m_pPower;
			m_Builds.push_back(pNewBui);
		}
		SCs vSCs = Data.GetSCs();
		for (i = 0; i < vSCs.size(); i++)
		{
			SC* pSC = new SC;
			pSC->m_pProvince = Data.GetSC(i)->m_pProvince;
			pSC->m_pPower = Data.GetSC(i)->m_pPower;
			m_SCs.push_back(pSC);
		}
		m_arStrMovingUnits.RemoveAll();
		for (i = 0; i < Data.m_arStrMovingUnits.GetSize(); i++)
		{
			CString strMovingUnit = Data.m_arStrMovingUnits[i];
			m_arStrMovingUnits.Add(strMovingUnit);
		}
	}

	return *this;
}


BOOL operator==(PSData& Data1, PSData& Data2)
{
	// I always smile when I look at this.
	if (Data1.GetYear() == Data2.GetYear() && 
		Data1.GetSeason() == Data2.GetSeason() && 
		Data1.GetNumberOfUnits() == Data2.GetNumberOfUnits() && 
		Data1.GetNumberOfRetreats() == Data2.GetNumberOfRetreats() && 
		Data1.GetNumberOfBuilds() == Data2.GetNumberOfBuilds() && 
		Data1.GetNumberOfDisbands() == Data2.GetNumberOfDisbands())
		return TRUE;

	return FALSE;
}


void PSData::DeleteRetreat(Retreat* pRet)
{
	for (Retreats::iterator pIt = m_Retreats.begin(); pIt < m_Retreats.end(); pIt++)
	{
		if (*pIt == pRet)
		{
			delete *pIt;
			m_Retreats.erase(pIt);
			return;
		}
	}
	ASSERT(0);
}


void PSData::DeleteDisband(Disband* pDis)
{
	for (Disbands::iterator pIt = m_Disbands.begin(); pIt < m_Disbands.end(); pIt++)
	{
		if (*pIt == pDis)
		{
			delete *pIt;
			m_Disbands.erase(pIt);
			return;
		}
	}
	ASSERT(0);
}


void PSData::DeleteBuild(Build* pBui)
{
	for (Builds::iterator pIt = m_Builds.begin(); pIt < m_Builds.end(); pIt++)
	{
		if (*pIt == pBui)
		{
			delete *pIt;
			m_Builds.erase(pIt);
			return;
		}
	}
	ASSERT(0);
}


void PSData::DeleteSC(PSProvince* pPrv, PSPower* pPow)
{
	for (SCs::iterator pIt = m_SCs.begin(); pIt < m_SCs.end(); pIt++)
	{
		if ((*pIt)->m_pProvince == pPrv && 
			(*pIt)->m_pPower == pPow)
		{
			delete *pIt;
			m_SCs.erase(pIt);
			return;
		}
	}
	ASSERT(0);
}


void PSData::DeleteUnit(PSUnit* pUnit)
{
	for (Units::iterator pIt = m_Units.begin(); pIt < m_Units.end(); pIt++)
	{
		if (*pIt == pUnit)
		{
			m_Units.erase(pIt);
			return;
		}
	}
	ASSERT(0);
}


void PSData::Clear()
{
	ClearRetreats();
	ClearDisbands();
	ClearBuilds();
	for (Units::iterator pUnit = m_Units.begin(); pUnit < m_Units.end(); pUnit++)
		delete *pUnit;
	m_Units.clear();

	for (SCs::iterator pSC = m_SCs.begin(); pSC < m_SCs.end(); pSC++)
		delete *pSC;
	m_SCs.clear();

	m_Season = SPRING;
	m_lYear = 1901;
}


Retreat* PSData::GetRetreat(int i)
{
	ASSERT(i < m_Retreats.size());
	return m_Retreats[i];
}


Disband* PSData::GetDisband(int i)
{
	ASSERT(i < m_Disbands.size());
	return m_Disbands[i];
}


Build* PSData::GetBuild(int i)
{
	ASSERT(i < m_Builds.size());
	return m_Builds[i];
}


PSUnit* PSData::GetUnit(int i)
{
	ASSERT(i < m_Units.size());
	return m_Units[i];
}


SC* PSData::GetSC(int i)
{
	ASSERT(i < m_SCs.size());
	return m_SCs[i];

}


CString PSData::GetAllInfo()
{
	CString strInfo;
	for (int i = 0; i < m_Retreats.size(); i++)
	{
		PSUnit* pUnit = m_Retreats[i]->m_pUnit;
		strInfo.Format("%s%s%s%s%s", pUnit->GetOwner()->m_strName, 
			" may retreat ", pUnit->GetType() == FLEET ? "fleet in " : "army in ", 
			pUnit->GetLocation()->m_strName, " to: ");

		for (int j = 0; j < m_Retreats[i]->m_Possib.size(); j++)
			strInfo += m_Retreats[i]->m_Possib[j]->m_strName + " ";
		
		strInfo += "\r\n";
		strInfo += strInfo;
	}

	return strInfo;
}


void PSData::Serialize(CArchive& arh)
{
	if (arh.IsStoring()) 
	{
		arh << TG.GetType() << GetYear() << (long)GetSeason() << GetStatus();
		int nNrOfMoves = m_arStrMovingUnits.GetSize(); arh << nNrOfMoves;
		for (int i = 0; i < nNrOfMoves; i++)
			arh << m_arStrMovingUnits[i];
		int nPowers = TG.GetNumberOfPowers();
		arh << (long)nPowers;
		for (i = 1; i <= nPowers; i++)
		{
			PSPower* pPow = TG.GetPower(i);
			arh << pPow->m_strName;
			Units vUnits = GetUnits(pPow);
			int nUnits = vUnits.size();
			arh << (long)nUnits;
			for (int j = 0; j < nUnits; j++)
			{
				PSUnit* pUnit = vUnits[j];
				pUnit->Serialize(this, arh);
			}
			SCs vSCs = GetSCs(pPow);
			int nSCs = vSCs.size();
			arh << (long)nSCs;
			for (j = 0; j < nSCs; j++)
			{
				PSProvince* pPrv = vSCs[j]->m_pProvince;
				arh << pPrv->m_strName;
			}
		}
		int nDisbands = GetNumberOfDisbands();
		arh << (long)nDisbands;
		for (i = 0; i < nDisbands; i++)
			arh << GetDisband(i)->m_pPower->m_strName;
		int nBuilds = GetNumberOfBuilds();
		arh << (long)nBuilds;
		for (i = 0; i < nBuilds; i++)
			arh << GetBuild(i)->m_pPower->m_strName;
	}
	else 
	{
		CString strIn;
		long lIn;

		arh >> lIn; SetYear(lIn);
		arh >> lIn; SetSeason((SEASON)lIn);
		arh >> lIn; SetStatus((STATUS)(lIn));
		arh >> lIn;
		for (int i = 0; i < lIn; i++)
		{
			arh >> strIn;
			m_arStrMovingUnits.Add(strIn);
		}
		long lPowers; arh >> lPowers;
		ASSERT(lPowers == TG.GetNumberOfPowers());
		for (i = 1; i <= lPowers; i++)
		{
			arh >> strIn;
			PSPower* pPow = TG.GetPower(strIn);
			ASSERT(pPow);
			long lUnits; arh >> lUnits;
			for (int j = 0; j < lUnits; j++)
			{
				long lType;
				arh >> lType;
				CString strLocation;
				arh >> strLocation;
				PSUnit* pUnit = NULL;
				PSProvince* pLoc = TG.GetProvince(strLocation);
				if (lType == (long)ARMY)
					pUnit = new PSArmy(pPow, pLoc);
				else
					pUnit = new PSFleet(pPow, pLoc);
				AddUnit(pUnit);
				pUnit->Serialize(this, arh);
			}
			long lSCs; arh >> lSCs;
			for (j = 0; j < lSCs; j++)
			{
				CString strSC; arh >> strSC;
				SC* pSC = new SC;
				PSProvince* pPrv = TG.GetProvince(strSC);
				pSC->m_pProvince = pPrv;
				pSC->m_pPower = pPow;
				AddSC(pSC);
			}
		}

		long lDisbands; arh >> lDisbands;
		for (i = 0; i < lDisbands; i++)
		{
			CString strDisband; arh >> strDisband;
			Disband* pDis = new Disband;
			pDis->m_pPower = TG.GetPower(strDisband);
			AddDisband(pDis);
		}
		long lBuilds; arh >> lBuilds;
		for (i = 0; i < lBuilds; i++)
		{
			CString strBuild; arh >> strBuild;
			Build* pBui = new Build;
			pBui->m_pPower = TG.GetPower(strBuild);
			AddBuild(pBui);
		}
	}
}


SCs PSData::GetSCs(PSPower* pPow)
{
	SCs vSCs;
	for (int i = 0; i < m_SCs.size(); i++)
	{
		SC* pSC = m_SCs[i];
		if (pSC->m_pPower == pPow)
		{
			vSCs.push_back(pSC);
		}
	}

	return vSCs;
}


Units PSData::GetUnits(PSPower* pPow)
{
	Units vUnits;
	for (int i = 0; i < m_Units.size(); i++)
	{
		PSUnit* pUnit = m_Units[i];
		if (pUnit->GetOwner() == pPow)
		{
			vUnits.push_back(pUnit);
		}
	}

	return vUnits;
}


Retreat* PSData::GetRetreat(PSUnit* pUnit)
{
	for (int i = 0; i < GetNumberOfRetreats(); i++)
	{
		Retreat* pRet = GetRetreat(i);
		if (pRet->m_pUnit == pUnit)
		{
			return pRet;
		}
	}

	return NULL;
}


void PSData::ClearRetreats()
{
	for (Retreats::iterator pRet = m_Retreats.begin(); pRet < m_Retreats.end(); pRet++)
		delete *pRet;
	m_Retreats.clear();
}


void PSData::ClearDisbands()
{
	for (Disbands::iterator pDis = m_Disbands.begin(); pDis < m_Disbands.end(); pDis++)
		delete *pDis;
	m_Disbands.clear();
}


void PSData::ClearBuilds()
{
	for (Builds::iterator pBui = m_Builds.begin(); pBui < m_Builds.end(); pBui++)
		delete *pBui;
	m_Builds.clear();
}

