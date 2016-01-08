// Order.cpp: implementation of the PSOrder class.
//

#include "stdafx.h"
#include "Judge.h"
#include "Order.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

extern PSGame TG;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PSOrder::PSOrder() : 
m_dwFlags(NULL), 
m_strOutput(""), 
m_pLocation(NULL), 
m_pSource(NULL), 
m_pTarget(NULL), 
m_pUnit(NULL)
{
}


// A constructor that takes the order in the form of a CString
PSOrder::PSOrder(CString strOrder) : 
m_dwFlags(NULL), 
m_strOutput(strOrder), 
m_pLocation(NULL), 
m_pSource(NULL), 
m_pTarget(NULL)
{
	CString strTemp;
	strTemp = strOrder.Left(3);
	m_pLocation = TG.GetProvince(strTemp);
	m_pUnit = m_pLocation->GetUnit();
	ASSERT(m_pUnit);
	switch (strOrder[4])
	{
	case 'H':
		m_Type = HOLD;
		break;
	case '-':
		m_Type = MOVE;
		break;
	case 'S':
		m_Type = SUPPORT;
		break;
	case 'C':
		m_Type = CONVOY;
		break;
	default:
		ASSERT (0);

	}
	if (m_Type == SUPPORT)
	{
		strTemp = strOrder.Mid(6, 3);
		m_pSource = TG.GetProvince(strTemp);
		if (strOrder[10] == 'H')
			m_pTarget = m_pSource;
		else
		{
			strTemp = strOrder.Right(3);
			m_pTarget = TG.GetProvince(strTemp);
		}
	}
	if (m_Type == MOVE)
	{
		strTemp = strOrder.Right(3);
		m_pTarget = TG.GetProvince(strTemp);
		m_pSource = NULL;
		m_pUnit->SetDesiredCoast(m_pTarget->GetCoast(0));
	}
	if (m_Type == CONVOY)
	{
		strTemp = strOrder.Right(3);
		m_pTarget = TG.GetProvince(strTemp);
		strTemp = strOrder.Mid(6, 3);
		m_pSource = TG.GetProvince(strTemp);
	}
	if (m_Type == HOLD)
	{
		m_pTarget = m_pLocation;
	}
	PSCoast* pCst = m_pTarget->GetCoast(0);
	m_pUnit->SetDesiredCoast(pCst);

	// Set the flags.
	if (strOrder.GetLength() > 9 && 
		m_Type == MOVE)
	{
		m_dwFlags |= !ORD_DIRECT;
		// Set the path. The path consists of the provinces of the convoying fleets only.
		int i = 1;
		strTemp = strOrder.Mid(6, 3);
		while (strTemp != strOrder.Right(3))
		{
			PSProvince* pPrv = TG.GetProvince(strTemp);
			ASSERT(pPrv);
			m_vPath.push_back(pPrv);
			i++;
			strTemp = strOrder.Mid(6 * i, 3);
		}
	}
	else 
		m_dwFlags |= ORD_DIRECT;

	// Get the desired coast.
	if (m_Type == MOVE && m_pTarget->IsBicoastal())
	{
		if (!m_pUnit->OrderToBicoastal(m_pLocation, m_pTarget))
			SetType(ILLEGAL);
	}
}


PSOrder::~PSOrder()
{
}


void PSOrder::SetType(ORDER_TYPE OType)
{
	switch (OType)
	{
	case VOID:
		m_strOutput += " Void; ";
		break;
	case ILLEGAL:
		m_strOutput += " Illegal; ";
		break;
	default:
		break;
	}

	m_Type = OType;
}
