// PSFile.h: interface for the PSIOFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PSFile_H__41EB9FE2_F1C4_11D1_A969_444553540000__INCLUDED_)
#define AFX_PSFile_H__41EB9FE2_F1C4_11D1_A969_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class PSIOFile : public CStdioFile  
{
public:
	PSIOFile();
	virtual ~PSIOFile();

	int ReadInt();
	virtual LPTSTR ReadString(LPTSTR, UINT);
	virtual BOOL ReadString(CString&);
};

#endif // !defined(AFX_PSFile_H__41EB9FE2_F1C4_11D1_A969_444553540000__INCLUDED_)
