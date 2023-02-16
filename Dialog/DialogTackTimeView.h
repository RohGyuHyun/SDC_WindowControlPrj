#pragma once

//#include "TactTimeData.h"

#define MAX_TT_UNIT_CNT	10

#define COMPLETE_FILM_REMOVE 0
#define INPUT_START_BUTTON 1
#define INSERT_JIG 2
#define IN_SHUTTLE 3
#define SHUTTLE_MOVE_GRAB_START 4
#define SHUTTLE_MOVE_GRAB_END	5
#define COMPLETE_GRAB 6
#define IN_JIG_B 7
#define PASS_JIG 8
#define TOTAL_TACT_TIME	9
// CDialogTackTimeView 대화 상자입니다.

class CDialogTackTimeView : public CDialog
{
	DECLARE_DYNAMIC(CDialogTackTimeView)

public:
	CDialogTackTimeView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogTackTimeView();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOP_TACTTIME_VIEW };

	CBtnEnh m_lblUnitName[MAX_TT_UNIT_CNT];
	CBtnEnh m_lblUnitTactTime[MAX_TT_UNIT_CNT];
	CBtnEnh m_lblUnitMinTactTime[MAX_TT_UNIT_CNT];
	CBtnEnh m_lblUnitMaxTactTime[MAX_TT_UNIT_CNT];

	BOOL m_bTTUpdate[MAX_TT_UNIT_CNT];
	long m_lTactTime[MAX_TT_UNIT_CNT];
	long m_lMinTactTime[MAX_TT_UNIT_CNT];
	long m_lMaxTactTime[MAX_TT_UNIT_CNT];

	BOOL IsTTUpdate(int nType);
	void SetTTUpdate(int nType, BOOL bState);
	double GetTactTime(int nType);
	double GetMinTactTime(int nType);
	double GetMaxTactTime(int nType);
	void SetTactTime(int nType, long time);
	void SetMinTactTime(int nType, long time);
	void SetMaxTactTime(int nType, long time);

	void DispTactTimeInfo();
	void InitialTotalTactTime();
	void WriteTactLog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

