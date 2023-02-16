#pragma once
#include "afxcmn.h"


// CDialogAxisOrigin ��ȭ �����Դϴ�.

class CDialogAxisOrigin : public CDialog
{
	DECLARE_DYNAMIC(CDialogAxisOrigin)

public:
	CDialogAxisOrigin(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogAxisOrigin();

// ��ȭ ���� �������Դϴ�.
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
	BOOL OriginRun(int iOrder, int &iAxis);	//��ü �� �Ҷ���...

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
