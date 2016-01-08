// Army.h: interface for the PSArmy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARMY_H__B5EE0102_062B_11D2_A969_444553540000__INCLUDED_)
#define AFX_ARMY_H__B5EE0102_062B_11D2_A969_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Unit.h"

/*
	Philosophy:

	An Army has only Central Coast.
*/

#ifdef EXPORTING
class __declspec(dllexport) PSArmy : public PSUnit
#else
class __declspec(dllimport) PSArmy : public PSUnit
#endif
{
// Operations
public:
	PSArmy();
	PSArmy(PSPower* pOwner, PSProvince* pLocation);
	virtual PSUnit* Clone()											{return new PSArmy(*this);}
	virtual ~PSArmy();
	virtual BOOL HasAdjacent(PSProvince*);
	virtual BOOL OrderToBicoastal(PSProvince*, PSProvince*)			{return TRUE;}
	
};

#endif // !defined(AFX_ARMY_H__B5EE0102_062B_11D2_A969_444553540000__INCLUDED_)
