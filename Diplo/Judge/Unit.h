// Unit.h: interface for the PSUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNIT_H__EABE61BF_AEB6_11D1_BF8A_00C04FDBD526__INCLUDED_)
#define AFX_UNIT_H__EABE61BF_AEB6_11D1_BF8A_00C04FDBD526__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Province.h"
#include "Coast.h"
class PSPower;

enum UNIT_TYPE { NOUNIT, ARMY, FLEET };

/*
	Philosophy: 
	
	This is an abstract class. 
	
	A Unit has two Coasts: an actual Coast and a desired Coast. Every time a Unit is 
	ordered to Move, Retreat or is being Built in a Province, the desired Coast is 
	defaulted to the Central Coast of the Province. If the Move, Retreat or Build 
	succeeds the actual Coast takes the value of the desired Coast.

	HasAdjacent(pPrv) returns TRUE if the Unit can move to pPrv. 

*/

#ifdef EXPORTING
class __declspec(dllexport) PSUnit : public CObject
#else
class __declspec(dllimport) PSUnit : public CObject
#endif
{
public:
	PSUnit();
	PSUnit(PSUnit&);
	virtual PSUnit* Clone()										{return NULL;}
	~PSUnit() = 0;

// Attributes
protected:
	PSProvince* m_pLocation;
	PSProvince* m_pDesiredLocation;
	PSPower* m_pOwner;
	UNIT_TYPE m_Type;
	PSCoast* m_pCoast;
	PSCoast* m_pDesiredCoast;
	CPoint m_ptPosition;
	BOOL m_bMoving;
	static CScrollView* m_pView;
	static m_nWidth;
	static m_nHeight;

public:
	int m_nDefenseStrength;
	int m_nAttackStrength;
	int m_nDislodgingStrength;
	int m_nBraceStrength;

// Operations
private:

public:
	static void SetView(CScrollView* pView)						{m_pView = pView;}
	static void SetSizes(int nWidth, int nHeight)				{m_nWidth = nWidth; m_nHeight = nHeight;}
	virtual void Draw();
	virtual void Destroy();
	virtual void Move(int nJump, BOOL bAnimate = TRUE);

	void SetDesiredCoast(PSCoast* pCst)							{m_pDesiredCoast = pCst;}
	void SetDesiredLocation(PSProvince* pProv)					{m_pDesiredLocation = pProv;}
	virtual void SetCoast(PSCoast* pCst)						{m_pCoast = pCst;}
	void SetOwner(PSPower* pPow)								{m_pOwner = pPow;}
	void SetPosition(CPoint ptPosition)							{m_ptPosition = ptPosition;}
	void SetMoving(PSProvince* pTarget, BOOL bMoving = TRUE);
	virtual BOOL OrderToBicoastal(PSProvince*, PSProvince*) = 0;

	UNIT_TYPE GetType()											{return m_Type;}
	virtual PSCoast* GetCoast()									{return m_pCoast;}
	PSCoast* GetDesiredCoast()									{return m_pDesiredCoast;}
	PSProvince* GetLocation()									{return m_pLocation;}
	PSProvince* GetDesiredLocation()							{return m_pDesiredLocation;}
	PSPower* GetOwner()											{return m_pOwner;}
	CPoint GetPosition()										{return m_ptPosition;}
	BOOL IsMoving()												{return m_bMoving;}
	Retreat* IsRetreating(PSData* = NULL);
	virtual BOOL HasAdjacent(PSProvince*) = 0;
	virtual void Serialize(PSData*, CArchive&);

};

typedef vector<PSUnit*> Units;

#endif // !defined(AFX_UNIT_H__EABE61BF_AEB6_11D1_BF8A_00C04FDBD526__INCLUDED_)
