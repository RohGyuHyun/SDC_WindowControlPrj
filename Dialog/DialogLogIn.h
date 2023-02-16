#pragma once


// CDialogLogIn 대화 상자입니다.
//#include "resource.h"

class CDialogLogIn : public CDialog
{
	DECLARE_DYNAMIC(CDialogLogIn)

public:
	CDialogLogIn(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogLogIn();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LOGIN };

	CBtnEnh	m_ctrlPassword;
	CBtnEnh	m_ctrlCancel;
	CBtnEnh	m_ctrlOk;
	CBtnEnh	m_ctrlChangePW;
	CBtnEnh	m_ctrlUserName;
	CBtnEnh	m_ctrlPW;
	CBtnEnh	m_ctrlUserOp;
	CBtnEnh	m_ctrlUserMaker;
	CBtnEnh	m_ctrlUserEngineer;

	void DispUserName();
	typeLogInData GetLogInData() { return m_stLogInData;}
	void LogInOp();

private:
	int m_nSelUser;
	typeLogInData m_stLogInData;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	afx_msg void OnClickOk();
	afx_msg void OnClickCancel();
	afx_msg void OnClickChangePassword();
	afx_msg void OnClickPassword();
	afx_msg void OnClickUserOp();
	afx_msg void OnClickUserEngineer();
	afx_msg void OnClickUserMaker();
	DECLARE_EVENTSINK_MAP()

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
