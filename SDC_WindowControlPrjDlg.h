
// SDC_WindowControlPrjDlg.h : 헤더 파일
//

#pragma once
#include "resource.h"

//Motion Process Status///////////////////////////////////////////////////////
typedef struct MotionProcessStatus
{
	//I/O status
	//int m_bIOIn;

	//Timeout
	double m_dTimeoutS;//시작 시간
	double m_dTimeoutE;//종료 시간(현재)
	double m_dTimeout;//지정된 시간
	
	//Status문자(에러 또는 정상종료시 출력할 문자열)
	char m_cNGStatus[MAX_CHAR];
	char m_cNGCode[ERR_CHAR];
	/*
	int m_iNGCode;
	int m_iNGCategory;
	int m_iNGLevel;
	*/
	//char m_cGoodStatus[MAX_CHAR];
	char m_iTypeStatus[MAX_CHAR];
	
	int m_iNGSeqIdx;

}typeMotionProcessSts;


//Process Result///////////////////////////////////////////////////////
typedef struct ProcessResult
{
	//Motion
	//double m_dMotPos[MAX_AXIS_NUM];

	
	//Loader/unloader
	/*
	BOOL m_bWaitLoading;
	BOOL m_bCompleteLoading;
	BOOL m_bCassetEmpty;
	
	char m_sLoaderUnloaderErrMsg[MAX_CHAR];
	int m_iLoaderUnloaderErrCode;
	

	//Inspect Sequence
	BOOL m_bStartManualInspect;
	BOOL m_bInspectionMove;
	*/
	
	int m_iFilmRemoveSeq;

}typeProcessResult;

//Motion Sequence///////////////////////////////////////////////////////
typedef struct MotionSeq
{
	//Sequence
	int m_iSubSeqIdx;
	int m_iDispSubSeqIdx;
	int m_iNextSubSeqIdx;

	int m_iSubRoutineIdx;

	//Flag
	BOOL m_bAutoStart;
	//BOOL m_bHomeOK;
	//BOOL m_bHomeStart;
	//BOOL m_bEmerAlarm;
	//BOOL m_bLotStart;//0 : 정상 상태, 1 : 남은상태(정상상태가 아니면 남은상태에 따라서 비젼 검사를 하지않는다)
	//BOOL m_bLotEnd;
	BOOL m_bAlarm;


	//버튼중심의 동작상태
	BOOL m_bOpSts;

	//정보
	int m_iDeviceIdx;


}typeMotionSeq;

//Motion flag///////////////////////////////////////////////////////
typedef struct MotionFlag
{
	BOOL m_bProcessThread;
	
	BOOL m_bAutoStart;
	//BOOL m_bMainHomeStart;

	BOOL m_bFirstStart;

	BOOL m_bAlarm;
}typeMotionFlag;


class CMotionData  
{
public:

	CMotionData();
	virtual ~CMotionData();

	//Motion process status
	typeMotionProcessSts m_ProSts[MOT_STATUS_MAX_NUM][MOT_STATUS_SUB_MAX_NUM];

	//Motion sequence
	typeMotionSeq m_Seq[MOT_STATUS_MAX_NUM];

	void ResetSeq();
};

// CSDC_WindowControlPrjDlg 대화 상자
class CSDC_WindowControlPrjDlg : public CDialogEx
{
// 생성입니다.
public:
	CSDC_WindowControlPrjDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SDC_WINDOWCONTROLPRJ_DIALOG };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

protected:
	int m_nOldViewCtrl;
	int m_nSelectTopTab;
	int m_nOldSelectTopTab;

	BOOL m_bOperatorFlag;
	BOOL m_bEngineerFlag;
	BOOL m_bMakerFlag;

	CString	m_strLogIn;

	BOOL m_bGrabStart;
	BOOL m_bGrabEnd;
	BOOL m_bInspectEnd;
	CString m_strInnerID;
	int m_nVisionResult;
	CString m_strStartTime, m_strEndTime;
	long m_lTotalCell;
	long m_lPassCell;
	long m_lFailCell;
	double m_dFailRatioCell;

	long m_lStartTime;
	long m_lEndTime;

	BOOL m_bClickDoorLock;
	
	//3D Resource//////////////////////////////////////////////////////
	CBtnEnh m_ctrlTitleLabel;
	CBtnEnh m_ctrlDateLabel;
	CBtnEnh m_ctrlModelNameLabel;
	CBtnEnh m_ctrlVersionLabel;
	CBtnEnh m_ctrlVisionPCStage;
	CBtnEnh m_ctrlSystemInfo;
	CBtnEnh m_ctrlLogIn;
	CBtnEnh m_ctrlMainOP_EQPID;
	CBtnEnh m_ctrlMainOP_Mode;
	CBtnEnh m_ctrlMainOP_State;
	CBtnEnh m_ctrlMainProduct_Total;
	CBtnEnh m_ctrlMainProduct_Pass;
	CBtnEnh m_ctrlMainProduct_Fail;
	CBtnEnh m_ctrlMainProduct_Fail_Ratio;
	CBtnEnh m_ctrlOpStart;
	CBtnEnh m_ctrlOpStop;
	CBtnEnh m_ctrlOpReset;
	CBtnEnh m_ctrlOpDoor;
	/////////////////////////////////////////////////////////////////

	CClientSock *m_pClient;
	BOOL m_bIsVisionConected;
	
	//Main Sequence Process
	CMotionData m_MDat[MOT_PROCESS_MAX_NUM];

	//Motion Flag
	typeMotionFlag m_Flag;

	//Process result
	typeProcessResult m_Rslt;

	CWinThread   *m_pThread;
    UINT static   MotionThread(LPVOID pParam);
	
	void MotionThread();
	int MotionProcess(int prc_idx, int seq_idx);
	void AlarmOperation(int pro_idx, int sts_idx, int sub_seq_idx);
	int StopModeOperation(int *prc_idx, int *seq_idx);
	void SetLotInit();
	void SetSeqIdx(int prc_idx, int seq_idx, int seq_sub_idx);
	void SetSeqIdx(int prc_idx, int seq_idx, int seq_sub_idx, int next_seq_sub_idx, int type);
	void SetSeqIdx(int prc_idx, int seq_idx, int seq_sub_idx, int next_seq_sub_idx, int type, int device_idx);
	int CheckTimeout(int prc_idx, int seq_idx, int seq_sub_idx, char* status_type);
	int CheckDelay(int prc_idx, int seq_idx, int delay);

	LRESULT OnMotMessage(WPARAM para0, LPARAM para1);
	///////////////////////////////////////////////

public:
	//SubDialog//////////////////////////////////////////////////////
	//TOP
	CDialogMainView					*m_pDlgMainView;
	CDialogTackTimeView			*m_pDlgTackTimeView;
	CDialogProductInfoView		*m_pDlgProductInfoView;
	CDialogAlarmTrendView		*m_pDlgAlarmTrendView;
	CDialogLogIn						*m_pDlgLogIn;
	CDialogErrMsgBox				*m_dlgLogAlarm;

	//BOTTOM
	CDialogManual						*m_pDlgManual;
	CDialogTeach						*m_pDlgTeach;
	CDialogMainCOM					*m_pDlgMainCOM;
	CDialogMainData					*m_pDlgMainData;
	//////////////////////////////////////////////////////////////////
	
	void CreateSheet();
	void SwitchingView(UINT nID);
	void ChangeTopTabStatus(BOOL bShow);
	void DisplayEQPStatus();
	void DisplayProductInfo();
	BOOL CreateClientSocket(CString strIP, int nPort);

	int AutoStart();
	void AutoStop();
	int AutoReset();

	void ShowManualOCheck();
	void ShowManualOrigin();
	void ShowTeachAxisJog();
	void ShowDataAxisManager();
	void DisplayPCSoket();
	void OnClickSoketRetry();

	void MakeDefaultDir();
	void OnSend(CString data);

	void SetModelName(CString strModelName) ;
	void SetLogInOperator();
	void SetLogIn();
	void SetOperator(BOOL bFlag)	{ m_bOperatorFlag = bFlag; }
	BOOL IsOperator()				{return m_bOperatorFlag;}
	void SetEngineer(BOOL bFlag)	{ m_bEngineerFlag = bFlag; }
	BOOL IsEngineer()				{return m_bEngineerFlag;}
	void SetMaker(BOOL bFlag)		{ m_bMakerFlag = bFlag; }
	BOOL IsMaker()					{return m_bMakerFlag;}	

	//Free Function
	void FreeDialog();
	void FreePointerVar();

	void SetFirstStart(BOOL state) { m_Flag.m_bFirstStart = state; }
	BOOL GetSocketState() { return m_bIsVisionConected; }
	BOOL GetDoorState() { return m_bClickDoorLock; }

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnConnect(WPARAM, LPARAM);
	afx_msg LRESULT OnReceive(WPARAM, LPARAM);
	afx_msg LRESULT OnClose(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBnClickedButtonTest();
	DECLARE_EVENTSINK_MAP()
	afx_msg void ClickTabWorkInfo();
	afx_msg void OnClickViewBtn();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ClickLogIn();
	afx_msg void OnClickAutoRun();
	void ClickMainOpInit();
	void ClickLogviewBtn();
	void ClickMainProductReset();
public:
	void ClickMainOpDoor();
};
