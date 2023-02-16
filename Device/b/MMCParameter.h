// @file MMCParameter.h: interface for the CMMCParameter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MMCPARAMETER_H__DCA25196_22E4_452B_BCBB_682DED6FE337__INCLUDED_)
#define AFX_MMCPARAMETER_H__DCA25196_22E4_452B_BCBB_682DED6FE337__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMMCParameter
{ 
public:
	CMMCParameter();
	virtual ~CMMCParameter();

protected: 
	int m_iControlVoltage;
	int m_iMotor;   	// Axis Configuration
	int m_iCurve;
	int m_iEncoder;
	int m_iControl;
	int m_iVoltage;
	int m_iLoop;
	int m_iFeedback;
	int m_iPosIMode;
	int m_iVelIMode;
	int m_iStepMode;
	int m_iCordinateDirection;
	int m_iEncoderDirection;

	long m_lLowestPos;  	 // Axis Software Limits
	int m_iLowestEvent;
	long m_lHighestPos;
	int m_iHighestEvent;
	long m_lErrorLimit;
	int m_iErrorLimitEvent;
	long m_lMaxVelocity;
	int m_iMaxAccel;
	long m_lInPosition;
	int m_iInPositionLevel;
	int m_iInPositionRequired;
	long m_lStopDec;
	long m_lEStopDec;
	double m_dGearRatio;

	int m_iPositiveLimitLevel;
	int m_iNegativeLimitLevel;
	int m_iHomeLimitLevel;
	int m_iAmpEnableLevel;
	int m_iAmpFaultLevel;
	int m_iHomeIndex;
	int m_iAmpFaultEvent;
	int m_iAmpFaultResetLevel;
//>> 130219 JSPark
	int m_iAxisUse; //130220 JSPark
	int m_iPortNo;
	int m_iControlType;
//>> 130317 JSPark
	int m_iMotorType;
	int m_iPulseMethod;
	int m_iLoadRatioType;
//<<
	int m_iOriginMethod;
	double m_dMm4Pulse;
	double m_dPulse4Mm;
//<<
public:
	void SetParasControlVoltage(int Voltage);
	void SetParasAmpEnableLevel(int l);
	void SetParasPosCoeff();
	void SetParasVelCoeff();
	void SetParasMotor(int Motor);
	void SetParasCurve(int Curve);
	void SetParasEncoder(int Encoder);
	void SetParasControl(int Voltage);
	void SetParasVoltage(int Voltage);
	void SetParasFeedback(int fb);
	void SetParasLoopMode(int mode);
	void SetParasPosIMode(int posimode);
	void SetParasVelIMode(int vmode);
	void SetParasStepMode(int step);
	void SetParasCordinateDirection(int);
	void SetParasEncoderDirection(int);
	void SetParasLowestPos(long);   	
	void SetParasLowestEvent(int);
	void SetParasHighestPos(long);
	void SetParasHighestEvent(int);
	void SetParasErrorLimit(long);
	void SetParasErrorLimitEvent(int);
	void SetParasMaxVelocity(long);
	void SetParasMaxAccel(int);
	void SetParasInPosition(long);
	void SetParasInPositionLevel(int);
	void SetParasInPositionRequired(int);
	void SetParasStopDec(long);
	void SetParasEStopDec(long);
	void SetParasGearRatio(double);
	void SetParasPositiveLimitLevel(int);
	void SetParasNegativeLimitLevel(int);
	void SetParasHomeLimitLevel(int);
	void SetParasHomeIndex(int);
	void SetParasAmpFaultEvent(int);
	void SetParasAmpFaultLevel(int);
	void SetParasAmpFaultResetLevel(int);
//>> 130219 JSPark
	void SetParasAxisUse(int); //130220 JSPark
	void SetParasPortNo(int);
	void SetParasControlType(int);
//>> 130317 JSPark
	void SetParasMotorType(int);
	void SetParasPulseMethod(int type);
	void SetParasLoadRatioType(int);
//<<
	void SetParasOriginMethod(int);
	void SetParasMm4Pulse(double);
	void SetParasPulse4Mm(double);

	int GetParasAxisUse(); //130220 JSPark
	int GetParasPortNo();
	int GetParasControlType();
//>> 130317 JSPark
	int GetParasMotorType();
	int GetParasPulseMethod();
	int GetParasLoadRatioType();
//<<
	int GetParasOriginMethod();
	double GetParasMm4Pulse();
	double GetParasPulse4Mm();
//<<
	int GetParasControlVoltage();
	int* GetParasCoeff();
	int GetParasAmpEnableLevel();
	int GetParasMotor();
	int GetParasCurve();
	int GetParasEncoder();
	int GetParasVoltage();
	int GetParascontrol();
	int GetParasFeedback();
	int GetParasLoopMode();
	int GetParasPosIMode();
	int GetParasVelIMode();
	int GetParasStepMode();
	int GetParasCordinateDirection();
	int GetParasEncoderDirection();
	long GetParasLowestPos();   	
	int GetParasLowestEvent();
	long GetParasHighestPos();
	int GetParasHighestEvent();
	long GetParasErrorLimit();
	int GetParasErrorLimitEvent();
	long GetParasMaxVelocity();
	int GetParasMaxAccel();
	long GetParasInPosition();
	int GetParasInPositionLevel();
	int GetParasInPositionRequired();
	long GetParasStopDec();
	long GetParasEStopDec();
	double GetParasGearRatio();
	int GetParasPositiveLimitLevel();
	int GetParasNegativeLimitLevel();
	int GetParasHomeLimitLevel();
	int GetParasHomeIndex();
	int GetParasAmpFaultEvent();
	int GetParasAmpFaultLevel();
	int GetParasAmpFaultResetLevel();
};

#endif // !defined(AFX_MMCPARAMETER_H__DCA25196_22E4_452B_BCBB_682DED6FE337__INCLUDED_)
