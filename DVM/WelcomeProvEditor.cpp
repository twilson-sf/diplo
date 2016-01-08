// WelcomeProvEditor.cpp : implementation file
//

#include "stdafx.h"
#include "dvm.h"
#include "WelcomeProvEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern __declspec(dllimport) PSGame TG;

/////////////////////////////////////////////////////////////////////////////
// PSWelcomeProvEditor dialog


PSWelcomeProvEditor::PSWelcomeProvEditor(CWnd* pParent /*=NULL*/)
	: CDialog(PSWelcomeProvEditor::IDD, pParent)
{
	//{{AFX_DATA_INIT(PSWelcomeProvEditor)
	m_nSelection = 0;
	m_strSelectedProv = _T("");
	//}}AFX_DATA_INIT
}


void PSWelcomeProvEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PSWelcomeProvEditor)
	DDX_Radio(pDX, IDC_NEW_PROV_RADIO, m_nSelection);
	DDX_LBString(pDX, IDC_PROV_LIST, m_strSelectedProv);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PSWelcomeProvEditor, CDialog)
	//{{AFX_MSG_MAP(PSWelcomeProvEditor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PSWelcomeProvEditor message handlers

BOOL PSWelcomeProvEditor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Fill the list control with the names of provinces.
	CListBox* pProvLB = (CListBox*) GetDlgItem(IDC_PROV_LIST);
	for (int i = 1; i <= TG.GetNumberOfProvinces(); i++)
	{
		PSProvince* pPrv = TG.GetProvince(i);
		ASSERT(pPrv);
		pProvLB->AddString(pPrv->m_strName);
	}
	ASSERT(i > 1);// This box should not be displayed if there is no game loaded.

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void PSWelcomeProvEditor::OnOK() 
{
	UpdateData();
	// If no province is selected and the choice was 'Modify', error.
	if (m_strSelectedProv == "" && 
		(m_nSelection == 1 || m_nSelection == 2))
	{
		MessageBox("Please select a province to continue.", "DipSheet", MB_ICONEXCLAMATION);
		return;
	}

	CDialog::OnOK();
}

