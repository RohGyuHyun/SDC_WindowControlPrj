#pragma once

#define  MS_OK   			  0x10
#define  MS_OKCANCEL 		  0x20
#define  MS_YESNO			  0x30
#define  MS_NGVIEW			  0x40
// CDialogMsgBox ��ȭ �����Դϴ�.

class CDialogMsgBox : public CDialog
{
	DECLARE_DYNAMIC(CDialogMsgBox)

public:
	//CDialogMsgBox(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	CDialogMsgBox(int iStyle = MS_OK, int level=0, CString strMsg = _T(""), CWnd* pParent = NULL);
	virtual ~CDialogMsgBox();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MSG_BOX };
	
	CBtnEnh	m_ctrlNo;
	CBtnEnh	m_ctrlOk;
	CBtnEnh	m_ctrlYes;
	CBtnEnh	m_ctrlMark1;
	CBtnEnh	m_ctrlMark2;
	CBtnEnh	m_ctrlMsg;

private:
	CString m_strMsg;
	int m_iStyle;
	int m_iLevel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	void ClickYes();
	void ClickOk();
	void ClickStop();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ClickNo();
};
