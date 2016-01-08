// History.h: interface for the PSHistory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HISTORY_H__828DEB63_1273_11D2_9FE6_00104B6FD296__INCLUDED_)
#define AFX_HISTORY_H__828DEB63_1273_11D2_9FE6_00104B6FD296__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Step.h"

#ifdef EXPORTING
class __declspec(dllexport) PSHistory : public CObject
#else
class __declspec(dllimport) PSHistory : public CObject
#endif
{
// Attributes:
private:
	Steps m_Steps;

public:

// Constructors, destructor:
public:
	PSHistory();
	PSHistory(PSHistory& History)							{m_Steps = History.GetSteps();}
	virtual ~PSHistory();

//Operations:
public:
	void AddStep(PSStep* pNewStep, PSStep* pCurrentStep = NULL);
	void EraseFrom(PSStep*);
	void EraseAll();
	PSStep* GetStep(int i);
	PSStep* GetStep(long lYear, SEASON, PHASE);
	PSStep* GetNextStep(PSStep*);
	PSStep* GetPreviousStep(PSStep*);
	PSStep* GetLastStep()								{return m_Steps.back();}
	int GetNumberOfSteps()								{return m_Steps.size();}
	void SetSteps(Steps steps)							{m_Steps = steps;}
	Steps GetSteps()									{return m_Steps;}

	virtual void Serialize(CArchive&);
	PSHistory operator=(PSHistory&);

};

#endif // !defined(AFX_HISTORY_H__828DEB63_1273_11D2_9FE6_00104B6FD296__INCLUDED_)
