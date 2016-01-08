// Coast.h: interface for the PSCoast class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_COAST_H__4BF5C1D3_0060_11D2_9FDE_00104B6FD296__INCLUDED_)
#define AFX_COAST_H__4BF5C1D3_0060_11D2_9FDE_00104B6FD296__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Province.h"

/*
	Philosophy: 
	A Province has either 1 or 3 Coasts. The default Coast has the name "Central"
	and has all the neighbors of the Province. The 'Bicoastal' Provinces have actually 
	3 Coasts - 2 more besides the Central.
*/

#ifdef EXPORTING
class __declspec(dllexport) PSCoast
#else
class __declspec(dllimport) PSCoast
#endif
{
public:
	PSCoast(PSProvince* pOwner = NULL);
	PSCoast(PSCoast*, PSProvince* pOwner = NULL);
	virtual ~PSCoast();

// Attributes
public:
	CString m_strName;
	CPoint m_ptDrawUnit;

private:
	Provinces m_Neighbors;

// Operations
public:
	PSProvince* GetNeighbor(int);
	BOOL HasNeighbor(PSProvince*);
	Provinces* GetNeighbors()					{return &m_Neighbors;}
	int GetNumberOfNeighbors()					{return m_Neighbors.size();}

	void AddNeighbor(PSProvince* pPrv)			{m_Neighbors.push_back(pPrv);}
	void SetNeighbors(Provinces Neighbors)		{m_Neighbors = Neighbors;}

private:

};

typedef vector<PSCoast*> Coasts;

#endif // !defined(AFX_COAST_H__4BF5C1D3_0060_11D2_9FDE_00104B6FD296__INCLUDED_)
