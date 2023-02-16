#pragma once
#include "afxcmn.h"


// CDialogMainCOM 대화 상자입니다.

class CDialogMainCOM : public CDialog
{
	DECLARE_DYNAMIC(CDialogMainCOM)

public:
	CDialogMainCOM(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogMainCOM();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAIN_COM };

	CIPAddressCtrl m_ctrlSocketVisionIP;
	CBtnEnh		   m_ctrlSocketVisionPCPortNum;
	CBtnEnh		   m_btnSocketVisionPCConnect;
	CComboBox		m_ctrlComboMsg;
	CListCtrl m_list_send;
	CListCtrl m_list_receive;
	CString m_strPacket;
	/*
	CIPAddressCtrl	m_ctrlSocketDataPCIP;
	CIPAddressCtrl	m_ctrlSocketAlignPCIP;
	CBtnEnh			m_lblSocketDataPCPortNum;
	CBtnEnh			m_btnSocketDataPCConnect;
	CBtnEnh			m_lblSocketAlignPortNum;
	CBtnEnh			m_btnSocketAlignConnect;
	
	CBtnEnh			m_ctrlDataName[MAX_PARAMETER];
	CString			m_ctrlDataEdit[MAX_PARAMETER];

	*/

	void DisplayReceiveData(CString rcv_dat);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();
	DECLARE_EVENTSINK_MAP()
	void ClickVision1Connect();
	void ClickVision1Disconnect();
	void ClickVision1Send();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void ClickVision1MsgSendClear();
	void ClickVision1MsgRecvClear();

	afx_msg void OnCbnSelchangeVision1Combo();
};
