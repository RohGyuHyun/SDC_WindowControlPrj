#pragma once


// CDialogErrMsgBox ��ȭ �����Դϴ�.

class CDialogErrMsgBox : public CDialog
{
	DECLARE_DYNAMIC(CDialogErrMsgBox)

public:
	CDialogErrMsgBox(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogErrMsgBox();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MSG_ERROR };

	CBtnEnh m_sMessage;
	CBtnEnh m_sCode;

	void SetMessage(char *code, char *msg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_EVENTSINK_MAP()
	void ClickBuzzerOff();
	void ClickOk();
	virtual BOOL OnInitDialog();
};
