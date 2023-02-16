#ifndef _UTILITY_FUNCTION_
#define _UTILITY_FUNCTION_

BOOL DeleteDirectoryFile(LPCTSTR RootDir);

BOOL CopyFolder(CString strSrcPath, CString strSrcName, CString strDestPath, CString strDestName);

int FileCountInDir(CString pn);

BOOL WriteLog(CString strMsg, int nKey);


void Delay(long ms, BOOL bEvent = 0);

void DoEvents();

CString GetExtractionDotMsg(CString& strMsg);

#endif