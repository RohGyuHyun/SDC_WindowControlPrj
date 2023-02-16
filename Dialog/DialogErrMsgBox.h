#pragma once


// CDialogErrMsgBox 대화 상자입니다.

class CDialogErrMsgBox : public CDialog
{
	DECLARE_DYNAMIC(CDialogErrMsgBox)

public:
	CDialogErrMsgBox(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogErrMsgBox();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MSG_ERROR };

	CBtnEnh m_sMessage;
	CBtnEnh m_sCode;

	void SetMessage(char *code, char *msg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_EVENTSINK_MAP()
	void ClickBuzzerOff();
	void ClickOk();
	virtual BOOL OnInitDialog();
};
