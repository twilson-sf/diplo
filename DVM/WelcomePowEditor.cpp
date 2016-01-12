// WelcomePowEditor.cpp : implementation file
//

#include "stdafx.h"
#include "dvm.h"
#include "WelcomePowEditor.h"
#include "PowProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern __declspec(dllimport) PSGame TG;

/////////////////////////////////////////////////////////////////////////////
// PSWelcomePowEditor dialog

PSWelcomePowEditor::PSWelcomePowEditor(CWnd* pParent /*=NULL*/)
	: CDialog(PSWelcomePowEditor::IDD, pParent)
{
	//{{AFX_DATA_INIT(PSWelcomePowEditor)
	m_nSelection = 0;
	m_strSelectedPow = _T("");
	//}}AFX_DATA_INIT
}

void PSWelcomePowEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PSWelcomePowEditor)
	DDX_Radio(pDX, IDC_NEW_PROV_RADIO, m_nSelection);
	DDX_LBString(pDX, IDC_PROV_LIST, m_strSelectedPow);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(PSWelcomePowEditor, CDialog)
	//{{AFX_MSG_MAP(PSWelcomePowEditor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PSWelcomePowEditor message handlers

BOOL PSWelcomePowEditor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Fill the list control with the names of provinces.
	CListBox* pProvLB = (CListBox*) GetDlgItem(IDC_PROV_LIST);
	int i;
	for (i = 1; i <= TG.GetNumberOfPowers(); i++)
	{
		PSPower* pPow = TG.GetPower(i);
		ASSERT(pPow);
		pProvLB->AddString(pPow->m_strName);
	}
	ASSERT(i > 1);// This box should not be displayed if there is no game loaded.

	pProvLB->SetCurSel(0);
	return TRUE;
}

void PSWelcomePowEditor::OnOK() 
{
	UpdateData();
	// If no power is selected and the choice was 'Modify', error.
	if (m_strSelectedPow == "" && 
		(m_nSelection == 1 || m_nSelection == 2))
	{
		MessageBox(L"Please select a power to continue.", L"DipSheet", MB_ICONEXCLAMATION);
		return;
	}

	PSPower* pCurPow = NULL;
	switch(m_nSelection)
	{
	case 0:// New
		{
		pCurPow = new PSPower;
		PSPowProp dlg;
		dlg.SetTitle(_T("New Power"));
		dlg.SetColor(::GetSysColor(COLOR_BTNFACE));
		dlg.m_strName = "";
		if (dlg.DoModal() == IDOK)
		{
			pCurPow->SetColor(dlg.GetColor());
			TG.AddPower(pCurPow);
			CListBox* pLB = (CListBox*)GetDlgItem(IDC_PROV_LIST);
			pLB->InsertString(-1, pCurPow->m_strName);
		}
		else 
			delete pCurPow;
		break;
		}
	case 1:// Modify
		{
		pCurPow = new PSPower(TG.GetPower(m_strSelectedPow));
		PSPowProp dlg;
		dlg.SetTitle(pCurPow->m_strName);
		dlg.SetColor(pCurPow->GetColor());
		dlg.m_strName = pCurPow->m_strName;
		if (dlg.DoModal() == IDOK)
		{
			pCurPow->SetColor(dlg.GetColor());
			TG.DeletePower(TG.GetPower(m_strSelectedPow));
			TG.AddPower(pCurPow);
			CListBox* pLB = (CListBox*)GetDlgItem(IDC_PROV_LIST);
			pLB->DeleteString(pLB->GetCurSel());
			pLB->InsertString(-1, pCurPow->m_strName);
		}
		else
			delete pCurPow;
		break;
		}
	case 2:// Remove
		pCurPow = TG.GetPower(m_strSelectedPow);
		ASSERT(pCurPow);
		if (MessageBox(L"Are you sure you wish to delete " + m_strSelectedPow + "?", L"DipSheet", MB_YESNO) == IDYES)
		{
			TG.DeletePower(pCurPow);
			CListBox* pLB = (CListBox*)GetDlgItem(IDC_PROV_LIST);
			pLB->DeleteString(pLB->GetCurSel());
		}
		return;
	default:
		ASSERT(0);
	}
}
