// TactTimeData.cpp: implementation of the CTactTimeData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SDC_WindowInspPrj.h"
#include "TactTimeData.h"
#include "WorkManageData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTactTimeData g_TactTimeData;
CCriticalSection g_TtIntlock;

CTactTimeData::CTactTimeData()
{
	m_iUpperWaitTime = 0;
	m_iLowerWaitTime = 0;

	m_bUpperUpdateWaitTime = false;
	m_bLowerUpdateWaitTime = false;
	m_bUpperOverTime = false;
	m_bLowerOverTime = false;

	SetTickTime();
	SetTactTimeItemName();

	m_lStartTime = 0;
	m_lEndTime = 0;
	m_strTact = _T("0.000");

	m_strLossStartTime = _T("");
	m_strLossEndTime = _T("");
	
	for(int i=0; i<MAX_TT_UNIT_CNT; i++)
	{
		m_lSubStartTime[i] = 0;
		m_lSubEndTime[i] = 0;
		m_strSubTact[i] = _T("0.000");
	}
}

CTactTimeData::~CTactTimeData()
{

}

void CTactTimeData::SetTactTimeItemName() 
{
	m_strTactTimeItem[TT_LINE]					= "TT_LINE";
	m_strTactTimeItem[TT_MACHINE]				= "TT_MACHINE";
	m_strTactTimeItem[TT_INPUT_TRAY]			= "TT_INPUT_TRAY";
	m_strTactTimeItem[TT_LOADER_PICKER]			= "TT_LOADER_PICKER";
	m_strTactTimeItem[TT_PRE_ALIGN]				= "TT_PRE_ALIGN";
	m_strTactTimeItem[TT_TR1]					= "TT_TR1";
	m_strTactTimeItem[TT_AZONE]					= "TT_AZONE";
	m_strTactTimeItem[TT_BZONE]					= "TT_BZONE";
	m_strTactTimeItem[TT_CZONE]					= "TT_CZONE";
	m_strTactTimeItem[TT_DZONE]					= "TT_DZONE";
	m_strTactTimeItem[TT_EZONE]					= "TT_EZONE";
	m_strTactTimeItem[TT_FZONE]					= "TT_FZONE";
	m_strTactTimeItem[TT_GZONE]					= "TT_GZONE";
	m_strTactTimeItem[TT_HZONE]					= "TT_HZONE";
}

void CTactTimeData::SetStartTime(int nIndex)
{
	long lTime = GetCurrentTime();
	m_tagTactTime[nIndex].lStartTime = lTime;
}

void CTactTimeData::SetEndTime(int nIndex) 
{
	m_csTactTimeDataSetInterlock.Lock();
	long lTime = GetCurrentTime();

	m_tagTactTime[nIndex].lEndTime = lTime;
	//나머지 값을 모두 계산 한다. 
	m_tagTactTime[nIndex].dTactTime = (double)(m_tagTactTime[nIndex].lEndTime-m_tagTactTime[nIndex].lStartTime)*0.001;

	if(m_tagTactTime[nIndex].dTactTime < 0.0)
		m_tagTactTime[nIndex].dTactTime = 0.0;

	// TactTime 관련 계산 모든 유닛 1Cell 기준으로 변경
//	m_tagTactTime[nIndex].dTactTime *= 0.25;

	//Min Tact Time
	if(m_tagTactTime[nIndex].m_bTTFirstUpdate)
	{
		m_tagTactTime[nIndex].dMinTactTime = m_tagTactTime[nIndex].dTactTime;
		m_tagTactTime[nIndex].m_bTTFirstUpdate = false;
			
	}
	else if(m_tagTactTime[nIndex].dMinTactTime >  m_tagTactTime[nIndex].dTactTime)
		m_tagTactTime[nIndex].dMinTactTime = m_tagTactTime[nIndex].dTactTime;
	//Max Tact Time
	if(m_tagTactTime[nIndex].dMaxTactTime <  m_tagTactTime[nIndex].dTactTime)
		m_tagTactTime[nIndex].dMaxTactTime = m_tagTactTime[nIndex].dTactTime;	

	m_tagTactTime[nIndex].m_bTTUpdate = true;

	m_csTactTimeDataSetInterlock.Unlock();
}

void CTactTimeData::SetTTUpdate(int nIndex, bool bFlag)
{
	m_tagTactTime[nIndex].m_bTTUpdate = bFlag;
}

void CTactTimeData::SetTTFirstUpdate(int nIndex, bool bFlag)
{
	m_tagTactTime[nIndex].m_bTTFirstUpdate = bFlag;
}

double CTactTimeData::GetTactTime(int nIndex)
{
	return m_tagTactTime[nIndex].dTactTime;
}

double CTactTimeData::GetMaxTactTime(int nIndex)
{
	return m_tagTactTime[nIndex].dMaxTactTime;
}

double CTactTimeData::GetMinTactTime(int nIndex)
{
	if(m_tagTactTime[nIndex].dMinTactTime < 0.1)
		m_tagTactTime[nIndex].dMinTactTime = m_tagTactTime[nIndex].dTactTime;

	return m_tagTactTime[nIndex].dMinTactTime;
}

CString CTactTimeData::GetTactTimeItem(int nIndex) 
{
	return m_strTactTimeItem[nIndex];
}

bool CTactTimeData::IsTTUpdate(int nIndex)
{
	return m_tagTactTime[nIndex].m_bTTUpdate;
}

bool CTactTimeData::IsTTFirstUpdate(int nIndex)
{
	return m_tagTactTime[nIndex].m_bTTFirstUpdate;
}

void CTactTimeData::SaveTactTime()
{
	m_csTactTimeDataSaveInterlock.Lock();

	DWORD dwRet;
	int iRet;
	CString strPath, strFilePath, strDayPath;
	CString tempStr, strTime, strDate;
	CStdioFile pFile;
	CFileFind finder; 
	
	CTime NowTime=CTime::GetCurrentTime();
	strDate = NowTime.Format(L"%Y%m%d");
	strPath = TACTTIME_DATA_PATH;
	strDayPath = strPath+strDate;

	dwRet = GetFileAttributes(strDayPath);
	if(dwRet != FILE_ATTRIBUTE_DIRECTORY)		// 폴더가 존재하지 않는 경우.
	{	
		iRet = CreateDirectory(strDayPath, NULL);
		if(iRet==0) // 생성하지 못한 경우. 
		{
			m_csTactTimeDataSaveInterlock.Unlock();
			return;
		}
	}

	strTime = GetTimeString(); // 저장시간 기록용
	strFilePath = strDayPath+"\\"+"TactTimeData.dat";

	BOOL bWorking = finder.FindFile(strFilePath);//파일을 찾는다..
	finder.Close();
	if(!bWorking) //파일을 못 찾았으면 파일을 새로 생성
	{
		if(!pFile.Open(strFilePath, CFile::modeCreate |CFile::modeWrite)) 
			return;
 
       tempStr.Format(L"Time,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
			m_strTactTimeItem[TT_LINE],
			m_strTactTimeItem[TT_MACHINE],
			m_strTactTimeItem[TT_INPUT_TRAY],
			m_strTactTimeItem[TT_LOADER_PICKER],
			m_strTactTimeItem[TT_PRE_ALIGN],
			m_strTactTimeItem[TT_TR1],
			m_strTactTimeItem[TT_AZONE],
			m_strTactTimeItem[TT_BZONE],
			m_strTactTimeItem[TT_CZONE],
			m_strTactTimeItem[TT_DZONE],
			m_strTactTimeItem[TT_EZONE],
			m_strTactTimeItem[TT_FZONE],
			m_strTactTimeItem[TT_GZONE],
			m_strTactTimeItem[TT_HZONE]);
		pFile.WriteString(tempStr);

	}
	else
	{
		if(!pFile.Open(strFilePath, CFile::modeNoInherit |CFile::modeWrite)) 
			return;
	}
	pFile.Seek(0L,CFile::end);
	tempStr.Format(L"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
		strTime,
		"TT_LINE",
		"TT_MACHINE",
		"TT_INPUT_TRAY",
		"TT_LOADER_PICKER",
		"TT_PRE_ALIGN",
		"TT_TR1",
		"TT_AZONE",
		"TT_BZONE",
		"TT_CZONE",
		"TT_DZONE",
		"TT_EZONE",
		"TT_FZONE",
		"TT_GZONE",
		"TT_HZONE");
	pFile.WriteString(tempStr);
	pFile.Close();
	m_csTactTimeDataSaveInterlock.Unlock();
}

void CTactTimeData::SetTactTimeStart()
{
	m_lStartTime = GetCurrentTime();
}

void CTactTimeData::SetSubTactTimeStart(int nSub)
{
	m_lSubStartTime[nSub] = GetCurrentTime();
}

void CTactTimeData::SetSubTactTimeEnd(int nSub)
{
	double	dTact(.0);
	CString sTemp("0.000");
	g_TtIntlock.Lock();
	m_lSubEndTime[nSub] = GetCurrentTime();
	if(m_lSubEndTime[nSub] > m_lSubStartTime[nSub])
	{
		dTact = (m_lSubEndTime[nSub] - m_lSubStartTime[nSub]) / 1000.;
		sTemp.Format(L"%.3f",dTact);
	}  
       
	m_strSubTact[nSub] = sTemp;      
	g_TtIntlock.Unlock();
}

CString CTactTimeData::GetSubTactTime(int nSub)
{        
	CString StrTact = m_strSubTact[nSub];

	return  StrTact;  
}

