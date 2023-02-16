#include "StdAfx.h"
#include "SystemData.h"

CSystemData::CSystemData(void)
{
	m_strWorkModelName.Empty();

	m_sCurrentUser.UserName = LOG_IN_NAME_OPERATOR;
	m_sOperator.Password = _T("123");
	m_sEngineer.Password = _T("123");
	m_sMaker.Password = _T("123");
	m_nPortNoVisionPC = 2000;
	m_strIPAddressVisionPC = _T("127.0.0.1");

	m_dVelRatio = 100.;
	m_nOpMode = 0;
	m_bManualMuting = FALSE;
	m_bManualSocket = FALSE;
	m_bManualSkipFilmRemove = FALSE;

	m_bSysPrepare = FALSE;
	m_bUnitInit = FALSE;
}


CSystemData::~CSystemData(void)
{
}


BOOL CSystemData::LoadSystemData()
{
	BOOL rslt = TRUE;
	CString strNode, strKey, strData, strFilePath;
	wchar_t temp[512];
	
	strFilePath.Format(_T("%s\\%s"), DATA_SYSTEM_PATH, DATA_SYSTEM_FILENAME);
	
	//file 존재 확인 
	HANDLE fd=CreateFile( strFilePath,
						GENERIC_READ|GENERIC_WRITE,
						0,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						NULL
						);
	if( ((LONG)fd) < 0 )
	{
		rslt = FALSE;
	}
	else
	{
		CloseHandle(fd);

		strNode.Format(_T("MODEL_INFO"));
		strKey.Format(_T("WORK_MODEL"));
		GetPrivateProfileString(strNode, strKey, _T(""), temp, sizeof(temp), strFilePath);
		SetWorkModelName(temp);


		strNode.Format(_T("LOGIN_INFO"));
		strKey.Format(_T("Operator P/W"));
		GetPrivateProfileString(strNode, strKey, _T(""), temp, sizeof(temp), strFilePath);
		SetOperatorPW(temp);

		strKey.Format(_T("Engineer P/W"));
		GetPrivateProfileString(strNode, strKey, _T(""), temp, sizeof(temp), strFilePath);
		SetEngineerPW(temp);

		strKey.Format(_T("Maker P/W"));
		GetPrivateProfileString(strNode, strKey, _T(""), temp, sizeof(temp), strFilePath);
		SetMakerPW(temp);

		strNode.Format(_T("SYSTEM_PARAM"));
		strKey.Format(_T("SYSYEM VELRATIO"));
		GetPrivateProfileString(strNode, strKey, _T("100"), temp, sizeof(temp), strFilePath);
		SetSystemVelRatio(_wtof(temp));

		strKey.Format(_T("IP ADDRESS VISION PC"));
		GetPrivateProfileString(strNode, strKey, _T("127.0.0.1"), temp, sizeof(temp), strFilePath);
		SetIPAddressVisionPC(temp);

		strKey.Format(_T("COMPORT NUM VISION PC"));
		GetPrivateProfileString(strNode, strKey, _T("2000"), temp, sizeof(temp), strFilePath);
		SetPortNoVisionPC(_ttoi(temp));

		strNode.Format(_T("OPTION_PARAM"));
		strKey.Format(_T("SYSYEM OPMODE"));
		GetPrivateProfileString(strNode, strKey, _T("0"), temp, sizeof(temp), strFilePath);
		SetSystemOpMode(_wtoi(temp));

		strKey.Format(_T("SYSYEM MANUAL MUTING"));
		GetPrivateProfileString(strNode, strKey, _T("0"), temp, sizeof(temp), strFilePath);
		SetSystemManualMuting(_wtoi(temp));

		strKey.Format(_T("SYSYEM MANUAL SOCKET"));
		GetPrivateProfileString(strNode, strKey, _T("0"), temp, sizeof(temp), strFilePath);
		SetSystemManualSocket(_wtoi(temp));

		strKey.Format(_T("SYSYEM MANUAL SKIP FILM REMOVE"));
		GetPrivateProfileString(strNode, strKey, _T("0"), temp, sizeof(temp), strFilePath);
		SetSystemManualSkipFilmRemove(_wtoi(temp));

	}
	fd = 0;

	return rslt;
}

BOOL CSystemData::SaveSystemData()
{
	BOOL rslt = TRUE;
	CString strNode, strKey, strFilePath, strTemp;
	
	strFilePath.Format(_T("%s\\%s"), DATA_SYSTEM_PATH, DATA_SYSTEM_FILENAME);
	
	//file 존재 확인 
	HANDLE fd=CreateFile( strFilePath,
						GENERIC_READ|GENERIC_WRITE,
						0,
						NULL,
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						NULL
						);
	if( ((LONG)fd) < 0 )
	{
		rslt = FALSE;
	}
	else
	{
		CloseHandle(fd);
		fd = NULL;

		strNode.Format(_T("MODEL_INFO"));
		strKey.Format(_T("WORK_MODEL"));
		WritePrivateProfileString(strNode, strKey, GetWorkModelName(), strFilePath);

		strNode.Format(_T("LOGIN_INFO"));
		strKey.Format(_T("Operator P/W"));
		WritePrivateProfileString(strNode, strKey, GetOperator().Password, strFilePath);

		strKey.Format(_T("Engineer P/W"));
		WritePrivateProfileString(strNode, strKey, GetEngineer().Password, strFilePath);

		strKey.Format(_T("Maker P/W"));
		WritePrivateProfileString(strNode, strKey, GetMaker().Password, strFilePath);

		strNode.Format(_T("SYSTEM_PARAM"));
		strKey.Format(_T("SYSYEM VELRATIO"));
		strTemp.Format(_T("%f"), GetSystemVelRatio());
		WritePrivateProfileString(strNode, strKey, strTemp, strFilePath);

		strKey.Format(_T("IP ADDRESS VISION PC"));
		WritePrivateProfileString(strNode, strKey, GetIPAddressVisionPC(), strFilePath);

		strKey.Format(_T("COMPORT NUM VISION PC"));
		strTemp.Format(_T("%d"), GetPortNoVisionPC());
		WritePrivateProfileString(strNode, strKey, strTemp, strFilePath);

		strNode.Format(_T("OPTION_PARAM"));
		strKey.Format(_T("SYSYEM OPMODE"));
		strTemp.Format(_T("%d"), GetSystemOpMode());
		WritePrivateProfileString(strNode, strKey, strTemp, strFilePath);

		strKey.Format(_T("SYSYEM MANUAL MUTING"));
		strTemp.Format(_T("%d"), GetSystemManualMuting());
		WritePrivateProfileString(strNode, strKey, strTemp, strFilePath);

		strKey.Format(_T("SYSYEM MANUAL SKIP FILM REMOVE"));
		strTemp.Format(_T("%d"), GetSystemManualSkipFilmRemove());
		WritePrivateProfileString(strNode, strKey, strTemp, strFilePath);
	}

	return rslt;
}