// Power.h: interface for the PSPower class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POWER_H__EABE61C0_AEB6_11D1_BF8A_00C04FDBD526__INCLUDED_)
#define AFX_POWER_H__EABE61C0_AEB6_11D1_BF8A_00C04FDBD526__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Province.h"
#include "mystruct.h"
#include "Unit.h"

class PSData;

/*
	Philosophy:

	m_HSCs is redundant information for faster access.
*/

#ifdef EXPORTING
class __declspec(dllexport) PSPower
#else
class __declspec(dllimport) PSPower
#endif
{
public:
	PSPower();
	PSPower(PSPower*);
	virtual ~PSPower();

// Attributes
private:
	COLORREF m_Color;
	Provinces m_HSCs;
	Provinces m_SCs;

public:
	CString m_strName;

// Operations
private:

public:
	CString GetAllInfo();
	void Clear();
	PSProvince* GetSC(int);
	int GetNumberOfSCs()								{return m_SCs.size();}
	int GetNumberOfUnits(PSData* pData = NULL);
	Provinces* GetHSCs()								{return &m_HSCs;}
	Provinces GetAvailableHSCs();
	BOOL HasHSCAvailable(PSProvince*);
	COLORREF GetColor()									{return m_Color;}
	int GetIndex();
	void Gains(PSProvince*, BOOL bUpdateData = TRUE);
	void Loses(PSProvince*);

	void SetColor(COLORREF color)						{m_Color = color;}
	void AddSC(PSProvince* pSC)							{m_HSCs.push_back(pSC);}
	void AddHSC(PSProvince* pPrv)						{m_HSCs.push_back(pPrv);}
	BOOL Owns(PSProvince*);

};

typedef vector<PSPower*> Powers;

#endif // !defined(AFX_POWER_H__EABE61C0_AEB6_11D1_BF8A_00C04FDBD526__INCLUDED_)
