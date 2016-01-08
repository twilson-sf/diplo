// History.cpp: implementation of the PSHistory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Judge.h"
#include "History.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern PSGame TG;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PSHistory::PSHistory()
{
}


PSHistory::~PSHistory()
{
	EraseAll();
}


void PSHistory::AddStep(PSStep* pNewStep, PSStep* pCurrentStep)
{
	if (pCurrentStep)
		EraseFrom(pCurrentStep);

	m_Steps.push_back(pNewStep);
}


PSStep* PSHistory::GetStep(int i)
{
	ASSERT (i < m_Steps.size());
	return m_Steps[i];
}


PSStep* PSHistory::GetStep(long lYear, SEASON Season, PHASE Phase)
{
	for (int i = 0; i < m_Steps.size(); i++)
	{
		PSStep* pStep = m_Steps[i];
		if (pStep->GetYear() == lYear && 
			pStep->GetSeason() == Season && 
			pStep->GetPhase() == Phase)
		{
			return pStep;
		}
	}

	return NULL;
}


PSStep* PSHistory::GetNextStep(PSStep* pStep)
{
	int nSize = m_Steps.size();
	ASSERT(nSize >= 1);
	for (int i = 0; i < nSize - 1; i++)
	{
		PSStep* pCurrentStep = m_Steps[i];
		if (pCurrentStep == pStep)
		{
			PSStep* pNextStep = m_Steps[i + 1];
			return pNextStep;
		}
	}

	return NULL;
}


PSStep* PSHistory::GetPreviousStep(PSStep* pStep)
{
	int nSize = m_Steps.size();
	ASSERT(nSize >= 1);
	for (int i = 1; i < nSize; i++)
	{
		PSStep* pCurrentStep = m_Steps[i];
		if (pCurrentStep == pStep)
		{
			PSStep* pPreviousStep = m_Steps[i - 1];
			return pPreviousStep;
		}
	}

	return NULL;
}


void PSHistory::EraseFrom(PSStep* pStep)
{
	for (int i = 0; i < m_Steps.size(); i++)
	{
		if (m_Steps[i] == pStep)
			break;
	}

	for (int j = i + 1; j < m_Steps.size(); j++)
	{
		delete m_Steps[j];
	}

	m_Steps.erase(m_Steps.begin() + i + 1, m_Steps.end());
}


void PSHistory::EraseAll()
{
	for (int i = 0; i < m_Steps.size(); i++)
	{
		PSStep* pStep = m_Steps[i];
		delete pStep;
	}

	m_Steps.clear();

}

	
PSHistory PSHistory::operator=(PSHistory& History)
{
	PSHistory* pNewHistory = new PSHistory;
	pNewHistory->SetSteps(History.GetSteps());
	return *pNewHistory;
}


void PSHistory::Serialize(CArchive& arh)
{
	if (arh.IsStoring())
	{
		int nSteps = GetNumberOfSteps();
		arh << nSteps;
		for (int i = 0; i < nSteps; i++)
		{
			PSStep* pStep = GetStep(i);
			PSData* pData = pStep->GetData();
			pData->Serialize(arh);
		}
	}
	else 
	{
		EraseAll();
		int nSteps;
		arh >> nSteps;
		for (int i = 0; i < nSteps; i++)
		{
			// Destroy all the units and SCs in TG.
			CString strType; arh >> strType;
			ASSERT(strType == TG.GetType());
			PSData* pData = new PSData;
			pData->Serialize(arh);
			PSStep* pStep = new PSStep(pData);
			delete pData;
			AddStep(pStep);
		}
	}
}

