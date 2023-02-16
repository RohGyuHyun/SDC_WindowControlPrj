#pragma once


// CDialogAxisJog ��ȭ �����Դϴ�.

class CDialogAxisJog : public CDialog
{
	DECLARE_DYNAMIC(CDialogAxisJog)

public:
	CDialogAxisJog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogAxisJog();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_AXIS_JOG };


	void InitJogDlgOpInfo();
	void AxisOpDispUpdate();
	void DisplayAxisPos();
	
	int m_iOpMode;

	CAxis* m_paxDlgX;
	CAxis* m_paxDlgY;

	CBtnEnh	m_dCurPosAxisX;
 	CBtnEnh	m_dCurPosAxisY;
	CBtnEnh	m_ctrlPosSensor;
	CBtnEnh	m_ctrlHomeSensor;
	CBtnEnh	m_ctrlNegSensor;
	CBtnEnh	m_ctrlPosSensorY;
	CBtnEnh	m_ctrlHomeSensorY;
	CBtnEnh	m_ctrlNegSensorY;

	BOOL m_bAxisOpDlgUpdate;
	BOOL m_bTeachPosXFlag;
	BOOL m_bTeachPosYFlag;
	double m_dTeachPosX;
	double m_dTeachPosY;
	double m_dJogRatio;
	double m_dStepPitch;
	double m_dJogSpeed;

	void SetTeachPosXFlag(bool bFlag) { m_bTeachPosXFlag = bFlag; }
	BOOL IsTeachPosXFlag() { return m_bTeachPosXFlag; }
	void SetTeachPosYFlag(bool bFlag) { m_bTeachPosYFlag = bFlag; }
	BOOL IsTeachPosYFlag() { return m_bTeachPosYFlag; }

	void SetTeachPosX(double dPos) { m_dTeachPosX = dPos; }
	double GetTeachPosX() { return m_dTeachPosX; }
	void SetTeachPosY(double dPos) { m_dTeachPosY = dPos; }
	double GetTeachPosY() { return m_dTeachPosY; }
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();

	DECLARE_EVENTSINK_MAP()
	void ClickBtnDlgCancel();
	void ClickAxisJogRight();
	void ClickAxisJogLeft();
	void ClickAxisJogBack();
	void ClickAxisJogFront();
	void ClickAxisOpJogSpeedLow();
	void ClickAxisOpJogSpeedMid();
	void ClickAxisOpJogSpeedHigh();
	void ClickSelectJog();
	void ClickSelectStep();
	void ClickStepMovePitch();
	void ClickExtend();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ClickBtnDlgMovingPos();
};
