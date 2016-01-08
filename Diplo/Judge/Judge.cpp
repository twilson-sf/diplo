// Judge.cpp : implementation file
//

#include "stdafx.h"
#include "Judge.h"

#include "Game.h"
#include "Unit.h"
#include "Army.h"
#include "Fleet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern PSGame TG;

BEGIN_MESSAGE_MAP(PSJudgeApp, CWinApp)
	//{{AFX_MSG_MAP(PSJudgeApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PSJudgeApp construction

PSJudgeApp::PSJudgeApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only PSJudgeApp object

PSJudgeApp theApp;
/////////////////////////////////////////////////////////////////////////////
// PSJudge

PSJudge::PSJudge() : 
m_pOrders(NULL),
m_InitialOrders(NULL)
{
}


PSJudge::~PSJudge()
{
}


// Before we get into the algorithm, here is how we resolve the battlegrounds.
void PSJudge::ResolveAllBattlegrounds()
{
	for (int i = 1; i <= TG.GetNumberOfProvinces(); i++)
	{
		PSProvince* pThisProv = TG.GetProvince(i);
		// For each Battleground:
		if (pThisProv->GetBattleground())
		{
			// Create a vector of Rivals
			Orders vRivals;

			// Find all participating attacks and put them in the vector of Rivals.
			Orders::iterator pO;
			for (pO = m_pOrders->begin(); pO != m_pOrders->end(); pO++)
			{
				if ((*pO)->GetType() == MOVE && 
					(*pO)->GetTarget() == pThisProv)
				{
					// The next two steps are necessary in case support has been already 
					// added - to decide for instance if a brace was balanced or not.
					(*pO)->GetUnit()->m_nAttackStrength = 1;
					(*pO)->GetUnit()->m_nDislodgingStrength = 1;

					vRivals.push_back((*pO));
					AddSupports((*pO));
				}
			}

			// Find the strongest attacks of all in the vector.
			int nMaxStrength = 0;
			for (Orders::iterator pJ = vRivals.begin(); pJ != vRivals.end(); pJ++)
			{
				int nStrength = (*pJ)->GetUnit()->m_nAttackStrength;
				if (nStrength > nMaxStrength)
					nMaxStrength = nStrength;
			}

			// Extra step to get the name(s) of the strongest unit(s).
			// Create a vector of strongest attacks.
			Orders vStrongest;
			for (pJ = vRivals.begin(); pJ != vRivals.end(); pJ++)
			{
				int nStrength = (*pJ)->GetUnit()->m_nAttackStrength;
				if (nStrength == nMaxStrength)
				{
					vStrongest.push_back((*pJ));
				}
			}

			// Bounce all active attacks but the strongest ones.
			for (pJ = vRivals.begin(); pJ != vRivals.end(); pJ++)
			{
				if ((*pJ)->GetUnit()->m_nAttackStrength < nMaxStrength)
				{
					(*pJ)->SetType(BOUNCE);
					CString strTemp;
					strTemp.Format(" Defeated by %s %d:%d;", 
						vStrongest.front()->GetLocation()->m_strName, 
						nMaxStrength, 
						(*pJ)->GetUnit()->m_nAttackStrength);
					(*pJ)->m_strOutput += strTemp;
				}
			}
			
			// If more than one attack remains, bounce them all.
			if (vStrongest.size() > 1)
			{
				for (pJ = vStrongest.begin(); pJ != (vStrongest.end() - 1); pJ++)
				{
					(*pJ)->SetType(BOUNCE);
					CString strTemp;
					strTemp.Format(" Bounce with %s %d:%d;", 
						vStrongest.back()->GetLocation()->m_strName, 
						(*pJ)->GetUnit()->m_nAttackStrength, 
						(*pJ)->GetUnit()->m_nAttackStrength);
					(*pJ)->m_strOutput += strTemp;
				}
				PSOrder* pLast = vStrongest.back();
				pLast->SetType(BOUNCE);
				CString strTemp;
				strTemp.Format(" Bounce with %s %d:%d;", 
					vStrongest.front()->GetLocation()->m_strName, 
					(*pJ)->GetUnit()->m_nAttackStrength, 
					(*pJ)->GetUnit()->m_nAttackStrength);
				pLast->m_strOutput += strTemp;

				// Mark the target as standoff.
				pThisProv->SetStandoff(TRUE);
			}

			else
			{
				// If the target province is either empty or occupied by 
				// an advancing unit, promote the attack to advancing.
				for (pJ = vRivals.begin(); pJ != vRivals.end(); pJ++)
				{
					if ((*pJ)->GetType() != BOUNCE)
						break;
				}

				// pJ should point now to the remaining attack.
				PSOrder* pTargetOrder = OrderSearch(pThisProv);
				if (!pThisProv->GetOccupant() || 
					pTargetOrder->GetType() == ADVANCING)
				{
					(*pJ)->SetType(ADVANCING);
				}
				else
				{
					// If the attacker's dislodging strength is greater than the defender's
					// defense strength, the attack succeeds and the defender is dislodged.
					AddSupports(pTargetOrder);
					if ((*pJ)->GetUnit()->m_nDislodgingStrength > 
						pTargetOrder->GetUnit()->m_nDefenseStrength)
					{
						(*pJ)->SetType(ADVANCING);
						// If the dislodged unit is a convoying fleet, disrupt the move order of 
						// the convoyed army.
						// TODO: Maybe I should void the rest of the convoy orders for this army.
						CString strTemp;
						if (pTargetOrder->GetType() == CONVOY)
						{
							PSOrder* pConvoyedArmyOrder = OrderSearch(pTargetOrder->GetSource());
							if (pConvoyedArmyOrder->GetType() == MOVE)
							{
								pConvoyedArmyOrder->SetType(DISRUPT);
								strTemp.Format(" Disrupted by %s;", 
									(*pJ)->GetLocation()->m_strName);
								pConvoyedArmyOrder->m_strOutput += strTemp;
							}
						}
						pTargetOrder->SetType(DISLODGED);
						strTemp.Format(" Dislodged by %s %d:%d;", 
							(*pJ)->GetLocation()->m_strName, 
							(*pJ)->GetUnit()->m_nDislodgingStrength, 
							pTargetOrder->GetUnit()->m_nDefenseStrength);
						pTargetOrder->m_strOutput += strTemp;
						// This unit will have to retreat (or disband). Add it to m_vRet, 
						// GetRetreats will skip it if it is in fact a disband.
						Retreat* pNewRetreat = new Retreat;
						pNewRetreat->m_pUnit = pTargetOrder->GetUnit();
						pNewRetreat->m_pOrig = (*pJ)->GetLocation();
						TG.AddRetreat(pNewRetreat);
					}
					else
					{
						(*pJ)->SetType(BOUNCE);
						CString strTemp;
						strTemp.Format(" Bounce with %s %d:%d;", 
							pTargetOrder->GetLocation()->m_strName, 
							pTargetOrder->GetUnit()->m_nDefenseStrength, 
							pTargetOrder->GetUnit()->m_nDefenseStrength);
						(*pJ)->m_strOutput += strTemp;
					}
				}
			}

			// Take the battleground mark off.
			pThisProv->SetBattleground(FALSE);
		}
	}
}


void PSJudge::AddSupports(PSOrder* pSupported)
{
	// Adds all the active supports to the unit ordered by pSupported
	Orders::iterator pO;
	for (pO = m_pOrders->begin(); pO != m_pOrders->end(); pO++)
	{
		if ((*pO)->GetType() == SUPPORT) 
		{
			if ((*pO)->GetSource() == pSupported->GetLocation())
			{
				ORDER_TYPE OType = pSupported->GetType();
				switch(OType)
				{
				case HOLD:
				case SUPPORT:
				case CONVOY:
				case CUT:
				case VOID:
					pSupported->GetUnit()->m_nDefenseStrength++;
					pSupported->GetUnit()->m_nAttackStrength = 0;
					pSupported->GetUnit()->m_nDislodgingStrength = 0;
					pSupported->GetUnit()->m_nBraceStrength = 0;
					break;
				case MOVE:
					pSupported->GetUnit()->m_nDefenseStrength = 1;
					pSupported->GetUnit()->m_nAttackStrength++;
					// Only a unit with a valid order to attack an occupied province
					// has a dislodgement strength. Don't increment its strength
					// if the support comes from a countryman of the defender or if the 
					// defender is a countryman of the attacker.
					PSProvince* pProv = pSupported->GetTarget();
					if (pProv->GetOccupant() && 
						pProv->GetOccupant() != (*pO)->GetUnit()->GetOwner() && 
						pProv->GetOccupant() != pSupported->GetUnit()->GetOwner())
						pSupported->GetUnit()->m_nDislodgingStrength++;

					break;
				}
			}
		}
	}
}


PSOrder* PSJudge::OrderSearch(PSProvince* pPrvSource)
{
	// Returns a pointer to the order given for the unit in pPrvSource.
	Orders::iterator pO;
	for (pO = m_pOrders->begin(); pO != m_pOrders->end(); pO++)
	{
		if ((*pO)->GetLocation() == pPrvSource)
			return *pO;
	}

	return NULL;
}


/*============================================
This is where the adjudication process starts.
============================================*/

CString PSJudge::Adjudicate(Orders* pvOrders)
{
	m_pOrders = pvOrders;
	m_InitialOrders = *pvOrders;
	Initialize();
	ValidateOrders();
	DemoteVoidOrders();// Stage 1
	DirectAttacksCutSupport();// Stage 2
	AttacksAgainstConvoys();// Stage 3
	ConvoyedAttacksCutSupport();// Stage 4
	ResolveBraces();// Stage 5
	RectifyRings();// Stage 6
	FightOrdinaryBattles();// Stage 7

	return WriteVerdict();
}


void PSJudge::Initialize()
{
	// Start from zero.

	// Clean the strengths.
	int nUnits = TG.GetNumberOfUnits();
	for (int i = 0; i < nUnits; i++)
	{
		PSUnit* pUnit = TG.GetUnit(i);
		pUnit->m_nDefenseStrength = 0;
		pUnit->m_nAttackStrength = 0;
		pUnit->m_nDislodgingStrength = 0;
		pUnit->m_nBraceStrength = 0;
	}

	// Consider Hold for unordered units.
	Orders::iterator pO;
	for (i = 1; i <= TG.GetNumberOfProvinces(); i++)
	{
		PSProvince* pCurrentProvince = TG.GetProvince(i);
		if (pCurrentProvince->GetOccupant())
		{
			BOOL bFlag = FALSE;
			for (pO = m_pOrders->begin(); pO != m_pOrders->end(); pO++)
			{
				if ((*pO)->GetLocation() == pCurrentProvince)
				{
					bFlag = TRUE;
					break;
				}
			}
			if (!bFlag)
			{
				CString strOrd;
				strOrd.Format("%s H", pCurrentProvince->m_strName);
				PSOrder* pOrd = new PSOrder(strOrd);
				// Add this order to the vector.
				m_pOrders->push_back(pOrd);
			}
		}
	}

	// Append ":" at the end of each order.
	for (pO = m_pOrders->begin(); pO != m_pOrders->end(); pO++)
	{
		(*pO)->m_strOutput += ":";
	}
}


void PSJudge::ValidateOrders()
{
	// Convoys done by units in dry lands.
	Orders::iterator pO;
	for (pO = m_pOrders->begin(); pO != m_pOrders->end(); pO++)
	{
		if ((*pO)->GetType() == CONVOY && 
			(*pO)->GetLocation()->GetType() == DRY_LAND)
		{
			TRACE("%s%s", (*pO)->GetLocation()->m_strName, 
				": Convoy by unit on  dry land\r\n");
			(*pO)->SetType(ILLEGAL);
			continue;
		}

		// Remote orders for fleets.
		if ((*pO)->GetType() == MOVE && 
			!((*pO)->m_dwFlags & ORD_DIRECT) && 
			(*pO)->GetUnit()->GetType() == FLEET)
		{
			TRACE("%s%s", (*pO)->GetLocation()->m_strName, 
				": Remote order for fleet\r\n");
			(*pO)->SetType(ILLEGAL);
			continue;
		}

		// Support given in remote regions.
		if ((*pO)->GetType() == SUPPORT && 
			!((*pO)->GetUnit()->HasAdjacent((*pO)->GetTarget())))
		{
			TRACE("%s%s", (*pO)->GetLocation()->m_strName, 
				": Support in a remote location\r\n");
			(*pO)->SetType(ILLEGAL);
			continue;
		}
	
		// Armies with a body of water as a target province.
		if ((*pO)->GetUnit()->GetType()== ARMY && 
			(*pO)->GetTarget()->GetType() == WATER_BODY)
		{
			TRACE("%s%s", (*pO)->GetLocation()->m_strName, 
				": Army with a body of water as a target province\r\n");
			(*pO)->SetType(ILLEGAL);
			continue;
		}

		// Target is not adjacent for direct moves.
		if ((*pO)->GetType() == MOVE && 
			(*pO)->m_dwFlags & ORD_DIRECT && 
			!((*pO)->GetUnit()->HasAdjacent((*pO)->GetTarget())))
		{
			TRACE("%s%s", (*pO)->GetLocation()->m_strName, 
				": Target is not adjacent for direct move\r\n");
			(*pO)->SetType(ILLEGAL);
			continue;
		}

		// Path is not specified or incorrect for remote orders.
		if ((*pO)->GetType() == MOVE && 
			!((*pO)->m_dwFlags & ORD_DIRECT))
		{
			// The path must be adjacent to the location.
			BOOL bFlag = FALSE;
			if (!(((*pO)->m_vPath).front()->HasNeighbor((*pO)->GetLocation())))
				bFlag = TRUE;

			// The steps must be adjacent one to the other.
			Provinces::iterator pJ;
			for (pJ = (*pO)->m_vPath.begin(); pJ != (*pO)->m_vPath.end() - 1; pJ++)
			{
				if (!((*pJ)->HasNeighbor(*(pJ + 1))))
					bFlag = TRUE;
			}

			// The last step must be adjacent to the target.
			if (!(((*pO)->m_vPath).back()->HasNeighbor((*pO)->GetTarget())))
				bFlag = TRUE;

			if (bFlag)
			{
				TRACE("%s%s", (*pO)->GetLocation()->m_strName, 
					": Interrupted convoy\r\n");
				(*pO)->SetType(ILLEGAL);
				continue;
			}
		}
	}
}


// Stage 1
void PSJudge::DemoteVoidOrders()
{
	// #1
	// Support in holding is void if there is no unit to be supported 
	// or if there is one, but it is ordered to move.
	Orders::iterator pO;
	for (pO = m_pOrders->begin(); pO != m_pOrders->end(); pO++)
	{
		if ((*pO)->GetType() == SUPPORT && 
			(*pO)->GetTarget() == (*pO)->GetSource())
		{
			PSOrder* pSupported = OrderSearch((*pO)->GetSource());
			if (!pSupported || (pSupported->GetType() == MOVE))
			{
				TRACE("%s%s", (*pO)->GetSource()->m_strName, ": Void in Stage 1:1\r\n");
				(*pO)->SetType(VOID);
			}
		}
	}

	// #2
	// Support of a move is void if the supported unit doesn't attempt the exact move.
	for (pO = m_pOrders->begin(); pO != m_pOrders->end(); pO++)
	{
		if ((*pO)->GetType() == SUPPORT && 
			(*pO)->GetTarget() != (*pO)->GetSource())
		{
			PSOrder* pSupported = OrderSearch((*pO)->GetSource());
			if (!pSupported || (pSupported->GetType() != MOVE) || 
				(*pO)->GetTarget() != pSupported->GetTarget())
			{
				TRACE("%s%s", (*pO)->GetLocation()->m_strName, ": Void in Stage 1:2\r\n");
				(*pO)->SetType(VOID);
			}
		}
	}

	// #3
	// Convoy is void if the Army to be convoyed doesn't attempt the exact move.
	for (pO = m_pOrders->begin(); pO != m_pOrders->end(); pO++)
	{
		if ((*pO)->GetType() == CONVOY)
		{
			PSOrder* pConvoyed = OrderSearch((*pO)->GetSource());
			if (!pConvoyed || pConvoyed->GetType() != MOVE || 
				pConvoyed->GetTarget() != (*pO)->GetTarget())
			{
				TRACE("%s%s", (*pO)->GetLocation()->m_strName, ": Void in Stage 1:3\r\n");
				(*pO)->SetType(VOID);
			}
		}
	}

	// #4
	// An order to move by convoy is void if any of the fleets in its sea-chain 
	// fails to attempt the proper convoy.
	for (pO = m_pOrders->begin(); pO != m_pOrders->end(); pO++)
	{
		// Search for moves by convoy.
		if ((*pO)->GetType() == MOVE && 
			!((*pO)->m_dwFlags & ORD_DIRECT))
		{
			Provinces::iterator pJ;
			BOOL bFlag = FALSE;
			for (pJ = (*pO)->m_vPath.begin(); pJ != (*pO)->m_vPath.end(); pJ++)
			{
				PSOrder* pConv = OrderSearch(*pJ);
				if (!pConv || 
					(pConv && 
					(pConv->GetType() != CONVOY || 
					pConv->GetSource() != (*pO)->GetLocation() || 
					pConv->GetTarget() != (*pO)->GetTarget())))
				{
					// This fleet does not have a valid convoy order.
					TRACE("%s%s", (*pO)->GetLocation()->m_strName, ": Void in Stage 1:4\r\n");
					(*pO)->SetType(VOID);
					bFlag = TRUE;
					break;
				}
			}
			if (bFlag)
			{
				// We also must void any supports to and convoys of that move.
				Orders::iterator pS;
				for (pS = m_pOrders->begin(); pS != m_pOrders->end(); pS++)
				{
					if ((*pS)->GetTarget() == (*pO)->GetTarget() && 
						(*pS)->GetSource() == (*pO)->GetLocation())
					{
						TRACE("%s%s", (*pS)->GetLocation()->m_strName, ": Void in Stage 1:4\r\n");
						(*pS)->SetType(VOID);
					}
				}
			}
		}
	}
}


// Stage 2
void PSJudge::DirectAttacksCutSupport()
{
	// Support is cut if the order is active, direct and flank
	// and the attacked unit is foreign and has an active order to support
	Orders::iterator pO;
	for (pO = m_pOrders->begin(); pO != m_pOrders->end(); pO++)
	{
		if ((*pO)->GetType() == MOVE && 
			(*pO)->m_dwFlags & ORD_DIRECT)
		{
			PSOrder* pAttacked = OrderSearch((*pO)->GetTarget());
			if (pAttacked && 
				pAttacked->GetUnit()->GetOwner() != (*pO)->GetUnit()->GetOwner() && 
				pAttacked->GetType() == SUPPORT && 
				pAttacked->GetTarget() != (*pO)->GetLocation()/*This is what 'flank' means*/)
			{
				pAttacked->SetType(CUT);
				CString strTemp;
				strTemp.Format(" Cut by %s;", 
					(*pO)->GetLocation()->m_strName);
				pAttacked->m_strOutput += strTemp;
			}
		}
	}
}


// Stage 3
void PSJudge::AttacksAgainstConvoys()
{
	Orders::iterator pO;
	for (pO = m_pOrders->begin(); pO != m_pOrders->end(); pO++)
	{
		// Flag as battleground all provinces with an attacked convoying fleet.
		if ((*pO)->GetType() == MOVE)
		{
			PSOrder* pAttacked = OrderSearch((*pO)->GetTarget());
			if (pAttacked && 
				pAttacked->GetType() == CONVOY)
			{
				(*pO)->GetTarget()->SetBattleground(TRUE);
				ResolveAllBattlegrounds();
			}
		}
	}
}


// Stage 4
void PSJudge::ConvoyedAttacksCutSupport()
{
	Orders::iterator pO;
	for (pO = m_pOrders->begin(); pO != m_pOrders->end(); pO++)
	{
		// An active, convoyed attack will cut the support of a foreign unit
		// with an active order to support.
		if (!((*pO)->m_dwFlags & ORD_DIRECT))
		{
			PSOrder* pAttacked = OrderSearch((*pO)->GetTarget());
			if (pAttacked && 
				pAttacked->GetType() == SUPPORT && 
				pAttacked->GetUnit()->GetOwner() != (*pO)->GetUnit()->GetOwner())
			{
				pAttacked->SetType(CUT);
				CString strTemp;
				strTemp.Format(" Cut by %s;", 
					(*pO)->GetLocation()->m_strName);
				pAttacked->m_strOutput += strTemp;
			}
		}
	}
}


// Stage 5
void PSJudge::ResolveBraces()
{
	// Identify each brace.
	Orders::iterator pO;
	for (pO = m_pOrders->begin(); pO != m_pOrders->end(); pO++)
	{
		if ((*pO)->GetType() == MOVE && 
			(*pO)->m_dwFlags & ORD_DIRECT)
		{
			PSOrder* pAttacked = OrderSearch((*pO)->GetTarget());
			if (pAttacked && 
				pAttacked->GetType() == MOVE && 
				pAttacked->m_dwFlags & ORD_DIRECT && 
				pAttacked->GetTarget() == (*pO)->GetLocation())
			{
				// This is a brace.
				// See if the brace is balanced or not.
				/*A brace is balanced if: 
				- its two units attack one another with equal strength, or 
				- the weaker unit's attack strength is at least as great as 
				the stronger unit's dislodging strength, or 
				- the two units are countrymen.*/
				AddSupports((*pO));
				AddSupports(pAttacked);
				if ((*pO)->GetUnit()->m_nAttackStrength == pAttacked->GetUnit()->m_nAttackStrength || 
					((*pO)->GetUnit()->m_nAttackStrength < pAttacked->GetUnit()->m_nAttackStrength && 
					(*pO)->GetUnit()->m_nAttackStrength >= pAttacked->GetUnit()->m_nDislodgingStrength) || 
					((*pO)->GetUnit()->m_nAttackStrength > pAttacked->GetUnit()->m_nAttackStrength && 
					(*pO)->GetUnit()->m_nAttackStrength <= pAttacked->GetUnit()->m_nDislodgingStrength) || 
					(*pO)->GetUnit()->GetOwner() == pAttacked->GetUnit()->GetOwner())
				{
					// This brace is balanced.
					// Assign both halves of the brace their brace strength. (The brace strength 
					// is simply the attack strength of the other half of the brace.)
					(*pO)->GetUnit()->m_nBraceStrength = pAttacked->GetUnit()->m_nAttackStrength;
					pAttacked->GetUnit()->m_nBraceStrength = (*pO)->GetUnit()->m_nAttackStrength;
					// Bounce both attacks.
					(*pO)->SetType(BOUNCE);
					pAttacked->SetType(BOUNCE);
					CString strTemp;
					strTemp.Format(" Bounce with %s %d:%d;", 
						(*pO)->GetLocation()->m_strName, 
						pAttacked->GetUnit()->m_nAttackStrength, 
						(*pO)->GetUnit()->m_nAttackStrength);
					pAttacked->m_strOutput += strTemp;
					strTemp.Format(" Bounce with %s %d:%d;", 
						pAttacked->GetLocation()->m_strName, 
						(*pO)->GetUnit()->m_nAttackStrength, 
						pAttacked->GetUnit()->m_nAttackStrength);
					(*pO)->m_strOutput += strTemp;

					// If either half of the brace is still the target of any active attack, flag that half as a battleground. If both are targets, flag both halves.
					Orders::iterator pU;
					for (pU = m_pOrders->begin(); pU != m_pOrders->end(); pU++)
					{
						if ((*pU)->GetType() == MOVE)
						{
							PSProvince* pi = (*pU)->GetTarget();
							if (pi == (*pO)->GetLocation())
							{
								(*pO)->GetUnit()->m_nDefenseStrength = (*pO)->GetUnit()->m_nBraceStrength;
								pi->SetBattleground(TRUE);
							}
							if (pi == pAttacked->GetLocation())
							{
								pAttacked->GetUnit()->m_nDefenseStrength = pAttacked->GetUnit()->m_nBraceStrength;
								pi->SetBattleground(TRUE);
							}
						}
					}
					// Resolve all active attacks on each battleground province, but let each defender 
					// use its brace strength in place of its ordinary defense strength.
					ResolveAllBattlegrounds();
				}
				else
				{
					// This brace is unbalanced.
					PSOrder* pStrong, *pWeak;
					if ((*pO)->GetUnit()->m_nAttackStrength > pAttacked->GetUnit()->m_nAttackStrength)
					{
						pStrong = (*pO);
						pWeak = pAttacked;
					}
					else
					{
						pStrong = pAttacked;
						pWeak = (*pO);
					}
					// Assign the stronger half of the brace its brace strength.
					pStrong->GetUnit()->m_nBraceStrength = pWeak->GetUnit()->m_nAttackStrength;
					// Bounce the weaker attack.
					pWeak->SetType(BOUNCE);
					CString strTemp;
					strTemp.Format(" Defeated by %s %d:%d;", 
						pStrong->GetLocation()->m_strName, 
						pStrong->GetUnit()->m_nAttackStrength, 
						pWeak->GetUnit()->m_nAttackStrength);
					pWeak->m_strOutput += strTemp;
					// Flag the weaker half as a battleground.
					pWeak->GetLocation()->SetBattleground(TRUE);
					// Resolve all active attacks on the battleground.
					ResolveAllBattlegrounds();
					// If the original province of the stronger half of the brace still remains 
					// the target of any active attack
					PSProvince* pPrv = pStrong->GetLocation();
					Orders::iterator pU;
					for (pU = m_pOrders->begin(); pU != m_pOrders->end(); pU++)
					{
						if ((*pU)->GetType() == MOVE && 
							(*pU)->GetTarget() == pPrv)
						{
							// Flag that province a battleground.
							pPrv->SetBattleground(TRUE);
							// Resolve all active attacks on the battleground -- but, if the original 
							// stronger half has bounced and must defend in place, let it use its brace strength in
							// place of its ordinary defense strength.
							if (pStrong->GetType() == BOUNCE)
							{
								pStrong->GetUnit()->m_nDefenseStrength = pStrong->GetUnit()->m_nBraceStrength;
							}
							ResolveAllBattlegrounds();
						}
					}
				}
			}
		}
	}
}


// Stage 6
void PSJudge::RectifyRings()
{
	// Identify rings. 
	Orders vRing;
	Orders::iterator pO;
	for (pO = m_pOrders->begin(); pO != m_pOrders->end(); pO++)
	{
		if ((*pO)->GetType() == MOVE)
		{
			// Mark this province as the beginning of the ring, in case there will be one.
			PSOrder* pBegin = (*pO);

			// Go from target to target and see if I get back to an order that is already in vRing. 
			// I assume that there are no more braces left.
			PSOrder* pCurrent = (*pO);
			do
			{
				vRing.push_back(pCurrent);
				pCurrent = OrderSearch(pCurrent->GetTarget());
				BOOL bYesItIs = FALSE;
				if (pCurrent && pCurrent->GetType() == MOVE)
				{
					Orders::iterator pO2;
					for (pO2 = vRing.begin(); pO2 != vRing.end(); pO2++)
					{
						if ((*pO2) == pCurrent)
						{
							bYesItIs = TRUE;
							// Delete all orders from vRing that are not actually in the ring.
							vRing.erase(vRing.begin(), pO2);
							break;
						}
					}
				}
				
				if (bYesItIs)
				{
					// I just found a ring. The orders in the ring are in vRing.

					// For each province in the ring
					Orders::iterator pJ;
					for (pJ = vRing.begin(); pJ != vRing.end(); pJ++)
					{
						PSProvince* pi = (*pJ)->GetLocation();
						// Of all the active attacks on the province, find the single strongest one. 
						// Create a vector of Rivals.
						Orders vRivals;

						// Find all participating attacks and put them in the vector of Rivals.
						Orders::iterator pU;
						for (pU = m_pOrders->begin(); pU != m_pOrders->end(); pU++)
						{
							if ((*pU)->GetType() == MOVE && 
								(*pU)->GetTarget() == pi)
							{
								vRivals.push_back(*pU);
								AddSupports(*pU);					
							}
						}

						// Find the strongest attack of all in the vector.
						int nMaxStrength = 0;
						Orders::iterator pV;
						for (pV = vRivals.begin(); pV != vRivals.end(); pV++)
						{
							int nStrength = (*pV)->GetUnit()->m_nAttackStrength;
							if (nStrength > nMaxStrength)
								nMaxStrength = nStrength;
						}

						// Extra step to get the name(s) of the strongest unit(s).
						// Create a vector of strongest attacks.
						Orders vStrongest;
						for (pV = vRivals.begin(); pV != vRivals.end(); pV++)
						{
							int nStrength = (*pV)->GetUnit()->m_nAttackStrength;
							if (nStrength == nMaxStrength)
							{
								vStrongest.push_back((*pV));
							}
						}

						// Bounce all active attacks but the strongest.
						for (pV = vRivals.begin(); pV != vRivals.end(); pV++)
						{
							if ((*pV)->GetUnit()->m_nAttackStrength < nMaxStrength)
							{
								(*pV)->SetType(BOUNCE);
								CString strTemp;
								strTemp.Format("%s%s%s%d%s%d%s", " Defeated by %s %d:%d;", 
									vStrongest.front()->GetLocation()->m_strName, 
									nMaxStrength,
									(*pV)->GetUnit()->m_nAttackStrength);
								(*pV)->m_strOutput += strTemp;
							}
						}
						
						// If more than one attack remains, bounce them all.
						if (vStrongest.size() > 1)
						{
							for (pV = vStrongest.begin(); pV != (vStrongest.end() - 1); pV++)
							{
								(*pV)->SetType(BOUNCE);
								CString strTemp;
								strTemp.Format(" Bounce with %s %d:%d;", 
									vStrongest.back()->GetLocation()->m_strName, 
									vStrongest.back()->GetUnit()->m_nAttackStrength, 
									vStrongest.front()->GetUnit()->m_nAttackStrength);
								(*pV)->m_strOutput += strTemp;
							}
							PSOrder* pLast = vStrongest.back();
							pLast->SetType(BOUNCE);
							CString strTemp;
							strTemp.Format(" Bounce with %s %d:%d;", 
								vStrongest.front()->GetLocation()->m_strName, 
								vStrongest.front()->GetUnit()->m_nAttackStrength, 
								vStrongest.back()->GetUnit()->m_nAttackStrength);
							pLast->m_strOutput += strTemp;

							// Mark the target as standoff.
							pi->SetStandoff(TRUE);
						}
					}

					// If every single attack within the ring itself remains active -- that is, 
					// if only attacks from outside the ring have bounced -- then promote all 
					// the attacks within the ring to advancing.
					BOOL bAllActive = TRUE;
					for (pJ = vRing.begin(); pJ != vRing.end(); pJ++)
					{
						if ((*pJ)->GetType() != MOVE)
							bAllActive = FALSE;
					}
					if (bAllActive)
					{
						for (pJ = vRing.begin(); pJ != vRing.end(); pJ++)
						{
							(*pJ)->SetType(ADVANCING);
						}
					}
					break;
				}
			}
			while(pCurrent && pCurrent->GetType() == MOVE);
			vRing.clear();
		}
	}
}


// Stage 7
void PSJudge::FightOrdinaryBattles()
{
	// Flag as battlegrounds all provinces that are targets except
	// the ones that are occupied by moving units.
	BOOL bTargets = TRUE;
	while (bTargets)
	{
		bTargets = FALSE;
		Orders::iterator pO;
		for (pO = m_pOrders->begin(); pO != m_pOrders->end(); pO++)
		{
			if ((*pO)->GetType() == MOVE)
			{
				PSProvince* pPrv = (*pO)->GetTarget();
				PSOrder* pAttacked = OrderSearch(pPrv);
				if (!pAttacked || 
					pAttacked->GetType() != MOVE)
				{
					pPrv->SetBattleground(TRUE);
					bTargets = TRUE;
				}
			}
		}
		if (bTargets)
		{
			ResolveAllBattlegrounds();
		}
	}
}


CString PSJudge::WriteVerdict()
{
	CString strVerdict = "";
	for (int j = 1; j <= TG.GetNumberOfPowers(); j++)
	{
		BOOL bPower = FALSE;
		Orders::iterator pO;
		for (pO = m_InitialOrders.begin(); pO != m_InitialOrders.end(); pO++)
		{
			if ((*pO)->GetUnit()->GetOwner() == TG.GetPower(j))
			{
				if (!bPower)
				{
					strVerdict += TG.GetPower(j)->m_strName;
					strVerdict += ":\r\n";
					bPower = TRUE;
				}
				strVerdict += (*pO)->m_strOutput + "   ";
				switch((*pO)->GetType())
				{
				case HOLD:
				case MOVE:
				case SUPPORT:
				case CONVOY:
				case ADVANCING:
					strVerdict += " OK";
					break;
				default:
					break;
				}
				strVerdict += "\r\n";
			}
		}
		if (bPower)
			strVerdict += "\r\n";
	}

	return strVerdict;
}


CString PSJudge::GetRetreats()
{
	// Fill in the Retreat vector and add them to verdict.
	CString strVerdict = "";
	for (int i = 0; i < TG.GetNumberOfRetreats(); i++)
	{
		strVerdict += "\r\n\r\n";
		Retreat* pThisRet = TG.GetRetreat(i);
		PSUnit* pUnit = pThisRet->m_pUnit;
        strVerdict += pUnit->GetOwner()->m_strName;
		CompatibleRetreats(pThisRet);
        if (!pThisRet->m_Possib.empty())
		{
			CString strRet;
			strRet.Format(" may retreat %s in %s to: ", 
				(pUnit->GetType() == ARMY) ? "army" : "fleet", 
				pUnit->GetLocation()->m_strName);
			strVerdict += strRet;
        	for (int j = 0; j < pThisRet->m_Possib.size(); j++)
			{
        		strVerdict += pThisRet->m_Possib[j]->m_strName;
        		strVerdict += " ";
        	}
        }
        else
		{
			strVerdict += " disbands unit in ";
        	strVerdict += pUnit->GetLocation()->m_strName;
			pThisRet->m_pUnit->Destroy();
			TG.DeleteRetreat(pThisRet);
			i--;// Because DeleteRetreat erases pRet from the vector.
		}
	}

	// Add the name of the best adjudicator.
	strVerdict += "\r\n\r\n*** Adjudicated with Diplo 2.8.2 ***\r\n\r\n\r\n";

	return strVerdict;
}	


void PSJudge::CompatibleRetreats(Retreat* pRet)
{
	// Fills m_Possib of pRet.
	pRet->m_Possib.clear();
	PSProvince* pLoc = pRet->m_pUnit->GetLocation();
	ASSERT(pLoc);
	// Look for adjacent provinces that are empty,
	// different than the one the attacker came from and 
	// where there was no standoff in.
	
	// TODO: If the attacker came convoyed, it might be able to move into 
	// its location.
	for (int i = 0; i < pLoc->GetNumberOfNeighbors(); i++)
	{
		PSProvince* pNeigh = pLoc->GetNeighbor(i);
		if (pRet->m_pUnit->HasAdjacent(pNeigh) && 
			pNeigh->GetOccupant() == NULL && 
			pNeigh != pRet->m_pOrig && 
			!pNeigh->GetStandoff())
		{
			pRet->m_Possib.push_back(pNeigh);
		}
	}
}


CString PSJudge::GetAdjustments()
{
	// Add builds and disbands.
	ASSERT(TG.GetSeason() == FALL);
	CString strVerdict = "";
	TG.ClearDisbands();
	TG.ClearBuilds();

	// See which SC-s have changed the ownership.
	for (int j = 1; j <= TG.GetNumberOfProvinces(); j++)
	{
		PSProvince* pj = TG.GetProvince(j);
		if (pj->GetSC())
		{
			PSPower* pOwner = pj->GetOwner();
			PSPower* pOccupant = pj->GetOccupant();
			if (pOwner != pOccupant && pOccupant)
			{
				if (pOwner)
				{
					pOwner->Loses(pj);
					pOccupant->Gains(pj);
					strVerdict += pj->GetOwner()->m_strName;
					strVerdict += " takes ";
					strVerdict += pj->m_strName;
					strVerdict += " from ";
					strVerdict += pOwner->m_strName;
					strVerdict += ".\r\n";
				}
				else
				{
					pOccupant->Gains(pj);
					strVerdict += pj->GetOwner()->m_strName;
					strVerdict += " gains ";
					strVerdict += pj->m_strName;
					strVerdict += ".\r\n";
				}
			}
		}
	}

	for (int i = 1; i <= TG.GetNumberOfPowers(); i++)
	{
		PSPower* pi = TG.GetPower(i);
		int k = pi->GetNumberOfSCs() - pi->GetNumberOfUnits();
		if (k > 0)
		{
			// Get the available HSC-s.
			Provinces AvHSCs = pi->GetAvailableHSCs();
			int nPossib = AvHSCs.size();
			if (k && !nPossib)
				AfxMessageBox(pi->m_strName + " has no available Home Supply Center to build.", 
				MB_ICONINFORMATION);
			k = min(k, nPossib);

			for (int j = 0; j < k; j++)
			{
				Build* pNewBuild = new Build;
				pNewBuild->m_pPower = pi;
				TG.AddBuild(pNewBuild);
			}
			if (k)
			{
				strVerdict += "\r\n";
				strVerdict += pi->m_strName;
				strVerdict += " may build ";
				char buf[3];
				_itoa(k, buf, 10);
				strVerdict += (CString) buf;
				strVerdict += (k == 1) ? " unit." : " units.";
			}
		}
		if (k < 0)
		{
			for (int j = k; j < 0; j++)
			{
				Disband* pNewDisband = new Disband;
				pNewDisband->m_pPower = pi;
				TG.AddDisband(pNewDisband);
			}
			strVerdict += "\r\n";
			strVerdict += pi->m_strName;
			strVerdict += " disbands ";
			char buf[3];
			_itoa(-k, buf, 10);
			strVerdict += (CString) buf;
			strVerdict += (k == -1) ? " unit." : " units.";
		}
	}

	return strVerdict;
}


BOOL PSJudge::GetRetreat(PSProvince* pPrv)
{
	int nRetreats = TG.GetNumberOfRetreats();
	ASSERT(nRetreats);
	Retreat* pThisRet = TG.GetRetreat(nRetreats - 1);
	for (int i = 0; i < pThisRet->m_Possib.size(); i++)
	{
		if (pPrv == pThisRet->m_Possib[i])
		{
			PSUnit* pThisUnit = pThisRet->m_pUnit;
			pThisUnit->SetDesiredCoast(pPrv->GetCoast(0));

			// See if it is a retreat to a bicoastal province.
			if (pPrv->IsBicoastal())
			{
				return (pThisUnit->OrderToBicoastal(pThisUnit->GetLocation(), pPrv));
			}
			return TRUE;
		}
	}
	return FALSE;
}


PSUnit* PSJudge::GetBuild(UNIT_TYPE uType, PSProvince* pLoc, PSPower* pPow)
{
	// Check for illegal entries.
	// The SC must be empty, must belong to the building power and be its HSC.
	PSUnit* pUnit = NULL;
	if (!pPow->HasHSCAvailable(pLoc))
		return NULL;

	if (uType == ARMY)
		pUnit = new PSArmy(pPow, pLoc);
	else
		pUnit = new PSFleet(pPow, pLoc);
	// Fleets must be built only on coastal provinces.
	if (uType == FLEET && !pLoc->IsCoastal())
	{
		delete pUnit;
		return NULL;
	}

	if (pLoc->IsBicoastal()) 
	{
		if (!pUnit->OrderToBicoastal(NULL, pLoc))
		{
			delete pUnit;
			return NULL;
		}
		return pUnit;
	}

	return pUnit;
}


BOOL PSJudge::GetBuildGame(PSUnit* pUnit)
{
	BOOL bLegal = TRUE;
	// See if a  fleet is being built in a non-coastal province.
	int k = 0;
	PSProvince* pLoc = pUnit->GetLocation();
	if (pUnit->GetType() == FLEET && pLoc->GetType() == DRY_LAND && !pLoc->IsCoastal())
	{
		AfxMessageBox("Fleets can be built only in water bodies or coastal provinces.", 
			MB_ICONEXCLAMATION);
		return FALSE;
	}

	// See if an army is being built in a body water.
	if (pUnit->GetType() == ARMY && pLoc->GetType() == WATER_BODY)
	{
		AfxMessageBox("Armies can be built only on dry land.", MB_ICONEXCLAMATION);
		return FALSE;
	}

	// Builds in bicoastal provinces.
	if (pLoc->IsBicoastal())
	{
		bLegal = pUnit->OrderToBicoastal(NULL, pLoc);
	}

	return bLegal;
}

