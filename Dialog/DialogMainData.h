#pragma once

#include "SystemData.h"

#define SYSTEM_OPTION_CNT	13
#define SYSTEM_PARAM_CNT	11
#define SYSTEM_OP_MODE		0
#define SYSTEM_MANUAL_MUTING 1
#define SYSTEM_MANUAL_SOCKET 2
#define SYSTEM_MANUAL_SKIPFILMREMOVE 3

#define SYSTEM_VEL_RATIO	0

#define SYSTEM_OP_MODE_AUTORUN	0
#define SYSTEM_OP_MODE_DRYRUN	1

#define SYSTEM_MANUAL_MUTING_OFF 0
#define SYSTEM_MANUAL_MUTING_ON	1

#define SYSTEM_MANUAL_SOCKET_OFF 0
#define SYSTEM_MANUAL_SOCKET_ON	1

#define SYSTEM_MANUAL_SKIPFILMREMOVE_OFF 0
#define SYSTEM_MANUAL_SKIPFILMREMOVE_ON	1
// CDialogMainData 대화 상자입니다.

class CDialogMainData : public CDialog
{
	DECLARE_DYNAMIC(CDialogMainData)

public:
	CDialogMainData(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogMainData();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAIN_DATA };

	CBtnEnh m_ctrlOpModeAutoRun;
	CBtnEnh m_ctrlOpModeDryRun;
	CBtnEnh m_ctrlManualMutingOn;
	CBtnEnh m_ctrlManualMutingOff;
	CBtnEnh m_ctrlManualSocketOn;
	CBtnEnh m_ctrlManualSocketOff;
	CBtnEnh m_ctrlManualSkipFilmRemoveOn;
	CBtnEnh m_ctrlManualSkipFilmRemoveOff;


	double m_dParamData[SYSTEM_PARAM_CNT];
	int m_iOptionData[SYSTEM_OPTION_CNT];

	void DisplaySystemData();
	void DisplaySystemOption();
	void DisplaySystemParam();
	void OnClickDataItemOff();
	void OnClickDataItemOn();
	void OnClickSystemParam();


	CString GetIPAddressVisionPC() { return m_pSystemData->GetIPAddressVisionPC(); }
	void SetIPAddressVisionPC(CString strIP) { m_pSystemData->SetIPAddressVisionPC(strIP); }
	int GetPortNoVisionPC() { return m_pSystemData->GetPortNoVisionPC(); }
	void SetPortNoVisionPC(int nPort) { m_pSystemData->SetPortNoVisionPC(nPort); }

	CString GetWorkModelName() { return m_pSystemData->GetWorkModelName(); }	// 모델로 보내자
	void SetWorkModelName(CString strModelName) { m_pSystemData->SetWorkModelName(strModelName); }

	void SetCurUserName(CString strName) { m_pSystemData->SetCurUserName(strName); }
	CString GetCurUserName() { return m_pSystemData->GetCurUserName(); }
	void SetCurUserPW(CString strPW) { m_pSystemData->SetCurUserPW(strPW);}
	CString GetCurUserPW() { return m_pSystemData->GetCurUserPW();}
	void SetOperatorName(CString strName) { m_pSystemData->SetOperatorName(strName);}
	CString GetOperatorName() { return m_pSystemData->GetOperatorName();}
	void SetOperatorPW(CString strPW) { m_pSystemData->SetOperatorPW(strPW);}
	CString GetOperatorPW() { return m_pSystemData->GetOperatorPW();}

	void SetEngineerName(CString strName) { m_pSystemData->SetEngineerName(strName);}
	CString GetEngineerName() { return m_pSystemData->GetEngineerName();}
	void SetEngineerPW(CString strPW) { m_pSystemData->SetEngineerPW(strPW);}
	CString GetEngineerPW() { return m_pSystemData->GetEngineerPW();}

	void SetMakerName(CString strName) { m_pSystemData->SetMakerName(strName);}
	CString GetMakerName() { return m_pSystemData->GetMakerName();}
	void SetMakerPW(CString strPW) { m_pSystemData->SetMakerPW(strPW);}
	CString GetMakerPW() { return m_pSystemData->GetMakerName();}

	//OPTION 관련
	void SetSystemVelRatio(double dvelRatio = 0.0) { m_pSystemData->SetSystemVelRatio(dvelRatio);}
	double GetSystemVelRatio() const{return m_pSystemData->GetSystemVelRatio(); }

	void SetSystemOpMode(int nMode = 0) { m_pSystemData->SetSystemOpMode(nMode); }
	int GetSystemOpMode() { return m_pSystemData->GetSystemOpMode(); }

	void SetSystemManualMuting(BOOL bMode = FALSE) { m_pSystemData->SetSystemManualMuting(bMode); }
	BOOL GetSystemManualMuting() { return m_pSystemData->GetSystemManualMuting(); }

	void SetSystemManualSocket(BOOL bMode = FALSE) { m_pSystemData->SetSystemManualSocket(bMode); }
	BOOL GetSystemManualSocket() { return m_pSystemData->GetSystemManualSocket(); }

	void SetSystemManualSkipFilmRemove(BOOL bMode = FALSE) { m_pSystemData->SetSystemManualSkipFilmRemove(bMode); }
	BOOL GetSystemManualSkipFilmRemove() { return m_pSystemData->GetSystemManualSkipFilmRemove(); }


	BOOL SaveSystemData() { return m_pSystemData->SaveSystemData(); }
	BOOL LoadSystemData() { return m_pSystemData->LoadSystemData(); }

protected:
	CSystemData *m_pSystemData;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	afx_msg void ClickAxisManager();
	afx_msg void OnClickSystemDataLoad();
	afx_msg void OnClickSystemDataSave();
	afx_msg void OnClickTowerLampManager();
	afx_msg void OnClickHistory();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();
};
