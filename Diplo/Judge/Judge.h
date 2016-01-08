#if !defined(AFX_JUDGE_H__D066C2A5_AE02_11D1_BF88_00C04FDBD526__INCLUDED_)
#define AFX_JUDGE_H__D066C2A5_AE02_11D1_BF88_00C04FDBD526__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Judge.h : header file
//

#include "resource.h"
#include "Game.h"
#include "Order.h"

class PSJudgeApp : public CWinApp
{
public:
	PSJudgeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PSJudgeApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(PSJudgeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// PSJudge window

/*
	Philosophy:
	
	The Judge is the workhorse. Adjudicate() will put in m_strVerdict the result of the 
	adjudication. However, this will not be displayed before the Retreats are discovered. 
	The caller of Adjudicate (PSDiploView in Diplo's case) will have to actually move the 
	advancing units and then call GetRetreats() which returns the verdict. After the Retreats 
	have been dealt with, call GetAdjustments().

	The Judge will also determine if Retreats and Builds are legal. GetBuildGame() will also 
	determine if a Unit can be created in a certain Province. 

*/

#ifdef EXPORTING
class __declspec(dllexport) PSJudge
#else
class __declspec(dllimport) PSJudge
#endif
{
// Construction
public:
	PSJudge();

//////////////
// Attributes
public:

private:
	Orders* m_pOrders;
	Orders m_InitialOrders;

//////////////
// Operations
public:
	CString Adjudicate(Orders*);
	CString GetRetreats(void);
	CString GetAdjustments(void);
	BOOL GetRetreat(PSProvince*);
	PSUnit* GetBuild(UNIT_TYPE, PSProvince*, PSPower*);
	BOOL GetBuildGame(PSUnit*);

private:
	void Initialize();
	void ValidateOrders();
	void DemoteVoidOrders();
	void AddSupports(PSOrder*);
	void DirectAttacksCutSupport();
	void AttacksAgainstConvoys();
	void ConvoyedAttacksCutSupport();
	void ResolveBraces();
	void RectifyRings();
	void FightOrdinaryBattles();
	void ResolveAllBattlegrounds();
	CString WriteVerdict();
	void CompatibleRetreats(Retreat*);

	// Returns a pointer to the Order for the unit in pPrvSource.
	PSOrder* OrderSearch(PSProvince* pPrvSource);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PSJudge)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~PSJudge();

	// Generated message map functions
protected:
};

#endif // !defined(AFX_JUDGE_H__D066C2A5_AE02_11D1_BF88_00C04FDBD526__INCLUDED_)
