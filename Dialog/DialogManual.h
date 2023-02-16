#pragma once

#include "IO.h"
#include "Motion.h"


#define INPUT_CHECK_MAX_NUM	20
// CDialogManual 대화 상자입니다.
class CDialogManual : public  CDialog
{
	DECLARE_DYNAMIC(CDialogManual)

public:
	CDialogManual(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogManual();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAIN_MANUAL };


protected:
	CIO		*m_pIO;
	CMotion *m_pMotion;

	long m_lEndTime, m_lStartTime;

public:
	CBtnEnh	m_ctrlAxisPos[MAX_AXIS_NUM];
	//LIMIT
	CBtnEnh	m_ctrlAxisPosLimit[MAX_AXIS_NUM];
	CBtnEnh	m_ctrlAxisHomeLimit[MAX_AXIS_NUM];
	CBtnEnh	m_ctrlAxisNegLimit[MAX_AXIS_NUM];
	//AMP STATE
	CBtnEnh	m_ctrlAxisAmp[MAX_AXIS_NUM];

	//OUTPUT
	CBtnEnh m_ctrlJigA_Vac, m_ctrlJigA_Cylinder;
	CBtnEnh m_ctrlJigB_Vac;
	CBtnEnh m_ctrlJigB_Cylinder0, m_ctrlJigB_Cylinder1, m_ctrlJigB_Cylinder2, m_ctrlJigB_Cylinder3, m_ctrlJigB_Cylinder4;
	CBtnEnh m_ctrlPnP_Vac, m_ctrlPnp_Cylinder;
	CBtnEnh m_ctrlPnp_Shuttle_Vac;
	CBtnEnh m_ctrlJigB_Auto;
	CBtnEnh m_ctrlOpDoor;

	//INPTU
	CBtnEnh m_ctrlInputCheck[INPUT_CHECK_MAX_NUM];

	void DispCurPos();
	void DispAxisState();
	void DispUnitState();
	void DisplayUnitMoveTime();


	//MOTION & I/O
	BOOL InitializeAJINX();
	BOOL DestroyAJINX();
	CAxis* MotGetAxis(int axis_no) { return m_pMotion->m_pAxis[axis_no]; }

	//	I/O
	BOOL ReadOutPut(int nBit)			  { return m_pIO->ReadDO(nBit); }
	BOOL ReadInput(int nBit)			  { return m_pIO->ReadDI(nBit); }
	void WriteOutput(int nBit, BOOL bVal) { m_pIO->WriteDO(nBit, bVal); }
	void SetLampWait();
	void SetLampRun();
	void SetLampOff();
	void SetLampError();
	void SetLampButton(BOOL bState);

	//	MOTION
	BOOL MotInitialize()	{ return m_pMotion->Initialize(); }
	BOOL MotLoadAxisConfig() { return m_pMotion->LoadAxisConfig(); }
	BOOL MotSaveAxisConfig() { return m_pMotion->SaveAxisConfig(); }
	BOOL MotAmpEnableAll()	{ return m_pMotion->AmpEnableAll(); }
	BOOL MotAmpDisableAll() { return m_pMotion->AmpDisableAll(); }

	BOOL MotSetOriginMethod(int axis_no) { return m_pMotion->SetOriginMethod(axis_no); }
	BOOL MotStartOrigin(int axis_no) { return m_pMotion->StartOrigin(axis_no); }
	DWORD MotReadOriginResult(int axis_no) { return m_pMotion->ReadOriginResult(axis_no); }


	// MOTION->AXIS
	BOOL AxisSetTriggerBlock(int axis_no, double dStartPos, double dEndPos, double dPeriodPos = 100) { return m_pMotion->m_pAxis[axis_no]->SetTriggerBlock(dStartPos, dEndPos, dPeriodPos); }
	BOOL AxisSetTriggerTimeLevel(int axis_no) { return m_pMotion->m_pAxis[axis_no]->SetTriggerTimeLevel(); }
	void AxisSetTriggerReset(int axis_no) { m_pMotion->m_pAxis[axis_no]->SetTriggerReset(); }
	BOOL AxisGetPosition(int axis_no, double &rPos, BOOL bEncoder = TRUE) { return m_pMotion->m_pAxis[axis_no]->GetPosition(rPos, bEncoder); }
	BOOL AxisSetPosition(int axis_no, double dPos) { return m_pMotion->m_pAxis[axis_no]->SetPosition(dPos); }
	void AxisSetOriginReturn(int axis_no, BOOL bState) { m_pMotion->m_pAxis[axis_no]->SetOriginReturn(bState); }
	void AxisSetInPosition(int axis_no, BOOL bUse) { m_pMotion->m_pAxis[axis_no]->SetInPosition(bUse); }
	void AxisReadInPosition(int axis_no, DWORD *dwState) { m_pMotion->m_pAxis[axis_no]->ReadInPosition(dwState); }
	
	BOOL AxisMoveDone(int axis_no) { return m_pMotion->m_pAxis[axis_no]->MotionDone(); }
	BOOL AxisStop(int axis_no) { return m_pMotion->m_pAxis[axis_no]->Stop(); }
	BOOL AxisEStop(int axis_no) { return m_pMotion->m_pAxis[axis_no]->EStop(); }
	BOOL AxisMove(int axis_no, double dPos,  double dVelRatio, BOOL bWaitFlag = TRUE) { return m_pMotion->m_pAxis[axis_no]->Move(dPos, dVelRatio, bWaitFlag); }
	BOOL AxisIsHomeFinished(int axis_no) { return m_pMotion->m_pAxis[axis_no]->IsHomeFinished(); }
	BOOL AxisGetAmpFaultState(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetAmpFaultState(); }
	BOOL AxisGetAmpOnState(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetAmpOnState(); }
	BOOL AxisResetAmpFault(int axis_no) { return m_pMotion->m_pAxis[axis_no]->ResetAmpFault(); }
	BOOL AxisCheckHomingDone(int axis_no) { return m_pMotion->m_pAxis[axis_no]->CheckHomingDone(); }
	BOOL AxisGetHomingErr(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetHomingErr(); }
	BOOL AxisAmpEnable(int axis_no) { return m_pMotion->m_pAxis[axis_no]->AmpEnable(); }
	BOOL AxisAmpDisable(int axis_no) { return m_pMotion->m_pAxis[axis_no]->AmpDisable(); }
	BOOL AxisGetPosLimitLevel(int axis_no, long level) { return m_pMotion->m_pAxis[axis_no]->GetPosLimitLevel(level); }
	BOOL AxisGetPosLimitSensor(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetPosLimitSensor(); }
	BOOL AxisGetNegLimitLevel(int axis_no, long level) { return m_pMotion->m_pAxis[axis_no]->GetNegLimitLevel(level); }
	BOOL AxisGetNegLimitSensor(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetNegLimitSensor(); }
	BOOL AxisGetHomeSensor(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetHomeSensor(); }


	void AxisSetHomeFinished(int axis_no, BOOL bFinished) { return m_pMotion->m_pAxis[axis_no]->SetHomeFinished(bFinished); }


	
	void SaveAxisConfig(int axis_no) { m_pMotion->m_pAxis[axis_no]->SaveAxisConfig(); }
	
	CString AxisGetAxisName(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetAxisName(); }
	void AxisSetAxisName(int axis_no, CString strName) { m_pMotion->m_pAxis[axis_no]->SetAxisName(strName); }
	int AxisGetAxisUse(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetAxisUse(); }
	void AxisSetAxisUse(int axis_no, BOOL bUse) { m_pMotion->m_pAxis[axis_no]->SetAxisUse(bUse); }
	double AxisGetNormVel(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetNormVel(); }
	void AxisSetNormVel(int axis_no, double dVel) { m_pMotion->m_pAxis[axis_no]->SetNormVel(dVel); }
	double AxisGetHomeVel(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetHomeVel(); }
	void AxisSetHomeVel(int axis_no, double dVel) { m_pMotion->m_pAxis[axis_no]->SetHomeVel(dVel); }
	double AxisGetHomeVel2(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetHomeVel2(); }
	void AxisSetHomeVel2(int axis_no, double dVel) { m_pMotion->m_pAxis[axis_no]->SetHomeVel2(dVel); }
	double AxisGetHomeVel3(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetHomeVel3(); }
	void AxisSetHomeVel3(int axis_no, double dVel) { m_pMotion->m_pAxis[axis_no]->SetHomeVel3(dVel); }

	double AxisGetJogVel(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetJogVel(); }
	void AxisSetJogVel(int axis_no, double dVel) { m_pMotion->m_pAxis[axis_no]->SetJogVel(dVel); }
	double AxisGetJogAcc(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetJogAcc(); }
	void AxisSetJogAcc(int axis_no, double dVel) { m_pMotion->m_pAxis[axis_no]->SetJogAcc(dVel); }
	double AxisGetJogDec(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetJogDec(); }
	void AxisSetJogDec(int axis_no, double dVel) { m_pMotion->m_pAxis[axis_no]->SetJogDec(dVel); }
	double AxisGetNormAcc(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetNormAcc(); }
	void AxisSetNormAcc(int axis_no, double dVel) { m_pMotion->m_pAxis[axis_no]->SetNormAcc(dVel); }
	double AxisGetNormDec(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetNormDec(); }
	void AxisSetNormDec(int axis_no, double dVel) { m_pMotion->m_pAxis[axis_no]->SetNormDec(dVel); }
	double AxisGetHomeAcc(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetHomeAcc(); }
	void AxisSetHomeAcc(int axis_no, double dVel) { m_pMotion->m_pAxis[axis_no]->SetHomeAcc(dVel); }
	double AxisGetHomeDec(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetHomeDec(); }
	void AxisSetHomeDec(int axis_no, double dVel) { m_pMotion->m_pAxis[axis_no]->SetHomeDec(dVel); }
	double AxisGetMaxVel(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetMaxVel(); }
	void AxisSetMaxVel(int axis_no, double dVel) { m_pMotion->m_pAxis[axis_no]->SetMaxVel(dVel); }
	double AxisGetMinVel(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetMinVel(); }
	void AxisSetMinVel(int axis_no, double dVel) { m_pMotion->m_pAxis[axis_no]->SetMinVel(dVel); }
	double AxisGetMaxAcc(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetMaxAcc(); }
	void AxisSetMaxAcc(int axis_no, double dVel) { m_pMotion->m_pAxis[axis_no]->SetMaxAcc(dVel); }
	double AxisGetMinAcc(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetMinAcc();  }
	void AxisSetMinAcc(int axis_no, double dVel) { m_pMotion->m_pAxis[axis_no]->SetMinAcc(dVel); }	
	double AxisGetPosSWLimit(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetPosSWLimit(); }
	void AxisSetPosSWLimit(int axis_no, double dVel) { m_pMotion->m_pAxis[axis_no]->SetPosSWLimit(dVel); }
	double AxisGetNegSWLimit(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetNegSWLimit(); }
	void AxisSetNegSWLimit(int axis_no, double dVel) { m_pMotion->m_pAxis[axis_no]->SetNegSWLimit(dVel); }
	double AxisGetMaxHomeVel(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetMaxHomeVel(); }
	void AxisSetMaxHomeVel(int axis_no, double dVel) { m_pMotion->m_pAxis[axis_no]->SetMaxHomeVel(dVel); }
	double AxisGetMinHomeVel(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetMinHomeVel(); }
	void AxisSetMinHomeVel(int axis_no, double dVel) { m_pMotion->m_pAxis[axis_no]->SetMinHomeVel(dVel); }
	double AxisGetMaxHomeAcc(int axis_no) {  return m_pMotion->m_pAxis[axis_no]->GetMaxHomeAcc(); }
	void AxisSetMaxHomeAcc(int axis_no, double dVel) { m_pMotion->m_pAxis[axis_no]->SetMaxHomeAcc(dVel); }
	double AxisGetMinHomeAcc(int axis_no) {  return m_pMotion->m_pAxis[axis_no]->GetMinHomeAcc(); }
	void AxisSetMinHomeAcc(int axis_no, double dVel) { m_pMotion->m_pAxis[axis_no]->SetMinHomeAcc(dVel); }
	double AxisGetMaxJogVel(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetMaxJogVel(); }
	void AxisSetMaxJogVel(int axis_no, double dVel) { m_pMotion->m_pAxis[axis_no]->SetMaxJogVel(dVel); }	
	double AxisGetMinJogVel(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetMinJogVel(); }
	void AxisSetMinJogVel(int axis_no, double dVel) { m_pMotion->m_pAxis[axis_no]->SetMinJogVel(dVel); }
	double AxisGetMaxJogAcc(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetMaxJogVel(); }
	void AxisSetMaxJogAcc(int axis_no, double dVel) { m_pMotion->m_pAxis[axis_no]->SetMaxJogAcc(dVel); }
	double AxisGetMinJogAcc(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetMinJogAcc(); }
	void AxisSetMinJogAcc(int axis_no, double dVel) { m_pMotion->m_pAxis[axis_no]->SetMinJogAcc(dVel); }
	double AxisGetTriggerPeriod(int axis_no) { return m_pMotion->m_pAxis[axis_no]->GetTriggerPeriod(); }
	void AxisSetTriggerPeriod(int axis_no, double nPos) { return m_pMotion->m_pAxis[axis_no]->SetTriggerPeriod(nPos); }


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();
	DECLARE_EVENTSINK_MAP()
	void ClickIoCheck();
	void ClickAxisOrigin();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ClickJigAMoveForward();
	void ClickJigBVacOn();
	void ClickJigBCylinderIn();
	void ClickJigBCylinderIn2();
	void ClickJigBCylinderIn3();
	void ClickJigBCylinderIn4();
	void ClickJigBCylinderIn5();
	void ClickJigAVacOn();
	void ClickPnpVacOn();
	void ClickPnpUp();
	void ClickShuttleVacOn();
	void ClickJigBAutoStart();
	void ClickGrabStartPos();
	void ClickGrabEndPos();
	void ClickInputPos();
	void ClickCameraCenterPos();
	void ClickMainOpDoor2();
};
