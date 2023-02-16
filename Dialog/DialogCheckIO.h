#pragma once
#include "afxcmn.h"

#include "IO.h"
// CDialogCheckIO 대화 상자입니다.

class CDialogCheckIO : public CDialog
{
	DECLARE_DYNAMIC(CDialogCheckIO)

public:
	CDialogCheckIO(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogCheckIO();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_IO_LIST };

protected:
	wchar_t m_sDILabel[MAX_INPUT_NUM][MAX_CHAR];
	wchar_t m_sDOLabel[MAX_OUTPUT_NUM][MAX_CHAR];
	BOOL m_bInputState[MAX_INPUT_NUM];
	BOOL m_bOutputState[MAX_OUTPUT_NUM];

	BOOL LoadIOLabelPara(CString path);
	void DisplayReadIO();

	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
