// Diplo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Diplo.h"
#include "PasswordDlg.h"

#include "MainFrm.h"
#include "DiploDoc.h"
#include "DiploView.h"
#include "HTMLinkStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PSDiploApp

BEGIN_MESSAGE_MAP(PSDiploApp, CWinApp)
	//{{AFX_MSG_MAP(PSDiploApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_HELP, OnHelp)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PSDiploApp construction

PSDiploApp::PSDiploApp()
{
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only PSDiploApp object

PSDiploApp theApp;

/////////////////////////////////////////////////////////////////////////////
// PSDiploApp initialization

BOOL PSDiploApp::InitInstance()
{
	SetRegistryKey("PlanetSoft");
	LoadStdProfileSettings(8);  // Load standard INI file options (including MRU)

	// Password check.
	PSPasswordDlg dlg;

	// Get the password from the registry.
	CString strSection       = "Settings";
	CString strStringItem    = "Password";
	dlg.m_strPassword = GetProfileString(strSection, strStringItem);
	if (!dlg.ValidatePassword())
	{
		if (dlg.DoModal() != IDOK)
			return FALSE;
		else 
			WriteProfileString(strSection, strStringItem, dlg.m_strPassword);
	}

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(PSDiploDoc),
		RUNTIME_CLASS(PSMainFrame),       // main SDI frame window
		RUNTIME_CLASS(PSDiploView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	EnableShellOpen();
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->DragAcceptFiles();
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// PSAboutDlg dialog used for App About

class PSAboutDlg : public CDialog
{
public:
	PSAboutDlg();

// Dialog Data
	//{{AFX_DATA(PSAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	PSHTMLinkStatic m_Link;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PSAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(PSAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

PSAboutDlg::PSAboutDlg() : CDialog(PSAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(PSAboutDlg)
	//}}AFX_DATA_INIT
}

void PSAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PSAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(PSAboutDlg, CDialog)
	//{{AFX_MSG_MAP(PSAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void PSDiploApp::OnAppAbout()
{
	PSAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// PSDiploApp commands

void PSDiploApp::OnFileOpen() 
{
	LPSTR lpszFilter = "Diplomacy Games (*.dip)|*.dip||";
	CFileDialog dlg(TRUE, NULL, NULL, 0, lpszFilter);
	if (dlg.DoModal() == IDOK)
		CWinApp::OpenDocumentFile(dlg.GetFileName());
}


BOOL PSAboutDlg::OnInitDialog() 
{
	m_Link.SetLink("http://homepages.infoseek.com/~diplo/planet.htm");
	CDialog::OnInitDialog();
	m_Link.SubclassDlgItem(IDC_PLANETSOFT, this);
	return TRUE;
}


void PSDiploApp::OnFileNew() 
{
	CWinApp::OnFileNew();
}

