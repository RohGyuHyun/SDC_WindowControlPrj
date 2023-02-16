// TactTimeData.h: interface for the CTactTimeData class.
//
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>
#include "Define.h"
#include "TimeCheck.h"
#include "AniUtil.h"

extern CCriticalSection g_TtIntlock;

typedef struct _tagTACTTIME
{
	bool m_bTTFirstUpdate;
	bool m_bTTUpdate;
	
	long lStartTime;
	long lEndTime;
	double dTactTime;
	double dMaxTactTime;
	double dMinTactTime;
	_tagTACTTIME(){
		lStartTime = 0;
		lEndTime = 0;
		dTactTime = 0.0;
		dMaxTactTime = 0.0;
		dMinTactTime = 0.0;
		
		m_bTTUpdate = false;
		m_bTTFirstUpdate = true;
	};
}tagTACTTIME;

class CTactTimeData  
{
public:
	void SetTickTime()
	{
		CTime time;
		int cTimeSecond,cTimeMin;
		time= CTime::GetCurrentTime();
		int oldTimeSecond = time.GetSecond();
		int OldTimeMin = time.GetMinute();
		while(1)
		{
			time = CTime::GetCurrentTime();
			cTimeSecond = time.GetSecond();
			cTimeMin = time.GetMinute();
			if (cTimeSecond > oldTimeSecond || cTimeMin > OldTimeMin)
			{
				m_TickTime = GetTickCount()%1000;
				break;
			}
		}
	}
	
	int GetTickTime()
	{
		int time = GetTickCount()%1000;
		if (m_TickTime > time)
			time += 1000 - m_TickTime;
		else
			time -= m_TickTime;
		return time;
	};

	void SetUpperWaitTime(int iTime) { m_iUpperWaitTime += iTime; }
	int GetUpperWaitTime() { return m_iUpperWaitTime; }

	void SetUpperUpdateWaitTime(bool bFlag) { m_bUpperUpdateWaitTime = bFlag; }
	bool IsUpperUpdateWaitTime() { return m_bUpperUpdateWaitTime; }
	void SetUpperOverTime(bool bFlag) { m_bUpperOverTime = bFlag; }
	bool IsUpperOverTime() { return m_bUpperOverTime; }
	// Lower
	void SetLowerWaitTime(int iTime) { m_iLowerWaitTime += iTime; }
	int GetLowerWaitTime() { return m_iLowerWaitTime; }

	void SetLowerUpdateWaitTime(bool bFlag) { m_bLowerUpdateWaitTime = bFlag; }
	bool IsLowerUpdateWaitTime() { return m_bLowerUpdateWaitTime; }
	void SetLowerOverTime(bool bFlag) { m_bLowerOverTime = bFlag; }
	bool IsLowerOverTime() { return m_bLowerOverTime; }

	void SetUpperWaitStartTime(long lTime) { m_lUpperWaitStartTime = lTime; }
	long GetUpperWaitStartTime() { return m_lUpperWaitStartTime; }
	void SetUpperWaitEndTime(long lTime) { m_lUpperWaitEndTime = lTime; }
	long GetUpperWaitEndTime() { return m_lUpperWaitEndTime; }
	
	void SetLowerWaitStartTime(long lTime) { m_lLowerWaitStartTime = lTime; }
	long GetLowerWaitStartTime() { return m_lLowerWaitStartTime; }
	void SetLowerWaitEndTime(long lTime) { m_lLowerWaitEndTime = lTime; }
	long GetLowerWaitEndTime() { return m_lLowerWaitEndTime; }

	void SetStartTime(int nIndex);
	void SetEndTime(int nIndex);
	
	void SetTTUpdate(int nIndex, bool bFlag);
	void SetTTFirstUpdate(int nIndex, bool bFlag);
	
	long GetStartTime(int nIndex);
	long GetEndTime(int nIndex);
	
	double GetTactTime(int nIndex);
	double GetMaxTactTime(int nIndex);
	double GetMinTactTime(int nIndex);
	
	CString GetTactTimeItem(int nIndex);
	
	bool IsTTUpdate(int nIndex);
	bool IsTTFirstUpdate(int nIndex);
	
	void SetTactTimeItemName();
	void SaveTactTime();

	void SetTactTimeStart();
	void SetTactTimeEnd();
	CString GetTactTime();

	void SetSubTactTimeStart(int nSub);
	void SetSubTactTimeEnd(int nSub);	
	CString GetSubTactTime(int nSub);

	// Loss time
	CString m_strLossStartTime;	
	CString m_strLossEndTime;	
	long m_lLossStopMinutetime;   
	long m_lLossStartMinutetime; 

	CTactTimeData();
	virtual ~CTactTimeData();

private:
	int m_TickTime;
	
	int m_iUpperWaitTime;
	bool m_bUpperUpdateWaitTime, m_bUpperOverTime;
	long m_lUpperWaitStartTime, m_lUpperWaitEndTime;

	int m_iLowerWaitTime;
	bool m_bLowerUpdateWaitTime, m_bLowerOverTime;
	long m_lLowerWaitStartTime, m_lLowerWaitEndTime;

	CString m_strTactTimeItem[MAX_TT_UNIT_CNT];
	tagTACTTIME m_tagTactTime[MAX_TT_UNIT_CNT];
	
	CCriticalSection m_csTactTimeDataSetInterlock;
	CCriticalSection m_csTactTimeDataSaveInterlock;

	long m_lStartTime;
	long m_lEndTime;
	CString m_strTact;

	long m_lSubStartTime[MAX_TT_UNIT_CNT];
	long m_lSubEndTime[MAX_TT_UNIT_CNT];	
	CString m_strSubTact[MAX_TT_UNIT_CNT];
//<<
};

extern CTactTimeData g_TactTimeData;

