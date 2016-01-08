// PSData.h: interface for the PSData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAME_H__C4614781_B758_11D1_BF95_00C04FDBD526__INCLUDED_)
#define AFX_GAME_H__C4614781_B758_11D1_BF95_00C04FDBD526__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Power.h"
#include "mystruct.h"
#include "PSFile.h"

enum SEASON {SPRING, FALL};
enum STATUS { NONE, GETORDER, BUILDGAME, ENDIPLO, GETBUILD, GETDISBAND, GETRETREAT };

/*
	Philosophy:

	The PSData is like a document. All the data about the game (except Orders and History) 
	is here. There's not much to this class, it basically encapsulates global functions.

	IMPORTANT: m_SCs should be kept in sync everytime a SC changes ownership.

*/

#ifdef EXPORTING
class __declspec(dllexport) PSData : public CObject
#else
class __declspec(dllimport) PSData : public CObject
#endif
{
public:
	PSData();
	PSData(PSData&);
	virtual ~PSData();

// Attributes:
private:
	STATUS m_nStatus;
	long m_lYear;
	SEASON m_Season;
	Retreats m_Retreats;
	Disbands m_Disbands;
	Builds m_Builds;
	Units m_Units;
	SCs m_SCs;

public:
	CStringArray m_arStrMovingUnits;

// Operations:
private:

public:
	Retreat* GetRetreat(PSUnit*);
	virtual void Serialize(CArchive&);
	void SetStatus(STATUS nStatus)				{m_nStatus = nStatus;}
	void SetYear(int nYear)						{m_lYear = nYear;}
	void SetSeason(SEASON Season)				{m_Season = Season;}

	STATUS GetStatus()							{return m_nStatus;}
	long GetYear()								{return m_lYear;}
	SEASON GetSeason()							{return m_Season;}

	Retreat* GetRetreat(int);
	Disband* GetDisband(int);
	Build* GetBuild(int);
	PSUnit* GetUnit(int);
	SC* GetSC(int);
	const SCs& GetSCs()							{return m_SCs;}

	int GetNumberOfRetreats()					{return m_Retreats.size();}
	int GetNumberOfDisbands()					{return m_Disbands.size();}
	int GetNumberOfBuilds()						{return m_Builds.size();}
	int GetNumberOfUnits()						{return m_Units.size();}
	SCs GetSCs(PSPower*);
	Units GetUnits(PSPower*);

	CString GetAllInfo();

	void AddRetreat(Retreat* pRet)				{m_Retreats.push_back(pRet);}
	void AddDisband(Disband* pDis)				{m_Disbands.push_back(pDis);}
	void AddBuild(Build* pBui)					{m_Builds.push_back(pBui);}
	void AddUnit(PSUnit* pUnit)					{m_Units.push_back(pUnit);}
	void AddSC(SC* pSC)							{m_SCs.push_back(pSC);}

	void DeleteRetreat(Retreat*);
	void DeleteDisband(Disband*);
	void DeleteBuild(Build*);
	void DeleteSC(PSProvince*, PSPower*);
	void DeleteUnit(PSUnit*);

	void Clear();
	void ClearRetreats();
	void ClearDisbands();
	void ClearBuilds();


	PSData& operator=(PSData&);

};

BOOL operator==(PSData&, PSData&);

#endif // !defined(AFX_GAME_H__C4614781_B758_11D1_BF95_00C04FDBD526__INCLUDED_)
