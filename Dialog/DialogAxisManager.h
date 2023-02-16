#pragma once


// CDialogAxisManager 대화 상자입니다.

class CDialogAxisManager : public CDialog
{
	DECLARE_DYNAMIC(CDialogAxisManager)

public:
	CDialogAxisManager(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogAxisManager();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_AXIS_MANAGER };

	CBtnEnh	m_ctrlMaxVel;
	CBtnEnh	m_ctrlMaxAcc;
	CBtnEnh	m_ctrlMinAcc;
	CBtnEnh	m_ctrlMinVel;
	CBtnEnh	m_ctrlAxisName;
	CBtnEnh	m_ctrlCurPos;
	CBtnEnh	m_ctrlAmpState;
	CBtnEnh	m_ctrlPosSensor;
	CBtnEnh	m_ctrlNegSensor;
	CBtnEnh	m_ctrlHomeSensor;
	CBtnEnh	m_ctrlVel;
	CBtnEnh	m_ctrlAcc;
	CBtnEnh	m_ctrlOriginVel;
	CBtnEnh	m_ctrlOriginVel2;
	CBtnEnh	m_ctrlOriginVel3;
	CBtnEnh	m_ctrlJogVel;
	CBtnEnh	m_ctrlOriginAcc;
	CBtnEnh	m_ctrlJogAcc;
	CBtnEnh	m_ctrlPosSWLimit;
	CBtnEnh	m_ctrlNegSWLimit;
	CBtnEnh m_ctrlTriggerPeriod;
private:
	void DispMotorParam();
	void DispUnitState();

	int m_iAxis;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void ClickOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void OnClickVelocity();
	void OnClickAccTime();
	void OnClickOriginVel();
	void OnClickOriginAcc();
	void OnClickJogVel();
	void OnClickJogAcc();
	void OnClickMaxVel();
	void OnClickMinVel();
	void OnClickMaxAcc();
	void OnClickMinAcc();
	void OnClickPosSwLimit();
	void OnClickNegSwLimit();
	void ClickSaveAxis();
	void ClickAmpOn();
	void ClickAmpOff();
	void ClickAmpReset();
	void ClickMotorParamPrev();
	void ClickMotorParamNext();
	void ClickTriggerPeriod();
	void ClickOriginVel2();
	void ClickOriginVel3();
};
