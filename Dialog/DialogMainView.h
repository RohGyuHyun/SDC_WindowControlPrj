#pragma once


#define MAIN_SEQUENCE_DISPALY		0
//#define MAIN_MESSAGEBOX_DISPLAY		1
#define DISPLAY_CELL_NUM			8

/*
#define DISPLAY_CELL_FILM_MOVE			0
#define DISPLAY_CELL_GLASS				1
#define DISPLAY_CELL_SHUTTLE			2
#define DISPLAY_CELL_GRAB				3
*/
#define DISPLAY_CELL_JIG_B_BACK			0
#define DISPLAY_CELL_JIG_B_FORWARD		1
#define DISPLAY_CELL_JIG_A_BACK			2
#define DISPLAY_CELL_JIG_A_FORWARD		3
#define DISPLAY_CELL_TRAY_SHUTTLE		4
#define DISPLAY_CELL_GRAB_START			5
#define DISPLAY_CELL_GRAB_END			6
#define DISPLAY_CELL_CAMERA				7

#define CELL_STATE_OK					0
#define CELL_STATE_NG					1
#define CELL_STATE_EMPTY			2
#define CELL_STATE_WORK				3

#define BUTTON_READY					0
#define BUTTON_START					1
#define BUTTON_RESET					2

// CDialogMainView 대화 상자입니다.

class CDialogMainView : public CDialog
{
	DECLARE_DYNAMIC(CDialogMainView)

public:
	CDialogMainView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogMainView();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOP_MAIN_VIEW };

	CBtnEnh m_ctrlReadyButton;//IDS_BUTTON_READY
	CBtnEnh m_ctrlStartButton;
	CBtnEnh m_ctrlResetButton;

	CBtnEnh m_ctrlDisplayCell[DISPLAY_CELL_NUM];
	int m_nStateCell[DISPLAY_CELL_NUM];
	int m_nCurrentStateCell;
	//BOOL m_bAutoClearMessageBox;

	BOOL m_bViewUpdate[DISPLAY_CELL_NUM];

	BOOL IsViewUpdate(int nType) { return m_bViewUpdate[nType]; }
	void SetVIewUpdate(int nType, BOOL bState) { m_bViewUpdate[nType] = bState; }

	void DisplayViewInfo();
	//void ShowAutoClearMessageBox();

	void SetStateCell(int nType, int nState);
	void SetStateNG();
	void SetAllEmpty();
	int GetStateCell(int nType) { return m_nStateCell[nType]; }
	//void SetAutoClearMessageBox(BOOL bState) { m_bAutoClearMessageBox = bState; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
