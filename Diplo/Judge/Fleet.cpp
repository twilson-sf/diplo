// Fleet.cpp: implementation of the PSFleet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "judge.h"
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

PSFleet::PSFleet()
{
	m_Type = FLEET;
}


PSFleet::PSFleet(PSPower* pOwner, PSProvince* pLocation)
{
	m_Type = FLEET;
	m_pLocation = pLocation;
	m_pCoast = pLocation->GetCoast(0);
	m_pDesiredCoast = pLocation->GetCoast(0);
	m_ptPosition = m_pCoast->m_ptDrawUnit;
	m_pOwner = pOwner;
}


PSFleet::~PSFleet()
{
}


BOOL PSFleet::OrderToBicoastal(PSProvince* pLocation, PSProvince* pTarget)
{
	// Returns FALSE if the coast was not specified for an ambiguous order, TRUE otherwise.
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// Is it ambiguous? It is if pLocation is a neighbor of both coasts or (in case of a build)
	// if pLocation is NULL.
	BOOL bAmbiguous = FALSE;
	ASSERT(pTarget->IsBicoastal());
	PSCoast* pFirstCoast = pTarget->GetCoast(1);
	PSCoast* pSecondCoast = pTarget->GetCoast(2);

	if (pFirstCoast->HasNeighbor(pLocation) && pSecondCoast->HasNeighbor(pLocation) || 
		pLocation == NULL)
	{
		// Yes.
		PSWhichCoast cst(pTarget);
		int nCoast = cst.DoModal();
		if (nCoast)
		{
			SetDesiredCoast((nCoast == 1) ? pFirstCoast : pSecondCoast);
			return TRUE;
		}
		else
			return FALSE;
	}
	else
	{
		// No.
		SetDesiredCoast((pFirstCoast->HasNeighbor(pLocation)) ? pFirstCoast : pSecondCoast);
		return TRUE;
	}
}


BOOL PSFleet::HasAdjacent(PSProvince* pPrv)
{
	for (int i = 0; i < m_pCoast->GetNumberOfNeighbors(); i++)
	{
		PSProvince* pCurrentProv = m_pCoast->GetNeighbor(i);
		// If pPrv is not a neighbor, continue.
		if (pCurrentProv != pPrv)
			continue;
		// If the location or the target are water bodies, return TRUE.
		if (pPrv->GetType() == WATER_BODY || GetLocation()->GetType() == WATER_BODY)
		{
			return TRUE;
		}
		else
		{
			// Return TRUE if m_pCoast and pPrv have at least one body of water as 
			// a common neighbor.
			for (int k = 0; k < m_pCoast->GetNumberOfNeighbors(); k++)
			{
				PSProvince* pCurrentNeighbor = m_pCoast->GetNeighbor(k);
				if (pCurrentNeighbor->GetType() == WATER_BODY)
				{
					if (pPrv->HasNeighbor(pCurrentNeighbor))
						return TRUE;
				}
			}
		}
	}

	return FALSE;
}

