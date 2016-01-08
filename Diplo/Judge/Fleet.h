// Fleet.h: interface for the PSFleet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLEET_H__B5EE0103_062B_11D2_A969_444553540000__INCLUDED_)
#define AFX_FLEET_H__B5EE0103_062B_11D2_A969_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Unit.h"

#ifdef EXPORTING
class __declspec(dllexport) PSFleet : public PSUnit
#else
class __declspec(dllimport) PSFleet : public PSUnit
#endif
{
// Operations
public:
	PSFleet();
	PSFleet(PSPower* pOwner, PSProvince* pLocation);
	virtual PSUnit* Clone()										{return new PSFleet(*this);}
	virtual ~PSFleet();
	virtual BOOL OrderToBicoastal(PSProvince*, PSProvince*);
	virtual BOOL HasAdjacent(PSProvince*);

private:

// Attributes


};

#endif // !defined(AFX_FLEET_H__B5EE0103_062B_11D2_A969_444553540000__INCLUDED_)
