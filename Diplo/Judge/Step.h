// Step.h: interface for the PSStep class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEP_H__B5561FC2_13BE_11D2_A969_444553540000__INCLUDED_)
#define AFX_STEP_H__B5561FC2_13BE_11D2_A969_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Data.h"
enum PHASE {ORDERS, RETREATS, ADJUSTMENTS};

#ifdef EXPORTING
class __declspec(dllexport) PSStep
#else
class __declspec(dllimport) PSStep
#endif
{
// Attributes:
private:
	PSData* m_pData;
	long m_lYear;
	SEASON m_Season;
	PHASE m_Phase;

public:

// Constructors, destructor:
public:
	PSStep(PSData*);
	virtual ~PSStep()								{delete m_pData;}

//Operations:
private:

public:
	PSData* GetData()								{return m_pData;}
	long GetYear()									{return m_lYear;}
	SEASON GetSeason()								{return m_Season;}
	PHASE GetPhase()								{return m_Phase;}

};

typedef vector<PSStep*> Steps;

#endif // !defined(AFX_STEP_H__B5561FC2_13BE_11D2_A969_444553540000__INCLUDED_)
