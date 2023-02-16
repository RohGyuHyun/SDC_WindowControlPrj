// Axis.h: interface for the CAxis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AXIS_H__E1DB6534_D6DE_45A6_96C9_100D3851DBB8__INCLUDED_)
#define AFX_AXIS_H__E1DB6534_D6DE_45A6_96C9_100D3851DBB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MMCWFP39.h"
#include "MMCParameter.h"

#include "Axis_def.h"
#ifdef _USE_FASTECH
#include "FAS_EziMOTIONPlusR.h" //FASTECH DRIVER 사용 
#include "MotionIo_def.h"
#endif
#ifdef _USE_AJINXL
#include "AXL.h"
#include "AXM.h"
#endif
#ifdef _USE_DNET
#include "dnetUserAPI.h"
#include "dnet.h"
using namespace dNet;
//>>
#endif
#ifdef _USE_NMC
#include "NMC_Motion.h"
#endif

#include <afxmt.h>

#define AXIS_DELAY			300

class CAxis : public CMMCParameter
{
public:
#ifdef _USE_DNET
	CString mActPos;
	CString m_strPort;
	int mDID;
	BOOLEAN isServoOn;
	int mIODeviceID;
#endif
#ifdef _USE_NMC
	MC_STATUS m_lRet;
#endif

	/*
	bool bAllParamSaveRom();
	bool bAjinMotorParamSaveAll();
	bool bAjinMotorParamLoadAll();
	BOOL bConnectFastechDriver();
	BOOL bDisConnectFastechDriver();
	BOOL GetInmotionState();
	BOOL GetSyncControl();
	BOOL SetSyncGain(long gain);
	BOOL SetSyncControl(int master, int slave);
	BOOL RetSyncControl(int master, int slave);

	CTimerCheck origin_timer;
	CTimerCheck origin_timer2;

	void SetIsServo(int axis_type)
	{
		m_bIsServo = (axis_type ? TRUE : FALSE);
	}	// MMC 설정은 위치제어 Servo도 MICRO_STEP/OpenLoop이지만 
	bool IsServo()
	{
		return m_bIsServo;
	}

	bool SetAmpfaultlevel();
	bool SetPositiveLevel();
	bool SetNegativeLevel();
	bool SetHomeLevel();
	bool SetLoopModeType();
	bool SetInPositionRequired();
	bool SetErrLimit();
	bool SetElectricGear();
	bool SetStepMode();
	bool SetAmpEnableLevel();
	bool SetAmpFaultResetLevel();
	bool SetInPositionData();
	bool SetDirection();
	bool SetPosition(double pos);
	bool SetAmpFault();
	bool ResetAmpFault();
	bool SetPositiveLimit(int act);
	bool SetNegativeLimit(int act);
	bool SetHome(int act);
	BOOL GetPosLimitSensor();
	BOOL GetNegLimitSensor();
	bool GetHomeSensor();
	
	
	BOOL Origin();
	BOOL SetHomingSetup(void);
	bool VMove(double Velocity, double Acceleration, double Deceleration);
	bool SetStopRate(int msec);
	bool ClearAxis();
	bool ClearStatus();
	int AxisState();
	bool FramesClear();
	bool SetIndexRequired();
	bool ResetIndexRequired();
	bool Stop();
	bool EStop();
	BOOL StopVMove();
	
	BOOL CheckAmpFault();
	
	BOOL GetHomingErr() { return m_bHomingErr; } 
	BOOL CheckHomingDone();
	void SetHomingOrder(int iOrder) { m_iHomingOrder = iOrder; }
	int GetHomingOrder() { return m_iHomingOrder; }
	void SetHomeFinished(BOOL bFinished);
	int GetPosition(double &rPos, bool bEncoder=true); //130228 JSPark
	
//>> 130317 JSPark
///<< AjinExtek MLII/SigmaV 사용시 서보모터 부하율 얻기....[2013.03.01 by LJY]
	bool SetServoLodRatio(int iType);
	bool GetServoLodRatio(double &dLoad);
//>>
	void SetMotorType();
	int GetMotorType() { return m_iMotorType; }
	void SetPulseMethod();
	int GetPulseMethod() { return m_iPulseMethod; }
	void SetLoadRatioType();
	int GetLoadRatioType() { return m_iLoadRatioType; }
//<<

	
	

	void SetJogVel(double dVel) { m_JogVel = dVel; }
	double GetJogVel() { return m_JogVel; }
	void SetJogAcc(double dAcc) { m_JogAcc = dAcc;}
	double GetJogAcc() { return m_JogAcc;}
	void SetJogDec(double dDec) { m_JogDec = dDec;}
	double GetJogDec() { return m_JogDec;}

	void SetHomeVel(double dVel) { m_HomeVel = dVel; }
	double GetHomeVel() { return m_HomeVel; }

	void SetHomeAcc(double dAcc) { m_HomeAcc = dAcc;}
	double GetHomeAcc() { return m_HomeAcc;}
	void SetHomeDec(double dDec) { m_HomeDec = dDec;}
	double GetHomeDec() { return m_HomeDec;}

	void SetAxisInitPos(double dVal) {m_dAxisInitPos = dVal;}
	double GetAxisInitPos()  {return m_dAxisInitPos;}
	void SetAxisSafetyPos(double dVal) {m_dAxisSafetyPos = dVal;}
	double GetAxisSafetyPos() {return m_dAxisSafetyPos;}
	void SetOrgOffset(double dVel) { m_dOrgOffset = dVel; }
	double GetOrgOffset() { return m_dOrgOffset; }

	void SetAxisAccelUnit();  // 단위 변화 

	void SetHomeFineVel(double dVel) { m_HomeFineVel = dVel; }
	double GetHomeFineVel() { return m_HomeFineVel; }
	void SetHomeFineAcc(double dAcc) { m_HomeFineAcc = dAcc;}
	double GetHomeFineAcc() { return m_HomeFineAcc;}

	
	void SetPosSWLimit(double dLimit) { m_dPosSWLimit = dLimit; }
	double GetPosSWLimit() { return m_dPosSWLimit; }
	void SetNegSWLimit(double dLimit) { m_dNegSWLimit = dLimit; }
	double GetNegSWLimit() { return m_dNegSWLimit; }

	void SetMaxJogAcc(double dAcc) { m_dMaxJogAccTime = dAcc; }
	double GetMaxJogAcc() { return m_dMaxJogAccTime; }
	void SetMinJogAcc(double dAcc) { m_dMinJogAccTime = dAcc; }
	double GetMinJogAcc() { return m_dMinJogAccTime; }
	void SetMaxJogVel(double dVel) { m_dMaxJogVel = dVel; }
	double GetMaxJogVel() { return m_dMaxJogVel; }
	void SetMinJogVel(double dVel) { m_dMinJogVel = dVel; }
	double GetMinJogVel() { return m_dMinJogVel; }

	void SetMaxHomeAcc(double dAcc) { m_dMaxHomeAccTime = dAcc; }
	double GetMaxHomeAcc() { return m_dMaxHomeAccTime; }
	void SetMinHomeAcc(double dAcc) { m_dMinHomeAccTime = dAcc; }
	double GetMinHomeAcc() { return m_dMinHomeAccTime; }
	void SetMaxHomeVel(double dVel) { m_dMaxHomeVel = dVel; }
	double GetMaxHomeVel() { return m_dMaxHomeVel; }
	void SetMinHomeVel(double dVel) { m_dMinHomeVel = dVel; }
	double GetMinHomeVel() { return m_dMinHomeVel; }

	void SetMaxAcc(double dAcc) { m_dMaxAccTime = dAcc; }
	double GetMaxAcc() { return m_dMaxAccTime; }
	void SetMinAcc(double dAcc) { m_dMinAccTime = dAcc; }
	double GetMinAcc() { return m_dMinAccTime; }
	void SetMaxVel(double dVel) { m_dMaxVel = dVel; }
	double GetMaxVel() { return m_dMaxVel; }
	void SetMinVel(double dVel) { m_dMinVel = dVel; }
	double GetMinVel() { return m_dMinVel; }

	BOOL GetStopRate(int &msec);
	BOOL GetEStopRate(int &msec);
	BOOL SetEStopRate();
	BOOL SetStopRate();
	BOOL GetHomeLevel(long &level);
	BOOL GetNegLimitLevel(long &level);
	BOOL GetPosLimitLevel(long &level);
	BOOL GetAmpFaultLevel(long &level);
	BOOL LoadAxisConfig();
	BOOL SaveAxisConfig();
	
	void SetAxisId(int id) { m_iAxisID = id; }
	int GetAxisID() { return m_iAxisID; }
	
	
	BOOL SetPosLimitLevel(long level);
	BOOL SetAmpFaultLevel(long level);
	BOOL SetNegLimitLevel(long level);
	BOOL SetHomeLevel(long level);
	BOOL GetAmpState(long State);
	BOOL AmpDisable();
	BOOL AmpEnable();
	BOOL MoveEStop();
	BOOL MoveStop();
	BOOL JogMove();
	BOOL JogStop();
	BOOL OffsetMove(double dPos);
	BOOL JogMove(double dVel, double dAcc=0.0, double dDec=0.0, double dJerk=0.0, double dDelay=0.0);
//>> 130303 JSPark
	BOOL StartVMove();
	BOOL StartVMove(double dVel, double dAcc=0.0, double dDec=0.0, double dJerk=0.0, double dDelay=0.0);
	BOOL StartVStop();
//<<
	BOOL StartGroupMove(double dPosY, double dPosT, double dVelY, double dVelT, double dAccY, double dAccT, double dDecY, double dDecT, int iGroup); //151104 JSLee
	BOOL RMove(double dPos, double dVel, double dAcc, double dDec=0.0, bool bCheckFlag=true);
	BOOL RMove(double dPos, bool bCheckFlag=true);
	BOOL StartRMove(double dPos);
	BOOL StartRMove(double dPos, double dVel, double dAcc, double dDec=0.0);
	
	
	BOOL GroupMove(double dPosY, double dPosT, double dVelY, double dVelT, double dAccY, double dAccT, double dDecY, double dDecT, int iGroup, bool bWaitFlag); //151104 JSLee

	BOOL SetGroupProfileSetup(double dPosY, double dPosT, double dVelY, double dVelT, double dAccY, double dAccT, double dDecY, double dDecT, int iGroup); //151103 JSLee
	BOOL SetProfileSetup(void);	// MC Tech.Set.Speed
	
	BOOL SetMultiGroup(int nGroupID);
	BOOL MultiMove(int nGroup);
	//>>20140310 lee
	BOOL Move(double dPos, double dVel, double dAcc, double dDec=0.0, double dJerk=0.0, double dDelay=0.0, bool bWaitFlag = true);
	BOOL Move(double dPos, bool bWaitFlag = true); 
	//<<

	BOOL MoveCPLine(int lCoordinate, double dPosX, double dPosY, double dVel, double dAcc);
	
	

//<<
//>> 130219 JSPark
	void SetPortNo();
	int GetPortNo() { return m_iPortNo;}



	void SetOriginMethod();
	int GetOriginMethod() { return m_iOriginMethod;}

//<<	
	void SetConnectState(bool bConnectState) { m_bConnectState = bConnectState;} //해당 축 통신 연결 상태 
	bool GetConnectState() { return m_bConnectState;}

	void SetCtrl(long lCtrl);
	long GetCtrl() { return m_lCtrl;}
	void SetAxisNo(long lAxis) { m_lAxis = lAxis;}
	long GetAxisNo() { return m_lAxis;}

	void SetSysVelRatio(double dRatio) { m_dSysVelRatio = dRatio;}
	double GetSysVelRatio() { return m_dSysVelRatio;}

	void SetThreadStarted(BOOL bStarted) { m_bThreadStarted = bStarted;}
	BOOL IsThreadStarted() { return m_bThreadStarted;}

	BOOL Homing4Only();							// Homing Operation Start(단일축 구동시 사용)
	void Homing4Multi();							// Homing Operation Start(여러축 동시 구동시 사용)		
	BOOL StopHoming();							// Homing Operation Stop
	
	long GetAxisStatus();						//축에대한 상태를 읽는다.
	long GetHomeStatus();						//모든축이 Homing이 된 상태인지를 얻는다.
	long IsHomeFinished();
	BOOL HomeProcess();

	static UINT HomeProcess(LPVOID lpVoid);

	*/
	CAxis();
	virtual ~CAxis();

	double m_NormVel, m_HomeVel, m_HomeFineVel, m_JogVel;
	double m_NormAcc, m_HomeAcc, m_HomeFineAcc, m_JogAcc;
	double m_NormDec, m_HomeDec, m_JogDec;
	double m_dOrgOffset,m_dAxisInitPos,m_dAxisSafetyPos; 

	void SetHomingErr(BOOL bErr) { m_bHomingErr = bErr; }
	BOOL ClearState();
	BOOL GetAmpOnState(); //130214 JSPark
	BOOL GetAmpFaultState();
	BOOL isOriginReturn();				// 원점복귀가 되어있는지를 check.
	BOOL CheckSWLimit(double dPos);
	BOOL Wait4Done(double dPos=0.0, bool bUpperInterfaceInterlockCheck=false, bool bLowerInterfaceInterlockCheck=false); //130315 JSPark
	BOOL MotionDone();

	BOOL StartMove(double dPos);
	BOOL StartMove(double dPos, double dVel, double dAcc, double dDec=0.0, double dJerk=0.0, double dDelay=0.0);	

	//>> 130220 JSPark
	void SetAxisUse();
	int GetAxisUse() { return m_iAxisUse;}
	void SetControlType();
	int GetControlType() { return m_iControlType;}
	void SetOriginReturn(BOOL orgine);	// 원점복귀 flag를 set 시킴.
	
	void SetMm4Pulse();
	double GetMm4Pulse() { return m_dMm4Pulse;}

	void SetPulse4Mm();
	double GetPulse4Mm() { return m_dPulse4Mm;}
	
	void SetNormVel(double dVel) { m_NormVel = dVel;}
	double GetNormVel() { return m_NormVel;}

	void SetAxisName(CString strName) { m_strAxisName = strName; }
	CString GetAxisName() { return m_strAxisName; }

	void SetNormDec(double dDec) { m_NormDec = dDec;}
	double GetNormDec() { return m_NormDec;}

	void SetNormAcc(double dAcc) { m_NormAcc = dAcc;}
	double GetNormAcc() { return m_NormAcc;}


#ifdef _USE_FASTECH
	int m_iMotionAlarmCode;
#endif


	//>>20141202 JYLee
	//BOOL RMoveMultiAxis(long lSubAxis, double dPos, double dSPos, bool bCheckFlag = true);
	//BOOL StartRMoveMultiAxis(long lSubAxis, double dPos, double dSPos);
	//BOOL MoveMultiAxis(long lSubAxis, double dPos, double dSPos, bool bCheckFlag = true, double dVel_Rate=0.0 /* dVel_Rate : 0 ~ 1 */);
	//BOOL StartMoveMultiAxis(long lSubAxis, double dPos, double dSPos, double dVel_Rate=0.0  /* dVel_Rate : 0 ~ 1 */);
	
	
// 20141212 JYLee
	//BOOL MoveUVWThetapos(double dUPos, double dVPos, double dWPos, bool bCheckFlag = true);
	//BOOL StartMoveUVWThetapos(double dUPos, double dVPos, double dWPos);
	//BOOL RMoveUVWThetapos(double dUPos, double dVPos, double dWPos, bool bCheckFlag = true);
	//BOOL StartRMoveUVWThetapos(double dUPos, double dVPos, double dWPos);
	//<<

	enum OriginStep 
	{
		ORG_INIT = 0,
		ORG_POS_SENSOR_SEARCH,
		ORG_POS_SENSOR_OUT,
		ORG_NEG_SENSOR_SEARCH,
		ORG_NEG_SENSOR_OUT,
		ORG_HOME_SENSOR_SEARCH_4_POS,
		ORG_HOME_SENSOR_SEARCH_4_NEG,
		ORG_HOME_SENSOR_OUT,
		ORG_Z_PHASE_SEARCH,
		ORG_ORIGIN_OFFSET_MOVE,
		ORG_ORIGIN_COMPLETE_CHECK
	};	

	enum HomeStatus 
	{
		HomeStatusReady = 0,
		HomeStatusError,
		HomeStatusProcessing,
		HomeStatusFinish
	};	

	enum OrignMethod 
	{
		ORG_METHOD_POS=0,
		ORG_METHOD_NEG,
		ORG_METHOD_POS_HOME,
		ORG_METHOD_NEG_HOME,
		ORG_METHOD_HOME,
		ORG_METHOD_POS_HOME_Z,
		ORG_METHOD_NEG_HOME_Z
	};	
//>> 130220 JSPark
	enum AxisUse
	{
		AXIS_NOT_USE=0,
		AXIS_USE,
		AXIS_NOT_ORIGIN
	};
//<<

//////////////////////////////////////////////////////////////////////////
#ifdef _USE_FASTECH
	// Fastech Motion I/O Use
	bool bCheckInputStatus(DWORD dwAdd);
	bool bCheckOutputStatus(DWORD dwAdd);
	bool SetOutput(DWORD dwAdd, int nOnOff);
#endif
//////////////////////////////////////////////////////////////////////////

protected:
	CCriticalSection g_AxisSyncIntlock;

	BOOL m_bOriginReturnFlag;
	bool m_bIsServo;
	BOOL m_bHomingErr;
	BOOL m_bNeg;
	BOOL m_bHome;
	BOOL m_bHomeLevel;
	BOOL m_bIndex;
	BOOL m_bThreadStarted;

	long m_lCtrl;
	long m_lAxis;

	int m_iHomingOrder;
	int m_MMC_Err;

	double m_dSysVelRatio;	
	double m_dHomeDelay;
	double m_dNormDelay;
	double m_dJogDelay;	
	double m_dMaxVel;
	double m_dMinVel;
	double m_dMaxAccTime;
	double m_dMinAccTime;

	CString m_strAxisName;

	int m_iAxisID;
	int m_iAxisUse;				//-> 축 사용 유무(Not use:0, Use:1, Not Origin:2)
	int m_iPulseOutMethod;		//-> Pulse Out Method for Ajin(0:OneHighLowHigh, 1:OneHighHighLow, 2:OneLowLowHigh, 3:OneLowHighLow 4: TwoCcwCwHigh, 5:TwoCcwCwLow, 6:TwoCwCcwHigh, 7:TwoCwCcwLow, 8:Two Phase , 9:Two Phase Reverse
	int m_iComPort;				//-> Motion Controller Com 통신 포트 설정
	int m_iControlType;		    //-> Motion Controller Type 설정(MMC:0, FASTECH:1, ADLINK:2, AJINXL:3, DNET:4)
	int m_iOriginMethod;		//-> Origin Method 설정(ORG_METHOD_POS:0, ORG_METHOD_NEG:1, ORG_METHOD_POS_HOME:2,ORG_METHOD_NEG_HOME:3, ORG_METHOD_HOME:4, ORG_METHOD_POS_HOME_Z:5, ORG_METHOD_NEG_HOME_Z:6)
	int m_iPositiveLimitLevel;	//-> +Limit 설정(High:1, Low:0)  2: 사용 안함             
	int m_iHomeLevel;			//-> Hmoe Sensor 설정(High:1, Low:0)  2: 사용 안함
	int m_iNegativeLimitLevel;	//-> -Limit Sensor 설정(High:1, Low:0)  2: 사용 안함
	int m_iAmpFaultLevel;		//-> Amp fault level 설정(High:1, Low:0) 
	int m_iAmpFaultResetLevel;	//-> Amp reset level 설정(High:1, Low:0)
	int m_iDirection;			//-> 축 이동발향 설정(CW:0 / CCW:1)
	int m_iE_Gear;				//-> Drive 전자 기어비 설정	

	bool m_bConnectState; //해당 Slave Axis 통신 연결 상태
	int m_iPortNo;        //COM Port No

	double m_dMinJogVel;
	double m_dMaxJogVel;
	double m_dMinHomeVel;
	double m_dMaxHomeVel;
	double m_dMinJogAccTime;
	double m_dMaxJogAccTime;
	double m_dMinHomeAccTime;
	double m_dMaxHomeAccTime;
	double m_dNegSWLimit;
	double m_dPosSWLimit;
	double m_dMm4Pulse;
	double m_dPulse4Mm;

private:
	BOOL m_bHomingStop;
	BOOL m_bNegativeLevel;
	BOOL m_bPositiveLevel;
	BOOL m_bFaultLevel;
//>> 130317 JSPark
	int m_iServoRatioType;
	int m_iMotorType;
	int m_iPulseMethod;
	int m_iLoadRatioType;
//<<
	bool m_bMotion;
};

extern HANDLE g_hMutex;

#endif // !defined(AFX_AXIS_H__E1DB6534_D6DE_45A6_96C9_100D3851DBB8__INCLUDED_)
