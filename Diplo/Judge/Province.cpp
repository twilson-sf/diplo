// Province.cpp: implementation of the PSProvince class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Judge.h"
#include "Province.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern PSGame TG;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PSProvince::PSProvince() : 
m_strName(""),
m_bStandoff(FALSE),
m_Type(DRY_LAND),
m_nFlags(0),
m_bBattleground(FALSE)
{
}


PSProvince::PSProvince(PSProvince* pPrv)
{
	if (pPrv == this)
		return;

	m_strName = pPrv->m_strName;
	m_bStandoff = pPrv->m_bStandoff;
	m_Type = pPrv->m_Type;
	m_nFlags = pPrv->m_nFlags;
	m_bBattleground = pPrv->m_bBattleground;
	m_SupplySquare.left = pPrv->m_SupplySquare.left;
	m_SupplySquare.top = pPrv->m_SupplySquare.top;
	m_SupplySquare.right = pPrv->m_SupplySquare.right;
	m_SupplySquare.bottom = pPrv->m_SupplySquare.bottom;
	int nVertexes = pPrv->m_Vertexes.GetSize();
	CPoint* pPt = new CPoint[nVertexes];
	for (int i = 0; i < nVertexes; i++)
	{
		CPoint* pPtSrc = (CPoint*)(pPrv->m_Vertexes).GetAt(i);
		(pPt + i)->x = pPtSrc->x;
		(pPt + i)->y = pPtSrc->y;
		m_Vertexes.Add(pPt + i);
	}
	m_poly.CreatePolygonRgn(pPt, nVertexes, ALTERNATE);
	for (i = 0; i < (pPrv->m_Coasts).size(); i++)
	{
		PSCoast* pCstSrc = pPrv->m_Coasts[i];
		PSCoast* pCst = new PSCoast(pCstSrc, this);
	}
}


PSProvince::~PSProvince()
{
	for (int i = 0; i < m_Coasts.size(); i++)
		delete m_Coasts[i];
	if (m_Vertexes.GetSize())
		delete[] m_Vertexes.GetAt(0);
}


PSUnit* PSProvince::GetUnit(PSData* pData)
{
	if (!pData)
		pData = TG.GetData();

	for (int i = 0; i < pData->GetNumberOfUnits(); i++)
	{
		PSUnit* pUnit = pData->GetUnit(i);
		if (pUnit->GetLocation() == this && 
			!pUnit->IsRetreating(pData))
		{
			return pUnit;
		}
	}

	return NULL;
}


BOOL PSProvince::HasNeighbor(PSProvince* pPrv)
{
	// Returns TRUE if this province has pPrv as a neighbor.
	PSCoast* pCentral = GetCoast(0);
	ASSERT(pCentral);
	return pCentral->HasNeighbor(pPrv);
}


BOOL PSProvince::IsCoastal()
{
	for (int k = 0; k < GetNumberOfNeighbors(); k++)
	{
		if (GetNeighbor(k)->GetType() == WATER_BODY)
		{
			return TRUE;
		}
	}

	return FALSE;
}


BOOL PSProvince::IsBicoastal()
{
	return (m_Coasts.size() > 1);
}


PSProvince* PSProvince::GetNeighbor(int i)
{
	PSCoast* pCentral = GetCoast(0);
	ASSERT(pCentral);
	return pCentral->GetNeighbor(i);
}


PSPower* PSProvince::GetHSC()
{
	// Returns a pointer to the power that has this province as a HSC, NULL if
	// it is neutral SC or no SC at all.
	if (!m_nFlags)
		return NULL;
	int nPower = m_nFlags>>1;
	if (nPower)
		return TG.GetPower(nPower);

	return NULL;
}


PSPower* PSProvince::GetOccupant(PSData* pData)
{
	// Returns a pointer to the Power that occupies this province or NULL if it is empty.
	if (!pData)
		 pData = TG.GetData();

	PSUnit* pUnit = GetUnit(pData);
	if (!pUnit)
		return NULL;

	return pUnit->GetOwner();
}


void PSProvince::Clear()
{
	m_bStandoff = FALSE;
	m_bBattleground = FALSE;
}


PSCoast* PSProvince::GetCoast(int nCoast)
{
	ASSERT(nCoast < m_Coasts.size());
	return m_Coasts[nCoast];
}


PSCoast* PSProvince::GetCoast(CString strCoast)
{
	ASSERT(IsBicoastal());
	int nSize = m_Coasts.size();
	ASSERT(nSize == 3);
	for (int i = 0; i < nSize; i++)
	{
		PSCoast* pCst = m_Coasts[i];
		if (pCst->m_strName == strCoast)
		{
			return pCst;
		}
	}

	return NULL;
}


int PSProvince::GetNumberOfNeighbors()
{
	PSCoast* pCentral = GetCoast(0);
	ASSERT(pCentral);
	return pCentral->GetNumberOfNeighbors();
}


Provinces* PSProvince::GetNeighbors()
{
	PSCoast* pCentral = GetCoast(0);
	ASSERT(pCentral);
	return pCentral->GetNeighbors();
}


void PSProvince::AddNeighbor(PSProvince* pPrv)
{
	PSCoast* pCentral = GetCoast(0);
	ASSERT(pCentral);
	pCentral->AddNeighbor(pPrv);
}


PSPower* PSProvince::GetOwner()
{
	for (int i = 1; i <= TG.GetNumberOfPowers(); i++)
	{
		PSPower* pPow = TG.GetPower(i);
		if (pPow->Owns(this))
		{
			return pPow;
		}
	}

	return NULL;
}


PSUnit* PSProvince::GetRetreatingUnit(PSData* pData)
{
	if (!pData)
		pData = TG.GetData();

	for (int i = 0; i < pData->GetNumberOfUnits(); i++)
	{
		PSUnit* pUnit = pData->GetUnit(i);
		if (pUnit->GetLocation() == this && 
			pUnit->IsRetreating(pData))
		{
			return pUnit;
		}
	}

	return NULL;
}


BOOL PSProvince::RemoveCoast(CString strName)
{
	PSCoast* pCst = GetCoast(strName);
	for (Coasts::iterator pIt = m_Coasts.begin(); pIt < m_Coasts.end(); pIt++)
	{
		if (*pIt == pCst)
		{
			delete *pIt;
			m_Coasts.erase(pIt);
			return TRUE;
		}
	}
	
	return FALSE;
}


void PSProvince::RemoveCoasts()
{
	for (Coasts::iterator pIt = m_Coasts.begin(); pIt < m_Coasts.end(); pIt++)
	{
		delete *pIt;
		m_Coasts.erase(pIt);
	}
}

