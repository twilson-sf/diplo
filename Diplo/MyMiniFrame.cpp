// MyMiniFrame.cpp : implementation file
//

#include "stdafx.h"
#include "diplo.h"
#include "MyMiniFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PSMiniFrame

IMPLEMENT_DYNCREATE(PSMiniFrame, CMiniDockFrameWnd)

PSMiniFrame::PSMiniFrame()
{
}

PSMiniFrame::~PSMiniFrame()
{
}


BEGIN_MESSAGE_MAP(PSMiniFrame, CMiniDockFrameWnd)
	//{{AFX_MSG_MAP(PSMiniFrame)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PSMiniFrame message handlers


void PSMiniFrame::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CMiniDockFrameWnd::OnShowWindow(bShow, nStatus);
	if (bShow)
		ModifyStyle(WS_SYSMENU, NULL);
}

