// whichcoast.cpp : implementation file
//

#include "stdafx.h"
#include "Judge.h"
#include "whichcoast.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

extern PSGame TG;

/////////////////////////////////////////////////////////////////////////////
// PSWhichCoast dialog


PSWhichCoast::PSWhichCoast(PSProvince* pDest, CWnd* pParent /*=NULL*/)
	: CDialog(PSWhichCoast::IDD, pParent)
{
	//{{AFX_DATA_INIT(PSWhichCoast)
	m_pPrv = pDest;
	//}}AFX_DATA_INIT
}

void PSWhichCoast::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PSWhichCoast)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(PSWhichCoast, CDialog)
	//{{AFX_MSG_MAP(PSWhichCoast)
	ON_BN_CLICKED(IDC_FIRST_COAST, OnFirstCoast)
	ON_BN_CLICKED(IDC_SECOND_COAST, OnSecondCoast)
	ON_BN_CLICKED(IDC_NOCOAST_BUTTON, OnNocoast)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// PSWhichCoast message handlers

void PSWhichCoast::OnNocoast() 
{
	EndDialog(0);	
}


void PSWhichCoast::OnFirstCoast()
{
	EndDialog(1);	
}


void PSWhichCoast::OnSecondCoast()
{
	EndDialog(2);	
}


BOOL PSWhichCoast::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strMessage;
	strMessage.Format("%s%s%s", L"Which coast of ", m_pPrv->m_strName, " ?");
	GetDlgItem(IDC_WHICHTEXT)->SetWindowText(strMessage);
	ASSERT(m_pPrv->IsBicoastal());
	GetDlgItem(IDC_FIRST_COAST)->SetWindowText(m_pPrv->GetCoast(1)->m_strName);
	GetDlgItem(IDC_SECOND_COAST)->SetWindowText(m_pPrv->GetCoast(2)->m_strName);
	
	return TRUE;
}

