// HistoryBar.cpp : implementation file
//

#include "stdafx.h"
#include "diplo.h"
#include "HistoryBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PSHistoryBar


PSHistoryBar::PSHistoryBar()
{
}

PSHistoryBar::~PSHistoryBar()
{
}


BEGIN_MESSAGE_MAP(PSHistoryBar, CToolBar)
	//{{AFX_MSG_MAP(PSHistoryBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PSHistoryBar message handlers

int PSHistoryBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	LoadToolBar(IDR_HISTORY_TOOLBAR);
	SetButtonStyle(4, TBBS_CHECKBOX);

    // Add the slider to the toolbar.
	CRect rectSlider, rectFirstButton;
    GetItemRect (0, &rectFirstButton);
	SetButtonInfo (2, ID_SLIDER, TBBS_SEPARATOR, rectFirstButton.Width() * 4);
    GetItemRect (2, &rectSlider);

    if (!m_Slider.Create (WS_CHILD | WS_VISIBLE | 
        TBS_HORZ | TBS_AUTOTICKS, rectSlider, this, ID_SLIDER))
        return -1;

	m_Slider.SetRange(2, 10);
	// Get the postion of the thumb from the registry.
	CString strSection       = "Settings";
	CString strStringItem    = "Animation Speed";
	int nSpeed = AfxGetApp()->GetProfileInt(strSection, strStringItem, 5);
	m_Slider.SetPos(nSpeed);

	return 0;
}


int PSHistoryBar::GetSliderPosition()
{
	return m_Slider.GetPos();
}
