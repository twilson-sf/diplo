// Province.h: interface for the PSProvince class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROVINCE_H__4984E682_F73C_11D1_A969_444553540000__INCLUDED_)
#define AFX_PROVINCE_H__4984E682_F73C_11D1_A969_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "mystruct.h"

class PSPower;
class PSCoast;
class PSUnit;
class PSData;
typedef vector<PSCoast*> Coasts;
enum PROVINCE_TYPE {WATER_BODY, DRY_LAND};

/*
	Philosophy: 
	A Province can be occupied by 0, 1 or 2 Units. When there are 2 Units in it, one of them
	is Retreating. GetOccupant() returns the Power that owns the non-Retreating Unit.

	m_dwFlags is defined as follows:
	From right to left:
	- first bit is 0 if the Province is not an SC, 1 if it is;
	- the following bits represent the index of the Power that has the Province as an HSC. 
	They are all '0' if the Province is a Neutral SC. The index of the Power is that Power's 
	position (starting with 1) in the PSData::m_Powers. 
*/

#ifdef EXPORTING
class __declspec(dllexport) PSProvince
#else
class __declspec(dllimport) PSProvince
#endif
{
public:
	PSProvince();
	PSProvince(PSProvince*);
	virtual ~PSProvince();

// Attributes
private:
	BOOL m_bStandoff;
	BOOL m_bBattleground;
	PROVINCE_TYPE m_Type;
	int m_nFlags;

protected:
	Coasts m_Coasts;

public:
	CString m_strName;
	CRgn m_poly;
	RECT m_SupplySquare;
	CPtrArray m_Vertexes;

// Operations
private:

public:
	BOOL RemoveCoast(CString strName);
	void RemoveCoasts();
	void Clear();
	void SetStandoff(BOOL bStandoff)			{m_bStandoff = bStandoff;}
	void SetBattleground(BOOL bBattleground)	{m_bBattleground = bBattleground;}
	void SetFlags(int nFlags)					{m_nFlags = nFlags;}
	void SetType(PROVINCE_TYPE type)			{m_Type = type;}
	void AddNeighbor(PSProvince* pPrv);
	void AddCoast(PSCoast* pCst)				{m_Coasts.push_back(pCst);}

	int GetNumberOfNeighbors();
	Provinces* GetNeighbors();
	BOOL GetStandoff()							{return m_bStandoff;}
	BOOL GetBattleground()						{return m_bBattleground;}
	PROVINCE_TYPE GetType()						{return m_Type;}
	BOOL GetSC()								{return m_nFlags;}
	BOOL IsBicoastal();
	BOOL IsCoastal();
	BOOL HasNeighbor(PSProvince*);
	PSProvince* GetNeighbor(int);
	PSPower* GetOccupant(PSData* pData = NULL);
	PSPower* GetOwner();
	PSPower* GetHSC();
	PSUnit* GetUnit(PSData* pData = NULL);
	PSUnit* GetRetreatingUnit(PSData* = NULL);
	PSCoast* GetCoast(int);
	PSCoast* GetCoast(CString);
};

/*
*/
#endif // !defined(AFX_PROVINCE_H__4984E682_F73C_11D1_A969_444553540000__INCLUDED_)
