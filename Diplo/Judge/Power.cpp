// Power.cpp: implementation of the PSPower class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Judge.h"
#include "Power.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern PSGame TG;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PSPower::PSPower() :
m_strName (""),
m_Color(RGB(127, 127, 127)) // Gray
{
}


PSPower::PSPower(PSPower* pPow)
{
	if (pPow == this)
		return;

	m_strName = pPow->m_strName;
	m_Color = pPow->m_Color;
	m_HSCs = pPow->m_HSCs;
	m_SCs = pPow->m_SCs;
}


PSPower::~PSPower()
{
}


BOOL PSPower::Owns(PSProvince* pPrv)
{
	// Returns true if this Power owns pPrv.
	for (int i = 0; i < m_SCs.size(); i++)
	{
		if (m_SCs[i] == pPrv)
		{
			return TRUE;
		}
	}

	return FALSE;
}


void PSPower::Clear()
{
	while (!m_SCs.empty())
	{
		PSProvince* pSC = m_SCs[0];
		Loses(pSC);
	}
	m_SCs.clear();
}


CString PSPower::GetAllInfo()
{
	CString strInfo, strSCs, strArmies, strFleets;
	int nArmies = 0;
	int nFleets = 0;
	SCs vSCs = TG.GetSCs(this);
	strSCs.Format("%d%s", vSCs.size(), (vSCs.size() != 1) ? " SC-s:" : " SC:");
	for (int i = 0; i < vSCs.size(); i++)
		strSCs += " " + vSCs[i]->m_pProvince->m_strName;

	Units vUnits = TG.GetUnits(this);
	for (i = 0; i < vUnits.size(); i++)
	{
		if (vUnits[i]->GetType() == ARMY)
		{
			nArmies++;
			strArmies += " " + vUnits[i]->GetLocation()->m_strName;
		}
		else
		{
			nFleets++;
			strFleets += " " + vUnits[i]->GetLocation()->m_strName;
			CString strName = vUnits[i]->GetCoast()->m_strName;
			if (strName != "Central")
			{
				strFleets += "(" + strName + ")";
			}
		}
	}

	if (!nArmies && ! nFleets && vSCs.empty())
		strInfo.Format("%s%s%s", m_strName, ":\r\n", "Eliminated\r\n\r\n");// TODO: Conditions may vary.
	else
		strInfo.Format("%s%s%s%s%d%s%s%s%d%s%s%s", m_strName, ":\r\n", strSCs, "\r\n", 
			nArmies, (nArmies != 1) ? " Armies: " : " Army: ", strArmies, "\r\n", 
			nFleets, (nFleets != 1) ? " Fleets: " : " Fleet: ", strFleets, "\r\n\r\n");

	return strInfo;
}


int PSPower::GetIndex()
{
	for (int i = 1; i <= TG.GetNumberOfPowers(); i++)
	{
		if (TG.GetPower(i) == this)
		{
			return i;
		}
	}

	ASSERT(0);
	return -1;
}


Provinces PSPower::GetAvailableHSCs()
{
	Provinces AvHSCs;
	for (int i = 0; i < m_HSCs.size(); i++)
	{
		PSProvince* pSC = m_HSCs[i];
		if (!pSC->GetOccupant() && pSC->GetOwner() == this)
		{
			AvHSCs.push_back(pSC);
		}
	}

	return AvHSCs;
}


BOOL PSPower::HasHSCAvailable(PSProvince* pPrv)
{
	Provinces AvHSCs = GetAvailableHSCs();
	for (int i = 0; i < AvHSCs.size(); i++)
	{
		if (pPrv == AvHSCs[i])
			return TRUE;
	}

	return FALSE;
}


int PSPower::GetNumberOfUnits(PSData* pData)
{
	if (!pData)
		pData = TG.GetData();

	Units vUnits = pData->GetUnits(this);
	return vUnits.size();
}


void PSPower::Gains(PSProvince* pPrv, BOOL bUpdateData)
{
	ASSERT(pPrv->GetSC());
	ASSERT(!Owns(pPrv));
	m_SCs.push_back(pPrv);
	if (!bUpdateData)
		return;

	SC* pSC = new SC;
	pSC->m_pPower = this;
	pSC->m_pProvince = pPrv;
	TG.GetData()->AddSC(pSC);
}


void PSPower::Loses(PSProvince* pPrv)
{
	ASSERT(pPrv->GetSC());
	ASSERT(Owns(pPrv));
	for (Provinces::iterator pIt = m_SCs.begin(); pIt < m_SCs.end(); pIt++)
	{
		if (*pIt == pPrv)
		{
			m_SCs.erase(pIt);
			PSData* pData = TG.GetData();
			pData->DeleteSC(pPrv, this);
			return;
		}
	}
	ASSERT(0);
}


PSProvince* PSPower::GetSC(int i)
{
	ASSERT(i < m_SCs.size());
	return m_SCs[i];
}


