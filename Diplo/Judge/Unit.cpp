// Unit.cpp: implementation of the PSUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Judge.h"
#include "Unit.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern PSGame TG;
CScrollView* PSUnit::m_pView = NULL;
int PSUnit::m_nHeight = 0;
int PSUnit::m_nWidth = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PSUnit::PSUnit() : 
m_Type(NOUNIT), 
m_pLocation(NULL),
m_pDesiredLocation(NULL),
m_pOwner(NULL),
m_nDefenseStrength(1), 
m_nAttackStrength(1), 
m_nDislodgingStrength(1),
m_nBraceStrength(1),
m_pCoast(NULL),
m_pDesiredCoast(NULL),
m_ptPosition(0, 0),
m_bMoving(FALSE)
{
}


PSUnit::~PSUnit()
{
}


PSUnit::PSUnit(PSUnit& Unit)
{
	m_pCoast = Unit.m_pCoast;
	m_pDesiredCoast = Unit.m_pDesiredCoast;
	m_pLocation = Unit.m_pLocation;
	m_pDesiredLocation = Unit.m_pDesiredLocation;
	m_pOwner = Unit.m_pOwner;
	m_Type = Unit.m_Type;
	m_pView = Unit.m_pView;
	m_nDefenseStrength = Unit.m_nDefenseStrength;
	m_nAttackStrength = Unit.m_nAttackStrength;
	m_nDislodgingStrength = Unit.m_nDislodgingStrength;
	m_nBraceStrength = Unit.m_nBraceStrength;
	m_ptPosition = m_pCoast->m_ptDrawUnit;
	m_bMoving = Unit.m_bMoving;
}


void PSUnit::SetMoving(PSProvince* pTarget, BOOL bMoving)
{
	m_bMoving = bMoving;
	m_pDesiredLocation = pTarget;
}


void PSUnit::Move(int nJump, BOOL bAnimate)
{
	// If bAnimate is TRUE, move nJump pixels towards m_pDesiredCoast, 
	// if not, move it in m_pDesiredCoast.

	// Keep the current position for erasing purposes.
	CPoint ptOldPos = m_ptPosition;
	if (bAnimate)
	{
		// Calculate the distance between the current position and Target.
		int nY = (m_pDesiredCoast->m_ptDrawUnit).y - m_ptPosition.y;
		int nX = (m_pDesiredCoast->m_ptDrawUnit).x - m_ptPosition.x;
		double dDist = sqrt((nX * nX) + (nY * nY));

		// If nDist is less than nJump pixels, end the move. 
		if (dDist < nJump)
		{
			m_bMoving = FALSE;
			m_pLocation = m_pDesiredLocation;
			m_pCoast = m_pDesiredCoast;
			m_ptPosition = m_pCoast->m_ptDrawUnit;
		}
		else 
		{
			// Calculate the new coordinates.
			int nNewX = m_ptPosition.x + ((m_pDesiredCoast->m_ptDrawUnit).x - m_ptPosition.x) / dDist * nJump;
			int nNewY = m_ptPosition.y + ((m_pDesiredCoast->m_ptDrawUnit).y - m_ptPosition.y) / dDist * nJump;
			m_ptPosition.x = nNewX;
			m_ptPosition.y = nNewY;
		}
	}
	else 
	{
		m_bMoving = FALSE;
		m_pLocation = m_pDesiredLocation;
		m_pCoast = m_pDesiredCoast;
		m_ptPosition = m_pCoast->m_ptDrawUnit;
	}
	// Erase the unit from the old position and draw it in the new one.
	CRect myRect(ptOldPos, CSize(m_nWidth, m_nHeight));
	CClientDC dc(m_pView);
	m_pView->OnPrepareDC(&dc);
	dc.LPtoDP(&myRect);
	m_pView->InvalidateRect(myRect, FALSE);
	Draw();
}


Retreat* PSUnit::IsRetreating(PSData* pData)
{
	if (!pData)
		pData = TG.GetData();

	return pData->GetRetreat(this);
}


void PSUnit::Draw()
{
	CRect myRect(m_ptPosition, CSize(m_nWidth, m_nHeight));
	CClientDC dc(m_pView);
	m_pView->OnPrepareDC(&dc);
	dc.LPtoDP(&myRect);
	m_pView->InvalidateRect(myRect, FALSE);
}


void PSUnit::Destroy()
{
	CRect myRect(m_ptPosition, CSize(m_nWidth, m_nHeight));
	CClientDC dc(m_pView);
	m_pView->OnPrepareDC(&dc);
	dc.LPtoDP(&myRect);
	TG.GetData()->DeleteUnit(this);
	delete this;

	// I hope I can do this after deleting this object, considerring that m_pView is a static member.
	m_pView->InvalidateRect(myRect, FALSE);
}


void PSUnit::Serialize(PSData* pData, CArchive& arh)
{
	if (arh.IsStoring()) 
	{
		arh << (long)m_Type;
		arh << m_pLocation->m_strName;
		if (m_Type == FLEET && m_pLocation->IsBicoastal())
			arh << GetCoast()->m_strName;
		Retreat* pRet = IsRetreating(pData);
		if (pRet)
		{
			long lSize = pRet->m_Possib.size();
			arh << lSize;
			for (long k = 0; k < lSize; k++)
			{
				arh << pRet->m_Possib[k]->m_strName;
			}
		}
		else
			arh << 0;
	}
	else 
	{
		CString strIn;
		if (m_Type == FLEET && m_pLocation->IsBicoastal())
		{
			CString strCoast; arh >> strCoast;
			ASSERT(strCoast != "");
			PSCoast* pCst = m_pLocation->GetCoast(strCoast);
			ASSERT(pCst);
			SetCoast(pCst);
			m_ptPosition = pCst->m_ptDrawUnit;
		}
		else
		{
			m_ptPosition = m_pLocation->GetCoast(0)->m_ptDrawUnit;
		}
		long lRetreating; arh >> lRetreating;
		if (lRetreating)
		{
			Retreat* pRet = new Retreat;
			pRet->m_pUnit = this;
			pRet->m_pOrig = NULL;
			for (int k = 0; k < lRetreating; k++)
			{
				arh >> strIn;
				pRet->m_Possib.push_back(TG.GetProvince(strIn));
			}
			pData->AddRetreat(pRet);
		}
	}
}


