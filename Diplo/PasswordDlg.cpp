// PasswordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "diplo.h"
#include "PasswordDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PSPasswordDlg dialog


PSPasswordDlg::PSPasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(PSPasswordDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(PSPasswordDlg)
	m_strPassword = _T("");
	m_strPasswordStatic = _T("");
	//}}AFX_DATA_INIT
}


void PSPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PSPasswordDlg)
	DDX_Control(pDX, IDC_DIPLO_LINK_STATIC, m_DiploLink);
	DDX_Text(pDX, IDC_EDIT1, m_strPassword);
	DDX_Text(pDX, IDC_PASSWORD_STATIC, m_strPasswordStatic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PSPasswordDlg, CDialog)
	//{{AFX_MSG_MAP(PSPasswordDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PSPasswordDlg message handlers

BOOL PSPasswordDlg::OnInitDialog() 
{
	m_strPasswordStatic.LoadString(IDS_UPGRADE_PASSWORD);
	m_nRetries = 0;
	m_DiploLink.SetLink("http://homepages.infoseek.com/~diplo");
	m_DiploLink.SubclassDlgItem(IDC_DIPLO_LINK_STATIC, this);

	CDialog::OnInitDialog();
	return TRUE;
}


void PSPasswordDlg::OnOK() 
{
	UpdateData();

	BOOL bValid = ValidatePassword();
	if (bValid)
	{
		EndDialog(IDOK);
	}
	else 
	{
		m_nRetries++;
		if (m_nRetries == 3)
		{
			m_nRetries = 0;
			MessageBox(L"Thank you for trying. Good Bye!", L"Diplo", MB_ICONEXCLAMATION);
			EndDialog(IDCANCEL);
		}
		else 
			MessageBox(L"Invalid Password", L"Diplo", MB_ICONEXCLAMATION);
	}
}


BOOL PSPasswordDlg::ValidatePassword() 
{
	// The password is valid if the month and the year encrypted in m_strPassword 
	// equal the current month and year. The password is a 8 digits string. 
	// The month is the 4-th digit and is 'modulo 13'.
	// The year is the 3-rd (decade) and 7-th (year) and is also 'modulo 13'.
	// The sum of the rest of the digits must equal 9 'modulo 17'.

	// Check for Permanent Password.
	if (m_strPassword == "Diplo_Forever")
		return TRUE;

	// Get the current month and year.
	CTime time = CTime::GetCurrentTime();

	// Password must be 8 digits long.
	if (m_strPassword.GetLength() != 8)
		return FALSE;

	// Check sum of digits 0, 1, 2, 5 and 6.
	int n0 = m_strPassword[0];
	int n1 = m_strPassword[1];
	int n2 = m_strPassword[2];
	int n5 = m_strPassword[5];
	int n6 = m_strPassword[6];

	int nSum = n0 + n1 + n2 + n5 + n6;
	if (nSum%17 != 9)
		return FALSE;

	// Check the month.
	int n4 = m_strPassword[4];
	int nCurrentMonth = time.GetMonth();
	if (n4%13 != nCurrentMonth)
		return FALSE;

	// Check the year.
	int n3 = m_strPassword[3];
	int n7 = m_strPassword[7];
	int nCurrentYear = time.GetYear();
	if (n3%13 * 10 + n7%13 + 1900 != nCurrentYear && 
		n3%13 * 10 + n7%13 + 2000 != nCurrentYear)
		return FALSE;

	return TRUE;
}


