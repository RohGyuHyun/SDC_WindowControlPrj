#pragma once


// CDialogAlarmTrendView ��ȭ �����Դϴ�.
#include <vector>

#define WORST_LIST_CNT	5

typedef struct _ALARM_WORST_LIST		
{
	CString sAlarmMsg; 
	CString sAlarmCode;
	//CString sAlarmUnit;
	int iAlarmCount;
}ALARM_LIST;

typedef std::vector<_ALARM_WORST_LIST> vtAlarmTrendList;

class CDialogAlarmTrendView : public CDialog
{
	DECLARE_DYNAMIC(CDialogAlarmTrendView)

public:
	CDialogAlarmTrendView(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogAlarmTrendView();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_TOP_ALARMTREND_VIEW };
	
	BOOL m_bCheckDYTime;

	CBtnEnh m_ctrlErrorCode[WORST_LIST_CNT];
	CBtnEnh m_ctrlErrorMsg[WORST_LIST_CNT];
	CBtnEnh m_ctrlErrorCount[WORST_LIST_CNT];

	vtAlarmTrendList m_vtAlarmTrendList;

	_ALARM_WORST_LIST m_AlarmWorstFive[WORST_LIST_CNT], m_AlarmWorstTemp;

	void InitAlarmWorstFive();
	void SaveAlarmData(BOOL bDeleteFile);
	void LoadAlarmData();
	void	GetAlarmWorstFive();
	void DisplayAlarmWorstFive();
	void ResetWorstFive();
	void DispAlarmInfo();

	void AddMachineErrorChart(CString strAlCode, /*CString strAlUnit,*/ CString strAlMsg);
	void AddAlarmItem(CString strAlarmCode, /*CString strAlarmUnit,*/ CString strAlarmMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_EVENTSINK_MAP()
	void ClickAlarmTrendDyReset();
	void ClickAlarmTrendDySave();
};
