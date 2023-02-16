#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "ModelData.h"

#define	MAX_CUR_POS_AXIS	2
#define MAX_POS_LIST_CNT	4
// CDialogTeach 대화 상자입니다.

class CDialogTeach : public CDialog
{
	DECLARE_DYNAMIC(CDialogTeach)

public:
	CDialogTeach(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogTeach();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAIN_TEACH };
protected:
	CModelData	*m_pModelData;

public:
	CBtnEnh	m_ctrlSelectModelName;
	CBtnEnh m_ctrlWorkModelName;
	CBtnEnh	m_ctrlAxisCurrentName[MAX_CUR_POS_AXIS];
	CBtnEnh	m_ctrlAxisCurrentPos[MAX_CUR_POS_AXIS];
	CBtnEnh	m_ctrlTeachPos_X[MAX_POS_LIST_CNT];
	CBtnEnh	m_ctrlTeachPos_Y[MAX_POS_LIST_CNT];
	CBtnEnh m_ctrl_progress;
	int m_nProcessMoveIdx;

	CListCtrl m_listctrl_model;
	int		m_nAxisID;
	int		m_nPosType;

	void DisplayModelData();
	void DisplayModelList();
	void InitListCtrlModel();
	void DisplayCurrentPos();
	void InitJogDlgOpInfo();

	double GetLoadPosX() { return m_pModelData->GetLP_LoadPosX(); }
	double GetInspPosX() { return m_pModelData->GetLP_InspPosX(); }
	double GetUnLoadPosX() { return m_pModelData->GetLP_UnloadPosX(); }
	double GetSparePosX() { return m_pModelData->GetLP_SparePosX(); }

	double GetLoadPosY() { return m_pModelData->GetIS_LoadPosY(); }
	double GetInspPosY() { return m_pModelData->GetIS_InspPosY(); }
	double GetUnLoadPosY() { return m_pModelData->GetIS_UnloadPosY(); }
	double GetCamCenterPosY() { return m_pModelData->GetIS_CamCenterPosY(); }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnNMClickListModel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	afx_msg void OnClickTeachSetAxis();
	afx_msg void OnClickTeachSetPos();
	afx_msg void OnClickTeachSetData();
	void ClickModelAdd();
	void ClickModelDelete();
	void ClickModelChange();
	void ClickModelDataSave();
	void ClickBtnJog();
	void ClickTeachSetTargetPos();
	void ClickTeachMovePos();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ClickTeachMoveStop();
	void ClickModelListUp();
	void ClickModelListDown();
};
