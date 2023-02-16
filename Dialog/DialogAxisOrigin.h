#pragma once
#include "afxcmn.h"


// CDialogAxisOrigin 대화 상자입니다.

class CDialogAxisOrigin : public CDialog
{
	DECLARE_DYNAMIC(CDialogAxisOrigin)

public:
	CDialogAxisOrigin(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogAxisOrigin();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_AXIS_ORIGIN };

	CBtnEnh	m_ctrlOk;
	CBtnEnh	m_ctrlCancel;
	CBtnEnh	m_ctrlOriginRun;
	CBtnEnh m_ctrlAmpAllOn;
	CBtnEnh m_ctrlAmpAllOff;
	CBtnEnh m_ctrlAxis[2];
	CBtnEnh m_ctrlSelectAll;
	
	BOOL m_bAmpOnFlag;

	int m_iAxisNum;
	BOOL m_bClickSelectAll;

	BOOL m_bClickAxis[MAX_AXIS_NUM];

	int m_nProcessMoveIdx;
	BOOL OriginRun(int iOrder, int &iAxis);	//전체 축 할때만...

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	afx_msg void OnClickOriginRun();
	afx_msg void OnClickAmpAllOff();
	afx_msg void OnClickAmpAllOn();
	void ClickOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	CBtnEnh m_ctrl_progress;
	void ClickCancel();
	void ClickSelectAllAxis();
};
