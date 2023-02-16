#include "StdAfx.h"

#include "SDC_WindowControlPrj.h"

#include "Axis.h"
#include "MsgBox.h"									//����

//DIALOG
#include "DialogErrMsgBox.h"
#include "Clientsok.h"
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



CCriticalSection g_AxisSyncIntlock;

CAxis::CAxis(void)
{
	m_lAxis					= 0;
	m_iAxisUse				= 0;
	m_NormVel				= 0.0;
	m_JogVel				= 0.0;
	m_iMotorType			= SERVO_MOTOR;
	m_IOriginMethod			= 0;
	m_bOriginReturnFlag		= FALSE;
	m_bHomingErr			= FALSE;
	m_bThreadStarted		= FALSE;

	m_dTriggerPeriodPos = 0.0;
}


CAxis::~CAxis(void)
{
}

	
BOOL CAxis::SetTriggerTimeLevel()
{
	BOOL rslt = TRUE;

	if( AXT_RT_SUCCESS != AxmTriggerSetTimeLevel(m_lAxis, 1, HIGH, 0, 0) )
		rslt = FALSE;

	return rslt;
}

BOOL CAxis::SetTriggerBlock(double dStartPos, double dEndPos, double dPeriodPos)
{
	BOOL rslt = TRUE;

	AxmTriggerSetBlock(m_lAxis, dStartPos , dEndPos, dPeriodPos);

	return rslt;
}

void CAxis::SetTriggerReset()
{
	AxmTriggerSetReset(m_lAxis);
}

DWORD CAxis::ReadOriginResult()
{
	DWORD	dwRetCode;

	AxmHomeGetResult(m_lAxis, &dwRetCode);

	return dwRetCode;
}

BOOL CAxis::StartOrigin()
{
	BOOL rslt = TRUE;
	DWORD	dwRetCode;

	//++ ������ �࿡ �����˻��� �����մϴ�.
	dwRetCode = AxmHomeSetStart(m_lAxis);	
	if(dwRetCode != AXT_RT_SUCCESS){
		rslt = FALSE;
	}

	return rslt;
}

BOOL CAxis::SetOriginMethod()
{
	DWORD	dwRetCode;
	BOOL rslt = TRUE;
	
	//++ ������ ���� �����˻� ����� �����մϴ�.
	//AxmHomeSetSignalLevel(m_lAxisNo, dwLevel);
	if( m_lAxis == AXIS_TRY_SHUTTLE )
	{
		dwRetCode	= AxmHomeSetSignalLevel(m_lAxis, 1);
		if(dwRetCode != AXT_RT_SUCCESS){
			rslt = FALSE;
		}

		//AxmHomeSetMethod(m_lAxisNo, lHomeDir, dwHomeSignal, dwZPhaseUse, dHomeClrTime, dHomeOffset);
		dwRetCode = AxmHomeSetMethod(m_lAxis, 0, 4, 0, 1000.0, 0.0);
		if(dwRetCode != AXT_RT_SUCCESS){
			rslt = FALSE;
		}

		//AxmHomeSetVel(m_lAxisNo, dVelFirst, dVelSecond, dVelThird, dVelLast, dAccFirst, dAccSecond);
		//dwRetCode = AxmHomeSetVel(m_lAxis, 500000, 200000, 10000.0, 500.0, 1000.0, 1000.0);
		dwRetCode = AxmHomeSetVel(m_lAxis, m_HomeVel, m_HomeVel2, m_HomeVel3, 500.0, m_HomeAcc, 1000.0);
		if(dwRetCode != AXT_RT_SUCCESS){
			rslt = FALSE;
		}
	}
	else if( m_lAxis == AXIS_LD_PNP )
	{
		dwRetCode	= AxmHomeSetSignalLevel(m_lAxis, 1);
		if(dwRetCode != AXT_RT_SUCCESS){
			rslt = FALSE;
		}

		//AxmHomeSetMethod(m_lAxisNo, lHomeDir, dwHomeSignal, dwZPhaseUse, dHomeClrTime, dHomeOffset);
		dwRetCode = AxmHomeSetMethod(m_lAxis, 0, 4, 0, 1000.0, 0.0);
		if(dwRetCode != AXT_RT_SUCCESS){
			rslt = FALSE;
		}

		//AxmHomeSetVel(m_lAxisNo, dVelFirst, dVelSecond, dVelThird, dVelLast, dAccFirst, dAccSecond);
		//dwRetCode = AxmHomeSetVel(m_lAxis, 500000, 200000, 10000.0, 500.0, 1000.0, 1000.0);
		dwRetCode = AxmHomeSetVel(m_lAxis, m_HomeVel, m_HomeVel2, m_HomeVel3, 500.0, m_HomeAcc, 1000.0);
		if(dwRetCode != AXT_RT_SUCCESS){
			rslt = FALSE;
		}
	}
	return rslt;
}

//�� ������Ʈ������ ������� �ʴ´�.
BOOL CAxis::AXTLibInit()
{
	BOOL bRet(TRUE);
	// Initialize library 
	DWORD Code = AxlOpenNoReset(7);
	if ( Code == AXT_RT_SUCCESS)
		TRACE("Library is initialized .\n");
	else
	{
		AfxMessageBox(_T("Failed initialization."));
		return FALSE;
	}

	return TRUE;
}

BOOL CAxis::AXTMotionInit()
{

	/*
	CString strInfo;
	DWORD Code;
	//Module Ȯ�� 
	DWORD dwStatus;

	if(!AxlIsOpened())
	{ 
		if(!AXTLibInit())
			return FALSE;
	}

	// �� [CAUTION] �Ʒ��� �ٸ� Mot����(��� ��������)�� ����� ��� ��θ� �����Ͻʽÿ�.
	char szFilePath[] = "D:\\SDC_WindowControlPrj\\MotFile\\MotionDefault.mot";
	
	//++ ������ Mot������ ��������� ��Ǻ����� ���������� �ϰ����� �����մϴ�.
	if(AxmMotLoadParaAll(szFilePath) != AXT_RT_SUCCESS)
	{
		TRACE(_T("Mot File Not Found."));
	}


	Code = AxmInfoIsMotionModule(&dwStatus);

	if(Code == AXT_RT_SUCCESS)
	{
		if(dwStatus == STATUS_EXIST)
		{
			// �� ������ �о� �´�. �⺻ ����...���� ���� �׸����� �ٽ� ���� �Ѵ�. 
 			m_pAxis[0]->bAjinMotorParamLoadAll();
		}
		else
			AfxMessageBox(_T("Ajin Motion Module �� Ȯ�ε��� �ʾҽ��ϴ�."));
	}
	*/

	return TRUE;
}

BOOL CAxis::GetNegLimitLevel(long &level)
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

	return TRUE;
}

BOOL CAxis::GetPosLimitLevel(long &level)
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

	return TRUE;
}

void CAxis::SetAxisAccelUnit()
{
	AxmMotSetAccelUnit(m_lAxis,1);
	AxmMotSetMinVel(m_lAxis,1);
}


BOOL CAxis::LoadAxisConfig()
{
	BOOL rslt = TRUE;
	CString strNode, strKey, strData, strFilePath;
	double ACC_DEC_RATIO(0.001);

	CProfileDataIF cDataIf;
	cDataIf.SetFilePath(DATA_AXIS_CONFIG_FILE);

	strNode = m_strAxisName + _T("_AXIS");

	m_lStopDec = cDataIf.GetInt(strNode, _T("Event Stop Rate"), 100); 
	m_NormVel = cDataIf.GetDouble(strNode, _T("Normal Velocity"), 1000.0);
	m_NormAcc = cDataIf.GetDouble(strNode, _T("Normal Accel Time"), 50) * ACC_DEC_RATIO;
	m_NormDec = cDataIf.GetDouble(strNode, _T("Normal Decel Time"), 50) * ACC_DEC_RATIO;
	m_dMaxVel = cDataIf.GetDouble(strNode, _T("Normal Max Velocity"), 1000000);
	m_dMinVel = cDataIf.GetDouble(strNode, _T("Normal Min Velocity"), 1.0);
	m_dMaxAccTime = cDataIf.GetDouble(strNode, _T("Normal Max Accel Time"), 2000) * ACC_DEC_RATIO;
	m_dMinAccTime = cDataIf.GetDouble(strNode, _T("Normal Min Accel Time"), 20) * ACC_DEC_RATIO;
	m_HomeVel = cDataIf.GetDouble(strNode, _T("Home Velocity"), 10000.0);
	m_HomeVel2 = cDataIf.GetDouble(strNode, _T("Home Velocity2"), 1000.0);
	m_HomeVel3 = cDataIf.GetDouble(strNode, _T("Home Velocity3"), 100.0);
	m_HomeAcc = cDataIf.GetDouble(strNode, _T("Home Accel Time"), 50) * ACC_DEC_RATIO;
	m_HomeDec = cDataIf.GetDouble(strNode, _T("Home Decel Time"), 50) * ACC_DEC_RATIO;
	m_HomeFineVel = cDataIf.GetDouble(strNode, _T("Home Fine Velocity"), 100.0);
	m_HomeFineAcc = cDataIf.GetDouble(strNode, _T("Home Fine Accel Time"), 50) * ACC_DEC_RATIO;
	m_dMaxHomeVel = cDataIf.GetDouble(strNode, _T("Home Max Velocity"), 100000.0);
	m_dMinHomeVel = cDataIf.GetDouble(strNode, _T("Home Min Velocity"), 1.0);
	m_dMaxHomeAccTime = cDataIf.GetDouble(strNode, _T("Home Max Accel Time"), 2000) * ACC_DEC_RATIO;
	m_dMinHomeAccTime = cDataIf.GetDouble(strNode, _T("Home Min Accel Time"), 20) * ACC_DEC_RATIO;
	m_JogVel = cDataIf.GetDouble(strNode, _T("Jog Velocity"), 1000.0);
	m_JogAcc = cDataIf.GetDouble(strNode, _T("Jog Accel Time"), 50) * ACC_DEC_RATIO;
	m_JogDec = cDataIf.GetDouble(strNode, _T("Jog Decel Time"), 50) * ACC_DEC_RATIO;
	m_dMaxJogVel = cDataIf.GetDouble(strNode, _T("Jog Max Velocity"), 200000.0);
	m_dMinJogVel = cDataIf.GetDouble(strNode, _T("Jog Min Velocity"), 1.0);
	m_dMaxJogAccTime = cDataIf.GetDouble(strNode, _T("Jog Max Accel Time"), 2000) * ACC_DEC_RATIO;
	m_dMinJogAccTime = cDataIf.GetDouble(strNode, _T("Jog Min Accel Time"), 20) * ACC_DEC_RATIO;
	m_dPosSWLimit = cDataIf.GetDouble(strNode, _T("Positive SW Limit Pos"), 100000.0);
	m_dNegSWLimit = cDataIf.GetDouble(strNode, _T("Negative SW Limit Pos"), -100000.0);
	//�߰� 
	m_dAxisInitPos = cDataIf.GetDouble(strNode, _T("Axis Init Pos"), 0.0);	
	m_dAxisSafetyPos = cDataIf.GetDouble(strNode, _T("Axis Safety Pos"), 0.0);
	m_dOrgOffset = cDataIf.GetDouble(strNode, _T("Axis Orgin Offset"), 0.0);

	m_dTriggerPeriodPos = cDataIf.GetDouble(strNode, _T("Axis Trigger Period"), 0.0);
	//int laxis = GetAxisNo();		

	return rslt;
}

BOOL CAxis::SaveAxisConfig() //130219 JSPark
{
	CProfileDataIF cDataIf;
	cDataIf.SetFilePath(DATA_AXIS_CONFIG_FILE);

	int msec = 0;
	double ACC_DEC_RATIO(1000.0); //�ڿ��� ��� �Ѵ�. 
	CString strNode;
	strNode = m_strAxisName + _T("_AXIS");

	cDataIf.SetInt(strNode, _T("Event E-Stop Rate"), msec);
	cDataIf.SetDouble(strNode, _T("Normal Velocity"), m_NormVel);
	cDataIf.SetDouble(strNode, _T("Normal Accel Time"), GetNormAcc() * ACC_DEC_RATIO);
	cDataIf.SetDouble(strNode, _T("Normal Decel Time"), GetNormDec() * ACC_DEC_RATIO);
	cDataIf.SetDouble(strNode, _T("Normal Max Velocity"), m_dMaxVel);
	cDataIf.SetDouble(strNode, _T("Normal Min Velocity"), m_dMinVel);
	cDataIf.SetDouble(strNode, _T("Normal Max Accel Time"), m_dMaxAccTime * ACC_DEC_RATIO);
	cDataIf.SetDouble(strNode, _T("Normal Min Accel Time"), m_dMinAccTime * ACC_DEC_RATIO);
	cDataIf.SetDouble(strNode, _T("Home Velocity"), m_HomeVel);
	cDataIf.SetDouble(strNode, _T("Home Velocity2"), m_HomeVel2);
	cDataIf.SetDouble(strNode, _T("Home Velocity3"), m_HomeVel3);
	cDataIf.SetDouble(strNode, _T("Home Accel Time"), GetHomeAcc() * ACC_DEC_RATIO);
	cDataIf.SetDouble(strNode, _T("Home Decel Time"), GetHomeDec() * ACC_DEC_RATIO);
	cDataIf.SetDouble(strNode, _T("Home Fine Velocity"), m_HomeFineVel);
	cDataIf.SetDouble(strNode, _T("Home Fine Accel Time"), GetHomeFineAcc() * ACC_DEC_RATIO);
	cDataIf.SetDouble(strNode, _T("Home Max Velocity"), m_dMaxHomeVel);
	cDataIf.SetDouble(strNode, _T("Home Min Velocity"), m_dMinHomeVel);
	cDataIf.SetDouble(strNode, _T("Home Max Accel Time"), m_dMaxHomeAccTime * ACC_DEC_RATIO);
	cDataIf.SetDouble(strNode, _T("Home Min Accel Time"), m_dMinHomeAccTime * ACC_DEC_RATIO);
	cDataIf.SetDouble(strNode, _T("Jog Velocity"), m_JogVel);
	cDataIf.SetDouble(strNode, _T("Jog Accel Time"), GetJogAcc() * ACC_DEC_RATIO);
	cDataIf.SetDouble(strNode, _T("Jog Decel Time"), GetJogDec() * ACC_DEC_RATIO);
	cDataIf.SetDouble(strNode, _T("Jog Max Velocity"), m_dMaxJogVel);
	cDataIf.SetDouble(strNode, _T("Jog Min Velocity"), m_dMinJogVel);
	cDataIf.SetDouble(strNode, _T("Jog Max Accel Time"), m_dMaxJogAccTime * ACC_DEC_RATIO);
	cDataIf.SetDouble(strNode, _T("Jog Min Accel Time"), m_dMinJogAccTime * ACC_DEC_RATIO);
	cDataIf.SetDouble(strNode, _T("Positive SW Limit Pos"), m_dPosSWLimit);
	cDataIf.SetDouble(strNode, _T("Negative SW Limit Pos"), m_dNegSWLimit);
	cDataIf.SetDouble(strNode, _T("Axis Orgin Offset"), m_dOrgOffset);
	//�߰�
	cDataIf.SetDouble(strNode, _T("Axis Init Pos"), m_dAxisInitPos);
	cDataIf.SetDouble(strNode, _T("Axis Safety Pos"), m_dAxisSafetyPos);
	cDataIf.SetDouble(strNode, _T("Axis Orgin Offset"), m_dOrgOffset);

	cDataIf.SetDouble(strNode, _T("Axis Trigger Period"), m_dTriggerPeriodPos);
	
	return TRUE;
}

BOOL CAxis::VMove(double Velocity, double Acceleration, double Deceleration)
{
	DWORD ret;
	
	//20130901 lee
	if(GetAxisUse() == NOT_USE) return TRUE;
	
	g_AxisSyncIntlock.Lock();			
	AxmMotSetAccelUnit(m_lAxis,1);
	AxmMotSetMinVel(m_lAxis,1);
	ret = AxmMoveVel(m_lAxis, Velocity, Acceleration, Acceleration);
	g_AxisSyncIntlock.Unlock();
		
	if (ret == AXT_RT_SUCCESS)
		return TRUE;
	else
		return FALSE;	

	return TRUE;
}

BOOL CAxis::GetHomeSensor()
{
	DWORD upStatus; 
	g_AxisSyncIntlock.Lock();		
	AxmHomeReadSignal (m_lAxis, &upStatus);
	g_AxisSyncIntlock.Unlock();
		
	if (upStatus)
		return TRUE;
	else
		return FALSE;	

	return TRUE;
}
BOOL CAxis::GetNegLimitSensor()
{
	DWORD upPositiveLevel, upNegativeLevel;
	g_AxisSyncIntlock.Lock();
	AxmSignalReadLimit (m_lAxis, &upPositiveLevel, &upNegativeLevel);
	g_AxisSyncIntlock.Unlock();
		
	if (upNegativeLevel)
		return TRUE;
	else
		return FALSE;	
	return TRUE;
}
BOOL CAxis::GetPosLimitSensor()
{
	DWORD upPositiveLevel, upNegativeLevel;
	g_AxisSyncIntlock.Lock();
	AxmSignalReadLimit (m_lAxis, &upPositiveLevel, &upNegativeLevel);
	g_AxisSyncIntlock.Unlock();
		
	if (upPositiveLevel)
		return TRUE;
	else
		return FALSE;	

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

BOOL CAxis::StartRMove(double dPos)
{
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
	
	int val=0;
	double dVel(0.);
	int nAcc(0), nDec(0);
	double dCurPos(0.);

	if(GetAxisUse() == NOT_USE) 
		return TRUE;

	if(!GetAmpOnState())
		return FALSE;

	if(!GetAmpFaultState())
		return FALSE;

	if(!isOriginReturn())
		return FALSE;

	if(!CheckSWLimit(dPos))
		return FALSE;	

	dPos = dPos * GetPulse4Mm();
	
	dVel = GetNormVel();
	//�ý��� �ӵ� ���� 
	dVel *= (pdlg->m_pDlgMainData->GetSystemVelRatio() / 100.);
	
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
		str.Format(_T("Ajin B/D Error�߻�  %s�� Motion Done Error"),GetAxisName());
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

BOOL CAxis::StartMove(double dPos,  double dVelRatio)
{
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	int val=0;
	DWORD ret(0);

	double dVel(0.);
	int    nAcc(0), nDec(0);
	CString str;
	DWORD uAbsRelMode = POS_ABS_MODE;
	DWORD uProfileMode = SYM_S_CURVE_MODE;

	if(GetAxisUse() == NOT_USE) 
		return TRUE;

	if(!GetAmpOnState())
		return FALSE;

	if(!GetAmpFaultState())
		return FALSE;

	if(!isOriginReturn())
		return FALSE;

	if(!CheckSWLimit(dPos))
		return FALSE;

	dPos = dPos * GetPulse4Mm();

	//������ �ӵ��� �ٸ��� �ϱ� ���� �ӵ��� �߰���.
	if( (1. > dVelRatio) || (8. < dVelRatio))
		dVelRatio = 1.;
	dVel = GetNormVel() * dVelRatio;	

	if(dVel == 0) dVel = 10000.0;

	//�ý��� �ӵ� ���� 
	dVel *= (pdlg->m_pDlgMainData->GetSystemVelRatio() / 100.);

	g_AxisSyncIntlock.Lock();
	ret = Wait4Done();
	if(!ret) 
	{
		g_AxisSyncIntlock.Unlock();
		str.Format(_T("Ajin B/D Error�߻�  %s�� Motion Done Error"),GetAxisName());
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
		str.Format(_T("Ajin B/D Error�߻�  %s�� Error CODE : %d"),GetAxisName(), ret);
		AfxMessageBox(str);
		return FALSE;	
	}

	return TRUE;
}

BOOL CAxis::Move(double dPos,  double dVelRatio, BOOL bWaitFlag) 
{
	int ret;
	double dCurPos;

	GetPosition(dCurPos);
	double temp = fabs(dPos - dCurPos); 

	if(temp == 0) return TRUE;

	ret = StartMove(dPos, dVelRatio);
	
	if(!ret)
		return FALSE;

	if(bWaitFlag)
		ret = Wait4Done(dPos);
	return ret;
}

BOOL CAxis::GetPosition(double &rPos, BOOL bEncoder) //130228 JSPark
{
	int ret(0);

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

	return TRUE;
}

BOOL CAxis::SetStopRate(int msec)
{
	DWORD ret(TRUE);
	g_AxisSyncIntlock.Lock();			
	m_lStopDec = msec;
	g_AxisSyncIntlock.Unlock();
		
	if (ret == AXT_RT_SUCCESS)
		return TRUE;
	else
		return FALSE;	

	return TRUE;
}

BOOL CAxis::SetServoLodRatio(int iType)
{
	g_AxisSyncIntlock.Lock();			
	DWORD ret = AxmStatusSetReadServoLoadRatio(m_lAxis, iType);
	g_AxisSyncIntlock.Unlock();
	if (ret != AXT_RT_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL CAxis::SetPositiveLimit(int act)
{
	/*
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

	*/
	return TRUE;
}

BOOL CAxis::SetNegativeLimit(int act)
{
	/*
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

	*/
	return TRUE;
}


BOOL CAxis::EStop()
{
	DWORD ret;
	g_AxisSyncIntlock.Lock();	
	ret = AxmMoveEStop (m_lAxis);
	g_AxisSyncIntlock.Unlock();
		
	if (ret == AXT_RT_SUCCESS)
		return TRUE;
	else
		return FALSE;

	return TRUE;
}

BOOL CAxis::Stop()
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

	return TRUE;
}

BOOL CAxis::MotionDone()
{
	DWORD Status;

	g_AxisSyncIntlock.Lock();
	AxmStatusReadInMotion((int)m_lAxis, &Status); //Motion ���̸� TRUE / �ƴϸ� FALSE
	g_AxisSyncIntlock.Unlock();

	if(Status != FALSE) 
		return FALSE; 
	else
		return TRUE;

	return TRUE;
}

BOOL CAxis::CheckHomingDone()
{
	if(!IsHomeFinished())
		return FALSE;

	return TRUE;
}

BOOL CAxis::Wait4Done(double dPos, BOOL bUpperInterfaceInterlockCheck, BOOL bLowerInterfaceInterlockCheck)
{
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



BOOL CAxis::SetAmpFault()
{
	BOOL rslt;

	if(ResetAmpFault()) 
		rslt = TRUE;
	else
		rslt = FALSE;

	return rslt;
}

BOOL CAxis::ResetAmpFault()
{
	BOOL rslt;
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
		rslt = TRUE;
	else
		rslt = FALSE;

	return rslt;
}

BOOL CAxis::SetPosition(double pos)
{
	BOOL rslt;
	DWORD ret;
	g_AxisSyncIntlock.Lock();
	//RTEX ���� �Լ��̳� ML2�� �Բ� ����ص� �����ϴ�.
	ret = AxmStatusSetPosMatch(m_lAxis, pos * GetMm4Pulse());

	g_AxisSyncIntlock.Unlock();	

	if (ret == AXT_RT_SUCCESS)
		rslt = TRUE;
	else
		rslt = FALSE;

	return rslt;
}

BOOL CAxis::SetElectricGear()
{
	   // ���� �ӵ� ������ RPM(Revolution Per Minute)���� ���߰� �ʹٸ�.
    // ex>    rpm ���:
    // 4500 rpm ?
    // unit/ pulse = 1 : 1�̸�      pulse/ sec �ʴ� �޽����� �Ǵµ�
    // 4500 rpm�� ���߰� �ʹٸ�     4500 / 60 �� : 75ȸ��/ 1��
    // ���Ͱ� 1ȸ���� �� �޽����� �˾ƾ� �ȴ�. �̰��� Encoder�� Z���� �˻��غ��� �˼��ִ�.
    // 1ȸ��:1800 �޽���� 75 x 1800 = 135000 �޽��� �ʿ��ϰ� �ȴ�.
    // AxmMotSetMoveUnitPerPulse�� Unit = 1, Pulse = 1800 �־� ���۽�Ų��.
    // ���ǻ��� : rpm���� �����ϰ� �ȴٸ� �ӵ��� ���ӵ� �� rpm���� ������ �����Ͽ��� �Ѵ�.
    
    // ���� ���� �޽� �� �����̴� �Ÿ��� �����Ѵ�.
	BOOL rslt;

	DWORD ret;
	long lAxisNo = 0;
	long lPulse = 1; 
	double dUnit = 1;

	g_AxisSyncIntlock.Lock();			
	ret = AxmMotSetMoveUnitPerPulse(m_lAxis, dUnit, lPulse);
	g_AxisSyncIntlock.Unlock();

	if (ret == AXT_RT_SUCCESS)
		rslt = TRUE;
	else
		rslt = FALSE;

	return rslt;
}

BOOL CAxis::SetDirection(DWORD dwMethod)
{
	//uMethod  0 :OneHighLowHigh, 1 :OneHighHighLow, 2 :OneLowLowHigh, 3 :OneLowHighLow, 4 :TwoCcwCwHigh
    //         5 :TwoCcwCwLow,    6 :TwoCwCcwHigh,   7 :TwoCwCcwLow,   8 :TwoPhase,      9 :TwoPhaseReverse

	BOOL rslt;
	DWORD ret;
	g_AxisSyncIntlock.Lock();			
	ret = AxmMotSetPulseOutMethod (m_lAxis, dwMethod); //20130330 lee
	g_AxisSyncIntlock.Unlock();

	if (ret == AXT_RT_SUCCESS)
		rslt = TRUE;
	else
		rslt = FALSE;

	return rslt;
}

BOOL CAxis::SetAmpFaultResetLevel(DWORD dwLv)
{
	 // ���� ���� Servo-Alarm Reset ��ȣ�� ��� ������ �����Ѵ�.
    // uLevel : LOW(0), HIGH(1)

	BOOL rslt;
	DWORD ret;
	g_AxisSyncIntlock.Lock();			
	ret = AxmSignalSetServoAlarmResetLevel (m_lAxis, dwLv);
	g_AxisSyncIntlock.Unlock();

	if (ret == AXT_RT_SUCCESS)
		rslt = TRUE;
	else
		rslt = FALSE;

	return rslt;
}

BOOL CAxis::SetAmpfaultlevel(DWORD dwLv)
{
	// ���� ���� �˶� ��ȣ �Է� �� ��� ������ ��� ���� �� ��ȣ �Է� ������ �����Ѵ�.
	// uLevel : LOW(0), HIGH(1), UNUSED(2), USED(3)
	BOOL rslt;
	DWORD ret;
	g_AxisSyncIntlock.Lock();			
	ret = AxmSignalSetServoAlarm (m_lAxis, dwLv);
	g_AxisSyncIntlock.Unlock();

	if (ret == AXT_RT_SUCCESS)
		rslt = TRUE;
	else
		rslt = FALSE;

	return rslt;
}

BOOL CAxis::SetAmpEnableLevel(DWORD dwLv)
{
	// ���� ���� Servo-On��ȣ�� ��� ������ �����Ѵ�
    // uLevel : LOW(0), HIGH(1)

	BOOL rslt;
	DWORD ret;

	g_AxisSyncIntlock.Lock();			
	ret = AxmSignalSetServoOnLevel (m_lAxis, dwLv);
	g_AxisSyncIntlock.Unlock();

	if (ret == AXT_RT_SUCCESS)
		rslt = TRUE;
	else
		rslt = FALSE;

	return rslt;
}

BOOL CAxis::SetHomeLevel(DWORD dwLv)
{
	// ���� ���� Home ���� Level �� �����Ѵ�.
    // uLevel : LOW(0), HIGH(1)

	BOOL rslt;
	DWORD ret; 
	g_AxisSyncIntlock.Lock();		
	ret = AxmHomeSetSignalLevel(m_lAxis, dwLv);
	g_AxisSyncIntlock.Unlock();

	if (ret == AXT_RT_SUCCESS)
		rslt = TRUE;
	else
		rslt = FALSE;

	return rslt;
}

 // ���� ���� end limit sensor�� ��� ���� �� ��ȣ�� �Է� ������ �����Ѵ�. 
    // end limit sensor ��ȣ �Է� �� �������� �Ǵ� �������� ���� ������ �����ϴ�.
    // uStopMode: EMERGENCY_STOP(0), SLOWDOWN_STOP(1)
    // uPositiveLevel, uNegativeLevel : LOW(0), HIGH(1), UNUSED(2), USED(3)
BOOL CAxis::SetNegativeLevel(DWORD dwLv)
{
	BOOL rslt;
	DWORD ret; 
	DWORD StopMode, PositiveLevel, NegativeLevel;	
	g_AxisSyncIntlock.Lock();			
	AxmSignalGetLimit (m_lAxis, &StopMode, &PositiveLevel, &NegativeLevel);
		
	NegativeLevel = dwLv;
	ret = AxmSignalSetLimit (m_lAxis, StopMode, PositiveLevel, NegativeLevel);
	g_AxisSyncIntlock.Unlock();

	if (ret == AXT_RT_SUCCESS)
		rslt = TRUE;
	else
		rslt = FALSE;

	return rslt;
}

 // ���� ���� end limit sensor�� ��� ���� �� ��ȣ�� �Է� ������ �����Ѵ�. 
    // end limit sensor ��ȣ �Է� �� �������� �Ǵ� �������� ���� ������ �����ϴ�.
    // uStopMode: EMERGENCY_STOP(0), SLOWDOWN_STOP(1)
    // uPositiveLevel, uNegativeLevel : LOW(0), HIGH(1), UNUSED(2), USED(3)
BOOL CAxis::SetPositiveLevel(DWORD dwLv)
{
	BOOL rslt;
	DWORD ret; 
	DWORD StopMode, PositiveLevel, NegativeLevel;	
		
	g_AxisSyncIntlock.Lock();			
	AxmSignalGetLimit (m_lAxis, &StopMode, &PositiveLevel, &NegativeLevel);
	PositiveLevel = dwLv;
	ret = AxmSignalSetLimit (m_lAxis, StopMode, PositiveLevel , NegativeLevel);
	g_AxisSyncIntlock.Unlock();
		
	if (ret == AXT_RT_SUCCESS)
		rslt = TRUE;
	else
		rslt = FALSE;

	return rslt;
}

BOOL CAxis::SetEncoderInput(DWORD dwLv)
{
	BOOL rslt;
	DWORD ret; 
	g_AxisSyncIntlock.Lock();		
	ret = AxmMotSetEncInputMethod(m_lAxis, dwLv);
	g_AxisSyncIntlock.Unlock();

	if (ret == AXT_RT_SUCCESS)
		rslt = TRUE;
	else
		rslt = FALSE;

	return rslt;
}

BOOL CAxis::SetZPhase(DWORD dwLv)
{
	BOOL rslt;
	DWORD ret; 
	g_AxisSyncIntlock.Lock();		
	ret = AxmSignalSetZphaseLevel(m_lAxis, dwLv);
	g_AxisSyncIntlock.Unlock();

	if (ret == AXT_RT_SUCCESS)
		rslt = TRUE;
	else
		rslt = FALSE;

	return rslt;
}


BOOL CAxis::bAjinMotorParamSaveAll()
{
	BOOL rslt;
	DWORD ret(TRUE);
	CString strFileName = SYSTEM_AXIS_AJINX_PATH;

	int len = strFileName.GetLength();
	char* mbszFileName = new char[len+1];
	//wcstombs(mbszFileName, strFileName.GetBuffer(0), len);
	sprintf(mbszFileName, "%S", strFileName);
	ret = AxmMotSaveParaAll(mbszFileName); // ��� ���� �ʱ�ȭ Data ����
	delete[] mbszFileName;

	if(!ret) 
		rslt = TRUE;
	else     
		rslt = FALSE;

	return rslt;
}

BOOL CAxis::bAjinMotorParamLoadAll()
{
	BOOL rslt;
	DWORD ret(TRUE);
	CString strFileName = SYSTEM_AXIS_AJINX_PATH;

	int len = strFileName.GetLength();
	char* mbszFileName = new char[len+1];
	//wcstombs(mbszFileName, strFileName.GetBuffer(0), len);
	sprintf(mbszFileName, "%S", strFileName);
	ret = AxmMotLoadParaAll(mbszFileName); // ��� ���� �ʱ�ȭ Data ����
	delete[] mbszFileName;

	if(!ret) 
		rslt = TRUE;
	else     
		rslt = FALSE;

	return rslt;
}


BOOL CAxis::AmpEnable()
{
	DWORD ret;
	BOOL rslt;

	g_AxisSyncIntlock.Lock();
	ret = AxmSignalServoOn(m_lAxis, ENABLE);
	g_AxisSyncIntlock.Unlock();
		
	if (ret == AXT_RT_SUCCESS)
		rslt = TRUE;
	else
		rslt = FALSE;	

	return rslt;
}

BOOL CAxis::AmpDisable()
{
	DWORD ret;
	BOOL rslt;

	g_AxisSyncIntlock.Lock();
	ret = AxmSignalServoOn(m_lAxis, DISABLE);
	g_AxisSyncIntlock.Unlock();
		
	if (ret == AXT_RT_SUCCESS)
	{
		m_bOriginReturnFlag = FALSE;//JMHWANG 130915 
		rslt = TRUE;
	}
	else
		rslt = FALSE;	

	return rslt;
}

BOOL CAxis::GetAmpOnState()
{
	DWORD upStatus;
	BOOL rslt = FALSE;

	g_AxisSyncIntlock.Lock();			
	AxmSignalIsServoOn(m_lAxis, &upStatus);
	g_AxisSyncIntlock.Unlock();
		
	if(upStatus)
		rslt = TRUE;
	else
	{
		SetOriginReturn(false); //131020 JSPark
		rslt = FALSE;
	}

	return rslt;
}

BOOL CAxis::GetAmpFaultState()
{
	DWORD upStatus;
	BOOL rslt = FALSE;

	g_AxisSyncIntlock.Lock();	
	AxmSignalReadServoAlarm(m_lAxis, &upStatus);
	g_AxisSyncIntlock.Unlock();
		
	if(upStatus)
	{
		SetOriginReturn(FALSE);
		rslt = FALSE;
	}
	else
		rslt = TRUE;

	return rslt;
}

BOOL CAxis::IsHomeFinished()
{
	BOOL rslt = FALSE;

	g_AxisSyncIntlock.Lock();
	if(isOriginReturn())
	{
		rslt = TRUE;
	}
	else
	{
		rslt = FALSE;
	}
	g_AxisSyncIntlock.Unlock();

	return rslt;
}

void CAxis::SetOriginReturn(BOOL org)
{
	CString strMsg;

	g_AxisSyncIntlock.Lock();
	m_bOriginReturnFlag = org;
	g_AxisSyncIntlock.Unlock();
}

BOOL CAxis::isOriginReturn()
{
	return m_bOriginReturnFlag;
}
