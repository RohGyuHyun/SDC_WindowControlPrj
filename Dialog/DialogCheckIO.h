#pragma once
#include "afxcmn.h"

#include "IO.h"
// CDialogCheckIO ��ȭ �����Դϴ�.

class CDialogCheckIO : public CDialog
{
	DECLARE_DYNAMIC(CDialogCheckIO)

public:
	CDialogCheckIO(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogCheckIO();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_IO_LIST };

protected:
	wchar_t m_sDILabel[MAX_INPUT_NUM][MAX_CHAR];
	wchar_t m_sDOLabel[MAX_OUTPUT_NUM][MAX_CHAR];
	BOOL m_bInputState[MAX_INPUT_NUM];
	BOOL m_bOutputState[MAX_OUTPUT_NUM];

	BOOL LoadIOLabelPara(CString path);
	void DisplayReadIO();

	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void ClickExitBtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CListCtrl m_listctrl_input;
	CListCtrl m_listctrl_output;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMClickListOutput(NMHDR *pNMHDR, LRESULT *pResult);
};
