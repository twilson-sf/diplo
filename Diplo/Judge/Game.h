// Game.h: interface for the PSGame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAME_H__EB561BA3_14CD_11D2_9FE7_00104B6FD296__INCLUDED_)
#define AFX_GAME_H__EB561BA3_14CD_11D2_9FE7_00104B6FD296__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "History.h"

#ifdef EXPORTING
class __declspec(dllexport) PSGame
#else
class __declspec(dllimport) PSGame
#endif
{
public:
	PSGame();
	virtual ~PSGame();

// Attributes:
private:
	CString m_strType;
	Provinces m_Provinces;
	Powers m_Powers;
	PSData m_Data;
	PSHistory* m_pHistory;
	PSStep* m_pCurrentStep;
	int m_nFirstYear;
	int m_nLastYear;

// Operations:
public:
	CString GetType()							{return m_strType;}
	PSHistory* GetHistory()						{return m_pHistory;}
	int GetNumberOfProvinces()					{return (m_Provinces.size() - 1);}
	int GetNumberOfPowers()						{return (m_Powers.size() - 1);}
	PSStep* GetCurrentStep()					{return m_pCurrentStep;}
	int GetFirstYear()							{return m_nFirstYear;}
	int GetLastYear()							{return m_nLastYear;}
	PSProvince* GetProvince(int i);
	PSProvince* GetProvince(CString str);
	PSPower* GetPower(int i);
	PSPower* GetPower(CString);
	int GetNumberOfSCs();

	SEASON GetSeason()							{return m_Data.GetSeason();}
	int GetNumberOfRetreats()					{return m_Data.GetNumberOfRetreats();}
	int GetNumberOfDisbands()					{return m_Data.GetNumberOfDisbands();}
	int GetNumberOfBuilds()						{return m_Data.GetNumberOfBuilds();}
	Retreat* GetRetreat(int i)					{return m_Data.GetRetreat(i);}
	Disband* GetDisband(int i)					{return m_Data.GetDisband(i);}
	Build* GetBuild(int i)						{return m_Data.GetBuild(i);}
	int GetNumberOfUnits()						{return m_Data.GetNumberOfUnits();}
	STATUS GetStatus()							{return m_Data.GetStatus();}
	long GetYear()								{return m_Data.GetYear();}
	SCs GetSCs(PSPower* pPow)					{return m_Data.GetSCs(pPow);}
	Units GetUnits(PSPower* pPow)				{return m_Data.GetUnits(pPow);}
	PSUnit* GetUnit(int i)						{return m_Data.GetUnit(i);}

	CString GetAllInfo();
	void Clear();

	void SetType(CString strType)				{m_strType = strType;}
	void SetHistory(PSHistory* pHistory)		{m_pHistory = pHistory;}
	void SetFirstYear(int nYear)				{m_nFirstYear = nYear;}
	void SetLastYear(int nYear)					{m_nLastYear = nYear;}
	void SetCurrentStep(PSStep*);
	void SetStatus(STATUS nStatus)				{m_Data.SetStatus(nStatus);}
	void SetYear(int nYear)						{m_Data.SetYear(nYear);}
	void SetSeason(SEASON Season)				{m_Data.SetSeason(Season);}
	void AddRetreat(Retreat* pRet)				{m_Data.AddRetreat(pRet);}
	void AddDisband(Disband* pDis)				{m_Data.AddDisband(pDis);}
	void AddBuild(Build* pBui)					{m_Data.AddBuild(pBui);}
	void AddProvince(PSProvince* pPrv)			{m_Provinces.push_back(pPrv);}
	void AddPower(PSPower* pPow)				{m_Powers.push_back(pPow);}

	void DeleteProvince(PSProvince*);
	void DeletePower(PSPower*);
	void ClearRetreats()						{m_Data.ClearRetreats();}
	void ClearDisbands()						{m_Data.ClearDisbands();}
	void ClearBuilds()							{m_Data.ClearBuilds();}
	void DeleteRetreat(Retreat* pRet)			{m_Data.DeleteRetreat(pRet);}
	void DeleteDisband(Disband* pDis)			{m_Data.DeleteDisband(pDis);}
	void DeleteBuild(Build* pBui)				{m_Data.DeleteBuild(pBui);}

	BOOL Load(CString);
	BOOL SaveHistory(LPCTSTR);
	BOOL LoadHistory();

	inline PSData* GetData()					{return &m_Data;}

private:

};

#endif // !defined(AFX_GAME_H__EB561BA3_14CD_11D2_9FE7_00104B6FD296__INCLUDED_)
