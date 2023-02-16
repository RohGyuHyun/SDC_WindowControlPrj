#include "stdafx.h"
#include "UtilFunc.h"


CString GetExtractionDotMsg(CString& strMsg)
{
	int count = strMsg.Find(_T('.'));
	CString m = strMsg.SpanExcluding(_T("."));
	if(count == -1)
	{
		strMsg = _T(""); 
		return m;
	}
	strMsg = strMsg.Mid(count+1);
	return m;
}


BOOL WriteLog(CString strMsg, int nKey)
{
	FILE*	stream;
	char	strFile[MAX_PATH], strToFile[MAX_PATH];
    long	result, line;
	char	szTime[MAX_CHAR]; 
	int limit_line = 500000;
	CString path, strTime, log, strKey, bak_file, strTopName;
	char buffer[MAX_CHAR];

	SYSTEMTIME	lpSystemTime;
	GetLocalTime(&lpSystemTime);
	strTime.Format(_T("%04d%02d%02d"), lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay);

	switch(nKey)
	{
	case SYSTEM_LOG:
		strKey.Format(_T("%s\\%s"), SYSYEM_LOG_PATH, strTime);
		strTopName.Format(_T("��¥,�ð�,�޽���"));
		break;
	case LOGIN_LOG:
		strKey.Format(_T("%s\\%s"), LOGIN_LOG_PATH, strTime);
		strTopName.Format(_T("��¥,�ð�,�޽���"));
		break;
	case ALARM_LOG:
		strKey.Format(_T("%s\\%s"), ALARM_LOG_PATH, strTime);
		strTopName.Format(_T("��¥,�ð�,�޽���"));
		break;
	case PACKET_LOG:
		strKey.Format(_T("%s\\%s"), PACKET_LOG_PATH, strTime);
		strTopName.Format(_T("��¥,�ð�,��Ŷ"));
		break;
	case RESULT_LOG:
		strKey.Format(_T("%s\\%s"), RESULT_LOG_PATH, strTime);
		strTopName.Format(_T("��¥,�ð�,�˻���۽ð�,�˻�����ð�,Inner ID,BIN"));
		break;
	case TACTTIME_LOG:
		strKey.Format(_T("%s\\%s"), TACTTIME_LOG_PATH, strTime);
		strTopName.Format(_T("��¥,�ð�,�ĸ��ʸ�����,��ŸƮ��ư�Է�,��������,��ǰ��Ʋ�̵�,�׷�������ġ�̵�,�׷�������ġ�̵�,��Ʋ�����ġ�̵�,��ǰ����B�̵�,���׹���,����"));
		break;
	default:

		break;
	}

	CreateDirectory(strKey, NULL);

	log.Format(_T("%04d-%02d-%02d,%02d:%02d:%02d:%03d,%s"), lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay, lpSystemTime.wHour, lpSystemTime.wMinute, lpSystemTime.wSecond, lpSystemTime.wMilliseconds, strMsg);

	sprintf(strFile, "%S\\%S_log.csv", strKey, strTime);

	if((stream = _fsopen(strFile, "a+", _SH_DENYNO)) == NULL)
	{
		HANDLE fd = CreateFile( (LPCWSTR)(LPCSTR)strFile,
							GENERIC_READ|GENERIC_WRITE,
							//GENERIC_WRITE,
							FILE_SHARE_READ|FILE_SHARE_WRITE,
							NULL,
							OPEN_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL
							);
		stream = _fsopen(strFile, "a+", _SH_DENYNO);
		if(stream == NULL)
		{
			CloseHandle(fd);
			return FALSE;
		}

		CloseHandle(fd);
	}

	//Check first time
    result = fseek(stream, 0L, SEEK_SET);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}
	line = 0;
	while(fgets(buffer, MAX_CHAR, stream)) 
	{
		line++;
		if(1 < line)
		{
			break;
		}
	}

	if( 0 == line )
		log = strTopName;


    result = fseek(stream, 0L, SEEK_END);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}


	memset(szTime, NULL, sizeof(szTime));
	//sprintf_s(szTime, log.GetLength() + 1, "%s", CT2A(log));
	sprintf(szTime, "%s", CT2A(log));
    fputs(szTime, stream);
	fputs("\n", stream);

    result = fseek(stream, 0L, SEEK_SET);
    if(result)
	{
		fclose(stream);
		return FALSE;
	}
	
	//20160511 �ּ�ó��.
	/*
	//Check limit line
	line = 0;
	if(limit_line)
	{
		while(fgets(buffer, MAX_CHAR, stream)) line++;
		if(line > limit_line)
		{
			sprintf_s(strToFile, path.GetLength() + bak_file.GetLength() + 2, "%S\\%S", path, bak_file); 
			CopyFile((LPCWSTR)(LPCSTR)strFile, (LPCWSTR)(LPCSTR)strToFile, FALSE);
			fclose(stream);
			DeleteFile((LPCWSTR)(LPCSTR)strFile);
		}
		else
		{
			fclose(stream);
		}
	}
	else
	{
		fclose(stream);
	}
	*/

	if(0 == line)
		WriteLog(strMsg, nKey);
	
	return TRUE;
}

BOOL DeleteDirectoryFile(LPCTSTR RootDir)
{
	if( RootDir == NULL )  
	{  
		return FALSE;  
	}  

	BOOL bRval = FALSE; 
	int nRval = 0; 

	CString szNextDirPath   = _T(""); 
	CString szRoot = _T("");
 

	// �ش� ���丮�� ��� ������ �˻��Ѵ�.
	szRoot.Format(_T("%s\\*.*"), RootDir);
 
	CFileFind find; 

	bRval = find.FindFile( szRoot );
 
	if( bRval == FALSE )  
	{        
		return bRval;  
		}
 
		while( bRval )  
		{  
		bRval = find.FindNextFile();
 
		// . or .. �� ��� ���� �Ѵ�.  
		if( find.IsDots() == FALSE )   
		{   
			// Directory �� ��� ���ȣ�� �Ѵ�.
			if( find.IsDirectory() )   
			{   
				DeleteDirectoryFile(find.GetFilePath());   
			}   
			// file�� ��� ���� 
			else   
			{    
				bRval = DeleteFile(find.GetFilePath()); 
			}  
		}
	} 

	find.Close();
	bRval = RemoveDirectory( RootDir ); 

	return bRval;
}


BOOL CopyFolder(CString strSrcPath, CString strSrcName, CString strDestPath, CString strDestName)
{
	int cnt=0, nt, i;
	CString srcfile, destfile, buf;
	CString* filename = new CString[cnt+1];
	
	CFileFind ff;
	
	buf.Format(_T("%s\\*.*"),strSrcPath);
	cnt = FileCountInDir(buf);
	
	if( !cnt )
	{
		//g_cMsgView.OkMsgBox(_T("Directory�� File�� �����ϴ�."));
		AfxMessageBox(_T("Directory�� File�� �����ϴ�."));
		return FALSE;
	}

	cnt=0;
	if( !ff.FindFile(buf,0) )
	{
		ff.Close();
		delete []filename;
		return FALSE;
	}
	
	nt = ff.FindNextFile();
	while(1)
	{
		if( !ff.IsDirectory() )
		{
			filename[cnt] = ff.GetFileName();
			cnt++;
		}
		if( !nt ) break;
		nt = ff.FindNextFile();
	}
	ff.Close();

	for(i=0; i<cnt; i++)
	{
		srcfile.Format(_T("%s\\%s"), strSrcPath, filename[i]);
		filename[i].Replace(strSrcName, strDestName);
		destfile.Format(_T("%s\\%s"), strDestPath, filename[i]);
		if(!CopyFile((LPCWSTR)srcfile,(LPCWSTR)destfile, FALSE))
		{
			delete[] filename;
			//g_cMsgView.OkMsgBox("File Copy�� Error�� �߻��Ͽ����ϴ�.");
			AfxMessageBox(_T("File Copy �� Error�� �߻��Ͽ����ϴ�."));
			return FALSE;
		}
		/*ProgWnd.StepIt();*/
	}
	
	delete[] filename;
	return TRUE;
}

int FileCountInDir(CString pn)
{
	CFileFind ff;
	int cnt=0, nt;
	
	if( !ff.FindFile(pn,0) )
	{
		ff.Close();
		return FALSE;
	}
	
	nt = ff.FindNextFile();
	while(1)
	{
		if( !ff.IsDirectory() ) cnt++;
		if( !nt ) break;
		nt = ff.FindNextFile();
	}
	
	ff.Close();
	
	return cnt;
}

void Delay(long ms, BOOL bEvent)
{
	double s, e;
	double Ms = (double) ms;
	s = (double) GetCurrentTime();
	do
	{
		if(bEvent)
			DoEvents();

		Sleep(1);
		e = (double) GetCurrentTime();
	}
	while ((e - s) < Ms);
}

void DoEvents()
{
	MSG message;
	if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
	{
		::TranslateMessage(&message);
		::DispatchMessage(&message);
	}
}
