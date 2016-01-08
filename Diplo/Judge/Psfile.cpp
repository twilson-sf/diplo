// PSFile.cpp: implementation of the PSIOFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Judge.h"
#include "PSFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAXLEN 100

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PSIOFile::PSIOFile()
{
}


PSIOFile::~PSIOFile()
{
}


int PSIOFile::ReadInt()
{
	char cVal[MAXLEN];
	ReadString(cVal, MAXLEN);

	return atoi(cVal);
}


LPTSTR PSIOFile::ReadString(LPTSTR lpsz, UINT nMax)
{
	LPTSTR lpszRet = CStdioFile::ReadString(lpsz, nMax);
	if (!lpszRet)
		AfxMessageBox(_T("Error while reading Data"), MB_ICONEXCLAMATION);

	return lpszRet;
}


BOOL PSIOFile::ReadString(CString& str)
{
	BOOL bResult = CStdioFile::ReadString(str);
	if (!bResult)
		AfxMessageBox(_T("Error while reading Data"), MB_ICONEXCLAMATION);
	
	return bResult;
}


