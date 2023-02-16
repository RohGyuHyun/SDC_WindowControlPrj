#include "StdAfx.h"
#include "ModelData.h"



CModelData::CModelData(void)
{
}


CModelData::~CModelData(void)
{
}


BOOL CModelData::LoadModelData(CString strModelName)
{
	BOOL rslt = TRUE;
	CString strNode, strKey, strData, strFilePath;
	wchar_t temp[512];
	
	strFilePath.Format(_T("%s\\%s\\%s_TeachPara.ini"), DATA_MODEL_PATH, strModelName, strModelName);
	
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

		strNode.Format(_T("MODEL_DATA"));
		strKey.Format(_T("LD_PnP_LOAD_POS_X"));
		GetPrivateProfileString(strNode, strKey, _T("0.0"), temp, sizeof(temp), strFilePath);
		m_dLP_LoadPosX = _wtof(temp);
		
		strKey.Format(_T("LD_PnP_INSP_POS_X"));
		GetPrivateProfileString(strNode, strKey, _T("0.0"), temp, sizeof(temp), strFilePath);
		m_dLP_InspPosX = _wtof(temp);

		strKey.Format(_T("LD_PnP_UNLOAD_POS_X"));
		GetPrivateProfileString(strNode, strKey, _T("0.0"), temp, sizeof(temp), strFilePath);
		m_dLP_UnloadPosX = _wtof(temp);

		strKey.Format(_T("LD_PnP_SPARE_POS_X"));
		GetPrivateProfileString(strNode, strKey, _T("0.0"), temp, sizeof(temp), strFilePath);
		m_dLP_SparePosX = _wtof(temp);

		strKey.Format(_T("LD_PnP_LOAD_POS_Y"));
		GetPrivateProfileString(strNode, strKey, _T("0.0"), temp, sizeof(temp), strFilePath);
		m_dIS_LoadPosY = _wtof(temp);

		strKey.Format(_T("LD_PnP_INSP_POS_Y"));
		GetPrivateProfileString(strNode, strKey, _T("0.0"), temp, sizeof(temp), strFilePath);
		m_dIS_InspPosY = _wtof(temp);

		strKey.Format(_T("LD_PnP_UNLOAD_POS_Y"));
		GetPrivateProfileString(strNode, strKey, _T("0.0"), temp, sizeof(temp), strFilePath);
		m_dIS_UnloadPosY = _wtof(temp);

		strKey.Format(_T("LD_PnP_CAM_CENTER_POS_Y"));
		GetPrivateProfileString(strNode, strKey, _T("0.0"), temp, sizeof(temp), strFilePath);
		m_dIS_CamCenterPosY = _wtof(temp);
	}
	fd = 0;

	return rslt;
}

BOOL CModelData::SaveModelData(CString strModelName)
{
	BOOL rslt = TRUE;
	CString strNode, strKey, strFilePath, strData;
	
	strFilePath.Format(_T("%s\\%s\\%s_TeachPara.ini"), DATA_MODEL_PATH, strModelName, strModelName);
	
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

		strNode.Format(_T("MODEL_DATA"));
		strKey.Format(_T("LD_PnP_LOAD_POS_X"));
		strData.Format(_T("%3.3f"), m_dLP_LoadPosX);
		WritePrivateProfileString(strNode, strKey, strData, strFilePath);

		strKey.Format(_T("LD_PnP_INSP_POS_X"));
		strData.Format(_T("%3.3f"), m_dLP_InspPosX);
		WritePrivateProfileString(strNode, strKey, strData, strFilePath);

		strKey.Format(_T("LD_PnP_UNLOAD_POS_X"));
		strData.Format(_T("%3.3f"), m_dLP_UnloadPosX);
		WritePrivateProfileString(strNode, strKey, strData, strFilePath);

		strKey.Format(_T("LD_PnP_SPARE_POS_X"));
		strData.Format(_T("%3.3f"), m_dLP_SparePosX);
		WritePrivateProfileString(strNode, strKey, strData, strFilePath);

		strNode.Format(_T("MODEL_DATA"));
		strKey.Format(_T("LD_PnP_LOAD_POS_Y"));
		strData.Format(_T("%3.3f"), m_dIS_LoadPosY);
		WritePrivateProfileString(strNode, strKey, strData, strFilePath);

		strKey.Format(_T("LD_PnP_INSP_POS_Y"));
		strData.Format(_T("%3.3f"), m_dIS_InspPosY);
		WritePrivateProfileString(strNode, strKey, strData, strFilePath);

		strKey.Format(_T("LD_PnP_UNLOAD_POS_Y"));
		strData.Format(_T("%3.3f"), m_dIS_UnloadPosY);
		WritePrivateProfileString(strNode, strKey, strData, strFilePath);

		strKey.Format(_T("LD_PnP_CAM_CENTER_POS_Y"));
		strData.Format(_T("%3.3f"), m_dIS_CamCenterPosY);
		WritePrivateProfileString(strNode, strKey, strData, strFilePath);
	}

	return rslt;
}