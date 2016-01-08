// PowProp.cpp : implementation file
//

#include "stdafx.h"
#include "dvm.h"
#include "PowProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PSPowProp dialog


PSPowProp::PSPowProp(CWnd* pParent /*=NULL*/)
	: CDialog(PSPowProp::IDD, pParent)
{
	//{{AFX_DATA_INIT(PSPowProp)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void PSPowProp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PSPowProp)
	DDX_Text(pDX, IDC_EDIT1, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PSPowProp, CDialog)
	//{{AFX_MSG_MAP(PSPowProp)
	ON_BN_CLICKED(IDC_COLOR_BUTTON, OnColorButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PSPowProp message handlers

BOOL PSPowProp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_ColorButton.SubclassDlgItem(IDC_COLOR_BUTTON, this);	
	SetWindowText(m_strTitle);
	return TRUE;
}

void PSPowProp::OnColorButton() 
{
	CColorDialog dlg(GetColor());
	if (dlg.DoModal() == IDOK)
		m_ColorButton.SetBckColor(dlg.GetColor());
}
