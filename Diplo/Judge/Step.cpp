// Step.cpp: implementation of the PSStep class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Judge.h"
#include "Step.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern PSGame TG;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PSStep::PSStep(PSData* pData)
{
	m_pData = new PSData(*pData);

	m_lYear = pData->GetYear();
	m_Season = pData->GetSeason();
	switch(m_pData->GetStatus())
	{
	case NONE:
	case GETORDER:
	case BUILDGAME:
	case ENDIPLO:
		m_Phase = ORDERS;
		break;
	case GETRETREAT:
		m_Phase = RETREATS;
		break;
	case GETBUILD:
	case GETDISBAND:
		m_Phase = ADJUSTMENTS;
		break;
	default:
		ASSERT(0);
		m_Phase = ORDERS;
	}
}

