// Coast.cpp: implementation of the PSCoast class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Coast.h"
#include "Game.h"

extern PSGame TG;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PSCoast::PSCoast(PSProvince* pOwner)
{
	if (pOwner)
		pOwner->AddCoast(this);
}


PSCoast::~PSCoast()
{
}


PSCoast::PSCoast(PSCoast* pCstSrc, PSProvince* pOwner)
{
	if (pCstSrc == this)
		return;

	m_strName = pCstSrc->m_strName;
	m_ptDrawUnit = pCstSrc->m_ptDrawUnit;
	for (int j = 0; j < (pCstSrc->m_Neighbors).size(); j++)
	{
		PSProvince* pNeigh = (pCstSrc->m_Neighbors)[j];
		m_Neighbors.push_back(pNeigh);
	}

	if (pOwner)
		pOwner->AddCoast(this);
}


BOOL PSCoast::HasNeighbor(PSProvince* pPrv)
{
	// Returns TRUE if this coast has pPrv as a neighbor.
	for (int i = 0; i < m_Neighbors.size(); i++)
	{
		if (m_Neighbors[i] == pPrv)
		{
			return TRUE;
		}
	}

	return FALSE;
}


PSProvince* PSCoast::GetNeighbor(int i)
{
	ASSERT(i < m_Neighbors.size());
	return m_Neighbors[i];
}

