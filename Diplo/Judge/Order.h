// Order.h: interface for the PSOrder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORDER_H__D066C2A6_AE02_11D1_BF88_00C04FDBD526__INCLUDED_)
#define AFX_ORDER_H__D066C2A6_AE02_11D1_BF88_00C04FDBD526__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "mystruct.h"
#include "Unit.h"
#include "Whichcoast.h"

#undef VOID	// VOID will be an ORDER_TYPE.
#define ORD_DIRECT		2
enum ORDER_TYPE { VOID, ILLEGAL, HOLD, MOVE, SUPPORT, CONVOY, 
				BOUNCE, CUT, DISRUPT, ADVANCING, DISLODGED };

/*
	Philosophy:

	An Order has 3 Provinces: Location, Source and Target. Location is the Province the Unit 
	is in. Target is the Province the Unit is Moving, Supporting or Convoying into. 
	For MOVE and HOLD Orders, Source is NULL, for SUPPORT and CONVOY Orders, Source is the 
	Location of the Supported or Convoyed Unit.
*/

#ifdef EXPORTING
class __declspec(dllexport) PSOrder  
#else
class __declspec(dllimport) PSOrder  
#endif
{
public:
	PSOrder();
	PSOrder(CString strOrder);
	virtual ~PSOrder();

///////////////////
// Attributes
private:
	ORDER_TYPE m_Type;
	PSProvince* m_pLocation;
	PSProvince* m_pSource;
	PSProvince* m_pTarget;
	PSUnit* m_pUnit;

public:
	CString m_strOutput;
	DWORD m_dwFlags;
	Provinces m_vPath;

///////////////////
// Operations
private:

public:
	ORDER_TYPE GetType()				{return m_Type;}
	PSProvince* GetLocation()			{return m_pLocation;}
	PSProvince* GetSource()				{return m_pSource;}
	PSProvince* GetTarget()				{return m_pTarget;}
	PSUnit* GetUnit()					{return m_pUnit;}

	void SetType(ORDER_TYPE OType);
	void SetLocation(PSProvince* pPrv)	{m_pLocation = pPrv;}
	void SetSource(PSProvince* pPrv)		{m_pSource = pPrv;}
	void SetTarget(PSProvince* pPrv)		{m_pTarget = pPrv;}

};

typedef vector <PSOrder*> Orders;

#endif // !defined(AFX_ORDER_H__D066C2A6_AE02_11D1_BF88_00C04FDBD526__INCLUDED_)
