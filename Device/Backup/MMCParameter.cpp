// MMCParameter.cpp: implementation of the CMMCParameter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SDC_WindowControlPrj.h"
#include "MMCParameter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMMCParameter::CMMCParameter()
{
}

CMMCParameter::~CMMCParameter()
{
}

//_____________________________________________________________________________
/** @brief Motor Type 설정  		   									  
*/
void CMMCParameter::SetParasMotor(int Motor)
{
	m_iMotor = Motor;
}

//_____________________________________________________________________________
/** @brief 사다리꼴 or Curve Profile로 설정								  
*/
void CMMCParameter::SetParasCurve(int Curve)
{
	m_iCurve = Curve;
}

//_____________________________________________________________________________
/** @brief Encoder사용 여부 설정											  
*/
void CMMCParameter::SetParasEncoder(int Encoder)
{
	m_iEncoder = Encoder;
}

//_____________________________________________________________________________
/** @brief Loop Mode 설정													  
*/
void CMMCParameter::SetParasLoopMode(int mode)
{
	m_iLoop = mode;
}
void CMMCParameter::SetParasVoltage(int Voltage)
{
	m_iVoltage = Voltage;
}

//_____________________________________________________________________________
/** @brief 지정축의 아나로그 출력을 Bipolar or Unipolar로 설정  			 
*/
void CMMCParameter::SetParasControlVoltage(int Control)
{
	m_iControlVoltage = Control;
}

//_____________________________________________________________________________
/** @brief 지정축을 속도제어 모드 또는 Torque제어 모드로 지정.				  
*/
void CMMCParameter::SetParasControl(int Control)
{
	m_iControl = Control;
}

//_____________________________________________________________________________ 
/** @brief Feed Back Type설정   											 
*/
void CMMCParameter::SetParasFeedback(int fb)
{
	m_iFeedback = fb;
}

//_____________________________________________________________________________ 
/** @brief 위치 Loop 적분 제어시 적분 제어 모드를 지정  	  				  
*/
void CMMCParameter::SetParasPosIMode(int Posimode)
{
	m_iPosIMode = Posimode;
}

//_____________________________________________________________________________ 
/** @brief 속도 Loop 적분 제어시 적분 제어 모드를 지정  	  				  
*/
void CMMCParameter::SetParasVelIMode(int Vmode)
{
	m_iVelIMode = Vmode;
}

//_____________________________________________________________________________ 
/** @brief 지정축의 Pulse출력 설정. Two Pulse or Sign Pulse				  
*/
void CMMCParameter::SetParasStepMode(int Step)
{
	m_iStepMode = Step;
}

//_____________________________________________________________________________ 
/** @brief 지정축의 Motor 방향설정				  
*/
void CMMCParameter::SetParasCordinateDirection(int CordinateDirection)
{
	m_iCordinateDirection = CordinateDirection;
}

//_____________________________________________________________________________ 
/** @brief 지정축의 Encoder 방향설정
*/
void CMMCParameter::SetParasEncoderDirection(int EncoderDirection)
{
	m_iEncoderDirection = EncoderDirection;
}

//_____________________________________________________________________________ 
/** @brief -방향으로 Motor가 이동할 수 있는 Limit위치 값					 

void CMMCParameter::SetParasLowestPos(long LowestPos)   	
{
	m_lLowestPos=LowestPos;
}

//_____________________________________________________________________________ 
/** @brief -방향의 Limit Position까지 움직였을 때 동작할 Event설정. 		 
*/
void CMMCParameter::SetParasLowestEvent(int LowestEvent)
{
	m_iLowestEvent = LowestEvent;
}

//_____________________________________________________________________________ 
/** @brief +방향으로 Motor가 이동할 수 있는 Limit위치 값					 
*/
void CMMCParameter::SetParasHighestPos(long HighestPos)
{
	m_lHighestPos = HighestPos;
}

//_____________________________________________________________________________ 
/** @brief +방향의 Limit Position까지 움직였을 때 동작할 Event설정. 		 
*/
void CMMCParameter::SetParasHighestEvent(int HighestEvent)
{
	m_iHighestEvent = HighestEvent;
}

//_____________________________________________________________________________ 
/** @brief 축의 동작중 위치 오차 Limit지정. 				 				  
*/
void CMMCParameter::SetParasErrorLimit(long ErrorLimit)
{
	m_lErrorLimit = ErrorLimit;
}

//_____________________________________________________________________________ 
/** @brief 축의 동작중 위치 오차 Limit이상일 때 발생되는 Event				  
*/
void CMMCParameter::SetParasErrorLimitEvent(int ErrorLimitEvent)
{
	m_iErrorLimitEvent = ErrorLimitEvent;
}

//_____________________________________________________________________________ 
/** @brief 축의 최대 동작 속도 											  
*/
void CMMCParameter::SetParasMaxVelocity(long MaxVelocity)
{
	m_lMaxVelocity = MaxVelocity;
}

//_____________________________________________________________________________ 
/** @brief 축의 최대 가속도	 											  
*/
void CMMCParameter::SetParasMaxAccel(int MaxAccel)
{
	m_iMaxAccel = MaxAccel;
}

//_____________________________________________________________________________ 
/** @brief 축의 위치 결정 완료 값을 지정.   			   				  
*/
void CMMCParameter::SetParasInPosition(long InPosition)
{
	m_lInPosition = InPosition;
}

//_____________________________________________________________________________ 
/** @brief Stop Event 수행시 감속 시간 지정								  
*/
void CMMCParameter::SetParasStopDec(long StopDec)
{
	m_lStopDec = StopDec;
}

//_____________________________________________________________________________ 
/** @brief EStop Event 수행시 감속 시간 지정								  
*/
void CMMCParameter::SetParasEStopDec(long EStopDec)
{
	m_lEStopDec = EStopDec;
}

//_____________________________________________________________________________ 
/** @brief 지정축을 기어비 지정.											  
*/
void CMMCParameter::SetParasGearRatio(double GearRatio)
{
	m_dGearRatio = GearRatio;
}

//_____________________________________________________________________________ 
/** @brief 원점 복귀시 Encoder의 C(원점)상 펄스 이용 여부를 설정			 
*/
void CMMCParameter::SetParasHomeIndex(int index)
{
	m_iHomeIndex = index;
}


//_____________________________________________________________________________ 
/** @brief Motor Type읽기   												 
*/
int CMMCParameter::GetParasMotor()
{
	return m_iMotor;
}

//_____________________________________________________________________________ 
/** @brief 사다리꼴 or Curve Profile로 읽기								  
*/
int CMMCParameter::GetParasCurve()
{
	return m_iCurve;
}

//_____________________________________________________________________________ 
/** @brief Encoder사용 여부 읽기											  
*/
int CMMCParameter::GetParasEncoder()
{
	return m_iEncoder;
}
int CMMCParameter::GetParasVoltage()
{
	return m_iVoltage;
}

//_____________________________________________________________________________ 
/** @brief 지정축을 속도제어 모드 또는 Torque제어 모드 읽기.				  
*/
int CMMCParameter::GetParascontrol()
{
	return m_iControl;
}

//_____________________________________________________________________________ 
/** @brief 지정축의 아나로그 출력을 Bipolar or Unipolar로 설정  			 
*/
int CMMCParameter::GetParasControlVoltage()
{
	return m_iControlVoltage;
}

//_____________________________________________________________________________ 
/** @brief Loop Mode 읽기													  
*/
int CMMCParameter::GetParasLoopMode()
{
	return m_iLoop;
}

//_____________________________________________________________________________ 
/** @brief Feed Back Type읽기   											 
*/
int CMMCParameter::GetParasFeedback()
{
	return m_iFeedback;
}

//_____________________________________________________________________________ 
/** @brief 위치 Loop 적분 제어시 적분 제어 모드를 읽기  	  				  
*/
int CMMCParameter::GetParasPosIMode()
{
	return m_iPosIMode;
}

//_____________________________________________________________________________ 
/** @brief 속도 Loop 적분 제어시 적분 제어 모드를 읽기  	  				  
*/
int CMMCParameter::GetParasVelIMode()
{
	return m_iVelIMode;
}

//_____________________________________________________________________________ 
/** @brief 지정축의 Pulse출력 읽기. Two Pulse or Sign Pulse				  
*/
int CMMCParameter::GetParasStepMode()
{
	return m_iStepMode;
}

//_____________________________________________________________________________ 
/** @brief 지정축의 Motor 제어 방향 읽기				  
*/
int CMMCParameter::GetParasCordinateDirection()
{
	return m_iCordinateDirection;
}

//_____________________________________________________________________________ 
/** @brief 지정축의 지정축의 Encoder 방향 읽기				  
*/
int CMMCParameter::GetParasEncoderDirection()
{
	return m_iEncoderDirection;
}

//_____________________________________________________________________________ 
/** @brief -방향으로 Motor가 이동할 수 있는 Limit위치 값					 
*/
long CMMCParameter::GetParasLowestPos()
{
	return m_lLowestPos;
}

//_____________________________________________________________________________ 
/** @brief -방향의 Limit Position까지 움직였을 때 동작할 Event읽기. 		 
*/
int CMMCParameter::GetParasLowestEvent()
{
	return m_iLowestEvent;
}

//_____________________________________________________________________________ 
/** @brief +방향으로 Motor가 이동할 수 있는 Limit위치 값					 
*/
long CMMCParameter::GetParasHighestPos()
{
	return m_lHighestPos;
}

//_____________________________________________________________________________ 
/** @brief +방향의 Limit Position까지 움직였을 때 동작할 Event설정. 		 
*/
int CMMCParameter::GetParasHighestEvent()
{
	return m_iHighestEvent;
}

//_____________________________________________________________________________ 
/** @brief 축의 동작중 위치 오차 Limit읽기. 				 				  
*/
long CMMCParameter::GetParasErrorLimit()
{
	return m_lErrorLimit;
}

//_____________________________________________________________________________ 
/** @brief 축의 동작중 위치 오차 Limit이상일 때 발생되는 Event				  
*/
int CMMCParameter::GetParasErrorLimitEvent()
{
	return m_iErrorLimitEvent;
}

//_____________________________________________________________________________ 
/** @brief 축의 최대 동작 속도 											  
*/
long CMMCParameter::GetParasMaxVelocity()
{
	return m_lMaxVelocity;
}

//_____________________________________________________________________________ 
/** @brief 축의 최대 가속도	 											  
*/
int CMMCParameter::GetParasMaxAccel()
{
	return m_iMaxAccel;
}

//_____________________________________________________________________________ 
/** @brief 축의 위치 결정 완료 값을 읽기.   			   				  
*/
long CMMCParameter::GetParasInPosition()
{
	return m_lInPosition;
}

//_____________________________________________________________________________ 
/** @brief Stop Event 수행시 감속 시간 읽기								  
*/
long CMMCParameter::GetParasStopDec()
{
	return m_lStopDec;
}

//_____________________________________________________________________________ 
/** @brief EStop Event 수행시 감속 시간 읽기								  
*/
long CMMCParameter::GetParasEStopDec()
{
	return m_lEStopDec;
}

//_____________________________________________________________________________ 
/** @brief 지정축을 기어비 읽기.											  
*/
double CMMCParameter::GetParasGearRatio()
{
	return m_dGearRatio;
}

//_____________________________________________________________________________ 
/** @brief 축의 + Limit Switch의 Active상태를 지정함.   	  				  
*/
void CMMCParameter::SetParasPositiveLimitLevel(int level)
{
	m_iPositiveLimitLevel = level;
}

//_____________________________________________________________________________ 
/** @brief 축의 - Limit Switch의 Active상태를 지정함.   	  				  
*/
void CMMCParameter::SetParasNegativeLimitLevel(int level)
{
	m_iNegativeLimitLevel = level;
}

//_____________________________________________________________________________ 
/** @brief 축의 Home Limit Switch의 Active상태를 지정함.	 				  
*/
void CMMCParameter::SetParasHomeLimitLevel(int level)
{
	m_iHomeLimitLevel = level;
}

//_____________________________________________________________________________ 
/** @brief Amp Enable의 Active Level 지정.  				  				  
*/
void CMMCParameter::SetParasAmpEnableLevel(int level)
{
	m_iAmpEnableLevel = level;
}

//_____________________________________________________________________________ 
/** @brief Amp Enable의 Active Level 읽기.  				  				  
*/
int CMMCParameter::GetParasAmpEnableLevel()
{
	return m_iAmpEnableLevel;
}

//_____________________________________________________________________________ 
/** @brief 축의 + Limit Switch의 Active상태를 읽기. 						  
*/
int CMMCParameter::GetParasPositiveLimitLevel()
{
	return m_iPositiveLimitLevel;
}

//_____________________________________________________________________________ 
/** @brief 축의 - Limit Switch의 Active상태를 읽기. 						  
*/
int CMMCParameter::GetParasNegativeLimitLevel()
{
	return m_iNegativeLimitLevel;
}

//_____________________________________________________________________________ 
/** @brief 축의 Home Limit Switch의 Active상태를 읽기.     				  
*/
int CMMCParameter::GetParasHomeLimitLevel()
{
	return m_iHomeLimitLevel;
}

//_____________________________________________________________________________ 
/** @brief 원점 복귀시 Encoder의 C(원점)상 펄스 이용 여부를 읽기			 
*/
int CMMCParameter::GetParasHomeIndex()
{
	return m_iHomeIndex;
}

//_____________________________________________________________________________ 
/** @brief Amp Drive에 Fault발생시 동작할 Event지정 						 
*/
void CMMCParameter::SetParasAmpFaultEvent(int event)
{
	m_iAmpFaultEvent = event;
}

//_____________________________________________________________________________ 
/** @brief Amp Drive에 Fault발생시 동작할 Event읽기 						 
*/
int CMMCParameter::GetParasAmpFaultEvent()
{
	return m_iAmpFaultEvent;
}

//_____________________________________________________________________________ 
/** @brief Amp Drive에 Fault발생시 동작할 Event Level 지정  				 
*/
void CMMCParameter::SetParasAmpFaultLevel(int level)
{
	m_iAmpFaultLevel = level;
}

//_____________________________________________________________________________ 
/** @brief Amp Drive에 Fault발생시 동작할 Event Level 읽음  				 
*/
int CMMCParameter::GetParasAmpFaultLevel()
{
	return m_iAmpFaultLevel;
}

//_____________________________________________________________________________ 
/** @brief Amp Drive에 Reset Level을 지정   								 
*/
void CMMCParameter::SetParasAmpFaultResetLevel(int level)
{
	m_iAmpFaultResetLevel = level;
}

//_____________________________________________________________________________ 
/** @brief Amp Drive에 Reset Level을 읽기   								 
*/
int CMMCParameter::GetParasAmpFaultResetLevel()
{
	return m_iAmpFaultResetLevel;
}

//_____________________________________________________________________________ 
/** @brief Amp Drive의 In_Position Level을 지정 							 
 @return 정상동작 : 0   												   
*/
void CMMCParameter::SetParasInPositionLevel(int level)
{
	m_iInPositionLevel = level;
}

//_____________________________________________________________________________ 
/** @brief Amp Drive의 In_Position Level을 읽음 							 
*/
int CMMCParameter::GetParasInPositionLevel()
{
	return m_iInPositionLevel;
}

//_____________________________________________________________________________ 
/** @brief Amp Drive에 Fault Level을 지정   								 
 @return 정상동작 : 0   												   
*/
void CMMCParameter::SetParasInPositionRequired(int req)
{
	m_iInPositionRequired = req;
}

//_____________________________________________________________________________ 
/** @brief Amp Drive에 Fault Level을 읽음   								 
*/
int CMMCParameter::GetParasInPositionRequired()
{
	return m_iInPositionRequired;
}
//>> 130220 JSPark
//_____________________________________________________________________________ 
/** @brief Axis 사용 유무 설정   								 
*/
void CMMCParameter::SetParasAxisUse(int use)
{
	m_iAxisUse = use;
}
//_____________________________________________________________________________ 
/** @brief Axis 사용 유무 읽음   								 
*/
int CMMCParameter::GetParasAxisUse()
{
	return m_iAxisUse;
}
//<<
//>> 130219 JSPark
//_____________________________________________________________________________ 
/** @brief Motion Controller Pot No 설정   								 
*/
void CMMCParameter::SetParasPortNo(int port)
{
	m_iPortNo = port;
}
//_____________________________________________________________________________ 
/** @brief Motion Controller Pot No 읽음   								 
*/
int CMMCParameter::GetParasPortNo()
{
	return m_iPortNo;
}

//_____________________________________________________________________________ 
/** @brief Motion Controller Type 설정   								 
*/
void CMMCParameter::SetParasControlType(int type)
{
	m_iControlType = type;
}
//_____________________________________________________________________________ 
/** @brief Motion Controller Type 읽음   								 
*/
int CMMCParameter::GetParasControlType()
{
	return m_iControlType;
}
//>> 130317 JSPark
//_____________________________________________________________________________ 
/** @brief Motion Motor Type 설정   								 
*/
void CMMCParameter::SetParasMotorType(int type)
{
	m_iMotorType = type;
}
//_____________________________________________________________________________ 
/** @brief Motion Motor Type 설정   								 
*/
void CMMCParameter::SetParasPulseMethod(int type)
{
	m_iPulseMethod = type;
}
//_____________________________________________________________________________ 
/** @brief Motion Motor Type 읽음   								 
*/
int CMMCParameter::GetParasMotorType()
{
	return m_iMotorType;
}
//_____________________________________________________________________________ 
/** @brief Motion Pulse Method 읽음   								 
*/
int CMMCParameter::GetParasPulseMethod()
{
	return m_iPulseMethod;
}
//_____________________________________________________________________________ 
/** @brief Motion Load Ratio Type 설정   								 
*/
void CMMCParameter::SetParasLoadRatioType(int type)
{
	m_iLoadRatioType = type;
}
//_____________________________________________________________________________ 
/** @brief Motion Load Ration Type 읽음   								 
*/
int CMMCParameter::GetParasLoadRatioType()
{
	return m_iLoadRatioType;
}
//<<
//_____________________________________________________________________________ 
/** @brief Origin Method 설정   								 
*/
void CMMCParameter::SetParasOriginMethod(int method)
{
	m_IOriginMethod = method;
}
//_____________________________________________________________________________ 
/** @brief Origin Method 읽음   								 
*/
int CMMCParameter::GetParasOriginMethod()
{
	return m_IOriginMethod;
}

//_____________________________________________________________________________ 
/** @brief 1Plus당 이동 거리값 설정								 
*/
void CMMCParameter::SetParasMm4Pulse(double mm)
{
	m_dMm4Pulse = mm;
}
//_____________________________________________________________________________ 
/** @brief 1Plus당 이동 거리값 읽음   								 
*/
double CMMCParameter::GetParasMm4Pulse()
{
	return m_dMm4Pulse;
}

//_____________________________________________________________________________ 
/** @brief 1mm 이동 시 Plus값 설정  								 
*/
void CMMCParameter::SetParasPulse4Mm(double plus)
{
	m_dPulse4Mm = plus;
}
//_____________________________________________________________________________ 
/** @brief 1mm 이동 시 Plus값 읽음  								 
*/
double CMMCParameter::GetParasPulse4Mm()
{
	return m_dPulse4Mm;
}
//<<
