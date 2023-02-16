// Axis.cpp: implementation of the CAxis class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "SystemData.h"
#include "ModelData.h"
#include "MsgBox.h"									//전역

//DIALOG


#include "Axis.h"
#include "GetTextDlg.h"								//화상키보드
#include "DialogNumberBox.h"						//화상숫자패드
#include "DialogTeach.h"
#include "DialogTackTimeView.h"
#include "DialogProductInfoView.h"
#include "DialogMsgBox.h"
#include "DialogManual.h"
#include "DialogMainView.h"
#include "DialogMainData.h"
#include "DialogMainCOM.h"
#include "DialogLogIn.h"
#include "DialogCheckIO.h"
#include "DialogAxisOrigin.h"
#include "DialogAxisManager.h"
#include "DialogAxisJog.h"
#include "DialogAlarmTrendView.h"

#include "SDC_WindowControlPrjDlg.h"

//#include <afxmt.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
HANDLE g_hMutex;

CAxis::CAxis()
{
#ifdef _USE_DNET
	mActPos =_T("");
	m_strPort =_T("");
	mDID = 0;
	m_iAxisUse=0;				
	m_iMotorType=0;			
	m_iPulseOutMethod=0;		
	m_iComPort=0;				
	m_iControlType=0;		    
	m_iOriginMethod=0;		
	m_iPositiveLimitLevel=0;	
	m_iHomeLevel=0;			
	m_iNegativeLimitLevel=0;	
	m_iAmpFaultLevel=0;		
	m_iAmpFaultResetLevel=0;	
	m_iDirection=0;			
	m_iE_Gear=0;				
#endif
#ifdef _USE_NMC
	m_lRet = MC_OK;
#endif

	m_lCtrl=0;
	m_lAxis = 0;

	m_NormVel = 0.0;
	m_HomeVel = 0.0;
	m_JogVel = 0.0;

	m_bNeg = TRUE;
	m_bHome = TRUE;
	m_bHomeLevel = TRUE;
	m_bIndex = TRUE;
	m_bThreadStarted = FALSE;
	m_bOriginReturnFlag = FALSE; 
	m_bHomingStop = FALSE;
	m_bFaultLevel = 1;
	m_dOrgOffset = 0.0; 
	m_dSysVelRatio = 100;

	m_iMotorType = 0;
	m_iServoRatioType = 0; //누적 값 측정


	m_bConnectState = false;
	SetHomingErr(FALSE);
	m_lStopDec = 0; 
	m_MMC_Err = 0;
#ifdef _USE_FASTECH
	m_iMotionAlarmCode = 0;
#endif
} 

CAxis::~CAxis()
{
	
}


BOOL CAxis::MotionDone()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		if (axis_done((int)m_lAxis) == 1)
			return TRUE; 
		else
			return FALSE;
	}
#endif
#ifdef _USE_AJINXL
	DWORD Status;
	if(GetControlType() == _AJINXL)
	{
		g_AxisSyncIntlock.Lock();
		AxmStatusReadInMotion((int)m_lAxis, &Status); //Motion 중이면 TRUE / 아니면 FALSE
		g_AxisSyncIntlock.Unlock();
		if(Status != FALSE) 
			return FALSE; 
		else
			return TRUE;
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		DWORD dwAxisStatus=0;
		EZISERVO_AXISSTATUS state;
	
		g_AxisSyncIntlock.Lock();
		m_MMC_Err = FAS_GetAxisStatus((BYTE)m_iPortNo, (BYTE)m_lAxis, &dwAxisStatus);
		g_AxisSyncIntlock.Unlock();
	
		state.dwValue = dwAxisStatus;

		if(m_MMC_Err == FMM_OK)
		{
			if(state.FFLAG_MOTIONING) //운전 중이면...1
			{
				return FALSE; 
			}
			else
			{
				return TRUE; 
			}
		}
		else
		{
			return FALSE;
		}
	}
#endif
#ifdef _USE_DNET
	int nRet = 0, nRet2=0;
	if (GetControlType() == _DNET)
	{
		g_AxisSyncIntlock.Lock();
		nRet = dnetIsMoving(m_iComPort, m_lAxis); //Motion 중이면 TRUE / 아니면 FALSE
		nRet2 = dnetIsInPosition(m_iComPort, m_lAxis);
		g_AxisSyncIntlock.Unlock();

		if((nRet==DNET_RETURN_FALSE) && (nRet2==DNET_ERR_NO))
		{
	// 		if(GetPosition()!=dPos)
	// 		{
	// 			return FALSE;
	// 		}
			return TRUE;
		}
		else
			return FALSE;
	}
#endif
#ifdef _USE_NMC
	if(GetControlType() == _NMC)
	{
		UINT32 Status = 0;
		MC_ReadAxisStatus(m_iPortNo, (int)m_iAxisID, &Status);
		if (Status & mcASStandStill)
			return TRUE; 
		else
			return FALSE;
	}
#endif
	//<<
	return TRUE; 
}

BOOL CAxis::Wait4Done(double dPos, bool bUpperInterfaceInterlockCheck, bool bLowerInterfaceInterlockCheck)
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif
	CTimerCheck Timer(20000);
	Timer.StartTimer();

	if(GetAxisUse() == NOT_USE) return TRUE;

	while(!MotionDone())
	{ 
		Delay(2, TRUE);
		if(Timer.IsTimeOver())
			return FALSE;
	}
	return TRUE; 
}

BOOL CAxis::CheckSWLimit(double dPos)
{
	if(dPos > m_dPosSWLimit)
	{
		return FALSE;
	}

	if(dPos < m_dNegSWLimit)
	{
		return FALSE;
	}

	return TRUE;
}


BOOL CAxis::isOriginReturn()
{
	return m_bOriginReturnFlag;
}

BOOL CAxis::GetAmpFaultState()
{
// #ifdef NOT_MACHINE
// 	if(GetControlType() != _FASTECH)
// 		return FALSE;
// #endif
	
	int source(0);

	//20130901 lee
	if(GetAxisUse() == NOT_USE) return TRUE;

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		source = axis_source((int)m_lAxis);
		m_MMC_Err =source & 0x000F;
		if(m_MMC_Err == 8 || m_MMC_Err == 9 || m_MMC_Err == 10 || m_MMC_Err == 12)
		{
			SetOriginReturn(false); //131020 JSPark
			return FALSE;
		}
		else
			return TRUE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD upStatus;
		g_AxisSyncIntlock.Lock();			
		AxmSignalReadServoAlarm (m_lAxis, &upStatus);
		g_AxisSyncIntlock.Unlock();
		
		if(upStatus)
		{
			SetOriginReturn(false); //131020 JSPark
			return FALSE;
		}
		else
			return TRUE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		DWORD dwAxisStatus=0;
		EZISERVO_AXISSTATUS state;
		
		m_MMC_Err = FAS_GetAxisStatus((BYTE)m_iPortNo, (BYTE)m_lAxis, &dwAxisStatus);
		
		state.dwValue = dwAxisStatus;

		if(m_MMC_Err == FMM_OK)
		{
// 			if(state.FFLAG_ERRORALL)
			if(dwAxisStatus&0x00000001)
			{
				SetOriginReturn(false); //131020 JSPark
				return FALSE;
			}
			else 
				return TRUE;
		}
		else
		{
			SetOriginReturn(false); //131020 JSPark
			return FALSE;
		}
	}
#endif
#ifdef _USE_DNET
	int nRet = 0;
	if (GetControlType() == _DNET)
	{
		g_AxisSyncIntlock.Lock();			
		nRet = dnetIsAlarm(m_iComPort, m_lAxis);
		g_AxisSyncIntlock.Unlock();

		if (nRet == DNET_ERR_PORT_FATAL)
		{
			SetOriginReturn(false);
			return FALSE;
		}
	}
#endif
#ifdef _USE_NMC
	UINT32 AxisInfo = 0;
	if(GetControlType() == _NMC)
	{
		m_lRet = MC_OK;
		m_lRet = MC_ReadAxisStatus(m_iPortNo, (int)m_iAxisID, &AxisInfo);
		if(m_lRet == MC_OK)
		{
			if(AxisInfo & mcErrorStop)
				return FALSE;
			else
				return TRUE;
		}
		else
			return FALSE;
	}
#endif
	return TRUE;
}


void CAxis::SetOriginReturn(BOOL org)
{ 
	CString strMsg;
	g_AxisSyncIntlock.Lock();
	m_bOriginReturnFlag = org;
	g_AxisSyncIntlock.Unlock();
}

void CAxis::SetControlType()
{
	m_iControlType = GetParasControlType();
}

BOOL CAxis::GetAmpOnState() //130214 JSPark
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif
	
#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		int source;
		
		source = axis_source((int)m_lAxis);
		
		if(source == 0 || source == 1 || source == 2 || source == 4 
			|| source == 256 || source == 257 || source == 258 || source == 260)
			return TRUE;
		else
		{
			SetOriginReturn(false); //131020 JSPark
			return FALSE;
		}
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD upStatus;
		g_AxisSyncIntlock.Lock();			
		AxmSignalIsServoOn (m_lAxis, &upStatus);
		g_AxisSyncIntlock.Unlock();
		
		if(upStatus)
			return TRUE;
		else
		{
			SetOriginReturn(false); //131020 JSPark
			return FALSE;
		}
	}
#endif
#ifdef _USE_DNET
	int upStatus = 0;
	if (GetControlType() == _DNET)
	{
		g_AxisSyncIntlock.Lock();			
		upStatus = dnetIsServoOn(m_iComPort, m_lAxis);
		g_AxisSyncIntlock.Unlock();

		if(upStatus != DNET_ERR_NO)
		{
			SetOriginReturn(false); //131020 JSPark
			return FALSE;	
		}
	}
#endif
#ifdef _USE_NMC
	UINT32 AxisInfo = 0;
	if(GetControlType() == _NMC)
	{
		m_lRet = MC_OK;
		m_lRet = MC_ReadAxisStatus(m_iPortNo, (int)m_iAxisID, &AxisInfo);
		if(m_lRet == MC_OK)
		{
			if(AxisInfo & mcStandStill)
				return TRUE;
			else
				return FALSE;
		}
		else
			return FALSE;
	}
#endif
	return TRUE;
}


void CAxis::SetAxisUse() //130220 JSPark
{
	m_iAxisUse = GetParasAxisUse();
}


BOOL CAxis::ClearState()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif
 
	int ret(0);
	int i = 0;
	CString tmp(_T(""));

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		while (1)
		{
			ret = clear_status((int)m_lAxis);
			if (ret == 0)
			{
				break;
			}
			else
			{
				Delay(AXIS_DELAY,TRUE);
				if (axis_state((int)m_lAxis) == 0)
					break;
				else if (i++ > 200)
				{
					return FALSE;
				}
			}
		}	
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		// Move 전 Amp reset 함수
		return TRUE;
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		DWORD dwAxisStatus=0;
		EZISERVO_AXISSTATUS state;
		
		m_MMC_Err = FAS_GetAxisStatus((BYTE)m_iPortNo, (BYTE)m_lAxis, &dwAxisStatus);
		
		state.dwValue = dwAxisStatus;

		if (state.FFLAG_ERRORALL | state.FFLAG_ERROVERCURRENT | state.FFLAG_ERROVERLOAD)
		{
			ret = FAS_ServoAlarmReset((BYTE)m_iPortNo, (BYTE)m_lAxis);
			if(ret == FMM_OK)
				return TRUE;
			else 
				return FALSE;
		}
	}
#endif
#ifdef _USE_DNET
	//Need2Check
#endif
	Delay(10,TRUE);
	return TRUE;
}

BOOL CAxis::StartMove(double dPos)
{	
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
	CSystemData* sys_data = pdlg->GetSystemData();

#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

	int val=0;
	int ret(0);

	double dVel(0.);
	int    nAcc(0), nDec(0);
	//20130901 lee
	if(GetAxisUse() == NOT_USE) 
		return TRUE;

	ClearState();
//>> 131020 JSPark
	if(!GetAmpOnState())
		return FALSE;

	if(!GetAmpFaultState())
		return FALSE;
//<<
	if(!isOriginReturn())  //20130508
		return FALSE;

	if(!CheckSWLimit(dPos))
		return FALSE;

	dPos = dPos * GetPulse4Mm();

	dVel = GetNormVel();

	//20130921 lee
	if(dVel == 0) dVel = 10000.0;

	//시스템 속도 적용 
	dVel *= (sys_data->GetSystemVelRatio() / 100.);

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		nAcc = (int)GetNormAcc();
		nDec = (int)GetNormDec();

		g_AxisSyncIntlock.Lock();
		ret = start_move((int)m_lAxis, dPos, dVel, nAcc);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == MMC_OK)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		CString str;
		DWORD ret;
		DWORD uAbsRelMode = POS_ABS_MODE;
		DWORD uProfileMode = SYM_S_CURVE_MODE;
		g_AxisSyncIntlock.Lock();

		ret = Wait4Done();
		if(!ret) 
		{
			g_AxisSyncIntlock.Unlock();  //20130329 lee
			str.Format(L"Ajin B/D Error발생  %s축 Motion Done Error",GetAxisName());
			AfxMessageBox(str);
			return FALSE;
		}
		AxmMotSetAccelUnit(m_lAxis,1);
		AxmMotSetMinVel(m_lAxis,1);
		AxmMotSetAbsRelMode(m_lAxis, uAbsRelMode);
		AxmMotSetProfileMode(m_lAxis, uProfileMode);

		ret = AxmMoveStartPos(m_lAxis, dPos, dVel, GetNormAcc(), GetNormDec());
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
		{	
			str.Format(L"Ajin B/D Error발생  %s축 Error CODE : %d",GetAxisName(), ret);
			AfxMessageBox(str);
			return FALSE;	
		}
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		long m_lMAxis[2] = {m_lAxis, m_lAxis};
		g_AxisSyncIntlock.Lock();




//////////////////////////////////////////////////////////////////////////
		MOTION_OPTION_EX opt = {0};
		opt.flagOption.BIT_USE_CUSTOMACCEL = 1;
		opt.flagOption.BIT_USE_CUSTOMDECEL = 1;
		opt.wCustomAccelTime = (long)GetNormAcc();
		opt.wCustomDecelTime = 	(long)GetNormDec();
		ret = FAS_MoveSingleAxisAbsPosEx((BYTE)m_iPortNo, (BYTE)m_lAxis, (long)dPos, (long)GetNormVel(), &opt);
//////////////////////////////////////////////////////////////////////////

//		ret = FAS_MoveSingleAxisAbsPos((BYTE)m_iPortNo, (BYTE)m_lAxis,(long)dPos,(long)GetNormVel());
		//		ret = FAS_MoveLinearAbsPos((BYTE)m_iPortNo, (BYTE)2, (BYTE)m_lAxis, (long)dPos,(long)GetNormVel(),(long)GetNormAcc());
		g_AxisSyncIntlock.Unlock();
		if (ret == FMM_OK)
		{
			g_ErrData.m_iFastectAlarmCode = -1;
			return TRUE;
		}
		else
		{
			g_ErrData.m_iFastectAlarmCode = ret;
			return FALSE;
		}
	}
#endif
#ifdef _USE_DNET
	int nRet = 0;
	nAcc = (int)GetNormAcc();
	nDec = (int)GetNormDec();
	if (GetControlType() == _DNET)
	{		
		//Operation mode PositionMode 로 설 정
		nRet = dnetEnable(m_iComPort, m_lAxis, DNET_MC_OPMODE_PPM);
		if (nRet != DNET_ERR_NO)
		{
				return FALSE;
		}
//		//이동 속도, acc, dec 설정
		nRet = dnetProfileSetup(m_iComPort, m_lAxis, (ULONG32)dVel, (ULONG32)nAcc, (ULONG32)nDec);
		if (nRet != DNET_ERR_NO)
		{
			return FALSE;
		}
		nRet = dnetMovePosition(m_iComPort, m_lAxis, (LONG32)dPos, FALSE, true);
		if (nRet != DNET_ERR_NO && nRet != 0)
		{			
			return FALSE;
		}
	}
#endif
#ifdef _USE_NMC
	if(GetControlType() == _NMC)
	{
		g_AxisSyncIntlock.Lock();
		ret = MC_MoveAbsolute(m_iPortNo, (int)m_iAxisID, dPos, dVel,
			GetNormVel()*1000.0f/(4.0f*GetNormAcc()),
			GetNormVel()*1000.0f/(4.0f*GetNormDec()),
			0, mcCurrentDirection, mcAborting);
		//arrAxisModel[nAxisNum].fJogMoveSpeed*(1000.0f/(4.0f*arrAxisModel[nAxisNum].nJogMoveAccel)),
		g_AxisSyncIntlock.Unlock();
		if (ret == MC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
	
	return TRUE;
}

BOOL CAxis::StartMove(double dPos, double dVel, double dAcc, double dDec, double dJerk, double dDelay)
{
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
	CSystemData* sys_data = pdlg->GetSystemData();

#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

	int val=0;
	int ret(0);
	//20130901 lee
	if(GetAxisUse() == NOT_USE) 
		return TRUE;

	ClearState();
//>> 131020 JSPark
	if(!GetAmpOnState())
		return FALSE;

	if(!GetAmpFaultState())
		return FALSE;
//<<	
	if(!isOriginReturn())  //20130508
		return FALSE;

	if(!CheckSWLimit(dPos))
		return FALSE;
	
	dPos = dPos * GetPulse4Mm();

	//시스템 속도 적용 

	if(dVel == 0.0)
	{
		dVel = GetNormVel();
		dVel *= (sys_data->GetSystemVelRatio() / 100.);
	}

#ifdef _USE_AJINXL
	if(GetControlType() == _MMC)
	{
		g_AxisSyncIntlock.Lock();
		ret = start_move((int)m_lAxis, dPos, dVel, (int)dAcc);
		g_AxisSyncIntlock.Unlock();

		if (ret == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		CString str;
		DWORD ret;
		DWORD uAbsRelMode = POS_ABS_MODE;
		DWORD uProfileMode = SYM_S_CURVE_MODE;
		//>> 20130917 kang
		g_AxisSyncIntlock.Lock();

		ret = Wait4Done();
		if(!ret) 
		{
			g_AxisSyncIntlock.Unlock();
			str.Format(L"Ajin B/D Error발생  %s축 Motion Done Error",GetAxisName());
			AfxMessageBox(str);
			return FALSE;
		}

		AxmMotSetAccelUnit(m_lAxis,1);
		AxmMotSetMinVel(m_lAxis,1);
		AxmMotSetAbsRelMode(m_lAxis, uAbsRelMode);
		AxmMotSetProfileMode(m_lAxis, uProfileMode);
		//<< 20130917 kang
		ret = AxmMoveStartPos(m_lAxis, dPos, dVel, dAcc, dDec);
		g_AxisSyncIntlock.Unlock();
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{	
		g_AxisSyncIntlock.Lock();
		ret = FAS_MoveSingleAxisAbsPos((BYTE)m_iPortNo, (BYTE)m_lAxis,(long)dPos,(long)dVel);
		g_AxisSyncIntlock.Unlock();
		if (ret == FMM_OK)
		{
			g_ErrData.m_iFastectAlarmCode = -1;
			return TRUE;
		}
		else
		{
			g_ErrData.m_iFastectAlarmCode = ret;
			return FALSE;			
		}
	}
#endif
#ifdef _USE_DNET
	int nRet = 0, i(0);
	dAcc = (int)GetNormAcc();
	dDec = (int)GetNormDec();

	if (GetControlType() == _DNET)
	{
		g_AxisSyncIntlock.Lock();
		//Operation mode PositionMode 로 설 정
		nRet = dnetEnable(m_iComPort, m_lAxis, DNET_MC_OPMODE_PPM);
		if (nRet != DNET_ERR_NO)
		{
			return FALSE;
		}

		//이동 속도, acc, dec 설정
		nRet = dnetProfileSetup(m_iComPort, m_lAxis, (ULONG32)dVel, (ULONG32)dAcc, (ULONG32)dDec);
		if (nRet != DNET_ERR_NO)
		{
			return FALSE;
		}

		//절대 포지션이동
		nRet = dnetMovePosition(m_iComPort, m_lAxis, (LONG32)dPos, FALSE, true);		
		if (nRet != DNET_ERR_NO)
		{
			return FALSE;
		}
	}

#endif
	return TRUE;
}




/*

//>> 130219 JSPark
void CAxis::SetPortNo()
{
	m_iPortNo = GetParasPortNo();
#ifdef _USE_DNET
	m_iComPort = g_Motion.portID;
#endif
}


//Need2Check
BOOL CAxis::SetMultiGroup(int nGroupID)
{
	int nRet = 0;
#ifdef  _USE_DNET
	if (GetControlType() == _DNET)
	{
		nRet = dnetSetSyncGroup(m_iComPort, m_lAxis, nGroupID);
		if (nRet != DNET_ERR_NO)
			return FALSE;
	}

#endif
	return TRUE;
}

//Need2Check
BOOL CAxis::MultiMove(int nGroup)
{
#ifdef  _USE_DNET
	int nRet = 0;
	double dPos(0.0);
	if (GetControlType() == _DNET)
	{
		if (nRet != DNET_ERR_NO)
			return FALSE;
	}
#endif
	return TRUE;
}


void CAxis::SetCtrl(long lCtrl)
{

}


void CAxis::Homing4Multi()
{
#ifdef NOT_MACHINE
	return;
#endif
	
	if(IsThreadStarted())
		return;

	m_bHomingErr = FALSE;

	SetThreadStarted(TRUE);

	//Thread 시작
	AfxBeginThread(HomeProcess,this);
	Delay(100,TRUE);
}

BOOL CAxis::Homing4Only()
{
#ifdef NOT_MACHINE
	return FALSE;
#endif

	if(IsThreadStarted())
		return TRUE;

	m_bHomingErr = FALSE;

	SetThreadStarted(TRUE);
	//Thread 시작
	m_bHomingErr = HomeProcess();
	Delay(100,TRUE);

	return m_bHomingErr;
}

BOOL CAxis::StopHoming()
{
#ifdef NOT_MACHINE
	return FALSE;
#endif
	return FALSE;
}

long CAxis::GetAxisStatus()
{
#ifdef NOT_MACHINE
	return FALSE;
#endif
	return FALSE;
}

long CAxis::GetHomeStatus()
{
	long lTemp=0;

	return lTemp;
}

long CAxis::IsHomeFinished()
{
#ifdef NOT_MACHINE
	return FALSE;
#endif
	g_AxisSyncIntlock.Lock();
	if(isOriginReturn())
	{
		g_AxisSyncIntlock.Unlock();
		return TRUE;
	}
	else
	{
		g_AxisSyncIntlock.Unlock();
		return FALSE;
	}

	return FALSE;
}

UINT CAxis::HomeProcess(LPVOID lpVoid) // 수정 필요
{
	CAxis* pAxis = (CAxis*) lpVoid;

	BOOL bStop=FALSE;
	long lAxis = pAxis->GetAxisNo();
	long lRet(0);

	if(pAxis->GetHomingErr())
	{
		AfxMessageBox(L"이런!");
	}
	pAxis->SetHomingErr(FALSE);

	CTimerCheck tLimit(500000);
	tLimit.StartTimer(); 

	lRet = pAxis->Origin();

	if(!pAxis->Wait4Done())
		lRet = HOME_ERROR;

	if(lRet == HOME_ERROR) 
		pAxis->Stop();

	pAxis->SetThreadStarted(FALSE);

	ReleaseMutex(g_hMutex);

	if(lRet == HOME_ERROR)
		pAxis->SetHomingErr(TRUE);
	else
		pAxis->SetHomingErr(FALSE);

	return 0;
}


BOOL CAxis::Move(double dPos, bool bWaitFlag) 
{
	int ret;
	double dCurPos;

	GetPosition(dCurPos);
	double temp = fabs(dPos - dCurPos); 

	if(temp == 0) return TRUE;

	ret = StartMove(dPos);
	if(!ret)
		return FALSE;

	if(bWaitFlag)
		ret = Wait4Done(dPos);
	return ret;
}

BOOL CAxis::Move(double dPos, double dVel, double dAcc, double dDec, double dJerk, double dDelay, bool bWaitFlag)
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif
	double dCurPos;
	int ret;

	GetPosition(dCurPos);
	double temp = fabs(dPos - dCurPos); 
	if(temp == 0) return TRUE;

	ret = StartMove(dPos, dVel, dAcc, dDec, dJerk, dDelay);

	if(!ret)
		return FALSE;

	ret = Wait4Done();

	return ret;
}

BOOL CAxis::HomeProcess()
{
	long lRet;
	BOOL bStop=FALSE;

	CTimerCheck tLimit(30000); 

	tLimit.StartTimer();

	lRet = Origin();
	if(!Wait4Done())
		lRet = HOME_ERROR;
	if(lRet == HOME_ERROR)
		Stop();

	SetThreadStarted(FALSE);

	if(lRet != HOME_ERROR)
		return TRUE;
	else
		return FALSE;
}




BOOL CAxis::GroupMove(double dPosY, double dPosT, double dVelY, double dVelT, double dAccY, double dAccT, double dDecY, double dDecT, int iGroup, bool bWaitFlag) //20131023 lee
{
 	int ret(0);
	double dCurPos;
	if(m_lAxis < 8)
		GetPosition(dCurPos,false);
	else
		GetPosition(dCurPos,true);
	double temp = fabs(dPosY - dCurPos);
	//>> 130318 JSPark
	if(temp == 0) 
	{
		m_bMotion = false;
		return TRUE;
	}
	else
		m_bMotion = true;
	//<<
	ret = StartGroupMove(dPosY, dPosT, dVelY, dVelT, dAccY, dAccT, dDecY, dDecT, iGroup);

	if(!ret)
		return FALSE;

	if(bWaitFlag)
		ret = Wait4Done();

	return ret;
}


BOOL CAxis::OffsetMove(double dPos)
{
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
	CSystemData* sys_data = pdlg->GetSystemData();
	
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif
	int val=0;
	int ret(0);

	double dVel(0.);
	int nAcc(0), nDec(0);

	ClearState();	

	if(!GetAmpOnState())
		return FALSE;

	if(!GetAmpFaultState())
		return FALSE;

	if(!isOriginReturn())  //20130508
		return FALSE;

	dPos = dPos * GetPulse4Mm();
	dVel = GetNormVel();

	//시스템 속도 적용 
	dVel *= (sys_data->GetSystemVelRatio() / 100.);

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		nAcc = (int)GetNormAcc();
		nDec = (int)GetNormDec();

		g_AxisSyncIntlock.Lock();
		ret = start_move((int)m_lAxis, dPos, dVel, nAcc);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == MMC_OK)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		CString str;
		DWORD ret;
		DWORD uAbsRelMode = POS_ABS_MODE;
		DWORD uProfileMode = SYM_S_CURVE_MODE;
		AxmMotSetAbsRelMode(m_lAxis, uAbsRelMode);
		AxmMotSetProfileMode(m_lAxis, uProfileMode);

		ret = Wait4Done();
		if(!ret) 
		{
			str.Format(L"Ajin B/D Error발생  %s축 Motion Done Error",GetAxisName());
			AfxMessageBox(str);
			return FALSE;
		}

		g_AxisSyncIntlock.Lock();
		ret = AxmMovePos(m_lAxis, dPos, dVel, GetNormAcc(), GetNormDec());
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
 		g_AxisSyncIntlock.Lock();
		Delay(1000);
		ret = FAS_MoveSingleAxisAbsPos((BYTE)m_iPortNo, (BYTE)m_lAxis,(long)dPos,(long)GetNormVel());

		if(ret != 0 )
		{
			Delay(2000, TRUE);
			ret = FAS_MoveSingleAxisAbsPos((BYTE)m_iPortNo, (BYTE)m_lAxis,(long)dPos,(long)GetNormVel());
		}
		g_AxisSyncIntlock.Unlock();
		if (ret == FMM_OK)
			return TRUE;
		else
			return FALSE;	
	}	
#endif
#ifdef _USE_DNET
	int nRet = 0;
	nAcc = (int)GetNormAcc();
	nDec = (int)GetNormDec();

	if (GetControlType() == _DNET)
	{
		//Operation mode PositionMode 로 설 정
		nRet = dnetEnable(m_iComPort, m_lAxis, DNET_MC_OPMODE_PPM);

		if (nRet != DNET_ERR_NO)
			return FALSE;
		//이동 속도, acc, dec 설정
		nRet = dnetProfileSetup(m_iComPort, m_lAxis, (ULONG32)dVel, (ULONG32)nAcc, (ULONG32)nDec);

		if (nRet != DNET_ERR_NO)
			return FALSE;
		//절대 포지션이동
		g_AxisSyncIntlock.Lock();
		nRet = dnetMovePosition(m_iComPort, m_lAxis, (LONG32)dPos, FALSE, true);		
		g_AxisSyncIntlock.Unlock();
		if (nRet != DNET_ERR_NO)
			return FALSE;
	}
#endif
#ifdef _USE_NMC
	if(GetControlType() == _NMC)
	{
		g_AxisSyncIntlock.Lock();
		ret = MC_MoveAbsolute(m_iPortNo, (int)m_iAxisID, dPos, dVel,
			GetNormVel()*1000.0f/(4.0f*GetNormAcc()),
			GetNormVel()*1000.0f/(4.0f*GetNormDec()),
			0, mcCurrentDirection, mcAborting);
		//arrAxisModel[nAxisNum].fJogMoveSpeed*(1000.0f/(4.0f*arrAxisModel[nAxisNum].nJogMoveAccel)),
		g_AxisSyncIntlock.Unlock();
		if (ret == MC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif

	//<<
	return TRUE;
}



BOOL CAxis::SetProfileSetup()
{
#ifdef _USE_DNET
	double dVel(0.0);
	int i(0), nRet = 0;
	int nAcc(0), nDec(0);
	dVel = GetNormVel();
	//20130921 lee
	if(dVel == 0) dVel = 10000.0;

	//시스템 속도 적용 
		
	dVel *= (g_SysData.GetSystemVelRatio() / 100.);

	nAcc = (int)GetNormAcc();
	nDec = (int)GetNormDec();

	//이동 속도, acc, dec 설정
	nRet = dnetProfileSetup(m_iComPort, m_lAxis, (ULONG32)dVel, (ULONG32)nAcc, (ULONG32)nDec);
	if (nRet != DNET_ERR_NO)
	{
		return FALSE;
	}
#endif
	return TRUE;
}



//Need2Check
BOOL CAxis::StartGroupMove(double dPosY, double dPosT, double dVelY, double dVelT, double dAccY, double dAccT, double dDecY, double dDecT, int iGroup)
{
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
	CSystemData* sys_data = pdlg->GetSystemData();

#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif
	int val=0;
	int ret(0);

	double dVel(0.0);
	int    nAcc(0), nDec(0);
	CString msg;

	ClearState(); //20130502 lee
	//>> 131020 JSPark
	//Amp On Check
	if(!GetAmpOnState())
		return FALSE;
	//Amp Fault Check
	if(!GetAmpFaultState()) 
		return FALSE;
#ifdef SIMUL_MODE
	return true;
#else
	//Origin Check
	if(!isOriginReturn())
	{
		msg.Format(L"isOriginReturn Error");
		//theApp.m_pSystemLog->LOG_DEBUG(msg);
		WriteLog(msg, SYSTEM_LOG);
		return FALSE;
	}
#endif
	//<<
	if(!CheckSWLimit(dPosY))
		return FALSE;

	dPosY = dPosY * GetPulse4Mm();
	dPosT = dPosT * GetPulse4Mm();
	//시스템 속도 적용 
	//dVel *= (g_SysData.GetSystemVelRatio() / 100.);

	//	TRACE("Start Move Call : %s축\n",GetAxisName());
	//<<GSM
#ifdef _USE_DNET
	int nRet = 0;

 	if (GetControlType() == _DNET)
 	{
		//>> 151103 JSLee
		SetGroupProfileSetup(dPosY, dPosT, dVelY, dVelT, dAccY, dAccT, dDecY, dDecT, iGroup);

		//동시 구동 포지션이동
		g_AxisSyncIntlock.Lock();
		nRet = 	dnetExeGroupMove(m_iComPort, iGroup);
		g_AxisSyncIntlock.Unlock();
		if (nRet != DNET_ERR_NO)
		{
			return FALSE;
		}
		//<<
 	}
#endif
	//<<
	return TRUE;
}
//Need2Check
BOOL CAxis::SetGroupProfileSetup(double dPosY, double dPosT, double dVelY, double dVelT, double dAccY, double dAccT, double dDecY, double dDecT, int iGroup) //151103 JSLee
{
#ifdef _USE_DNET

#endif
	return TRUE;
}

BOOL CAxis::RMove(double dPos, bool bCheckFlag)
{
	if(dPos == 0.0)
		return TRUE;

	int ret = StartRMove(dPos);

	if(!ret)
		return FALSE;

	if(bCheckFlag)
		ret = Wait4Done(); 
	
	return ret;

}

BOOL CAxis::RMove(double dPos, double dVel, double dAcc, double dDec, bool bCheckFlag)
{
	if(dPos == 0.0)
		return TRUE;

	int ret = StartRMove(dPos, dVel, dAcc, dDec);

	if(!ret)
		return FALSE;

	if(bCheckFlag)
		ret = Wait4Done(); 
	
	return ret;

}

BOOL CAxis::StartRMove(double dPos)
{
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
	CSystemData* sys_data = pdlg->GetSystemData();

#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

	int val=0;
	double dVel(0.);
	int nAcc(0), nDec(0);
	double dCurPos(0.);
	//20130901 lee
	if(GetAxisUse() == NOT_USE) 
		return TRUE;

	ClearState();
//>> 131020 JSPark
	if(!GetAmpOnState())
		return FALSE;

	if(!GetAmpFaultState())
		return FALSE;
//<<
	if(!isOriginReturn())  //20130508
		return FALSE;

	if(!CheckSWLimit(dPos))
		return FALSE;	

	dPos = dPos * GetPulse4Mm();
	
	dVel = GetNormVel();
	//시스템 속도 적용 
	dVel *= (sys_data->GetSystemVelRatio() / 100.);

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		nAcc = (int)GetNormAcc();
		nDec = (int)GetNormDec();

		m_MMC_Err = start_move((int)m_lAxis, dPos, dVel, nAcc);
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		CString str;
		DWORD ret;
		DWORD uAbsRelMode = POS_ABS_MODE;
		DWORD uProfileMode = SYM_S_CURVE_MODE;
		AxmMotSetAccelUnit(m_lAxis,1);
		AxmMotSetMinVel(m_lAxis,1);
		AxmMotSetAbsRelMode(m_lAxis, uAbsRelMode);
		AxmMotSetProfileMode(m_lAxis, uProfileMode);


		ret = Wait4Done();
		if(!ret) 
		{
			str.Format(L"Ajin B/D Error발생  %s축 Motion Done Error",GetAxisName());
			AfxMessageBox(str);
			return FALSE;
		}
		
		g_AxisSyncIntlock.Lock();
		ret = AxmMoveStartPos(m_lAxis, dPos, dVel,GetNormAcc(), GetNormDec());
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
 		m_MMC_Err = FAS_MoveSingleAxisIncPos((BYTE)m_iPortNo, (BYTE)m_lAxis,(long)dPos,(long)GetNormVel());
		if (m_MMC_Err == FMM_OK)
			return TRUE;
		else
			return FALSE;			
	}
#endif
#ifdef _USE_DNET
	int nRet = 0;
	nAcc = (int)GetNormAcc();
	nDec = (int)GetNormDec();
	bool bRet = TRUE;

	if (GetControlType() == _DNET)
	{
		//Operation mode PositionMode 로 설 정
		nRet = dnetEnable(m_iComPort, m_lAxis, DNET_MC_OPMODE_PPM);
		if (nRet != DNET_ERR_NO)
		{
			return FALSE;
		}
		//이동 속도, acc, dec 설정
		nRet = dnetProfileSetup(m_iComPort, m_lAxis, (ULONG32)dVel, (ULONG32)nAcc, (ULONG32)nDec);
		if (nRet != DNET_ERR_NO)
		{
			return FALSE;
		}
		//상대 포지션이동
		g_AxisSyncIntlock.Lock();
		nRet = dnetMoveDistance(m_iComPort, m_lAxis, (LONG32)dPos, FALSE, bRet);	
		g_AxisSyncIntlock.Unlock();
		if (nRet != DNET_ERR_NO)
		{
			return FALSE;
		}
	}
#endif
#ifdef _USE_NMC
	if(GetControlType() == _NMC)
	{
		m_lRet = MC_MoveRelative(m_iPortNo, (int)m_iAxisID, dPos, GetNormVel(),
			GetNormVel()*1000.0f/(4.0f*GetNormAcc()),
			GetNormVel()*1000.0f/(4.0f*GetNormDec()),
			0,mcAborting);

		// 		g_AxisSyncIntlock.Unlock();
		if (m_lRet == MC_OK)
			return TRUE;
		else
			return FALSE;				
	}
#endif
	return TRUE;
}

BOOL CAxis::StartRMove(double dPos, double dVel, double dAcc, double dDec)
{
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
	CSystemData* sys_data = pdlg->GetSystemData();

#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif
	int val=0;
	//20130901 lee
	if(GetAxisUse() == NOT_USE) 
		return TRUE;

	ClearState();
//>> 131020 JSPark
	if(!GetAmpOnState())
		return FALSE;

	if(!GetAmpFaultState())
		return FALSE;
//<<	
	if(!isOriginReturn())  //20130508
		return FALSE;
	
	double dCurPos;
	GetPosition(dCurPos);
	
	dPos=dPos + dCurPos;

	if(!CheckSWLimit(dPos))
		return FALSE;

	dPos = dPos * GetPulse4Mm();

	//시스템 속도 적용 
	dVel *= (sys_data->GetSystemVelRatio() / 100.);

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = start_t_move((int)m_lAxis, dPos, dVel, (int)dAcc, (int)dDec);  
		if(m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;
	}
#endif

#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret;
		DWORD uAbsRelMode = POS_ABS_MODE;
		DWORD uProfileMode = SYM_S_CURVE_MODE;
		AxmMotSetAccelUnit(m_lAxis,1);
		AxmMotSetMinVel(m_lAxis,1);
		AxmMotSetAbsRelMode(m_lAxis, uAbsRelMode);
		AxmMotSetProfileMode(m_lAxis, uProfileMode);
		
		g_AxisSyncIntlock.Lock();
		ret = AxmMoveStartPos((int)m_lAxis, dPos, dVel,dAcc, dDec);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif

#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{	
		m_MMC_Err = FAS_MoveSingleAxisIncPos((BYTE)m_iPortNo, (BYTE)m_lAxis,(long)dPos,(long)dVel);
		if (m_MMC_Err == FMM_OK)
			return TRUE;
		else
			return FALSE;					
	}
#endif
#ifdef _USE_DNET
	//Need2Check

#endif
#ifdef _USE_NMC
	if(GetControlType() == _NMC)
	{
		// 		g_AxisSyncIntlock.Lock();
		//		m_MMC_Err = MC_MoveRelative(m_iPortNo, (int)m_iAxisID, dPos, GetNormVel(),
		m_lRet = MC_MoveRelative(m_iPortNo, (int)m_iAxisID, dPos, GetNormVel(),
			GetNormVel()*1000.0f/(4.0f*dAcc),
			GetNormVel()*1000.0f/(4.0f*dDec),
			0,mcAborting);

		// 		g_AxisSyncIntlock.Unlock();
		if (m_lRet == MC_OK)
			return TRUE;
		else
			return FALSE;				
	}
#endif

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
BOOL CAxis::JogMove()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

	int val = 0;
	
	ClearState();
//>> 131020 JSPark
	if(!GetAmpOnState())
		return FALSE;

	if(!GetAmpFaultState())
		return FALSE;
	
	if(!CheckHomingDone())
		return FALSE;
	
	if(!isOriginReturn())  //20130508
		return FALSE;

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = v_move((int)m_lAxis, GetJogVel(), (int)GetJogAcc());
		if(m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret;
		g_AxisSyncIntlock.Lock();
		AxmMotSetAccelUnit(m_lAxis,1);
		AxmMotSetMinVel(m_lAxis,1);
		ret = AxmMoveVel(m_lAxis, GetJogVel(), GetJogAcc(),GetJogAcc());
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{	
		long dir;
		if(GetJogVel() > 0)
			dir = 0;
		else                
			dir = 1;
		m_MMC_Err = FAS_MoveVelocity((BYTE)m_iPortNo, (BYTE)m_lAxis,(long)GetJogVel(),dir);
		if (m_MMC_Err == FMM_OK)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_DNET
	//Need2Check

#endif
	//<<
	return FALSE;
}

BOOL CAxis::JogMove(double dVel, double dAcc, double dDec, double dJerk, double dDelay)
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

	int val = 0;

	ClearState();
//>> 131020 JSPark
	if(!GetAmpOnState())
		return FALSE;

	if(!GetAmpFaultState())
		return FALSE;
//<<
	if(!isOriginReturn())  //20130508
		return FALSE;

	if(dDec==0.0)
		dDec = dAcc;

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = v_move((int)m_lAxis,dVel,(int)dAcc);		
		if(m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret;
		g_AxisSyncIntlock.Lock();
		AxmMotSetAccelUnit(m_lAxis,1);
		AxmMotSetMinVel(m_lAxis,1);
		ret = AxmMoveVel(m_lAxis, dVel, dAcc,dDec);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{	
		long dir;
		if(GetJogVel() > 0) 
			dir = 0;
		else                
			dir = 1;

		m_MMC_Err = FAS_MoveVelocity((BYTE)m_iPortNo, (BYTE)m_lAxis,(long)dVel,dir);
		if (m_MMC_Err == FMM_OK)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_DNET
	//Need2Check

#endif
	return TRUE;
}

BOOL CAxis::JogStop()
{
	int ret(0);

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		ret = v_move_stop((int)m_lAxis);
		if (AxisState() != NO_EVENT)
			return FALSE;
		if (ret == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret;
		g_AxisSyncIntlock.Lock();
		ret = AxmMoveSStop(m_lAxis);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_DNET

	if(GetControlType() == _DNET)
	{
		g_AxisSyncIntlock.Lock();
		ret = dnetQStop(m_iComPort,m_lAxis);
		g_AxisSyncIntlock.Unlock();
		if (ret != DNET_ERR_NO) 
		{
			return FALSE;
		}
	}
#endif
#ifdef _USE_NMC
	if(GetControlType() == _NMC)
	{
		m_lRet = MC_Halt(m_iPortNo,(int)m_iAxisID,((GetJogVel()*2)*(1000.f)/(4.0f*(INT)(GetJogDec()))),0,mcAborting);
	}
#endif
	return TRUE;
}

//>> 130227 JSPark
BOOL CAxis::StartVMove()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

	int val = 0;
	//20130901 lee
	if(GetAxisUse() == NOT_USE) 
		return TRUE;
	
	ClearState();
//>> 131020 JSPark
	if(!GetAmpOnState())
		return FALSE;

	if(!GetAmpFaultState())
		return FALSE;
//<<	
	if(!isOriginReturn())  //20130508
		return FALSE;
	
#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = v_move((int)m_lAxis, GetNormVel(), (int)GetNormAcc());
		if(m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret;
		g_AxisSyncIntlock.Lock();
		AxmMotSetAccelUnit(m_lAxis,1);
		AxmMotSetMinVel(m_lAxis,1);
		ret = AxmMoveVel(m_lAxis, GetNormVel(), GetNormAcc(),GetNormAcc());
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{	
		long dir;
		if(GetJogVel() > 0)
			dir = 0;
		else                
			dir = 1;
		m_MMC_Err = FAS_MoveVelocity((BYTE)m_iPortNo, (BYTE)m_lAxis,(long)GetNormVel(),dir);
		if (m_MMC_Err == FMM_OK)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_DNET
		//Need2Check

#endif
	return FALSE;
}

BOOL CAxis::StartVMove(double dVel, double dAcc, double dDec, double dJerk, double dDelay)
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

	int val = 0;

	ClearState();
//>> 131020 JSPark
	if(!GetAmpOnState())
		return FALSE;

	if(!GetAmpFaultState())
		return FALSE;
//<<		
	if(!isOriginReturn())  //20130508
		return FALSE;
	
	if(dDec==0.0)
		dDec = dAcc;

#ifdef _USE_AJINXL
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = v_move((int)m_lAxis,dVel,(int)dAcc);		
		if(m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret;
		g_AxisSyncIntlock.Lock();
		AxmMotSetAccelUnit(m_lAxis,1);
		AxmMotSetMinVel(m_lAxis,1);
		ret = AxmMoveVel(m_lAxis, dVel, dAcc,dDec);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{	
		long dir;
		if(GetJogVel() > 0) 
			dir = 0;
		else                
			dir = 1;

		m_MMC_Err = FAS_MoveVelocity((BYTE)m_iPortNo, (BYTE)m_lAxis,(long)dVel,dir);
		if (m_MMC_Err == FMM_OK)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_DNET
	//Need2Check

#endif
	return TRUE;
}

BOOL CAxis::StartVStop()
{
	int ret(0);
	if(!isOriginReturn())  //20130508
		return FALSE;

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		ret = v_move_stop((int)m_lAxis);
		if (AxisState() != NO_EVENT)
			return FALSE;
		if (ret == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret;
		g_AxisSyncIntlock.Lock();
		ret = AxmMoveSStop(m_lAxis);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_DNET
	//Need2Check

#endif

	return TRUE;
}
//<<
BOOL CAxis::MoveStop()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = set_stop((int)m_lAxis);  // 내부적으로 STOP EVENT 발생시킴.
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret;
		g_AxisSyncIntlock.Lock();
		ret = AxmMoveStop(m_lAxis, GetNormDec());
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		m_MMC_Err = FAS_MoveStop((BYTE)m_iPortNo, (BYTE)m_lAxis);
		if (m_MMC_Err == FMM_OK)
			return TRUE;
		else
			return FALSE;
	}
#endif
#ifdef _USE_DNET
	int nRet = 0;
	if (GetControlType() == _DNET)
	{
		g_AxisSyncIntlock.Lock();
		nRet = dnetQStop(m_iComPort, m_lAxis);	
		g_AxisSyncIntlock.Unlock();

		if (nRet != DNET_ERR_NO) 
		{
			return FALSE;
		}
	}
#endif
#ifdef _USE_NMC
	if (GetControlType() == _NMC)
	{
		m_lRet = MC_Stop(m_iPortNo, (int)m_iAxisID, false, (GetNormVel()*(1000.0f/(4.0f*(INT)(GetNormDec())))), 0);
		if (m_lRet == MC_OK)
			return TRUE;
		else
			return FALSE;		

	}
#endif
	return TRUE;
}

BOOL CAxis::MoveEStop()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = set_e_stop((int)m_lAxis);
		
		ClearState();
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret;
		g_AxisSyncIntlock.Lock();
		ret = AxmMoveEStop(m_lAxis);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		m_MMC_Err = FAS_EmergencyStop((BYTE)m_iPortNo, (BYTE)m_lAxis);
		if (m_MMC_Err == FMM_OK)
			return TRUE;
		else
			return FALSE;
	}
#endif
#ifdef _USE_DNET
	int nRet = 0;
	if (GetControlType() == _DNET)
	{
		g_AxisSyncIntlock.Lock();
		nRet = dnetQStop(m_iComPort, m_lAxis);	
		g_AxisSyncIntlock.Unlock();

		if (nRet != DNET_ERR_NO) 
		{
			return FALSE;
		}
	}
#endif
#ifdef _USE_NMC
	if (GetControlType() == _NMC)
	{
		m_lRet = MC_Stop(m_iPortNo, (int)m_iAxisID, false, (GetNormVel()*(1000.0f/(4.0f*(INT)(GetNormDec())))), 0);
		if (m_lRet == MC_OK)
			return TRUE;
		else
			return FALSE;		

	}
#endif
	return TRUE;
}

BOOL CAxis::AmpEnable()
{

#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		AmpDisable();
		Delay(100,TRUE);
		m_MMC_Err = set_amp_enable((int)m_lAxis, TRUE);
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret;
		g_AxisSyncIntlock.Lock();
		ret = AxmSignalServoOn(m_lAxis, ENABLE);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{	
		//통신 연결 안되어 있으면 한번 시도한다. 
		if(!GetConnectState())
		{
			if(!bConnectFastechDriver())
				return FALSE;
		}
		m_MMC_Err = FAS_ServoEnable((BYTE)m_iPortNo, (BYTE)m_lAxis,TRUE);
		if(m_MMC_Err != FMM_OK)
			return FALSE;
		else
			return TRUE;
	}
#endif
#ifdef _USE_DNET
	int nRet = 0;

	if (GetControlType() == _DNET)
	{
		g_AxisSyncIntlock.Lock();
		nRet = dnetEnable(m_iComPort, m_lAxis, DNET_MC_OPMODE_PPM);
		g_AxisSyncIntlock.Unlock();
		if (nRet != DNET_ERR_NO) return FALSE;
	}
#endif
#ifdef _USE_NMC
	if(GetControlType() == _NMC)
	{
		MC_Reset(m_iPortNo,(int)m_iAxisID);
		m_lRet = MC_Power(m_iPortNo,(int)m_iAxisID,ENABLE);
		if(m_lRet == MC_OK)
			return TRUE;
		else
			return FALSE;
	}
#endif
	return TRUE;
}

BOOL CAxis::AmpDisable()
{

#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif
	
#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = set_amp_enable((int)m_lAxis, FALSE);
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL  
	if(GetControlType() == _AJINXL)
	{   
		DWORD ret;
		g_AxisSyncIntlock.Lock();
		ret = AxmSignalServoOn(m_lAxis, DISABLE);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
		{
			m_bOriginReturnFlag = FALSE;//JMHWANG 130915 
			return TRUE;
		}
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		
		m_MMC_Err = FAS_ServoEnable((BYTE)m_iPortNo, (BYTE)m_lAxis,FALSE);
		if(m_MMC_Err != FMM_OK)
			return FALSE;
		else
		{
			SetOriginReturn(FALSE);//JMHWANG 150531
			return TRUE;
		}
	}
#endif
#ifdef _USE_DNET
	int count = 0;
	int nRet = 0;

	if (GetControlType() == _DNET)
	{
		g_AxisSyncIntlock.Lock();
		nRet = dnetDisable(m_iComPort, m_lAxis);
		g_AxisSyncIntlock.Unlock();
		if (nRet != DNET_ERR_NO) return FALSE;
	}
#endif
#ifdef _USE_NMC
	if(GetControlType() == _NMC)
	{
		m_lRet = MC_Power(m_iPortNo,(int) m_iAxisID,DISABLE);
		if(m_lRet == MC_OK)
			return TRUE;
		else
			return FALSE;
	}
#endif
	return TRUE;
}

BOOL CAxis::GetAmpState(long State)
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif
	//20130901 lee
	if(GetAxisUse() == NOT_USE) return TRUE;

#ifdef _USE_MMC
	short m(0);
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = get_amp_enable((int)m_lAxis, &m);		
		if (m == ENABLE)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD Status;
		g_AxisSyncIntlock.Lock();
		AxmSignalIsServoOn(m_lAxis, &Status);
		g_AxisSyncIntlock.Unlock();

		if (Status == ENABLE)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		DWORD dwAxisStatus=0;
		EZISERVO_AXISSTATUS state;
		
		m_MMC_Err = FAS_GetAxisStatus((BYTE)m_iPortNo, (BYTE)m_lAxis, &dwAxisStatus);

		state.dwValue = dwAxisStatus;

		if(m_MMC_Err == FMM_OK)
		{
//			if(state.FFLAG_SERVOON == 1) 
			if(dwAxisStatus&0x00100000) 
				return TRUE;
			else 
				return FALSE;
		}
		else
			return FALSE;		
	}
#endif
#ifdef _USE_DNET
	//Need2Check
#endif
	return TRUE;
}
//Sensor Event Set
bool CAxis::SetPositiveLimit(int act)
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = set_positive_limit((int)m_lAxis, act);
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret; 
		DWORD StopMode, PositiveLevel, NegativeLevel;	
		g_AxisSyncIntlock.Lock();			
		AxmSignalGetLimit (m_lAxis, &StopMode, &PositiveLevel, &NegativeLevel);
		
		if(act == STOP_EVENT)	
			ret = AxmSignalSetLimit (m_lAxis, SLOWDOWN_STOP, PositiveLevel, NegativeLevel);
		else if(act == E_STOP_EVENT || act == ABORT_EVENT)
			ret = AxmSignalSetLimit (m_lAxis, EMERGENCY_STOP, PositiveLevel, NegativeLevel);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{	
		return TRUE;
	}
#endif
#ifdef _USE_DNET
	//Need2Check
#endif
#ifdef _USE_NMC
	//Need2Check
#endif
	return TRUE;
}
//Sensor Event Set
bool CAxis::SetNegativeLimit(int act)
{
#ifdef NOT_MACHINE 
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif
	
#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = set_negative_limit((int)m_lAxis, act);
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret; 
		DWORD StopMode, PositiveLevel, NegativeLevel;	
		g_AxisSyncIntlock.Lock();			
		AxmSignalGetLimit (m_lAxis, &StopMode, &PositiveLevel, &NegativeLevel);
		
		if(act == STOP_EVENT)	
			ret = AxmSignalSetLimit (m_lAxis, SLOWDOWN_STOP, PositiveLevel, NegativeLevel);
		else if(act == E_STOP_EVENT || act == ABORT_EVENT)
			ret = AxmSignalSetLimit (m_lAxis, EMERGENCY_STOP, PositiveLevel, NegativeLevel);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{	
		return TRUE;
	}
#endif
#ifdef _USE_DNET
	int nRet = 0;
	if (GetControlType() == _DNET)
	{
		if(act == DNET_HOME_METHOD_NEGATIVE)	
		{
			nRet = dnetMoveToNegativeLimit(m_iComPort, m_lAxis);
			if (nRet != DNET_ERR_NO) 
			{
				return FALSE;
			}
		}
		else if(act == DNET_HOME_METHOD_POSITIVE)
		{
			nRet = dnetMoveToPositiveLimit(m_iComPort, m_lAxis);	
			if (nRet != DNET_ERR_NO) 
			{
				return FALSE;
			}
		}
		else if(act == DNET_HOME_METHOD_NEGATIVE_HOME)
		{
			nRet = dnetMoveToNegativeHome(m_iComPort, m_lAxis);
			if (nRet != DNET_ERR_NO) 
			{
				return FALSE;
			}
		}
		else if(act == DNET_HOME_METHOD_POSITIVE_HOME)
		{
			nRet = dnetMoveToPositiveHome(m_iComPort, m_lAxis);
			if (nRet != DNET_ERR_NO) 
			{
				return FALSE;
			}
		}
		else if(act == STOP_EVENT)
		{
			nRet = dnetQStop(m_iComPort, m_lAxis);
			if (nRet != DNET_ERR_NO) 
			{
				return FALSE;
			}
		}
		if (nRet != DNET_ERR_NO) return FALSE;
		g_AxisSyncIntlock.Unlock();
	}

#endif
#ifdef _USE_NMC
	//Need2Check
#endif
	return TRUE;
}

//Sensor Event Set
//Need2Check
bool CAxis::SetHome(int act)
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = set_home((int)m_lAxis, act);
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		return TRUE;
	}
#endif
	return TRUE;
}


BOOL CAxis::GetPosLimitSensor()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		if (pos_switch((int)m_lAxis) == 1)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD upPositiveLevel, upNegativeLevel;
		g_AxisSyncIntlock.Lock();
		AxmSignalReadLimit (m_lAxis, &upPositiveLevel, &upNegativeLevel);
		g_AxisSyncIntlock.Unlock();
		
		if (upPositiveLevel)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
//		DWORD dwInput=0;
		ULONGLONG dwInput=0;
		
		g_AxisSyncIntlock.Lock();
		m_MMC_Err = FAS_GetIOInput((BYTE)m_iPortNo, (BYTE)m_lAxis, &dwInput);
		g_AxisSyncIntlock.Unlock();
		if(m_MMC_Err == FMM_OK)
		{
			if(dwInput & SERVO_IN_BITMASK_LIMITP)
				return TRUE;
			else
				return FALSE;
		}
		else
			return FALSE;
	}
#endif
#ifdef _USE_DNET
	int nRet = 0;
	if (GetControlType() == _DNET)
	{
		g_AxisSyncIntlock.Lock();
		nRet = dnetIsSensingDInputPositive(m_iComPort, m_lAxis);
		g_AxisSyncIntlock.Unlock();
		if (nRet == DNET_RETURN_TRUE)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_NMC
	if(GetControlType() == _NMC)
	{

		UINT32 pAxisInfo;
		m_lRet = MC_ReadAxisStatus(m_iPortNo, (int)m_iAxisID, &pAxisInfo);
		if (pAxisInfo & mcLimitSwitchNeg)
			return TRUE;
		else
			return FALSE;	
	}
#endif
	return TRUE;
}

BOOL CAxis::GetNegLimitSensor()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		if (neg_switch((int)m_lAxis) == 1)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD upPositiveLevel, upNegativeLevel;
		g_AxisSyncIntlock.Lock();
		AxmSignalReadLimit (m_lAxis, &upPositiveLevel, &upNegativeLevel);
		g_AxisSyncIntlock.Unlock();
		
		if (upNegativeLevel)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		//DWORD dwInput;
		ULONGLONG dwInput;
		
		g_AxisSyncIntlock.Lock();
		m_MMC_Err = FAS_GetIOInput((BYTE)m_iPortNo, (BYTE)m_lAxis, &dwInput);
		g_AxisSyncIntlock.Unlock();
		if(m_MMC_Err == FMM_OK)
		{
			if(dwInput & SERVO_IN_BITMASK_LIMITN)
				return TRUE;
			else
				return FALSE;
		}
		else
			return FALSE;
	}
#endif
#ifdef _USE_DNET
	int nRet = 0;
	if (GetControlType() == _DNET)
	{
		nRet = dnetIsSensingDInputNegative(m_iComPort, m_lAxis);
		if (nRet == DNET_RETURN_TRUE)
			return TRUE;
		else
			return FALSE;
	}


#endif
#ifdef _USE_NMC
	if(GetControlType() == _NMC)
	{
		UINT32 pAxisInfo;
		m_lRet = MC_ReadAxisStatus(m_iPortNo,(int)m_iAxisID,&pAxisInfo);

		if (pAxisInfo & mcLimitSwitchNeg)
			return TRUE;
		else
			return FALSE;		
	}
#endif
	return TRUE;
}

bool CAxis::GetHomeSensor()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		if (home_switch((int)m_lAxis) == 1)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD upStatus; 
		g_AxisSyncIntlock.Lock();		
		AxmHomeReadSignal (m_lAxis, &upStatus);
		g_AxisSyncIntlock.Unlock();
		
		if (upStatus)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		//DWORD dwInput;
		ULONGLONG dwInput;
		
		g_AxisSyncIntlock.Lock();
		m_MMC_Err = FAS_GetIOInput((BYTE)m_iPortNo, (BYTE)m_lAxis, &dwInput);
		g_AxisSyncIntlock.Unlock();
		if(m_MMC_Err == FMM_OK)
		{
			if(dwInput & SERVO_IN_BITMASK_ORIGIN)
				return TRUE;
			else
				return FALSE;
		}
		else
		{
			return FALSE;
		}		
	}
#endif
#ifdef _USE_DNET
	int nRet = 0;
	if (GetControlType() == _DNET)
	{
		nRet = dnetIsSensingDInputHome(m_iComPort, m_lAxis);
		if (nRet == DNET_RETURN_TRUE)
			return TRUE;
		else
			return FALSE;
	}


#endif
#ifdef _USE_NMC
	if(GetControlType() == _NMC)
	{
		UINT32 pAxisInfo;
		m_lRet = MC_ReadAxisStatus(m_iPortNo, (int)m_iAxisID, &pAxisInfo);
		if (pAxisInfo & mcHomeAbsSwitch)
			return TRUE;
		else
			return FALSE;		
	}
#endif
	return TRUE;
}

// level : 0 => active low, else active high 
BOOL CAxis::SetHomeLevel(long level)
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = set_home_level((int)m_lAxis, (int)level);
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret; 
		g_AxisSyncIntlock.Lock();		
		ret = AxmHomeSetSignalLevel(m_lAxis, level);
		g_AxisSyncIntlock.Unlock();
		
		if(ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		BYTE PinNo; 
		PinNo = 2;

		//m_MMC_Err = FAS_SetIOAssignMap((BYTE)m_iPortNo, (BYTE)m_lAxis,PinNo,SERVO_IN_BITMASK_ORIGIN,(BYTE)level);
		m_MMC_Err = FAS_SetInputAssignMap((BYTE)m_iPortNo, (BYTE)m_lAxis,PinNo,SERVO_IN_BITMASK_ORIGIN,(BYTE)level);
		if(m_MMC_Err == FMM_OK)
			return TRUE;
		else
			return FALSE;
	}
#endif
#ifdef _USE_DNET
	//Need2Check
#endif
#ifdef _USE_NMC
	//Need2Check
#endif
	return TRUE;
}

// level : 0 => active low, else active high 
BOOL CAxis::SetNegLimitLevel(long level)
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif


#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = set_negative_level((int)m_lAxis, (int)level);
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret; 
		DWORD StopMode, PositiveLevel, NegativeLevel;	
		g_AxisSyncIntlock.Lock();			
		AxmSignalGetLimit (m_lAxis, &StopMode, &PositiveLevel, &NegativeLevel);
		ret = AxmSignalSetLimit (m_lAxis, StopMode, level, level);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		BYTE PinNo; 
		
		PinNo = 1;
		
		//m_MMC_Err = FAS_SetIOAssignMap((BYTE)m_iPortNo, (BYTE)m_lAxis,PinNo,SERVO_IN_BITMASK_LIMITN,(BYTE)level);
		m_MMC_Err = FAS_SetInputAssignMap((BYTE)m_iPortNo, (BYTE)m_lAxis,PinNo,SERVO_IN_BITMASK_LIMITN,(BYTE)level);
		if(m_MMC_Err == FMM_OK)
			return TRUE;
		else
			return FALSE;
	}
#endif
	return TRUE;
}

// level : 0 => active low, else active high 
BOOL CAxis::SetPosLimitLevel(long level)
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = set_positive_level((int)m_lAxis, (int)level);
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret;
		DWORD StopMode, PositiveLevel, NegativeLevel;	
		g_AxisSyncIntlock.Lock();			
		AxmSignalGetLimit (m_lAxis, &StopMode, &PositiveLevel, &NegativeLevel);
		ret = AxmSignalSetLimit (m_lAxis, StopMode, level, level);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		BYTE PinNo; 
		
		PinNo = 0;
		
		//m_MMC_Err = FAS_SetIOAssignMap((BYTE)m_iPortNo, (BYTE)m_lAxis, PinNo,SERVO_IN_BITMASK_LIMITP,(BYTE)level);
		m_MMC_Err = FAS_SetInputAssignMap((BYTE)m_iPortNo, (BYTE)m_lAxis, PinNo,SERVO_IN_BITMASK_LIMITP,(BYTE)level);
		if(m_MMC_Err == FMM_OK)
			return TRUE;
		else
			return FALSE;
		
	}
#endif
#ifdef _USE_DNET
	//Need2Check
#endif
	return TRUE;
}

// level : 0 => active low, else active high 
BOOL CAxis::SetAmpFaultLevel(long level)
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = set_amp_fault_level((int)m_lAxis, (int)level);
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret;
		g_AxisSyncIntlock.Lock();			
		ret = AxmSignalSetServoAlarm (m_lAxis, level);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		BYTE PinNo; 
		PinNo = 0; //초기0번 출력을 Alarm으로 한다. 

		//m_MMC_Err = FAS_SetIOAssignMap((BYTE)m_iPortNo, (BYTE)m_lAxis, 12 + PinNo, SERVO_OUT_BITMASK_ALARM,(BYTE)level);
		m_MMC_Err = FAS_SetInputAssignMap((BYTE)m_iPortNo, (BYTE)m_lAxis, 12 + PinNo, SERVO_OUT_BITMASK_ALARM,(BYTE)level);
		if(m_MMC_Err == FMM_OK)
			return TRUE;
		else
			return FALSE;
	}
#endif
#ifdef _USE_DNET
	//Need2Check

#endif
	return TRUE;
}

BOOL CAxis::SaveAxisConfig() //130219 JSPark
{
	CProfileDataIF cDataIf;
	cDataIf.SetFilePath(DATA_AXIS_CONFIG_FILE);

	CString strNode;
	strNode = m_strAxisName + _T("_AXIS");

	double ACC_DEC_RATIO;
	if(m_iControlType == _MMC) 
		ACC_DEC_RATIO = 10.0;
	else if(m_iControlType == _AJINXL)  
		ACC_DEC_RATIO = 1000.0; //뒤에서 계산 한다. 

	int msec;
	if(m_iControlType == _MMC) 
		GetEStopRate(msec);
	else                       
		msec = 0;

	cDataIf.SetInt(strNode,_T("Event E-Stop Rate"), msec);
	cDataIf.SetDouble(strNode,_T("Normal Velocity"), m_NormVel);
	cDataIf.SetDouble(strNode,_T("Normal Accel Time"), GetNormAcc() * ACC_DEC_RATIO);
	cDataIf.SetDouble(strNode,_T("Normal Decel Time"), GetNormDec() * ACC_DEC_RATIO);
	cDataIf.SetDouble(strNode,_T("Home Velocity"), m_HomeVel);
	cDataIf.SetDouble(strNode,_T("Home Accel Time"), GetHomeAcc() * ACC_DEC_RATIO);
	cDataIf.SetDouble(strNode,_T("Home Decel Time"), GetHomeDec() * ACC_DEC_RATIO);

	cDataIf.SetDouble(strNode,_T("Home Fine Velocity"), m_HomeFineVel);
	cDataIf.SetDouble(strNode,_T("Home Fine Accel Time"), GetHomeFineAcc() * ACC_DEC_RATIO);

	cDataIf.SetDouble(strNode,_T("Jog Velocity"), m_JogVel);
	cDataIf.SetDouble(strNode,_T("Jog Accel Time"), GetJogAcc() * ACC_DEC_RATIO);
	cDataIf.SetDouble(strNode,_T("Jog Decel Time"), GetJogDec() * ACC_DEC_RATIO);

	cDataIf.SetDouble(strNode,_T("Normal Max Velocity"), m_dMaxVel);
	cDataIf.SetDouble(strNode,_T("Normal Min Velocity"), m_dMinVel);
	cDataIf.SetDouble(strNode,_T("Normal Max Accel Time"), m_dMaxAccTime * ACC_DEC_RATIO);
	cDataIf.SetDouble(strNode,_T("Normal Min Accel Time"), m_dMinAccTime * ACC_DEC_RATIO);

	cDataIf.SetDouble(strNode,_T("Home Max Velocity"), m_dMaxHomeVel);
	cDataIf.SetDouble(strNode,_T("Home Min Velocity"), m_dMinHomeVel);
	cDataIf.SetDouble(strNode,_T("Home Max Accel Time"), m_dMaxHomeAccTime * ACC_DEC_RATIO);
	cDataIf.SetDouble(strNode,_T("Home Min Accel Time"), m_dMinHomeAccTime * ACC_DEC_RATIO);

	cDataIf.SetDouble(strNode,_T("Jog Max Velocity"), m_dMaxJogVel);
	cDataIf.SetDouble(strNode,_T("Jog Min Velocity"), m_dMinJogVel);
	cDataIf.SetDouble(strNode,_T("Jog Max Accel Time"), m_dMaxJogAccTime * ACC_DEC_RATIO);
	cDataIf.SetDouble(strNode,_T("Jog Min Accel Time"), m_dMinJogAccTime * ACC_DEC_RATIO);

	cDataIf.SetDouble(strNode,_T("Positive SW Limit Pos"), m_dPosSWLimit);
	cDataIf.SetDouble(strNode,_T("Negative SW Limit Pos"), m_dNegSWLimit);
	cDataIf.SetDouble(strNode,_T("Axis Orgin Offset"), m_dOrgOffset);
	//추가
	cDataIf.SetDouble(strNode,_T("Axis Init Pos"), m_dAxisInitPos);
	cDataIf.SetDouble(strNode,_T("Axis Safety Pos"), m_dAxisSafetyPos);
	cDataIf.SetDouble(strNode,_T("Axis Orgin Offset"), m_dOrgOffset);
	
	return TRUE;
}

BOOL CAxis::LoadAxisConfig() //130219 JSPark
{
	CProfileDataIF cDataIf;
	cDataIf.SetFilePath(DATA_AXIS_CONFIG_FILE);
	CString strNode;

	double ACC_DEC_RATIO(1.0);
	
	strNode = m_strAxisName + _T("_AXIS");

	if(m_iControlType == _MMC) ACC_DEC_RATIO = 10.0;
	else if(m_iControlType == _AJINXL)  ACC_DEC_RATIO = 0.001; //뒤에서 계산 한다. 
	// Event Stop Rate
	//m_lStopDec = cDataIf.GetInt(strNode, _T("Event Stop Rate"), 100); 
	m_NormVel = cDataIf.GetDouble(strNode, _T("Normal Velocity"), 1000.0);
	m_NormAcc = cDataIf.GetDouble(strNode, _T("Normal Accel Time"), 50) * ACC_DEC_RATIO;
	m_NormDec = cDataIf.GetDouble(strNode, _T("Normal Decel Time"), 50) * ACC_DEC_RATIO;
	m_HomeVel = cDataIf.GetDouble(strNode, _T("Home Velocity"), 100.0);
	m_HomeAcc = cDataIf.GetDouble(strNode, _T("Home Accel Time"), 50) * ACC_DEC_RATIO;
	m_HomeDec = cDataIf.GetDouble(strNode, _T("Home Decel Time"), 50) * ACC_DEC_RATIO;
	m_HomeFineVel = cDataIf.GetDouble(strNode, _T("Home Fine Velocity"), 100.0);
	m_HomeFineAcc = cDataIf.GetDouble(strNode, _T("Home Fine Accel Time"), 50) * ACC_DEC_RATIO;
	m_JogVel = cDataIf.GetDouble(strNode, _T("Jog Velocity"), 1000.0);
	m_JogAcc = cDataIf.GetDouble(strNode, _T("Jog Accel Time"), 50) * ACC_DEC_RATIO;
	m_JogDec = cDataIf.GetDouble(strNode, _T("Jog Decel Time"), 50) * ACC_DEC_RATIO;
	m_dMaxVel = cDataIf.GetDouble(strNode, _T("Normal Max Velocity"), 800000.0);
	m_dMinVel = cDataIf.GetDouble(strNode, _T("Normal Min Velocity"), 1.0);
	m_dMaxAccTime = cDataIf.GetDouble(strNode, _T("Normal Max Accel Time"), 2000) * ACC_DEC_RATIO;
	m_dMinAccTime = cDataIf.GetDouble(strNode, _T("Normal Min Accel Time"), 20) * ACC_DEC_RATIO;
	m_dMaxHomeVel = cDataIf.GetDouble(strNode, _T("Home Max Velocity"), 100000.0);
	m_dMinHomeVel = cDataIf.GetDouble(strNode, _T("Home Min Velocity"), 1.0);
	m_dMaxHomeAccTime = cDataIf.GetDouble(strNode, _T("Home Max Accel Time"), 2000) * ACC_DEC_RATIO;
	m_dMinHomeAccTime = cDataIf.GetDouble(strNode, _T("Home Min Accel Time"), 20) * ACC_DEC_RATIO;
	m_dMaxJogVel = cDataIf.GetDouble(strNode, _T("Jog Max Velocity"), 200000.0);
	m_dMinJogVel = cDataIf.GetDouble(strNode, _T("Jog Min Velocity"), 1.0);
	m_dMaxJogAccTime = cDataIf.GetDouble(strNode, _T("Jog Max Accel Time"), 2000) * ACC_DEC_RATIO;
	m_dMinJogAccTime = cDataIf.GetDouble(strNode, _T("Jog Min Accel Time"), 20) * ACC_DEC_RATIO;
	m_dPosSWLimit = cDataIf.GetDouble(strNode, _T("Positive SW Limit Pos"), 10000.0);
	m_dNegSWLimit = cDataIf.GetDouble(strNode, _T("Negative SW Limit Pos"), -10000.0);
	//추가 
	m_dAxisInitPos = cDataIf.GetDouble(strNode, _T("Axis Init Pos"), 0.0);	
	m_dAxisSafetyPos = cDataIf.GetDouble(strNode, _T("Axis Safety Pos"), 0.0);
	m_dOrgOffset = cDataIf.GetDouble(strNode, _T("Axis Orgin Offset"), 0.0);
	int laxis = GetAxisNo();
	return TRUE;
}

BOOL CAxis::GetAmpFaultLevel(long &level)
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif
//>> 130220 JSPark
	//축 사용 유무 설정에 따른 Amp Fault 상태 Check
	if(GetAxisUse() == AXIS_NOT_USE)
		return TRUE;
//<<
	short m(0);

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = get_amp_reset_level((int)m_lAxis, &m);
		level = (long) m;
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret;
		DWORD upStatus;
		g_AxisSyncIntlock.Lock();			
		ret = AxmSignalGetServoAlarm (m_lAxis, &upStatus);
		g_AxisSyncIntlock.Unlock();
		
		level = upStatus;

		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		BYTE PinNo; 
		BYTE Level;
		BYTE LogicNo;
		PinNo = 0; //초기0번 출력을 Alarm으로 한다. 
		
		//m_MMC_Err = FAS_GetIOAssignMap((BYTE)m_iPortNo, (BYTE)m_lAxis, 12 + PinNo, &LogicNo,&Level);
		m_MMC_Err = FAS_GetInputAssignMap((BYTE)m_iPortNo, (BYTE)m_lAxis, 12 + PinNo, &LogicNo,&Level);
		if(m_MMC_Err == FMM_OK)
		{
			level = Level;
			return TRUE;
		}
		else
		{
			level = FALSE;
			return FALSE;		
		}
	}
#endif
#ifdef _USE_DNET
	//Need2Check

#endif
	return TRUE;
}

BOOL CAxis::GetPosLimitLevel(long &level)
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

	short l(0);

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = get_positive_level((int)m_lAxis, &l);
		level = (long) l;
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret; 
		DWORD StopMode, PositiveLevel, NegativeLevel;	
		g_AxisSyncIntlock.Lock();			
		ret = AxmSignalGetLimit (m_lAxis, &StopMode, &PositiveLevel, &NegativeLevel);
		g_AxisSyncIntlock.Unlock();
		level = PositiveLevel;
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		BYTE PinNo; 
		BYTE Level;
		BYTE LogicNo;
		PinNo = 0; //Positive Sensor
		
		//m_MMC_Err = FAS_GetIOAssignMap((BYTE)m_iPortNo, (BYTE)m_lAxis, PinNo, &LogicNo,&Level);
		m_MMC_Err = FAS_GetInputAssignMap((BYTE)m_iPortNo, (BYTE)m_lAxis, PinNo, &LogicNo,&Level);
		if(m_MMC_Err == FMM_OK)
		{
			level = Level;
			return TRUE;
		}
		else
		{
			level = FALSE;
			return FALSE;		
		}		
	}
#endif
#ifdef _USE_DNET
	//Need2Check
#endif
	return true;
}

BOOL CAxis::GetNegLimitLevel(long &level)
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

	short l(0);

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = get_negative_level((int)m_lAxis, &l);
		level = (long) l;
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret; 
		DWORD StopMode, PositiveLevel, NegativeLevel;	
		g_AxisSyncIntlock.Lock();			
		ret = AxmSignalGetLimit (m_lAxis, &StopMode, &PositiveLevel, &NegativeLevel);
		g_AxisSyncIntlock.Unlock();
		level = NegativeLevel;
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		BYTE PinNo; 
		BYTE Level;
		BYTE LogicNo;
		PinNo = 1; //Positive Sensor
		
		//m_MMC_Err = FAS_GetIOAssignMap((BYTE)m_iPortNo, (BYTE)m_lAxis, PinNo, &LogicNo,&Level);
		m_MMC_Err = FAS_GetInputAssignMap((BYTE)m_iPortNo, (BYTE)m_lAxis, PinNo, &LogicNo,&Level);
		if(m_MMC_Err == FMM_OK)
		{
			level = Level;
			return TRUE;
		}
		else
		{
			level = FALSE;
			return FALSE;		
		}		
	}
#endif
#ifdef _USE_DNET
	//Need2Check
#endif
	return TRUE;
}

BOOL CAxis::GetHomeLevel(long &level)
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

	short l(0);

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = get_home_level((int)m_lAxis, &l);
		level = (long) l;
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret;
		DWORD upStatus;
		g_AxisSyncIntlock.Lock();			
		ret = AxmHomeGetSignalLevel (m_lAxis, &upStatus);
		g_AxisSyncIntlock.Unlock();
		level = upStatus;
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		BYTE PinNo; 
		BYTE Level;
		BYTE LogicNo;
		PinNo = 2; //Positive Sensor
		
		//m_MMC_Err = FAS_GetIOAssignMap((BYTE)m_iPortNo, (BYTE)m_lAxis, PinNo, &LogicNo,&Level);
		m_MMC_Err = FAS_GetInputAssignMap((BYTE)m_iPortNo, (BYTE)m_lAxis, PinNo, &LogicNo,&Level);
		if(m_MMC_Err == FMM_OK)
		{
			level = Level;
			return TRUE;
		}
		else
		{
			level = FALSE;
			return FALSE;		
		}		
	}
#endif
#ifdef _USE_DNET
	//Need2Check
#endif
	return TRUE;
}

BOOL CAxis::SetStopRate()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = fset_stop_rate((int)m_lAxis, (int)GetParasStopDec());
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		// Ajin B/D 는 AxmMoveSignalSearch를 사용해서 Sensor를 찾는다. 
		// -방향으로 이동하면서 홈센서의 Up Edge를 찾는다. 
		// long lAxisNo = 0; 
		// double dMaxVelocity = -100; 
		// double dMaxAccel = 200;
		// long lDetectSignal = HomeSensor;
		// long lSignalEdge = UP_EDGE;
		// long lSignalMethod = 0; // [0]감속정지, [1]급정지
		// AxmMoveSignalSearch(lAxisNo, dMaxVelocity, dMaxAccel, lDetectSignal, lSignalEdge, lSignalMethod);
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		return TRUE;
	}
#endif
#ifdef _USE_DNET
	//Need2Check
#endif
	return TRUE;
}

BOOL CAxis::SetEStopRate()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = fset_e_stop_rate((int)m_lAxis, (int)GetParasEStopDec());
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		// Ajin B/D 는 AxmMoveSignalSearch를 사용해서 Sensor를 찾는다. 
		// -방향으로 이동하면서 홈센서의 Up Edge를 찾는다. 
		// long lAxisNo = 0; 
		// double dMaxVelocity = -100; 
		// double dMaxAccel = 200;
		// long lDetectSignal = HomeSensor;
		// long lSignalEdge = UP_EDGE;
		// long lSignalMethod = 0; // [0]감속정지, [1]급정지
		// AxmMoveSignalSearch(lAxisNo, dMaxVelocity, dMaxAccel, lDetectSignal, lSignalEdge, lSignalMethod);
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		return TRUE;		
	}
#endif
#ifdef _USE_DNET
	//Need2Check
#endif
#ifdef _USE_NMC
	//Need2Check
#endif
	return TRUE;
}

BOOL CAxis::GetStopRate(int &msec)
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

	short m(0);

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = get_stop_rate((int)m_lAxis, &m);
		msec = (int) m;
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		msec = GetParasStopDec();
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		msec = 0;
		return TRUE;	
	}
#endif
#ifdef _USE_DNET
	//Need2Check
#endif
	return TRUE;
}

BOOL CAxis::GetEStopRate(int &msec)
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

	short m(0);

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = get_e_stop_rate((int)m_lAxis, &m);
		msec = (int) m;
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		msec = 0;
		return TRUE;			
	}
#endif
	return TRUE;
}




int CAxis::GetPosition(double &rPos, bool bEncoder) //130228 JSPark
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif
	int ret(0);

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		if(bEncoder)
			ret = get_position((int)m_lAxis, &rPos);
		else
			ret = get_counter((int)m_lAxis, &rPos); 
		rPos *= GetMm4Pulse();
		if (ret == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		g_AxisSyncIntlock.Lock();
		if(bEncoder)
			ret = AxmStatusGetActPos(m_lAxis, &rPos);
		else
			ret = AxmStatusGetCmdPos (m_lAxis, &rPos);
		rPos *= GetMm4Pulse();
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		long lValue;
	
		if(!GetConnectState())
		{
			rPos = 0.0;
			return FALSE;
		}

		if(bEncoder)
			ret = FAS_GetActualPos((BYTE)m_iPortNo, (BYTE)m_lAxis,&lValue);
		else
			ret = FAS_GetCommandPos((BYTE)m_iPortNo, (BYTE)m_lAxis,&lValue);

		rPos = lValue * GetMm4Pulse();
		if(ret == FMM_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_DNET
	LONG32 lValue;
	if (GetControlType() == _DNET)
	{
		g_AxisSyncIntlock.Lock();
		ret = dnetGetPositionActual(m_iComPort, m_lAxis, &lValue);
		//<<GSM
		//rPos = lValue * GetMm4Pulse();
		rPos = lValue / GetPulse4Mm();
		//<<
		g_AxisSyncIntlock.Unlock();

		if (ret != DNET_ERR_NO)
		{
			return FALSE;
		}
		else
		{
			return FALSE;
		}
	}

#endif
#ifdef _USE_NMC
	if(GetControlType() == _NMC)
	{
		ret = MC_ReadActualPosition(m_iPortNo, (int)m_iAxisID, &rPos);
		rPos *= GetMm4Pulse();

		if (ret == MC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
	return TRUE;
}

void CAxis::SetHomeFinished(BOOL bFinished)
{
	SetOriginReturn(bFinished);
}

BOOL CAxis::CheckHomingDone()
{
	if(!IsHomeFinished())
		return FALSE;

	return TRUE;
}

BOOL CAxis::CheckAmpFault()
{
	if(GetAmpFaultState())		return TRUE;
	else						return FALSE;
}

bool CAxis::SetStepMode()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

	int ret(0);

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		ret = fset_step_mode((int)m_lAxis, GetParasStepMode());
		
		if (ret == MMC_OK)
			return true;
		else
			return false;		
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		return true;
	}
#endif
#ifdef _USE_NMC
	//Need2Check
#endif
	return true;
}

bool CAxis::SetAmpEnableLevel()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

	int ret(0);

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		ret = fset_amp_enable_level((int)m_lAxis, GetParasAmpEnableLevel());
		
		if (ret == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret;
		g_AxisSyncIntlock.Lock();			
		ret = AxmSignalSetServoOnLevel (m_lAxis, GetParasAmpEnableLevel());
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		m_MMC_Err = FAS_SetParameter((BYTE)m_iPortNo,(BYTE)m_lAxis,SERVO_SERVOONLOGIC,(long)GetParasAmpEnableLevel());
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_DNET
	//Need2Check
#endif
#ifdef _USE_NMC
	//Need2Check
#endif
	return true;
}

void CAxis::SetAxisAccelUnit()
{
#ifdef _USE_AJINXL
	AxmMotSetAccelUnit(m_lAxis,1);
	AxmMotSetMinVel(m_lAxis,1);
#endif
}


bool CAxis::SetAmpFaultResetLevel()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = fset_amp_reset_level((int)m_lAxis, GetParasAmpFaultResetLevel());
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret;
		DWORD AmpFaultResetLevel;
		g_AxisSyncIntlock.Lock();			
		AmpFaultResetLevel = GetParasAmpFaultResetLevel();
		ret = AxmSignalSetServoAlarmResetLevel (m_lAxis, AmpFaultResetLevel);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		m_MMC_Err = FAS_SetParameter((BYTE)m_iPortNo,(BYTE)m_lAxis,SERVO_SERVORESETLOGIC,(long)GetParasAmpFaultResetLevel());
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_DNET
	//Need2Check
#endif
#ifdef _USE_NMC
	//Need2Check
#endif
	return true;
}

bool CAxis::SetInPositionData()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = fset_in_position((int)m_lAxis, (double) GetParasInPosition());
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		
		m_MMC_Err = FAS_SetParameter((BYTE)m_iPortNo,(BYTE)m_lAxis,SERVO_INPOSITIONVALUE,(long)GetParasInPosition());
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
	return true;
}

bool CAxis::SetDirection()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = fset_coordinate_direction((int)m_lAxis, GetParasCordinateDirection());
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{

		//모터 방향 바뀌는지 확인 하자!~
		DWORD ret;
		g_AxisSyncIntlock.Lock();			
		ret = AxmMotSetPulseOutMethod (m_lAxis, GetParasPulseMethod()); //20130330 lee
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		m_MMC_Err = FAS_SetParameter((BYTE)m_iPortNo,(BYTE)m_lAxis,SERVO_MOTIONDIR,(long)GetParasCordinateDirection());
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_DNET

// 	int nRet = 0;
// 	int count = 0;
// 	if (GetControlType() == _DNET)
// 	{
// 		g_AxisSyncIntlock.Lock();
// 
// 		nRet = dnetReversedDirection(m_iComPort, m_lAxis,GetParasCordinateDirection());
// 		if (nRet != DNET_ERR_NO)
// 		{
// 			g_AxisSyncIntlock.Unlock();
// 			return FALSE;
// 		}
// 		nRet = dnetReversedEncoder(m_iComPort, m_lAxis,GetParasCordinateDirection());	
// 		g_AxisSyncIntlock.Unlock();
// 	}
// 	else
// 	{
// 		return FALSE;
// 	}
// 
// 	if (nRet != DNET_ERR_NO) 
// 	{
// 		return FALSE;
// 	}
#endif
#ifdef _USE_NMC
	//Need2Check
#endif

	return true;
}

bool CAxis::SetPosition(double pos)
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = set_position((int)m_lAxis, pos);
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret;
		g_AxisSyncIntlock.Lock();
		//RTEX 전용 함수이나 ML2와 함께 사용해도 무관하다.
		ret = AxmStatusSetPosMatch(m_lAxis, pos * GetMm4Pulse());

//		ret2 = AxmStatusSetCmdPos(m_lAxis, pos * GetMm4Pulse());
//		ret1 = AxmStatusSetActPos(m_lAxis, pos * GetMm4Pulse());

		g_AxisSyncIntlock.Unlock();	
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		//Cmd Pos 과 Actual Pos와 동시에 해주어 한다. 
		m_MMC_Err = FAS_SetCommandPos((BYTE)m_iPortNo, (BYTE)m_lAxis,(long)pos * 1000);
		if (m_MMC_Err != MMC_OK)
			return FALSE;
		m_MMC_Err = FAS_SetActualPos((BYTE)m_iPortNo, (BYTE)m_lAxis,(long)pos * 1000);
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;
	}
#endif
#ifdef _USE_DNET
	int nRet = 0;
	int count = 0;
	if (GetControlType() == _DNET)
	{
		g_AxisSyncIntlock.Lock();	
		nRet = dnetToZero(m_iComPort, m_lAxis);	
		g_AxisSyncIntlock.Unlock();
		if (nRet != DNET_ERR_NO) 
		{
			return FALSE;
		}
	}

	
#endif
#ifdef _USE_NMC
	if (GetControlType() == _NMC)
	{
		m_lRet = MC_OK;
		m_lRet = MC_SetPosition(m_iPortNo, (int)m_iAxisID, pos, false, mcImmediately);
		if (m_lRet != MC_OK)
		{
			CString str;
			str.Format(_T("NMC Set Position 에러 (축번호 : %d , 에러코드: %d"), m_lAxis, m_MMC_Err);
			TRACE(str);
			return FALSE;	
		}
		else
			return TRUE;
	}
#endif
	return true;
}

bool CAxis::SetAmpFault()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = amp_fault_set((int)m_lAxis);
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{

		if(ResetAmpFault()) return TRUE;
		else                return FALSE;
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		return true;
	}
#endif
#ifdef _USE_NMC
	if(ResetAmpFault()) return TRUE;
	else                return FALSE;
#endif
	return true;
}

bool CAxis::ResetAmpFault()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		amp_fault_reset((int)m_lAxis);
		Delay(100,TRUE);
		m_MMC_Err = amp_fault_set((int)m_lAxis);
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret;
		g_AxisSyncIntlock.Lock();			
		ret = AxmSignalServoAlarmReset (m_lAxis, ENABLE);
		if (ret != AXT_RT_SUCCESS)
		{
			ret = AxmSignalServoAlarmReset (m_lAxis, DISABLE);
			g_AxisSyncIntlock.Unlock();
			return FALSE;	
		}

		Delay(100,TRUE);

		ret = AxmSignalServoAlarmReset (m_lAxis, DISABLE);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		m_MMC_Err = FAS_ServoAlarmReset((BYTE)m_iPortNo, (BYTE)m_lAxis);
		if(m_MMC_Err == FMM_OK)
			return TRUE;
		else 
			return FALSE;
	}
#endif
#ifdef _USE_DNET
	int nRet = 0;
	int count = 0;
	if (GetControlType() == _DNET)
	{
		g_AxisSyncIntlock.Lock();	
		nRet = dnetAlarmReset(m_iComPort, m_lAxis, DNET_ALARM_REACTION_ENABLE_MOTOR);	
		g_AxisSyncIntlock.Unlock();

		if (nRet != DNET_ERR_NO) 
		{
			return FALSE;
		}
	}

#endif
#ifdef _USE_NMC
	if (GetControlType() == _NMC)
	{
		m_lRet = MC_Reset(m_iPortNo, (int)m_iAxisID);
		if (m_lRet == MC_OK)
			return TRUE;
		else
			return FALSE;
	}
#endif
	return true;
}

bool CAxis::SetElectricGear()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = fset_electric_gear((int)m_lAxis, GetParasGearRatio());
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		//1:1로 Setting 하고 Pulse 수로 제어 한다. 기어비는 Servo Pack에서 !!
		DWORD ret;
		long lAxisNo = 0;
		long lPulse = 1; 
		double dUnit = 1;
		g_AxisSyncIntlock.Lock();			
		ret = AxmMotSetMoveUnitPerPulse(m_lAxis, dUnit, lPulse);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		return true;	
	}
#endif
#ifdef _USE_DNET
	//Need2Check
#endif
#ifdef _USE_NMC
	//Need2Check
#endif
	return true;
}

bool CAxis::SetErrLimit()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = fset_error_limit((int)m_lAxis, (double) GetParasErrorLimit(), GetParasErrorLimitEvent());
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		return true;
	}
#endif
#ifdef _USE_NMC
	//Need2Check
#endif
	return true;
}

bool CAxis::SetInPositionRequired()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = set_inposition_required((int)m_lAxis, GetParasInPositionRequired());
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret;
		g_AxisSyncIntlock.Lock();			
		ret = AxmSignalSetInpos (m_lAxis, GetParasInPositionRequired());
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		return true;	
	}
#endif
#ifdef _USE_DNET
	//Need2Check
#endif
#ifdef _USE_NMC
	//Need2Check
#endif
	return true;	
}

bool CAxis::SetLoopModeType()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = fset_closed_loop((int)m_lAxis, GetParasLoopMode());	
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		return true;
	}
#endif
	return true;
}

BOOL CAxis::SetHomingSetup()
{
#ifdef _USE_DNET
	int i=0, nRet=0;
	// 이동 속도, acc, dec, offset 설정
	// acc : dnetHomingSetup 요함수는 acc을 time이 아닌 pulse로 사용한다.. 임시로 *4000
	double dHomeAcc (0.0);
	for(i=0; i<MAX_AXIS_NUM; i++)
	{
		dHomeAcc = m_HomeVel/(m_HomeAcc/1000);
		nRet = dnetHomingSetup(m_iComPort, i+1, (ULONG32)m_HomeVel, (ULONG32)m_HomeFineVel, (ULONG32)dHomeAcc, (LONG32)m_dOrgOffset);
		if (nRet != DNET_ERR_NO)
		{
			TRACE("dnetHomingSetup:%d\n", nRet);
			return HOME_ERROR;
		}
	}
#endif
	return TRUE;
}

BOOL CAxis::Origin() //130219 JSPark
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif	
	int OrgStep = ORG_INIT;

	SetOriginReturn(FALSE);
	ClearAxis();
	Delay(500,TRUE);
	ClearState(); 
	ResetAmpFault();
	Delay(500,TRUE);
	AmpEnable();
	Delay(500,TRUE);		
	SetStopRate(10); 

//>> 130220 JSPark
	//축 사용 유무 설정에 따른 Origin 동작 결정
	if(GetAxisUse() != USE)
	{
		SetOriginReturn(TRUE);
//>> 130317 JSPark
		if(m_iMotorType == SERVO_MOTOR) // Servo Motor일 경우 부하셋팅 한다....
		{
			//Home Setting이 완료되면 부하Check Option을 활성화 시킨다. [2013.03.01 by LJY]
			SetServoLodRatio(m_iServoRatioType);
		}
//<<
		return TRUE;
	}
//<<
	SetNegativeLimit(E_STOP_EVENT);
	SetPositiveLimit(E_STOP_EVENT); 
	
	if(!GetAmpFaultState()) 
		return HOME_ERROR; 

	origin_timer.SetCheckTime(60000);

	while(1)
	{
		Delay(1, TRUE);
		switch(OrgStep)
		{	
			//초기 조건 분기 : 
			//Home Sensor만 있을 경우 혹은 Neg / Pos Sensor부터 시작할 경우 
			case ORG_INIT:
				// Ajin 홈 써치 방법을 사용하고자 할때 사용!!
				if(GetControlType() == _AJINXL)
				{
					//20120329 lee
#ifdef _USE_AJINXL
					if(AxmHomeSetResult (m_lAxis, HOME_ERR_USER_BREAK) == AXT_RT_SUCCESS)
					{
						long ret, lHmDir; 
						DWORD lHmsig, dwZphas; 
						double dwHClrTim, dwHOffset; 
						double dVelFirst, dVelSecond, dVelThird, dvelLast, dAccFirst, dAccSecond;

						//20130329 lee
						if(GetOriginMethod() == ORG_METHOD_NEG || GetOriginMethod() == ORG_METHOD_NEG_HOME 
							|| GetOriginMethod() == ORG_METHOD_NEG_HOME_Z || GetOriginMethod() == ORG_METHOD_HOME)
							lHmDir = CW;    //AJIN 은 CW가 1이다. MMC는 0이다. 
						else
							lHmDir = CCW;    //AJIN 은 CCW가 0이다. MMC는 1이다. 


						if(GetOriginMethod() == ORG_METHOD_POS_HOME || GetOriginMethod() == ORG_METHOD_NEG_HOME || 
							GetOriginMethod() == ORG_METHOD_POS_HOME_Z || GetOriginMethod() == ORG_METHOD_NEG_HOME_Z ||
							GetOriginMethod() == ORG_METHOD_HOME)
							lHmsig = ORG_METHOD_HOME;
						else
							lHmsig = GetOriginMethod();


						//20130329 lee	
						if(GetOriginMethod() == ORG_METHOD_POS_HOME_Z || GetOriginMethod() == ORG_METHOD_NEG_HOME_Z)
							dwZphas = 1;
						else
							dwZphas = 0;

						dwHClrTim = 1000.0;
						dwHOffset = m_dOrgOffset * GetPulse4Mm();  //환산 해 줘야 한다!!!

						dVelFirst = m_HomeVel;
						dVelSecond  = m_HomeVel / 2;
						dVelThird = m_HomeFineVel;
						dvelLast = m_HomeFineVel / 2;
						dAccFirst = m_HomeAcc;
						dAccSecond = m_HomeFineAcc;
	
						AxmMotSetAccelUnit(m_lAxis, 1);

						ret = AxmHomeSetMethod(m_lAxis, lHmDir, lHmsig, dwZphas, dwHClrTim, dwHOffset);
						if(ret != AXT_RT_SUCCESS)
							return HOME_ERROR;

						ret = AxmHomeSetVel(m_lAxis,dVelFirst, dVelSecond, dVelThird, dvelLast, dAccFirst, dAccSecond);
						if(ret != AXT_RT_SUCCESS)
							return HOME_ERROR;

//						AxmSignalServoOn(m_lAxis, DISABLE);
						ret = AxmHomeSetStart (m_lAxis);
						if(ret != AXT_RT_SUCCESS)
							return HOME_ERROR;

						Delay(1000, TRUE);
						//원점 복귀 Time Out 설정 
						origin_timer.SetCheckTime(60000);
						origin_timer.StartTimer();
						OrgStep = ORG_ORIGIN_COMPLETE_CHECK;
					}
					else
						return HOME_ERROR;
#endif
				}
				else if(GetControlType() == _DNET)
				{
#ifdef _USE_DNET
					int nRet = 0;
					// Operation mode PositionMode 로 설정
					nRet = dnetEnableHomingMode(m_iComPort, m_lAxis);
					if (nRet != DNET_ERR_NO)
					{
						return HOME_ERROR;
					}
					Sleep(50);
					// 						// 이동 속도, acc, dec, offset 설정
					// 						// acc : dnetHomingSetup 요함수는 acc을 time이 아닌 pulse로 사용한다.. 임시로 *4000
					// 						double dHomeAcc (0.0);
					// 						dHomeAcc = m_HomeVel/(m_HomeAcc/1000);
					// 						nRet = dnetHomingSetup(m_iComPort, m_lAxis, (ULONG32)m_HomeVel, (ULONG32)m_HomeFineVel, (ULONG32)dHomeAcc, (LONG32)m_dOrgOffset);
					// 						if (nRet != DNET_ERR_NO)
					// 						{
					// 							TRACE("dnetHomingSetup:%d\n", nRet);
					// 							return HOME_ERROR;
					// 						}

					// Origin Start
					nRet = dnetHomingStart(m_iComPort, m_lAxis, m_iOriginMethod);
					if (nRet != DNET_ERR_NO)
					{
						return HOME_ERROR;
					}

					Delay(1000, TRUE);
					//원점 복귀 Time Out 설정 
					origin_timer.SetCheckTime(60000);
					origin_timer.StartTimer();
					OrgStep = ORG_ORIGIN_COMPLETE_CHECK;
#endif
				}
				else if(GetControlType() == _NMC)
				{
#ifdef _USE_NMC
					m_lRet = MC_WriteParameter(m_iPortNo, (int)m_iAxisID,mcpHomingVelocity,m_HomeVel);
					m_lRet = MC_WriteParameter(m_iPortNo, (int)m_iAxisID,mcpHomingAcceleration,m_HomeVel*1000.0f/(4.0f*m_HomeAcc));
					m_lRet = MC_WriteParameter(m_iPortNo, (int)m_iAxisID,mcpHomingDeceleration,m_HomeVel*1000.0f/(4.0f*m_HomeDec));
					m_lRet = MC_WriteParameter(m_iPortNo, (int)m_iAxisID,mcpHomingJerk,0);
					m_lRet = MC_WriteParameter(m_iPortNo, (int)m_iAxisID,mcpHomingCreepVelocity,m_HomeFineVel);

					m_lRet = MC_Home(m_iPortNo,(int)m_iAxisID,(DOUBLE)m_iOriginMethod,mcAborting);
					origin_timer.SetCheckTime(60000);
					origin_timer.StartTimer();
					OrgStep = ORG_ORIGIN_COMPLETE_CHECK;
#endif
				}
				else
				{
					if(GetOriginMethod() == ORG_METHOD_POS_HOME) 
						OrgStep = ORG_POS_SENSOR_SEARCH;				// Pos Sensor->Home Sensor 방법 
					else if(GetOriginMethod() == ORG_METHOD_POS) 
						OrgStep = ORG_POS_SENSOR_SEARCH;				// Pos Sensor 방법 
					else if(GetOriginMethod() == ORG_METHOD_NEG_HOME) 
						OrgStep = ORG_NEG_SENSOR_SEARCH;			// Neg Sensor->Home Sensor 방법
					else if(GetOriginMethod() == ORG_METHOD_NEG) 
						OrgStep = ORG_NEG_SENSOR_SEARCH;				// Neg Sensor 방법 
					else if(GetOriginMethod() == ORG_METHOD_HOME) 
						OrgStep = ORG_HOME_SENSOR_SEARCH_4_POS;		// Home Sensor 방법 우선 Neg 로 Home을 찾는다. 
					else if(GetOriginMethod() == ORG_METHOD_POS_HOME_Z) 
						OrgStep = ORG_POS_SENSOR_SEARCH;		// Pos->Home->ZPhase 방법 
					else if(GetOriginMethod() == ORG_METHOD_NEG_HOME_Z) 
						OrgStep = ORG_NEG_SENSOR_SEARCH;		// Neg->Home->ZPhase 방법 
					else 
						return HOME_ERROR;
				}
				break;
				//Pos Sensor Search
			case ORG_POS_SENSOR_SEARCH:
				ClearAxis();  
				Delay(300, TRUE);
				SetPositiveLimit(STOP_EVENT);
				VMove(GetHomeVel(), GetHomeAcc(),GetHomeDec());	
				Delay(100, TRUE);
				
				origin_timer.SetCheckTime(50000);
				origin_timer.StartTimer();
				while(!MotionDone())
				{
					if(origin_timer.IsTimeOver())
					{
						EStop();
						return HOME_ERROR;
					}
					Delay(1, TRUE);
				}
				
				if(!GetPosLimitSensor())
				{
					EStop();
					return HOME_ERROR;
				}

				if(GetOriginMethod() == ORG_METHOD_POS) 
					OrgStep = ORG_POS_SENSOR_OUT;            //Pos Sensor 이탈 Step으로 
				else							        
					OrgStep = ORG_HOME_SENSOR_SEARCH_4_POS;  //Home Sensor Search Step으로 
				break;
				//Pos Sensor 이탈 
			case ORG_POS_SENSOR_OUT:
				ClearAxis();
				Delay(300,TRUE);
				SetPositiveLimit(NO_EVENT);
				VMove(-1.0*GetHomeFineVel(), GetHomeFineAcc(),GetHomeFineAcc());
				
//				origin_timer.SetCheckTime(10000);
				origin_timer.SetCheckTime(20000);
				origin_timer.StartTimer();
				while(GetPosLimitSensor())
				{
					if(origin_timer.IsTimeOver())
					{
						EStop();
						return HOME_ERROR;
					}
					Delay(1,TRUE);
				}
				EStop();
				Delay(0, TRUE);
				
				if(GetPosLimitSensor())
				{
					EStop();
					return HOME_ERROR;
				}
				ClearAxis();
				ClearState();
				Delay(1000, TRUE);

				SetPosition(0.0);
				OrgStep = ORG_ORIGIN_OFFSET_MOVE;
				break;
				//Neg Sensor Search
			case ORG_NEG_SENSOR_SEARCH:
				SetNegativeLimit(STOP_EVENT);
				SetHome(NO_EVENT);
				VMove(-1.0*GetHomeVel(), GetHomeAcc(),GetHomeDec());
				Delay(100, TRUE);

				origin_timer2.SetCheckTime(60000);
				origin_timer2.StartTimer();
				while(!MotionDone())
				{
					if(origin_timer2.IsTimeOver())
					{
						EStop();
						return HOME_ERROR;
					}
					Delay(1, TRUE); 
				}

				if(!GetNegLimitSensor())
				{
					EStop();
					return HOME_ERROR;
				}

				if(GetOriginMethod() == ORG_METHOD_NEG) 
					OrgStep = ORG_NEG_SENSOR_OUT;            //Neg Sensor 이탈 Step으로 
				else							        
					OrgStep = ORG_HOME_SENSOR_SEARCH_4_NEG;  //Home Sensor Search Step으로 
				break;
				//Neg Sensor 이탈 
			case ORG_NEG_SENSOR_OUT:
				ClearAxis();
				Delay(100, TRUE);
				SetNegativeLimit(NO_EVENT);
				VMove(GetHomeFineVel(), GetHomeFineAcc(),GetHomeFineAcc());
				
//				origin_timer.SetCheckTime(10000);
				origin_timer.SetCheckTime(20000);
				origin_timer.StartTimer();
				while(GetNegLimitSensor())
				{
					if(origin_timer.IsTimeOver())
					{
						EStop();
						return HOME_ERROR;
					}
					Delay(1, TRUE);
				}
				EStop();
				Stop();
				
				if(GetNegLimitSensor())
				{
					EStop();
					return HOME_ERROR;
				}
				ClearAxis();
				ClearState();
				Delay(1000, TRUE);

				SetPosition(0.0);		
				OrgStep = ORG_ORIGIN_OFFSET_MOVE;
				break;
				//Home Sensor Search for Pos Sensor 
			case ORG_HOME_SENSOR_SEARCH_4_POS:
				ClearAxis();
				Delay(100, TRUE);
				SetPositiveLimit(NO_EVENT);
				SetHome(STOP_EVENT);
				VMove(-1.0*GetHomeVel(), GetHomeAcc(),GetHomeDec());
				
				origin_timer.SetCheckTime(30000);
				origin_timer.StartTimer();
				while(1)
				{
					if(GetControlType() == _AJINXL || GetControlType() == _FASTECH)
					{
						if(GetHomeSensor()) 
						{
							EStop();
							break;
						}
					}
					if(origin_timer.IsTimeOver())
					{
						EStop();
						return HOME_ERROR;
					}
					Delay(1, TRUE);
				}
				
				if(!GetHomeSensor())
				{
					Delay(500,TRUE);
					if(!GetHomeSensor())
					{
						EStop();
						return HOME_ERROR;
					}
				}
				
				OrgStep = ORG_HOME_SENSOR_OUT;
				break;
			//Home Sensor Search for Neg Sensor 
			case ORG_HOME_SENSOR_SEARCH_4_NEG:
				ClearAxis();
				Delay(100, TRUE);
				SetNegativeLimit(NO_EVENT);
				SetHome(STOP_EVENT);
				VMove(GetHomeVel(), GetHomeAcc(),GetHomeDec());

				origin_timer.SetCheckTime(30000);
				origin_timer.StartTimer();
				while(1)
				{
					if(GetControlType() == _AJINXL || GetControlType() == _FASTECH)
					{
						if(GetHomeSensor()) 
						{
							EStop();
							break;
						}
					}
					if(origin_timer.IsTimeOver())
					{
						EStop();
						return HOME_ERROR;
					}
					Delay(1, TRUE); 
				}

				if(!GetHomeSensor())
				{
					EStop();
					return HOME_ERROR;
				}

				origin_timer.SetCheckTime(10000);
				origin_timer.StartTimer();
				while(!MotionDone())
				{
					if(origin_timer.IsTimeOver())
					{
						EStop();
						return HOME_ERROR;
					}
					Delay(1, TRUE); 
				}

				OrgStep = ORG_HOME_SENSOR_OUT;
				break;

			//Home Sensor Out
			case ORG_HOME_SENSOR_OUT: 
				ClearAxis();
				SetHome(NO_EVENT); 
				Delay(100, TRUE);
				ClearAxis();
				VMove(-1.0*GetHomeFineVel(), GetHomeFineAcc(),GetHomeFineAcc());

//				origin_timer.SetCheckTime(10000);
				origin_timer.SetCheckTime(20000);
				origin_timer.StartTimer();				
				while(GetHomeSensor())
				{
					if(origin_timer.IsTimeOver())
					{
						EStop();
						return HOME_ERROR;
					}
					Delay(1, TRUE);
				}
				Delay(0, TRUE);
				Stop();
				
				if(GetHomeSensor())
				{
					EStop();
					return HOME_ERROR;
				}
				ClearAxis();
				ClearState();
				Delay(1000, TRUE);

				SetPosition(0.0);
				OrgStep = ORG_ORIGIN_OFFSET_MOVE;
				break;
			//Z Signal Search ->Pos Sensor 방향으로 잡는다!
			case ORG_Z_PHASE_SEARCH:
#ifdef _USE_MMC	
				if(GetControlType() == _MMC)
				{
					ClearAxis();
					Delay(1000, TRUE);
					SetHome(NO_EVENT);
					ClearAxis();
					Delay(1000, TRUE);
					set_index_required((int)m_lAxis, TRUE);
					VMove(GetHomeFineVel(), GetHomeFineAcc());
				}
#endif
#ifdef _USE_FASTECH	
				if(GetControlType() == _FASTECH)
				{
					//Z상 원점 복귀로 셋팅
					Delay(100, TRUE); 
					m_MMC_Err = FAS_SetParameter((BYTE)m_iPortNo,(BYTE)m_lAxis,SERVO_ORGMETHOD,1);
					if (m_MMC_Err != FMM_OK) 
						return FALSE; 

					Delay(300, TRUE); 
					
					//Z상 찾아라!
					m_MMC_Err = FAS_MoveOriginSingleAxis((BYTE)m_iPortNo,(BYTE)m_lAxis);
					if(m_MMC_Err != FMM_OK) 
						return FALSE;				
				}
#endif
				origin_timer.SetCheckTime(20000);
				origin_timer.StartTimer();
				while(1)
				{
					if(GetControlType() == _AJINXL || GetControlType() == _FASTECH)
					{
						if(GetHomeSensor()) 
						{
							EStop();
							break;
						}
					}
					if(origin_timer.IsTimeOver())
					{
		 				EStop();
						return HOME_ERROR;
					}
					Delay(1, TRUE);
				}					
#ifdef _USE_MMC	
				set_index_required((int)m_lAxis, FALSE);
#endif

				SetNegativeLimit(E_STOP_EVENT);
				SetPositiveLimit(E_STOP_EVENT);
				
				ClearAxis();
				ClearState();
				Delay(1000, TRUE);

				SetPosition(0.0);
				OrgStep = ORG_ORIGIN_OFFSET_MOVE;
				break;
			case ORG_ORIGIN_OFFSET_MOVE:
				//Offset Move
				SetOriginReturn(TRUE);
				Delay(1000, TRUE);
				if(Wait4Done() == TRUE)
				{
					SetPosition(0.0);
					if(!OffsetMove(m_dOrgOffset))
					{
						Delay(500, TRUE);
						SetPosition(0.0);
						if(!OffsetMove(m_dOrgOffset))
						{

							EStop();
							SetOriginReturn(FALSE);
							return HOME_ERROR;
						}
					}
				}
				else
				{
					EStop();
					SetOriginReturn(FALSE);
					return HOME_ERROR;
				}
				Delay(800, TRUE);


				ClearAxis();
				ClearState();
				SetPosition(0.0);
				return TRUE;
				break;
			case ORG_ORIGIN_COMPLETE_CHECK:
				//time Out Check
				if (origin_timer.IsTimeOver())
				{
					EStop();
					return HOME_ERROR;
				}
#ifdef _USE_AJINXL
				DWORD uHomeResult;
				//원점 복귀 상태 확인 
				AxmHomeGetResult(m_lAxis, &uHomeResult);
				if(uHomeResult == HOME_SUCCESS) 
				{
					SetOriginReturn(TRUE);	
					break;
// 					return TRUE;  //20130329 lee
				}
				else if(uHomeResult == HOME_SEARCHING)
					break;
				else if(uHomeResult == HOME_ERR_USER_BREAK)
					return HOME_ERROR;
				else //Error 발생 
					return HOME_ERROR;
#endif
#ifdef _USE_DNET
				int nRet=-1;
				nRet = dnetIsHomingComplete(m_iComPort, m_lAxis);
				if (nRet == DNET_ERR_NO)
				{
					SetOriginReturn(TRUE);
				}
#endif
#ifdef _USE_NMC
				if(GetControlType() == _NMC)
				{
					UINT32 AxisInfo = 0;
					MC_ReadAxisStatus(m_iPortNo,(int)m_iAxisID, &AxisInfo);
					if (AxisInfo & mcIsHomed)
					{								
						ClearAxis();
						ClearState();
						Delay(100,TRUE);
						SetPosition(0.0);
						Delay(500,TRUE);
						SetOriginReturn(TRUE);
						if(!Move(m_dOrgOffset))
						{
							Delay(500, TRUE);
							SetPosition(0.0);
							if(!Move(m_dOrgOffset))
							{
								EStop();
								SetOriginReturn(FALSE);
								return HOME_ERROR;
							}
						}
						Delay(500,TRUE);
						SetPosition(0.0);
//						SetOriginReturn(TRUE);
					}					
				}
#endif
			break;   
		}
		if(isOriginReturn())
			break;
	}
//>> 130317 JSPark
	if(isOriginReturn())
	{
		if(m_iMotorType == SERVO_MOTOR) // Servo Motor일 경우 부하셋팅 한다....
		{
			//Home Setting이 완료되면 부하Check Option을 활성화 시킨다. [2013.03.01 by LJY]
			SetServoLodRatio(m_iServoRatioType);
		}
	}
//<<
	return TRUE;
}

bool CAxis::ClearAxis()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

	if(ClearStatus())
		return FALSE;
	Delay(10,TRUE);		 

	return FramesClear();
}

bool CAxis::SetStopRate(int msec)
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = fset_stop_rate((int)m_lAxis, msec);
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret(TRUE);
		g_AxisSyncIntlock.Lock();			
		SetParasStopDec((long)msec);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		
	}
#endif
#ifdef _USE_NMC
	//Need2Check
#endif

	return TRUE;
}

bool CAxis::VMove(double Velocity, double Acceleration, double Deceleration)
{

#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif 

	int ret(0);
	
//	SetNegativeLimit(STOP_EVENT);
//	SetPositiveLimit(STOP_EVENT);

	//20130901 lee
	if(GetAxisUse() == NOT_USE) return TRUE;

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		ret = v_move((int)m_lAxis, Velocity, (int)Acceleration);
		if (AxisState() != NO_EVENT)
			return FALSE;
		if (ret == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret;
		g_AxisSyncIntlock.Lock();			
		AxmMotSetAccelUnit(m_lAxis,1);
		AxmMotSetMinVel(m_lAxis,1);
		ret = AxmMoveVel(m_lAxis, Velocity, Acceleration, Acceleration);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH	
	if(GetControlType() == _FASTECH)
	{	
		int iDir;
		DWORD lValue;
		
		lValue = (DWORD)Velocity;
		if(Velocity > 0)
			iDir = DIR_INC;
		else
		{
			lValue *= -1;
			iDir = DIR_DEC;
		}
		
		ret = FAS_MoveVelocity((BYTE)m_iPortNo,(BYTE)m_lAxis,lValue,iDir);
		if(ret == FMM_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_DNET
	int nRet = 0;

	if (GetControlType() == _DNET)
	{
		g_AxisSyncIntlock.Lock();
		dnetEnable(m_iComPort, m_lAxis, DNET_MC_OPMODE_PVM);
// 		dnetSetProfileAcceleration(m_iComPort, m_lAxis, (int)Acceleration);
// 		dnetSetProfileDeceleration(m_iComPort, m_lAxis, (int)Acceleration);
		dnetSetProfileAccTm(m_iComPort, m_lAxis, (int)Acceleration);
		dnetSetProfileDecTm(m_iComPort, m_lAxis, (int)Acceleration);

		nRet = dnetSetTargetVelocity(m_iComPort, m_lAxis, (ULONG32)Velocity);
		g_AxisSyncIntlock.Unlock();

		if (nRet != DNET_ERR_NO)
		{
			return FALSE;
		}
	}
#endif
#ifdef _USE_NMC
	if(GetControlType() == _NMC)
	{
		if (Velocity < 0 )
		{
			Velocity = -1 * Velocity;
			ret = MC_MoveVelocity(m_iPortNo, (int)m_iAxisID, Velocity,
				Velocity*(1000.0f/(4.0f*Acceleration)), 
				Velocity*(1000.0f/(4.0f*Deceleration)),
				0, mcNegativeDirection, mcAborting);
		}
		else
		{
			ret = MC_MoveVelocity(m_iPortNo, (int)m_iAxisID, Velocity,
				Velocity*(1000.0f/(4.0f*Acceleration)), 
				Velocity*(1000.0f/(4.0f*Deceleration)),
				0, mcPositiveDirection, mcAborting);
		}
		if (ret == MC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
	return TRUE;
}

bool CAxis::ClearStatus()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif
	int ret(0);
	int i = 0;
	CString tmp(_T(""));

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		while (1)
		{
			ret = clear_status((int)m_lAxis);
			if (ret == 0)
			{
				break;
			}
			else
			{
				Delay(AXIS_DELAY,TRUE);
				if (axis_state((int)m_lAxis) == 0)
					break;
				else if (i++ > 200)
				{
					return FALSE;
				}
			}
		}		
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		return true;
	}
#endif
	return true;
}

int CAxis::AxisState()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif
	int ret(0);

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		ret = axis_state((int)m_lAxis);
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		ret = TRUE;
	}
#endif
	return ret; 
}

bool CAxis::FramesClear()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

	CTimerCheck time_check(AXIS_DELAY);
	int i = 0;
	CString tmp;

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		while (1)
		{
			m_MMC_Err = frames_clear((int)m_lAxis);
			time_check.StartTimer();
			if (m_MMC_Err)
			{
				while (time_check.IsTimeOver())
					Delay(1,TRUE);
			}
			
			if (axis_state((int)m_lAxis) == 0)
				break;
			else if (i++ > 200)
			{
				return FALSE;
			}
		}		
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		return true;
	}
#endif
#ifdef _USE_NMC
	//Need2Check
#endif
	return true;
}

bool CAxis::SetIndexRequired()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = set_index_required((int)m_lAxis, TRUE);
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		
	}
#endif
	return true;
}

bool CAxis::ResetIndexRequired()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = set_index_required((int)m_lAxis, FALSE);
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		return true;		
	}
#endif
	return true;
}

bool CAxis::Stop()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		SetStopRate(20); //130315 JSPark 
		m_MMC_Err = set_stop((int)m_lAxis);  // 내부적으로 STOP EVENT 발생시킴.
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
		return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		int msec(100);
		DWORD ret;
		g_AxisSyncIntlock.Lock();			
		GetStopRate(msec);
		ret = AxmMoveStop (m_lAxis, msec * 0.001);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		m_MMC_Err = FAS_MoveStop((BYTE)m_iPortNo, (BYTE)m_lAxis);
		if (m_MMC_Err == FMM_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_DNET
	int nRet = 0;
	if (GetControlType() == _DNET)
	{
		g_AxisSyncIntlock.Lock();
		nRet = dnetQStop(m_iComPort, m_lAxis);	
		g_AxisSyncIntlock.Unlock();

		if (nRet != DNET_ERR_NO) 
		{
			return FALSE;
		}
	}
#endif
#ifdef _USE_NMC
	if (GetControlType() == _NMC)
	{
		m_lRet = MC_Halt(m_iPortNo, (int)m_iAxisID,  (GetNormVel()*10.0f,0,mcAborting),0,mcAborting);
		if (m_lRet == MC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif

	return true;
}

bool CAxis::EStop()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif
	
#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = set_e_stop((int)m_lAxis);  // 내부적으로 STOP EVENT 발생시킴.
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret;
		g_AxisSyncIntlock.Lock();	
		ret = AxmMoveEStop (m_lAxis);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		m_MMC_Err = FAS_MoveStop((BYTE)m_iPortNo, (BYTE)m_lAxis);
		if (m_MMC_Err == FMM_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_DNET
	int nRet = 0;
	if (GetControlType() == _DNET)
	{
		g_AxisSyncIntlock.Lock();
		nRet = dnetQStop(m_iComPort, m_lAxis);	
		g_AxisSyncIntlock.Unlock();

		if (nRet != DNET_ERR_NO) 
		{
			return FALSE;
		}	
	}
#endif
#ifdef _USE_NMC
	if (GetControlType() == _NMC)
	{
		m_lRet = MC_Stop(m_iPortNo, (int)m_iAxisID,true, (GetNormVel()*10.0f,0,mcAborting),0);
		if (m_lRet == MC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
	return true;
}

bool CAxis::SetPositiveLevel()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = fset_positive_level((int)m_lAxis, GetParasPositiveLimitLevel());
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret; 
		DWORD StopMode, PositiveLevel, NegativeLevel;	
		
		g_AxisSyncIntlock.Lock();			
		AxmSignalGetLimit (m_lAxis, &StopMode, &PositiveLevel, &NegativeLevel);
		PositiveLevel = (DWORD)GetParasPositiveLimitLevel();
		ret = AxmSignalSetLimit (m_lAxis, StopMode, PositiveLevel , NegativeLevel);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH	
	if(GetControlType() == _FASTECH)
	{
		long lParamVal;

		m_MMC_Err = FAS_GetParameter((BYTE)m_iPortNo,(BYTE)m_lAxis,SERVO_LIMITSENSORLOGIC,&lParamVal);
		if(m_MMC_Err == FMM_OK)
		{
			if(lParamVal == (long)GetParasPositiveLimitLevel()) 
				return TRUE;
			else
			{
				m_MMC_Err = FAS_SetParameter((BYTE)m_iPortNo,(BYTE)m_lAxis,SERVO_LIMITSENSORLOGIC,(long)GetParasPositiveLimitLevel());
				if (m_MMC_Err == FMM_OK)
				{
					m_MMC_Err = FAS_SaveAllParameters((BYTE)m_iPortNo,(BYTE)m_lAxis);
					if(m_MMC_Err == FMM_OK) return TRUE;
					else                    return FALSE;	
				}
				else
					return FALSE;	
			}
		}
		else
		{
			return FALSE;
		}
	}
#endif
#ifdef _USE_DNET
	//int nRet = 0;
	//if (GetControlType() == _DNET)
	//{
	//	nRet = dnetEnableDInputMaskPositiveLimit(m_iComPort, m_lAxis, (int)GetParasPositiveLimitLevel());

	//	if (nRet != DNET_ERR_NO)
	//		return FALSE;	
	//	//
	//	nRet = dnetEnableDInputExeLimitP(m_iComPort, m_lAxis, (int)GetParasPositiveLimitLevel());

	//	if (nRet != DNET_ERR_NO)
	//		return FALSE;		
	//	//
	//	nRet = dnetEnableDInputPolarityPositive(m_iComPort, m_lAxis, (int)GetParasPositiveLimitLevel());

	//	if (nRet != DNET_ERR_NO)
	//		return FALSE;	
	//}
	//else
	//{
	//	FALSE;
	//}
#endif
#ifdef _USE_NMC
	//Need2Check
#endif
	return TRUE;
}

bool CAxis::SetNegativeLevel()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = fset_negative_level((int)m_lAxis, GetParasNegativeLimitLevel());
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret; 
		DWORD StopMode, PositiveLevel, NegativeLevel;	
		g_AxisSyncIntlock.Lock();			
		AxmSignalGetLimit (m_lAxis, &StopMode, &PositiveLevel, &NegativeLevel);
		
		NegativeLevel = (DWORD)GetParasNegativeLimitLevel();
		ret = AxmSignalSetLimit (m_lAxis, StopMode, PositiveLevel, NegativeLevel);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		long lParamVal;

		m_MMC_Err = FAS_GetParameter((BYTE)m_iPortNo,(BYTE)m_lAxis,SERVO_LIMITSENSORLOGIC,&lParamVal);
		if(m_MMC_Err == FMM_OK)
		{
			if(lParamVal == (long)GetParasPositiveLimitLevel()) 
				return TRUE;
			else
			{
				m_MMC_Err = FAS_SetParameter((BYTE)m_iPortNo,(BYTE)m_lAxis,SERVO_LIMITSENSORLOGIC,(long)GetParasPositiveLimitLevel());
				if (m_MMC_Err == FMM_OK)
				{
					m_MMC_Err = FAS_SaveAllParameters((BYTE)m_iPortNo,(BYTE)m_lAxis);
					if(m_MMC_Err == FMM_OK) return TRUE;
					else                    return FALSE;	
				}
				else
					return FALSE;	
			}
		}
		else
		{
			return FALSE;
		}
	}
#endif
#ifdef _USE_DENT
	//int nRet = 0;
	//if (GetControlType() == _DNET)
	//{
		//nRet = dnetEnableDInputMaskNegativeLimit(m_iComPort, m_lAxis, (int)GetParasNegativeLimitLevel());

		//if (nRet != DNET_ERR_NO)
		//	return FALSE;		
		////
		//nRet = dnetEnableDInputExeLimitN(m_iComPort, m_lAxis, (int)GetParasNegativeLimitLevel());

		//if (nRet != DNET_ERR_NO)
		//	return FALSE;	
		////
		//nRet = dnetEnableDInputPolarityNegative(m_iComPort, m_lAxis, (int)GetParasNegativeLimitLevel());

		//if (nRet != DNET_ERR_NO)
		//	return FALSE;	
	//}
	//else
	//{
	//	return FALSE;
	//}
#endif
#ifdef _USE_NMC
	//Need2Check
#endif
	return TRUE;
}

bool CAxis::SetHomeLevel()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = fset_home_level((int)m_lAxis, GetParasHomeLimitLevel());
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret; 
		DWORD HomeLevel;
		g_AxisSyncIntlock.Lock();		
		HomeLevel = (DWORD)GetParasHomeLimitLevel();
		ret = AxmHomeSetSignalLevel(m_lAxis, HomeLevel);
		g_AxisSyncIntlock.Unlock();
		
		if(ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH	
	if(GetControlType() == _FASTECH)
	{		
		m_MMC_Err = FAS_SetParameter((BYTE)m_iPortNo,(BYTE)m_lAxis,SERVO_ORGSENSORLOGIC,(long)GetParasHomeLimitLevel());
		if (m_MMC_Err == FMM_OK)
			return TRUE;
		else
			return FALSE;			
	}
#endif
#ifdef USE_DNET
	//int nRet = 0;
	//if (GetControlType() == _DNET)
	//{
		//nRet = dnetEnableDInputMaskHome(m_iComPort, m_lAxis, (int)GetParasHomeLimitLevel());

		//if (nRet != DNET_ERR_NO)
		//	return FALSE;		
		////
		//nRet = dnetEnableDInputExeHome(m_iComPort, m_lAxis, (int)GetParasHomeLimitLevel());

		//if (nRet != DNET_ERR_NO)
		//	return FALSE;		
		////
		//nRet = dnetEnableDInputPolarityHome(m_iComPort, m_lAxis, (int)GetParasHomeLimitLevel());

		//if (nRet != DNET_ERR_NO)
		//	return FALSE;	
	//}
	//else
	//{
	//	return FALSE;
	//}

#endif
	return TRUE;
}

bool CAxis::SetAmpfaultlevel()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = fset_amp_fault_level((int)m_lAxis, GetParasAmpFaultLevel());
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret;
		DWORD AmpFaultLevel;
		g_AxisSyncIntlock.Lock();			
		AmpFaultLevel = (DWORD)GetParasAmpFaultLevel();
		ret = AxmSignalSetServoAlarm (m_lAxis, AmpFaultLevel);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH	
	if(GetControlType() == _FASTECH)
	{			
		m_MMC_Err = FAS_SetParameter((BYTE)m_iPortNo,(BYTE)m_lAxis,SERVO_SERVOALARMLOGIC,(long)GetParasAmpFaultLevel());
		if (m_MMC_Err == FMM_OK)
			return TRUE;
		else
			return FALSE;			
	}
#endif
#ifdef _USE_DNET
	//Need2Check
#endif
#ifdef _USE_NMC
	//Need2Check
#endif

	return TRUE;
}

BOOL CAxis::SetSyncControl(int master, int slave)
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		//Axis Mapping
		m_MMC_Err = set_sync_map_axes(master,slave);
		if (m_MMC_Err != MMC_OK) return FALSE;		

		m_MMC_Err = set_sync_control(1);	
		if (m_MMC_Err == MMC_OK) return TRUE;
		else                     return FALSE;	
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret;
		g_AxisSyncIntlock.Lock();			
		ret = AxmGantrySetEnable(master, slave, TRUE, 0.0, 0.0);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif	
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		return TRUE;		
	}
#endif
#ifdef _USE_DNET
	//Need2Check
#endif
	return TRUE;
}

BOOL CAxis::RetSyncControl(int master, int slave)
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = set_sync_control(0);
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;			
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret;
		g_AxisSyncIntlock.Lock();			
		ret = AxmGantrySetDisable(master, slave);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		return TRUE;	
	}
#endif
#ifdef _USE_DNET
#endif
	return TRUE;
}

BOOL CAxis::SetSyncGain(long gain)
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		m_MMC_Err = set_sync_gain(gain);
		
		if (m_MMC_Err == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		return TRUE;		
	}
#endif
	return TRUE;
}

BOOL CAxis::GetSyncControl()
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif

	INT control = FALSE;

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		get_sync_control(&control);		
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		return FALSE;	
	}
#endif
	return control;
}

BOOL CAxis::GetInmotionState()
{
#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		return in_motion((int)m_lAxis);			
	}
#endif
#ifdef _USE_FASTECH	
	if(GetControlType() == _FASTECH)
	{
		DWORD dwAxisStatus=0;
		EZISERVO_AXISSTATUS state;
		
		m_MMC_Err = FAS_GetAxisStatus((BYTE)m_iPortNo, (BYTE)m_lAxis, &dwAxisStatus);

		state.dwValue = dwAxisStatus;

		if(m_MMC_Err == FMM_OK)
			return TRUE;
		else
			return FALSE;	
		
		if(dwAxisStatus & state.FFLAG_MOTIONING) return TRUE;
		else									 return FALSE;
	}
#endif
	return TRUE;
}

BOOL CAxis::bConnectFastechDriver()
{
//	DWORD dwBaudrate = 115200; // Baudrate 값. (초기값입니다. 설정에 의해 변경될 수 있습니다)
	DWORD dwBaudrate = 921600; // Baudrate 값. (초기값입니다. 설정에 의해 변경될 수 있습니다)
#ifdef _USE_FASTECH	
	if(FAS_Connect(m_iPortNo,dwBaudrate) == FALSE) //Default Baudrate
		return FALSE;
	else
	{
		if(FAS_IsSlaveExist((BYTE)m_iPortNo,(BYTE)m_lAxis))
			SetConnectState(true);
		else
		{
			SetConnectState(false);
			return FALSE;
		}
	}
#endif
	return TRUE;
}

BOOL CAxis::bDisConnectFastechDriver()
{
#ifdef _USE_FASTECH	
	FAS_Close(m_iPortNo);
	SetConnectState(false);
#endif
	return TRUE;
}

bool CAxis::bAjinMotorParamSaveAll()
{
	DWORD ret(TRUE);
	CString strFileName = SYSTEM_AXIS_AJINX_PATH;
#ifdef _USE_AJINXL
	int len = strFileName.GetLength();
	char* mbszFileName = new char[len+1];
	wcstombs(mbszFileName, strFileName.GetBuffer(0), len);
	ret = AxmMotSaveParaAll(mbszFileName); // 모든 축의 초기화 Data 저장 
		delete[] mbszFileName;
#endif
	if(!ret) return TRUE;
	else     return FALSE;
}

bool CAxis::bAjinMotorParamLoadAll()
{
	DWORD ret(TRUE);
	CString strFileName = SYSTEM_AXIS_AJINX_PATH;
#ifdef _USE_AJINXL
	int len = strFileName.GetLength();
	char* mbszFileName = new char[len+1];
	wcstombs(mbszFileName, strFileName.GetBuffer(0), len);
	ret = AxmMotLoadParaAll(mbszFileName); // 모든 축의 초기화 Data 저장
	delete[] mbszFileName;
#endif
	if(!ret) return TRUE;
	else     return FALSE;
}

bool CAxis::bAllParamSaveRom()
{
#ifdef _USE_FASTECH
	if(FAS_SaveAllParameters((BYTE)m_iPortNo,(BYTE)m_lAxis))
		return TRUE;
	else
		return FALSE;
#endif
	return true;
}

BOOL CAxis::StopVMove()
{
	int ret(0);

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		ret = v_move_stop((int)m_lAxis);
		if (AxisState() != NO_EVENT)
			return FALSE;
		if (ret == MMC_OK)
			return TRUE;
		else
			return FALSE;		
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		DWORD ret;
		g_AxisSyncIntlock.Lock();
		ret = AxmMoveSStop(m_lAxis);
		g_AxisSyncIntlock.Unlock();
		
		if (ret == AXT_RT_SUCCESS)
			return TRUE;
		else
			return FALSE;	
	}
#endif
#ifdef _USE_DNET
	//Need2Check
#endif

	return TRUE;
}

BOOL CAxis::MoveCPLine(int lCoordinate, double dPosX, double dPosY, double dVel, double dAcc)
{
	//연속으로 이어진 Axis 축기준으로 Mapping 된다. 
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif
	
	int ret(TRUE);
	long lPosSize = 2;
	short axis[2] = {(short)m_lAxis,(short)m_lAxis};
	long laxis[2] = {m_lAxis,m_lAxis};
	
	//20130901 lee
	if(GetAxisUse() == NOT_USE) return TRUE;

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		double dPos[2];

		dPos[0] = dPosX * GetPulse4Mm();
		dPos[1] = dPosY * GetPulse4Mm();
		
		map_axes(2,axis);	
		
		spl_line_move2(dPos, dVel, (INT)dAcc);
		
		ret = Wait4Done();
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		CString str;
		double dPos[2];

		dPos[0] = dPosX * GetPulse4Mm();
		dPos[1] = dPosY * GetPulse4Mm();

		AxmMotSetAccelUnit(m_lAxis,1);
		AxmMotSetAccelUnit(m_lAxis,1);

		AxmContiWriteClear(lCoordinate);
		AxmContiSetAxisMap(lCoordinate, lPosSize, laxis);
		// 상대위치 구동. 
		AxmContiSetAbsRelMode(lCoordinate, 0);
		// 직선보간 구동. 
		ret = AxmLineMove(lCoordinate, dPos, dVel, dAcc, dAcc);

		if(ret == AXT_RT_SUCCESS)
			return TRUE;
		else
		{	
			str.Format(L"AxmLineMove / lCoordinate[%d] : %d번 Error Return", lCoordinate, ret);
			AfxMessageBox(str);
			return FALSE;	
		}
	}
#endif
#ifdef _USE_DNET
	//Need2Check
#endif
	
	return ret;
}



void CAxis::SetOriginMethod()
{
	m_iOriginMethod = GetParasOriginMethod();
}

void CAxis::SetMm4Pulse()
{
	m_dMm4Pulse = GetParasMm4Pulse();
}

void CAxis::SetPulse4Mm()
{
	m_dPulse4Mm = GetParasPulse4Mm();
}
//<<

//>> 130317 JSPark
void CAxis::SetMotorType()
{
	m_iMotorType = GetParasMotorType();
}

void CAxis::SetPulseMethod()
{
	m_iPulseMethod = GetParasPulseMethod();
}

void CAxis::SetLoadRatioType()
{
	m_iLoadRatioType = GetParasLoadRatioType();
}


/// AjinExtek MLII/SigmaV 사용시 서보모터 부하율 얻기....[2013.03.01 by LJY]
bool CAxis::SetServoLodRatio(int iType)
{
#ifdef NOT_MACHINE
	return true;
#endif
#ifdef _USE_AJINXL
    // 지정축의부하율을읽을수있도록설정합니다.(MLII, Sigma-5 전용)
    // dwSelMon = 0 : Accumulated load ratio		// 가속부하
    // dwSelMon = 1 : Regenerative load ratio		// 동작부하
    // dwSelMon = 2 : Reference Torque load ratio	// 기준 회전력 부하
	if(GetControlType() == _AJINXL)
	{
		g_AxisSyncIntlock.Lock();			
		DWORD ret = AxmStatusSetReadServoLoadRatio(m_lAxis, iType);
		g_AxisSyncIntlock.Unlock();
		if (ret != AXT_RT_SUCCESS)
			return false;	
	}
#endif
	return true;
}

bool CAxis::GetServoLodRatio(double &dRatio)
{
#ifdef NOT_MACHINE
	return true;
#endif
	double	dValue = 0.0;
#ifdef _USE_AJINXL
	// 지정축의부하율을반환한다.(MLII, Sigma-5 전용)
	if(GetControlType() == _AJINXL)
	{
		if(GetMotorType() == 0) // Servo Motor일 경우 부하율 측정 한다....
		{	
			g_AxisSyncIntlock.Lock();			
			DWORD ret = AxmStatusReadServoLoadRatio(m_lAxis, &dValue);
			g_AxisSyncIntlock.Unlock();
			if (ret != AXT_RT_SUCCESS)
				return false;
		}
	}
#endif
#ifdef _USE_DNET
	//Need2Check
#endif
	dRatio = dValue;
	return true;
}


//////////////////////////////////////////////////////////////////////////
#ifdef _USE_FASTECH
// Fastech Motion I/O Use

bool CAxis::bCheckInputStatus(DWORD dwAdd)
{
	bool bRet(true);
	//DWORD dwIOInput;
	ULONGLONG dwIOInput;

	g_AxisSyncIntlock.Lock();
	if(FAS_GetIOInput((BYTE)m_iPortNo, (BYTE)m_lAxis, &dwIOInput) == FMM_OK)
	{
		if(dwIOInput & dwAdd)
			bRet = true;
		else
			bRet = false;
	}
	else
		bRet = false;
	g_AxisSyncIntlock.Unlock();

	return bRet;
}

bool CAxis::bCheckOutputStatus(DWORD dwAdd)
{
	bool bRet(true);
	DWORD dwIOOutput;

	g_AxisSyncIntlock.Lock();
	if(FAS_GetIOOutput((BYTE)m_iPortNo, (BYTE)m_lAxis, &dwIOOutput) == FMM_OK)
	{
		if(dwIOOutput & dwAdd)
			bRet = true;
		else
			bRet = false;
	}
	else
		bRet = false;
	g_AxisSyncIntlock.Unlock();

	return bRet;
}

bool CAxis::SetOutput(DWORD dwAdd, int nOnOff)
{
	bool bRet(true);
	DWORD dwIOCLRMask = 0;

	g_AxisSyncIntlock.Lock();
	if (nOnOff == ON)
	{
		if(FAS_SetIOOutput((BYTE)m_iPortNo, (BYTE)m_lAxis, dwAdd, dwIOCLRMask) != FMM_OK)
			bRet = false;
	}
	else
	{
		if(FAS_SetIOOutput((BYTE)m_iPortNo, (BYTE)m_lAxis, dwIOCLRMask, dwAdd) != FMM_OK)
			bRet = false;
	}
	g_AxisSyncIntlock.Unlock();

	return bRet;
}

#endif
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//>> 141202 LJY

/*
//IMSI 20160326
BOOL CAxis::MoveMultiAxis(long lSubAxis, double dPos, double dSPos, bool bCheckFlag, double dVel_Rate)
{
	if(dPos == 0.0 && dSPos == 0.0)
		return TRUE;

	BOOL bret = StartMoveMultiAxis(lSubAxis, dPos, dSPos, dVel_Rate);

	if(!bret)
		return FALSE;

	if(bCheckFlag)
	{
		if(Wait4Done())
			bret = g_Motion.m_pAxis[lSubAxis]->Wait4Done();
		else
			bret = FALSE;
	}

	return bret;
}

BOOL CAxis::StartMoveMultiAxis(long lSubAxis, double dPos, double dSPos, double dVel_Rate)
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif
	int val=0;
	double dVel(0.0);
	int nAcc(0), nDec(0);

	//20141212 JYLee
	if(GetAxisUse() == NOT_USE || g_Motion.m_pAxis[lSubAxis]->GetAxisUse() == NOT_USE) 
		return TRUE;

	ClearState();
	g_Motion.m_pAxis[lSubAxis]->ClearAxis();

	if(!GetAmpOnState() || !g_Motion.m_pAxis[lSubAxis]->GetAmpOnState())
		return FALSE;
	if(!GetAmpFaultState() || !g_Motion.m_pAxis[lSubAxis]->GetAmpFaultState())
		return FALSE;
	if(!isOriginReturn() || !g_Motion.m_pAxis[lSubAxis]->isOriginReturn())  
		return FALSE;

	if(!CheckSWLimit(dPos) || !g_Motion.m_pAxis[lSubAxis]->CheckSWLimit(dSPos))
		return FALSE;	

	dPos = dPos * GetPulse4Mm();
	dSPos = dSPos * g_Motion.m_pAxis[lSubAxis]->GetPulse4Mm();

	// JJH.Need2Check.속도 확인
	if(dVel_Rate == 0)
	{
		dVel = GetNormVel();
	}
	else
	{
		dVel = dVel_Rate;
	}

	// JJH.Need2Check.시스템 속도 적용 
	// dVel *= (g_SysData.GetSystemVelRatio() / 100.);

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		return TRUE;
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		return TRUE;
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		g_AxisSyncIntlock.Lock();

		BYTE nNoOfSlaves = 2;
		BYTE lAxis[2] = {(BYTE)m_lAxis,(BYTE)lSubAxis};
		long lPos[2] = {(long)dPos, (long)dSPos};

		m_MMC_Err = FAS_MoveLinearAbsPos((BYTE)m_iPortNo, nNoOfSlaves, lAxis, lPos, (long)dVel, (WORD)GetNormAcc()*5);

		g_AxisSyncIntlock.Unlock();
		if (m_MMC_Err == FMM_OK)
			return TRUE;
		else
			return FALSE;			
	}
#endif
	return TRUE;
}
//<<

//>> 141202 LJY
BOOL CAxis::RMoveMultiAxis(long lSubAxis, double dPos, double dSPos, bool bCheckFlag)
{
	if(dPos == 0.0 && dSPos == 0.0)
		return TRUE;

	int ret = StartRMoveMultiAxis(lSubAxis, dPos, dSPos);

	if(!ret)
		return FALSE;

	if(bCheckFlag)
	{
		if(Wait4Done())
			ret = g_Motion.m_pAxis[lSubAxis]->Wait4Done();
		else
			ret = FALSE;
	}

	return ret;
}

BOOL CAxis::StartRMoveMultiAxis(long lSubAxis, double dPos, double dSPos)
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif
	int val=0;
	double dVel(0.), dCurPos(0.);
	int nAcc(0), nDec(0);

	//20141212 JYLee
	if(GetAxisUse() == NOT_USE || g_Motion.m_pAxis[lSubAxis]->GetAxisUse() == NOT_USE) 
		return TRUE;

	ClearState();
	g_Motion.m_pAxis[lSubAxis]->ClearAxis();

	if(!GetAmpOnState() || !g_Motion.m_pAxis[lSubAxis]->GetAmpOnState())
		return FALSE;
	if(!GetAmpFaultState() || !g_Motion.m_pAxis[lSubAxis]->GetAmpFaultState())
		return FALSE;
	if(!isOriginReturn() || !g_Motion.m_pAxis[lSubAxis]->isOriginReturn())  
		return FALSE;

// 	GetPosition(dCurPos);
// 	dPos= dPos + dCurPos;
// 	g_Motion.m_pAxis[lSubAxis]->GetPosition(dCurPos);
// 	dSPos= dSPos + dCurPos;

	if(!CheckSWLimit(dPos) || !g_Motion.m_pAxis[lSubAxis]->CheckSWLimit(dSPos))
		return FALSE;	

	dPos = dPos * GetPulse4Mm();
	dSPos = dSPos * g_Motion.m_pAxis[lSubAxis]->GetPulse4Mm();

	dVel = GetNormVel();
	//시스템 속도 적용 
	dVel *= (g_SysData.GetSystemVelRatio() / 100.);
#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		return TRUE;
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		return TRUE;
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		BYTE nNoOfSlaves = 2;
		BYTE lAxis[2] = {(BYTE)m_lAxis,(BYTE)lSubAxis};
		long lPos[2] = {(long)dPos, (long)dSPos};

		g_AxisSyncIntlock.Lock();
		m_MMC_Err = FAS_MoveLinearIncPos((BYTE)m_iPortNo, nNoOfSlaves, lAxis, lPos, (long)GetNormVel(), (WORD)GetNormAcc());
		g_AxisSyncIntlock.Unlock();

		if (m_MMC_Err == FMM_OK)
			return TRUE;
		else
			return FALSE;			
	}
#endif
	return TRUE;
}
//<<
*/

/*
//IMSI 20160326
// 20141212 JYLee	//Need2Check
BOOL CAxis::MoveUVWThetapos(double dUPos, double dVPos, double dWPos, bool bCheckFlag )
{
	if(dUPos == 0.0 && dVPos == 0.0 && dWPos == 0.0)
		return TRUE;

	int ret = StartMoveUVWThetapos(dUPos, dVPos, dWPos);

	if(!ret)
		return FALSE;

	if(bCheckFlag)
	{
		if(Wait4Done())
		{
			if(g_Motion.m_pAxis[m_lAxis]->Wait4Done())
				ret = g_Motion.m_pAxis[m_lAxis+2]->Wait4Done();
			else
				ret = FALSE;
		}
		else
			ret = FALSE;
	}

	return ret;
}

//Need2Check
BOOL CAxis::StartMoveUVWThetapos(double dUPos, double dVPos, double dWPos)
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif
	int val=0;
	double dVel(0.);
	int nAcc(0), nDec(0);
	//20141212 JYLee
	if(GetAxisUse() == NOT_USE || g_Motion.m_pAxis[m_lAxis]->GetAxisUse() == NOT_USE || g_Motion.m_pAxis[m_lAxis+2]->GetAxisUse() == NOT_USE) 
		return TRUE;

	ClearState();
	g_Motion.m_pAxis[m_lAxis]->ClearAxis();
	g_Motion.m_pAxis[m_lAxis+2]->ClearAxis();

	if(!GetAmpOnState() || !g_Motion.m_pAxis[m_lAxis]->GetAmpOnState() || !g_Motion.m_pAxis[m_lAxis+2]->GetAmpOnState())
		return FALSE;
	if(!GetAmpFaultState() || !g_Motion.m_pAxis[m_lAxis]->GetAmpFaultState() || !g_Motion.m_pAxis[m_lAxis+2]->GetAmpFaultState())
		return FALSE;
	if(!isOriginReturn() || !g_Motion.m_pAxis[m_lAxis]->isOriginReturn() || !g_Motion.m_pAxis[m_lAxis+2]->isOriginReturn())  
		return FALSE;

	if(!CheckSWLimit(dUPos) || !g_Motion.m_pAxis[m_lAxis]->CheckSWLimit(dVPos) || !g_Motion.m_pAxis[m_lAxis+2]->CheckSWLimit(dWPos))
		return FALSE;	

	dUPos = dUPos * GetPulse4Mm();
	dVPos = dVPos * g_Motion.m_pAxis[m_lAxis]->GetPulse4Mm();
	dWPos = dWPos * g_Motion.m_pAxis[m_lAxis+2]->GetPulse4Mm();

	dVel = GetNormVel();
	//시스템 속도 적용 
	dVel *= (g_SysData.GetSystemVelRatio() / 100.);

#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		return TRUE;
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		return TRUE;
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		g_AxisSyncIntlock.Lock();

		BYTE nNoOfSlaves = 3;
		BYTE lAxis[3] = {(BYTE)m_lAxis,(BYTE)m_lAxis,(BYTE)m_lAxis+2};
		long lPos[3] = {(long)dUPos, (long)dVPos, (long)dWPos};

		m_MMC_Err = FAS_MoveLinearAbsPos((BYTE)m_iPortNo, nNoOfSlaves, lAxis, lPos, (long)GetNormVel(), (WORD)GetNormAcc());

		g_AxisSyncIntlock.Unlock();
		if (m_MMC_Err == FMM_OK)
			return TRUE;
		else
			return FALSE;			
	}
#endif
	return TRUE;
}

//Need2Check
BOOL CAxis::RMoveUVWThetapos(double dUPos, double dVPos, double dWPos, bool bCheckFlag)
{
	if(dUPos == 0.0 && dVPos == 0.0 && dWPos == 0.0)
		return TRUE;

	int ret = StartRMoveUVWThetapos(dUPos, dVPos, dWPos);

	if(!ret)
		return FALSE;

	if(bCheckFlag)
	{
		if(Wait4Done())
		{
			if(g_Motion.m_pAxis[m_lAxis]->Wait4Done())
				ret = g_Motion.m_pAxis[m_lAxis+2]->Wait4Done();
			else
				ret = FALSE;
		}
		else
			ret = FALSE;
	}

	return ret;
}

BOOL CAxis::StartRMoveUVWThetapos(double dUPos, double dVPos, double dWPos)
{
#ifdef NOT_MACHINE
	if(GetControlType() != _FASTECH)
		return FALSE;
#endif
	int val=0;
	double dVel(0.), dCurPos(0.);
	int nAcc(0), nDec(0);

	//20141212 JYLee
	if(GetAxisUse() == NOT_USE || g_Motion.m_pAxis[m_lAxis]->GetAxisUse() == NOT_USE || g_Motion.m_pAxis[m_lAxis+2]->GetAxisUse() == NOT_USE) 
		return TRUE;

	ClearState();
	g_Motion.m_pAxis[m_lAxis]->ClearAxis();
	g_Motion.m_pAxis[m_lAxis+2]->ClearAxis();

	if(!GetAmpOnState() || !g_Motion.m_pAxis[m_lAxis]->GetAmpOnState() || !g_Motion.m_pAxis[m_lAxis+2]->GetAmpOnState())
		return FALSE;
	if(!GetAmpFaultState() || !g_Motion.m_pAxis[m_lAxis]->GetAmpFaultState() || !g_Motion.m_pAxis[m_lAxis+2]->GetAmpFaultState())
		return FALSE;
	if(!isOriginReturn() || !g_Motion.m_pAxis[m_lAxis]->isOriginReturn() || !g_Motion.m_pAxis[m_lAxis+2]->isOriginReturn())  
		return FALSE;

	if(!CheckSWLimit(dUPos) || !g_Motion.m_pAxis[m_lAxis]->CheckSWLimit(dVPos) || !g_Motion.m_pAxis[m_lAxis+2]->CheckSWLimit(dWPos))
		return FALSE;	

// 	GetPosition(dCurPos);
// 	dUPos= dUPos + dCurPos;
// 	g_Motion.m_pAxis[m_lAxis]->GetPosition(dCurPos);
// 	dVPos= dVPos + dCurPos;
// 	g_Motion.m_pAxis[m_lAxis+2]->GetPosition(dCurPos);
// 	dWPos= dWPos + dCurPos;

	dUPos = dUPos * GetPulse4Mm();
	dVPos = dVPos * g_Motion.m_pAxis[m_lAxis]->GetPulse4Mm();
	dWPos = dWPos * g_Motion.m_pAxis[m_lAxis+2]->GetPulse4Mm();

	dVel = GetNormVel();
	//시스템 속도 적용 
	dVel *= (g_SysData.GetSystemVelRatio() / 100.);
#ifdef _USE_MMC
	if(GetControlType() == _MMC)
	{
		return TRUE;
	}
#endif
#ifdef _USE_AJINXL
	if(GetControlType() == _AJINXL)
	{
		return TRUE;
	}
#endif
#ifdef _USE_FASTECH
	if(GetControlType() == _FASTECH)
	{
		BYTE nNoOfSlaves = 3;
		BYTE lAxis[3] = {(BYTE)m_lAxis, (BYTE)m_lAxis, (BYTE)m_lAxis+2};
		long lPos[3] = {(long)dUPos, (long)dVPos, (long)dWPos};

		g_AxisSyncIntlock.Lock();
		m_MMC_Err = FAS_MoveLinearIncPos((BYTE)m_iPortNo, nNoOfSlaves, lAxis, lPos, (long)GetNormVel(), (WORD)GetNormAcc());
		g_AxisSyncIntlock.Unlock();

		if (m_MMC_Err == FMM_OK)
			return TRUE;
		else
			return FALSE;			
	}
#endif
	return TRUE;
}
*/
