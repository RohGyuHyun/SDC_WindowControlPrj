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
/** @brief Motor Type ����  		   									  
*/
void CMMCParameter::SetParasMotor(int Motor)
{
	m_iMotor = Motor;
}

//_____________________________________________________________________________
/** @brief ��ٸ��� or Curve Profile�� ����								  
*/
void CMMCParameter::SetParasCurve(int Curve)
{
	m_iCurve = Curve;
}

//_____________________________________________________________________________
/** @brief Encoder��� ���� ����											  
*/
void CMMCParameter::SetParasEncoder(int Encoder)
{
	m_iEncoder = Encoder;
}

//_____________________________________________________________________________
/** @brief Loop Mode ����													  
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
/** @brief �������� �Ƴ��α� ����� Bipolar or Unipolar�� ����  			 
*/
void CMMCParameter::SetParasControlVoltage(int Control)
{
	m_iControlVoltage = Control;
}

//_____________________________________________________________________________
/** @brief �������� �ӵ����� ��� �Ǵ� Torque���� ���� ����.				  
*/
void CMMCParameter::SetParasControl(int Control)
{
	m_iControl = Control;
}

//_____________________________________________________________________________ 
/** @brief Feed Back Type����   											 
*/
void CMMCParameter::SetParasFeedback(int fb)
{
	m_iFeedback = fb;
}

//_____________________________________________________________________________ 
/** @brief ��ġ Loop ���� ����� ���� ���� ��带 ����  	  				  
*/
void CMMCParameter::SetParasPosIMode(int Posimode)
{
	m_iPosIMode = Posimode;
}

//_____________________________________________________________________________ 
/** @brief �ӵ� Loop ���� ����� ���� ���� ��带 ����  	  				  
*/
void CMMCParameter::SetParasVelIMode(int Vmode)
{
	m_iVelIMode = Vmode;
}

//_____________________________________________________________________________ 
/** @brief �������� Pulse��� ����. Two Pulse or Sign Pulse				  
*/
void CMMCParameter::SetParasStepMode(int Step)
{
	m_iStepMode = Step;
}

//_____________________________________________________________________________ 
/** @brief �������� Motor ���⼳��				  
*/
void CMMCParameter::SetParasCordinateDirection(int CordinateDirection)
{
	m_iCordinateDirection = CordinateDirection;
}

//_____________________________________________________________________________ 
/** @brief �������� Encoder ���⼳��
*/
void CMMCParameter::SetParasEncoderDirection(int EncoderDirection)
{
	m_iEncoderDirection = EncoderDirection;
}

//_____________________________________________________________________________ 
/** @brief -�������� Motor�� �̵��� �� �ִ� Limit��ġ ��					 

void CMMCParameter::SetParasLowestPos(long LowestPos)   	
{
	m_lLowestPos=LowestPos;
}

//_____________________________________________________________________________ 
/** @brief -������ Limit Position���� �������� �� ������ Event����. 		 
*/
void CMMCParameter::SetParasLowestEvent(int LowestEvent)
{
	m_iLowestEvent = LowestEvent;
}

//_____________________________________________________________________________ 
/** @brief +�������� Motor�� �̵��� �� �ִ� Limit��ġ ��					 
*/
void CMMCParameter::SetParasHighestPos(long HighestPos)
{
	m_lHighestPos = HighestPos;
}

//_____________________________________________________________________________ 
/** @brief +������ Limit Position���� �������� �� ������ Event����. 		 
*/
void CMMCParameter::SetParasHighestEvent(int HighestEvent)
{
	m_iHighestEvent = HighestEvent;
}

//_____________________________________________________________________________ 
/** @brief ���� ������ ��ġ ���� Limit����. 				 				  
*/
void CMMCParameter::SetParasErrorLimit(long ErrorLimit)
{
	m_lErrorLimit = ErrorLimit;
}

//_____________________________________________________________________________ 
/** @brief ���� ������ ��ġ ���� Limit�̻��� �� �߻��Ǵ� Event				  
*/
void CMMCParameter::SetParasErrorLimitEvent(int ErrorLimitEvent)
{
	m_iErrorLimitEvent = ErrorLimitEvent;
}

//_____________________________________________________________________________ 
/** @brief ���� �ִ� ���� �ӵ� 											  
*/
void CMMCParameter::SetParasMaxVelocity(long MaxVelocity)
{
	m_lMaxVelocity = MaxVelocity;
}

//_____________________________________________________________________________ 
/** @brief ���� �ִ� ���ӵ�	 											  
*/
void CMMCParameter::SetParasMaxAccel(int MaxAccel)
{
	m_iMaxAccel = MaxAccel;
}

//_____________________________________________________________________________ 
/** @brief ���� ��ġ ���� �Ϸ� ���� ����.   			   				  
*/
void CMMCParameter::SetParasInPosition(long InPosition)
{
	m_lInPosition = InPosition;
}

//_____________________________________________________________________________ 
/** @brief Stop Event ����� ���� �ð� ����								  
*/
void CMMCParameter::SetParasStopDec(long StopDec)
{
	m_lStopDec = StopDec;
}

//_____________________________________________________________________________ 
/** @brief EStop Event ����� ���� �ð� ����								  
*/
void CMMCParameter::SetParasEStopDec(long EStopDec)
{
	m_lEStopDec = EStopDec;
}

//_____________________________________________________________________________ 
/** @brief �������� ���� ����.											  
*/
void CMMCParameter::SetParasGearRatio(double GearRatio)
{
	m_dGearRatio = GearRatio;
}

//_____________________________________________________________________________ 
/** @brief ���� ���ͽ� Encoder�� C(����)�� �޽� �̿� ���θ� ����			 
*/
void CMMCParameter::SetParasHomeIndex(int index)
{
	m_iHomeIndex = index;
}


//_____________________________________________________________________________ 
/** @brief Motor Type�б�   												 
*/
int CMMCParameter::GetParasMotor()
{
	return m_iMotor;
}

//_____________________________________________________________________________ 
/** @brief ��ٸ��� or Curve Profile�� �б�								  
*/
int CMMCParameter::GetParasCurve()
{
	return m_iCurve;
}

//_____________________________________________________________________________ 
/** @brief Encoder��� ���� �б�											  
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
/** @brief �������� �ӵ����� ��� �Ǵ� Torque���� ��� �б�.				  
*/
int CMMCParameter::GetParascontrol()
{
	return m_iControl;
}

//_____________________________________________________________________________ 
/** @brief �������� �Ƴ��α� ����� Bipolar or Unipolar�� ����  			 
*/
int CMMCParameter::GetParasControlVoltage()
{
	return m_iControlVoltage;
}

//_____________________________________________________________________________ 
/** @brief Loop Mode �б�													  
*/
int CMMCParameter::GetParasLoopMode()
{
	return m_iLoop;
}

//_____________________________________________________________________________ 
/** @brief Feed Back Type�б�   											 
*/
int CMMCParameter::GetParasFeedback()
{
	return m_iFeedback;
}

//_____________________________________________________________________________ 
/** @brief ��ġ Loop ���� ����� ���� ���� ��带 �б�  	  				  
*/
int CMMCParameter::GetParasPosIMode()
{
	return m_iPosIMode;
}

//_____________________________________________________________________________ 
/** @brief �ӵ� Loop ���� ����� ���� ���� ��带 �б�  	  				  
*/
int CMMCParameter::GetParasVelIMode()
{
	return m_iVelIMode;
}

//_____________________________________________________________________________ 
/** @brief �������� Pulse��� �б�. Two Pulse or Sign Pulse				  
*/
int CMMCParameter::GetParasStepMode()
{
	return m_iStepMode;
}

//_____________________________________________________________________________ 
/** @brief �������� Motor ���� ���� �б�				  
*/
int CMMCParameter::GetParasCordinateDirection()
{
	return m_iCordinateDirection;
}

//_____________________________________________________________________________ 
/** @brief �������� �������� Encoder ���� �б�				  
*/
int CMMCParameter::GetParasEncoderDirection()
{
	return m_iEncoderDirection;
}

//_____________________________________________________________________________ 
/** @brief -�������� Motor�� �̵��� �� �ִ� Limit��ġ ��					 
*/
long CMMCParameter::GetParasLowestPos()
{
	return m_lLowestPos;
}

//_____________________________________________________________________________ 
/** @brief -������ Limit Position���� �������� �� ������ Event�б�. 		 
*/
int CMMCParameter::GetParasLowestEvent()
{
	return m_iLowestEvent;
}

//_____________________________________________________________________________ 
/** @brief +�������� Motor�� �̵��� �� �ִ� Limit��ġ ��					 
*/
long CMMCParameter::GetParasHighestPos()
{
	return m_lHighestPos;
}

//_____________________________________________________________________________ 
/** @brief +������ Limit Position���� �������� �� ������ Event����. 		 
*/
int CMMCParameter::GetParasHighestEvent()
{
	return m_iHighestEvent;
}

//_____________________________________________________________________________ 
/** @brief ���� ������ ��ġ ���� Limit�б�. 				 				  
*/
long CMMCParameter::GetParasErrorLimit()
{
	return m_lErrorLimit;
}

//_____________________________________________________________________________ 
/** @brief ���� ������ ��ġ ���� Limit�̻��� �� �߻��Ǵ� Event				  
*/
int CMMCParameter::GetParasErrorLimitEvent()
{
	return m_iErrorLimitEvent;
}

//_____________________________________________________________________________ 
/** @brief ���� �ִ� ���� �ӵ� 											  
*/
long CMMCParameter::GetParasMaxVelocity()
{
	return m_lMaxVelocity;
}

//_____________________________________________________________________________ 
/** @brief ���� �ִ� ���ӵ�	 											  
*/
int CMMCParameter::GetParasMaxAccel()
{
	return m_iMaxAccel;
}

//_____________________________________________________________________________ 
/** @brief ���� ��ġ ���� �Ϸ� ���� �б�.   			   				  
*/
long CMMCParameter::GetParasInPosition()
{
	return m_lInPosition;
}

//_____________________________________________________________________________ 
/** @brief Stop Event ����� ���� �ð� �б�								  
*/
long CMMCParameter::GetParasStopDec()
{
	return m_lStopDec;
}

//_____________________________________________________________________________ 
/** @brief EStop Event ����� ���� �ð� �б�								  
*/
long CMMCParameter::GetParasEStopDec()
{
	return m_lEStopDec;
}

//_____________________________________________________________________________ 
/** @brief �������� ���� �б�.											  
*/
double CMMCParameter::GetParasGearRatio()
{
	return m_dGearRatio;
}

//_____________________________________________________________________________ 
/** @brief ���� + Limit Switch�� Active���¸� ������.   	  				  
*/
void CMMCParameter::SetParasPositiveLimitLevel(int level)
{
	m_iPositiveLimitLevel = level;
}

//_____________________________________________________________________________ 
/** @brief ���� - Limit Switch�� Active���¸� ������.   	  				  
*/
void CMMCParameter::SetParasNegativeLimitLevel(int level)
{
	m_iNegativeLimitLevel = level;
}

//_____________________________________________________________________________ 
/** @brief ���� Home Limit Switch�� Active���¸� ������.	 				  
*/
void CMMCParameter::SetParasHomeLimitLevel(int level)
{
	m_iHomeLimitLevel = level;
}

//_____________________________________________________________________________ 
/** @brief Amp Enable�� Active Level ����.  				  				  
*/
void CMMCParameter::SetParasAmpEnableLevel(int level)
{
	m_iAmpEnableLevel = level;
}

//_____________________________________________________________________________ 
/** @brief Amp Enable�� Active Level �б�.  				  				  
*/
int CMMCParameter::GetParasAmpEnableLevel()
{
	return m_iAmpEnableLevel;
}

//_____________________________________________________________________________ 
/** @brief ���� + Limit Switch�� Active���¸� �б�. 						  
*/
int CMMCParameter::GetParasPositiveLimitLevel()
{
	return m_iPositiveLimitLevel;
}

//_____________________________________________________________________________ 
/** @brief ���� - Limit Switch�� Active���¸� �б�. 						  
*/
int CMMCParameter::GetParasNegativeLimitLevel()
{
	return m_iNegativeLimitLevel;
}

//_____________________________________________________________________________ 
/** @brief ���� Home Limit Switch�� Active���¸� �б�.     				  
*/
int CMMCParameter::GetParasHomeLimitLevel()
{
	return m_iHomeLimitLevel;
}

//_____________________________________________________________________________ 
/** @brief ���� ���ͽ� Encoder�� C(����)�� �޽� �̿� ���θ� �б�			 
*/
int CMMCParameter::GetParasHomeIndex()
{
	return m_iHomeIndex;
}

//_____________________________________________________________________________ 
/** @brief Amp Drive�� Fault�߻��� ������ Event���� 						 
*/
void CMMCParameter::SetParasAmpFaultEvent(int event)
{
	m_iAmpFaultEvent = event;
}

//_____________________________________________________________________________ 
/** @brief Amp Drive�� Fault�߻��� ������ Event�б� 						 
*/
int CMMCParameter::GetParasAmpFaultEvent()
{
	return m_iAmpFaultEvent;
}

//_____________________________________________________________________________ 
/** @brief Amp Drive�� Fault�߻��� ������ Event Level ����  				 
*/
void CMMCParameter::SetParasAmpFaultLevel(int level)
{
	m_iAmpFaultLevel = level;
}

//_____________________________________________________________________________ 
/** @brief Amp Drive�� Fault�߻��� ������ Event Level ����  				 
*/
int CMMCParameter::GetParasAmpFaultLevel()
{
	return m_iAmpFaultLevel;
}

//_____________________________________________________________________________ 
/** @brief Amp Drive�� Reset Level�� ����   								 
*/
void CMMCParameter::SetParasAmpFaultResetLevel(int level)
{
	m_iAmpFaultResetLevel = level;
}

//_____________________________________________________________________________ 
/** @brief Amp Drive�� Reset Level�� �б�   								 
*/
int CMMCParameter::GetParasAmpFaultResetLevel()
{
	return m_iAmpFaultResetLevel;
}

//_____________________________________________________________________________ 
/** @brief Amp Drive�� In_Position Level�� ���� 							 
 @return ������ : 0   												   
*/
void CMMCParameter::SetParasInPositionLevel(int level)
{
	m_iInPositionLevel = level;
}

//_____________________________________________________________________________ 
/** @brief Amp Drive�� In_Position Level�� ���� 							 
*/
int CMMCParameter::GetParasInPositionLevel()
{
	return m_iInPositionLevel;
}

//_____________________________________________________________________________ 
/** @brief Amp Drive�� Fault Level�� ����   								 
 @return ������ : 0   												   
*/
void CMMCParameter::SetParasInPositionRequired(int req)
{
	m_iInPositionRequired = req;
}

//_____________________________________________________________________________ 
/** @brief Amp Drive�� Fault Level�� ����   								 
*/
int CMMCParameter::GetParasInPositionRequired()
{
	return m_iInPositionRequired;
}
//>> 130220 JSPark
//_____________________________________________________________________________ 
/** @brief Axis ��� ���� ����   								 
*/
void CMMCParameter::SetParasAxisUse(int use)
{
	m_iAxisUse = use;
}
//_____________________________________________________________________________ 
/** @brief Axis ��� ���� ����   								 
*/
int CMMCParameter::GetParasAxisUse()
{
	return m_iAxisUse;
}
//<<
//>> 130219 JSPark
//_____________________________________________________________________________ 
/** @brief Motion Controller Pot No ����   								 
*/
void CMMCParameter::SetParasPortNo(int port)
{
	m_iPortNo = port;
}
//_____________________________________________________________________________ 
/** @brief Motion Controller Pot No ����   								 
*/
int CMMCParameter::GetParasPortNo()
{
	return m_iPortNo;
}

//_____________________________________________________________________________ 
/** @brief Motion Controller Type ����   								 
*/
void CMMCParameter::SetParasControlType(int type)
{
	m_iControlType = type;
}
//_____________________________________________________________________________ 
/** @brief Motion Controller Type ����   								 
*/
int CMMCParameter::GetParasControlType()
{
	return m_iControlType;
}
//>> 130317 JSPark
//_____________________________________________________________________________ 
/** @brief Motion Motor Type ����   								 
*/
void CMMCParameter::SetParasMotorType(int type)
{
	m_iMotorType = type;
}
//_____________________________________________________________________________ 
/** @brief Motion Motor Type ����   								 
*/
void CMMCParameter::SetParasPulseMethod(int type)
{
	m_iPulseMethod = type;
}
//_____________________________________________________________________________ 
/** @brief Motion Motor Type ����   								 
*/
int CMMCParameter::GetParasMotorType()
{
	return m_iMotorType;
}
//_____________________________________________________________________________ 
/** @brief Motion Pulse Method ����   								 
*/
int CMMCParameter::GetParasPulseMethod()
{
	return m_iPulseMethod;
}
//_____________________________________________________________________________ 
/** @brief Motion Load Ratio Type ����   								 
*/
void CMMCParameter::SetParasLoadRatioType(int type)
{
	m_iLoadRatioType = type;
}
//_____________________________________________________________________________ 
/** @brief Motion Load Ration Type ����   								 
*/
int CMMCParameter::GetParasLoadRatioType()
{
	return m_iLoadRatioType;
}
//<<
//_____________________________________________________________________________ 
/** @brief Origin Method ����   								 
*/
void CMMCParameter::SetParasOriginMethod(int method)
{
	m_IOriginMethod = method;
}
//_____________________________________________________________________________ 
/** @brief Origin Method ����   								 
*/
int CMMCParameter::GetParasOriginMethod()
{
	return m_IOriginMethod;
}

//_____________________________________________________________________________ 
/** @brief 1Plus�� �̵� �Ÿ��� ����								 
*/
void CMMCParameter::SetParasMm4Pulse(double mm)
{
	m_dMm4Pulse = mm;
}
//_____________________________________________________________________________ 
/** @brief 1Plus�� �̵� �Ÿ��� ����   								 
*/
double CMMCParameter::GetParasMm4Pulse()
{
	return m_dMm4Pulse;
}

//_____________________________________________________________________________ 
/** @brief 1mm �̵� �� Plus�� ����  								 
*/
void CMMCParameter::SetParasPulse4Mm(double plus)
{
	m_dPulse4Mm = plus;
}
//_____________________________________________________________________________ 
/** @brief 1mm �̵� �� Plus�� ����  								 
*/
double CMMCParameter::GetParasPulse4Mm()
{
	return m_dPulse4Mm;
}
//<<
