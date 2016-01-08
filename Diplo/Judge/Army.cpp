// Army.cpp: implementation of the PSArmy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "judge.h"
#include "Army.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern PSGame TG;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PSArmy::PSArmy()
{
	m_Type = ARMY;
}


PSArmy::PSArmy(PSPower* pOwner, PSProvince* pLocation)
{
	m_Type = ARMY;
	m_pLocation = pLocation;
	m_pCoast = pLocation->GetCoast(0);
	m_pDesiredCoast = pLocation->GetCoast(0);
	m_ptPosition = m_pCoast->m_ptDrawUnit;
	m_pOwner = pOwner;
}


PSArmy::~PSArmy()
{
}


BOOL PSArmy::HasAdjacent(PSProvince* pPrv)
{
	for (int i = 0; i < m_pCoast->GetNumberOfNeighbors(); i++)
	{
		PSProvince* pCurrentProv = m_pCoast->GetNeighbor(i);
		if (pCurrentProv == pPrv && pCurrentProv->GetType() == DRY_LAND)
		{
			return TRUE;
		}
	}

	return FALSE;
}

