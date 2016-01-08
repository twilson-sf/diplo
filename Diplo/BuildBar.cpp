// BuildBar.cpp : implementation file
//
#include "stdafx.h"
#include "diplo.h"
#include "BuildBar.h"
#include "DiploView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern __declspec(dllimport) PSGame TG;

/////////////////////////////////////////////////////////////////////////////
// PSBuildBar

PSBuildBar::PSBuildBar()
{
}


PSBuildBar::~PSBuildBar()
{
}


BEGIN_MESSAGE_MAP(PSBuildBar, CToolBar)
	//{{AFX_MSG_MAP(PSBuildBar)
	ON_WM_CREATE()
	ON_CBN_SELENDOK(ID_YEAR_COMBO, OnYearCombo)
	ON_COMMAND(ID_SEASON_BUTTON, OnSeasonButton)
	ON_UPDATE_COMMAND_UI(ID_YEAR_COMBO, OnUpdateYearCombo)
	ON_UPDATE_COMMAND_UI(ID_OK_BUILD_BUTTON, OnUpdateOKBuildButton)
	ON_UPDATE_COMMAND_UI(ID_SEASON_BUTTON, OnUpdateSeasonButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PSBuildBar message handlers

int PSBuildBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Load the build bar.
	// Get the directory from the registry.
	CString strSection       = "Directories";
	CString strStringItem    = "Current";
	CWinApp* pApp = AfxGetApp();
	CString strValue;
	strValue = pApp->GetProfileString(strSection, strStringItem);

	// Load the build bar bitmap.
	int nPowers = TG.GetNumberOfPowers();
	HBITMAP hUnits = (HBITMAP) ::LoadImage(AfxGetInstanceHandle(), 
		strValue + TG.GetType() + "Units.bmp", 
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (!hUnits)
		MessageBox("The " + TG.GetType() + "Units.bmp file was not found.\r\n"
			"Please make sure this file is located in the same folder as Diplo.exe");
	SetBitmap(hUnits);
	UINT* uButtons = new UINT[nPowers * 3 + 3];
	for (int i = 0; i < nPowers; i++)
	{
		uButtons[i + nPowers * 0] = ID_FIRST_ARMY + i;
		uButtons[i + nPowers * 1] = ID_FIRST_FLEET + i;
		uButtons[i + nPowers * 2] = ID_FIRST_SC + i;
	}
	uButtons[nPowers * 3 + 0] = ID_SEASON_BUTTON;
	//uButtons[nPowers * 3 + 1] = ID_FALL_BUTTON;
	uButtons[nPowers * 3 + 1] = ID_YEAR_COMBO;
	uButtons[nPowers * 3 + 2] = ID_OK_BUILD_BUTTON;

	SetButtons(uButtons, nPowers * 3 + 3);
	delete uButtons;

	// Get the sizes of units
    PSMainFrame* pMain = (PSMainFrame*) GetParent();
	SIZE sizeUnit = pMain->GetUnitSize();
	CSize sizeButton = (CSize)sizeUnit + CSize(7, 6);
	SetSizes(sizeButton, sizeUnit);

	SetButtonStyle(nPowers * 1 - 1, GetButtonStyle(0) | TBBS_WRAPPED);
	SetButtonStyle(nPowers * 2 - 1, GetButtonStyle(0) | TBBS_WRAPPED);
	SetButtonStyle(nPowers * 3 - 1, GetButtonStyle(0) | TBBS_WRAPPED);

	// Add the fourth row to the build bar.
	CRect firstButton;
	GetItemRect(0, &firstButton);
	CRect rect;

	GetItemRect(nPowers * 2, &rect);
	rect.top = rect.bottom;
	rect.bottom += sizeButton.cy;
	rect.right = rect.left + sizeButton.cx * 2;

	// Create the Spring button.
	DWORD dwStyle = WS_CHILD | WS_VISIBLE;
	if (!m_SpringButton.Create("Spring", dwStyle, rect, this, ID_SEASON_BUTTON))
		return -1;

	// Create the Fall button.
	/*rect.left = rect.right;
	rect.right = rect.left + sizeButton.cx * 2;
	if (!m_FallButton.Create("Fall", dwStyle, rect, this, ID_FALL_BUTTON))
		return -1;*/

	// Create the Year combo box.
	CClientDC dc (this);
	TEXTMETRIC tm;
	dc.GetTextMetrics (&tm);
	int cyChar = tm.tmHeight + tm.tmExternalLeading;
	rect.left = rect.right;
	rect.right = rect.left + sizeButton.cx * 2;// Twice wider.
	rect.bottom = rect.top + (cyChar * 14);// Size of dropdown.
	if (!m_comboYear.Create (dwStyle | WS_VSCROLL | CBS_DROPDOWNLIST, 
		rect, this, ID_YEAR_COMBO))
        return -1;

	CString strYears;
	i = TG.GetFirstYear();
	while (i <= TG.GetLastYear())
	{
        strYears.Format ("%d", i++);
        m_comboYear.AddString (strYears);
    }
	
	// Create the Close button.
	rect.left = rect.right;
	rect.right = rect.left + sizeButton.cx;
	rect.bottom = rect.top + sizeButton.cy;
	if (!m_CloseButton.Create("OK", dwStyle, rect, this, ID_OK_BUILD_BUTTON))
		return -1;

	EnableDocking(0);	
	return 0;
}


void PSBuildBar::OnYearCombo() 
{
    char szSize[8];
    int nIndex = m_comboYear.GetCurSel();
    m_comboYear.GetLBText (nIndex, szSize);

    PSDiploView* pView =
        (PSDiploView*) ((CFrameWnd*) AfxGetMainWnd())->GetActiveView();

	pView->SetYear(atoi(szSize));	
}


void PSBuildBar::OnUpdateYearCombo(CCmdUI* pCmdUI) 
{
    CWnd* pWnd = GetFocus ();
    if (pWnd == &m_comboYear)
        return;

	PSDiploView* pView =
        (PSDiploView*) ((CFrameWnd*) AfxGetMainWnd())->GetActiveView();
	
    char szSize[5];
    m_comboYear.GetWindowText (szSize, sizeof (szSize));
    int nYear = atoi (szSize);
    int nCurrentYear = TG.GetYear();

    if (nCurrentYear != nYear)
	{
        CString strYear;
        strYear.Format ("%d", nCurrentYear);
        if (m_comboYear.SelectString(-1, strYear) == CB_ERR)
            m_comboYear.SetCurSel(-1);
    }

	pCmdUI->Enable(TG.GetStatus() == BUILDGAME);
}


void PSBuildBar::OnSeasonButton() 
{
	if (TG.GetSeason() == SPRING)
		TG.SetSeason(FALL);
	else 
		TG.SetSeason(SPRING);
}


void PSBuildBar::OnUpdateSeasonButton(CCmdUI* pCmdUI) 
{
	if (TG.GetSeason() == SPRING)
		pCmdUI->SetText("Spring");
	else 
		pCmdUI->SetText("Fall");

	if (TG.GetStatus() == BUILDGAME)
		pCmdUI->Enable(TRUE);
	else 
		pCmdUI->Enable(FALSE);
}


void PSBuildBar::OnUpdateOKBuildButton(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TG.GetStatus() == BUILDGAME);
}


