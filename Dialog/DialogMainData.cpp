// DialogMainData.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SDC_WindowControlPrj.h"

#include "Axis.h"
#include "MsgBox.h"									//전역

#include "Clientsok.h"
#include "DialogAxisJog.h"
#include "DialogErrMsgBox.h"
#include "DialogTeach.h"
#include "DialogTackTimeView.h"
#include "DialogProductInfoView.h"
#include "DialogMsgBox.h"
#include "DialogManual.h"
#include "DialogMainView.h"
#include "DialogMainData.h"
#include "DialogMainCOM.h"
#include "DialogLogIn.h"
#include "DialogCheckIO.h"
#include "DialogAxisOrigin.h"
#include "DialogAxisManager.h"
#include "DialogAxisJog.h"
#include "DialogAlarmTrendView.h"

#include "SDC_WindowControlPrjDlg.h"

#include "afxdialogex.h"


// CDialogMainData 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogMainData, CDialog)

CDialogMainData::CDialogMainData(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMainData::IDD, pParent)
{
	m_pSystemData			= NULL;
	m_pSystemData			= new CSystemData;
}

CDialogMainData::~CDialogMainData()
{
	if(NULL != m_pSystemData)
	{
		delete m_pSystemData;
		m_pSystemData = NULL;
	}
}

void CDialogMainData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATA_MODE_AUTO, m_ctrlOpModeAutoRun);
	DDX_Control(pDX, IDC_DATA_MODE_DRYRUN, m_ctrlOpModeDryRun);
	DDX_Control(pDX, IDC_DATA_ITEM_ON_1, m_ctrlManualMutingOn);
	DDX_Control(pDX, IDC_DATA_ITEM_OFF_1, m_ctrlManualMutingOff);
	DDX_Control(pDX, IDC_DATA_ITEM_ON_2, m_ctrlManualSocketOn);
	DDX_Control(pDX, IDC_DATA_ITEM_OFF_2, m_ctrlManualSocketOff);
	DDX_Control(pDX, IDC_DATA_ITEM_ON_3, m_ctrlManualSkipFilmRemoveOn);
	DDX_Control(pDX, IDC_DATA_ITEM_OFF_3, m_ctrlManualSkipFilmRemoveOff);
}


BEGIN_MESSAGE_MAP(CDialogMainData, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDialogMainData 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CDialogMainData, CDialog)
	ON_EVENT(CDialogMainData, IDB_AXIS_MANAGER, DISPID_CLICK, CDialogMainData::ClickAxisManager, VTS_NONE)
	ON_EVENT(CDialogMainData, IDB_TOWER_LAMP_MANAGER, DISPID_CLICK, CDialogMainData::OnClickTowerLampManager, VTS_NONE)
	ON_EVENT(CDialogMainData, IDB_HISTORY, DISPID_CLICK, CDialogMainData::OnClickHistory, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_SYSTEM_DATA_LOAD, DISPID_CLICK, CDialogMainData::OnClickSystemDataLoad, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_SYSTEM_DATA_SAVE, DISPID_CLICK, CDialogMainData::OnClickSystemDataSave, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_DATA_MODE_AUTO, DISPID_CLICK, CDialogMainData::OnClickDataItemOn, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_DATA_MODE_DRYRUN, DISPID_CLICK, CDialogMainData::OnClickDataItemOn, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_DATA_MODE_BYPASS, DISPID_CLICK, CDialogMainData::OnClickDataItemOn, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_OPTION_LAN_KOREA, DISPID_CLICK, CDialogMainData::OnClickDataItemOn, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_OPTION_LAN_NONE, DISPID_CLICK, CDialogMainData::OnClickDataItemOn, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_OPTION_LAN_ENGLISH, DISPID_CLICK, CDialogMainData::OnClickDataItemOn, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_DATA_ITEM_ON_1, DISPID_CLICK, CDialogMainData::OnClickDataItemOn, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_DATA_ITEM_OFF_1, DISPID_CLICK, CDialogMainData::OnClickDataItemOff, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_DATA_ITEM_ON_2, DISPID_CLICK, CDialogMainData::OnClickDataItemOn, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_DATA_ITEM_OFF_2, DISPID_CLICK, CDialogMainData::OnClickDataItemOff, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_DATA_ITEM_ON_3, DISPID_CLICK, CDialogMainData::OnClickDataItemOn, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_DATA_ITEM_OFF_3, DISPID_CLICK, CDialogMainData::OnClickDataItemOff, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_DATA_ITEM_ON_4, DISPID_CLICK, CDialogMainData::OnClickDataItemOn, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_DATA_ITEM_OFF_4, DISPID_CLICK, CDialogMainData::OnClickDataItemOff, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_DATA_ITEM_ON_5, DISPID_CLICK, CDialogMainData::OnClickDataItemOn, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_DATA_ITEM_OFF_5, DISPID_CLICK, CDialogMainData::OnClickDataItemOff, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_DATA_ITEM_ON_6, DISPID_CLICK, CDialogMainData::OnClickDataItemOn, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_DATA_ITEM_OFF_6, DISPID_CLICK, CDialogMainData::OnClickDataItemOff, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_DATA_ITEM_ON_7, DISPID_CLICK, CDialogMainData::OnClickDataItemOn, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_DATA_ITEM_OFF_7, DISPID_CLICK, CDialogMainData::OnClickDataItemOff, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_DATA_ITEM_ON_8, DISPID_CLICK, CDialogMainData::OnClickDataItemOn, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_DATA_ITEM_OFF_8, DISPID_CLICK, CDialogMainData::OnClickDataItemOff, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_DATA_ITEM_ON_9, DISPID_CLICK, CDialogMainData::OnClickDataItemOn, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_DATA_ITEM_OFF_9, DISPID_CLICK, CDialogMainData::OnClickDataItemOff, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_DATA_ITEM_ON_10, DISPID_CLICK, CDialogMainData::OnClickDataItemOn, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_DATA_ITEM_OFF_10, DISPID_CLICK, CDialogMainData::OnClickDataItemOff, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_DATA_ITEM_ON_11, DISPID_CLICK, CDialogMainData::OnClickDataItemOn, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_DATA_ITEM_OFF_11, DISPID_CLICK, CDialogMainData::OnClickDataItemOff, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_SYSTEM_PARAM_1, DISPID_CLICK, CDialogMainData::OnClickSystemParam, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_SYSTEM_PARAM_2, DISPID_CLICK, CDialogMainData::OnClickSystemParam, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_SYSTEM_PARAM_3, DISPID_CLICK, CDialogMainData::OnClickSystemParam, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_SYSTEM_PARAM_4, DISPID_CLICK, CDialogMainData::OnClickSystemParam, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_SYSTEM_PARAM_5, DISPID_CLICK, CDialogMainData::OnClickSystemParam, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_SYSTEM_PARAM_6, DISPID_CLICK, CDialogMainData::OnClickSystemParam, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_SYSTEM_PARAM_7, DISPID_CLICK, CDialogMainData::OnClickSystemParam, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_SYSTEM_PARAM_8, DISPID_CLICK, CDialogMainData::OnClickSystemParam, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_SYSTEM_PARAM_9, DISPID_CLICK, CDialogMainData::OnClickSystemParam, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_SYSTEM_PARAM_10, DISPID_CLICK, CDialogMainData::OnClickSystemParam, VTS_NONE)
	ON_EVENT(CDialogMainData, IDC_SYSTEM_PARAM_11, DISPID_CLICK, CDialogMainData::OnClickSystemParam, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogMainData::ClickAxisManager()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
	
	pdlg->ShowDataAxisManager();
}


BOOL CDialogMainData::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_pSystemData->LoadSystemData();//Load SystemData

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialogMainData::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;


	if(bShow)
	{
		m_pSystemData->LoadSystemData();
		m_dParamData[SYSTEM_VEL_RATIO] = m_pSystemData->GetSystemVelRatio();
		m_iOptionData[SYSTEM_OP_MODE] = m_pSystemData->GetSystemOpMode();
		m_iOptionData[SYSTEM_MANUAL_MUTING] = m_pSystemData->GetSystemManualMuting();
		m_iOptionData[SYSTEM_MANUAL_SOCKET] = m_pSystemData->GetSystemManualSocket();
		m_iOptionData[SYSTEM_MANUAL_SKIPFILMREMOVE] = m_pSystemData->GetSystemManualSkipFilmRemove();

		DisplaySystemData();

		if(m_pSystemData->GetCurUserName() == LOG_IN_NAME_OPERATOR)
		{
			GetDlgItem(IDC_SYSTEM_DATA_SAVE)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_SYSTEM_DATA_SAVE)->EnableWindow(TRUE);
		}
	}
}

void CDialogMainData::DisplaySystemData()
{
	DisplaySystemOption();
	DisplaySystemParam();
}

void CDialogMainData::DisplaySystemOption()
{
	switch(m_iOptionData[SYSTEM_OP_MODE])
	{
	case SYSTEM_OP_MODE_AUTORUN:
		m_ctrlOpModeAutoRun.SetValue(TRUE);
		m_ctrlOpModeDryRun.SetValue(FALSE);
		break;
	case SYSTEM_OP_MODE_DRYRUN:
		m_ctrlOpModeAutoRun.SetValue(FALSE);
		m_ctrlOpModeDryRun.SetValue(TRUE);
		break;
	default:

		break;
	}
	
	switch(m_iOptionData[SYSTEM_MANUAL_MUTING])
	{
	case SYSTEM_MANUAL_MUTING_OFF:
		m_ctrlManualMutingOn.SetValue(FALSE);
		m_ctrlManualMutingOff.SetValue(TRUE);
		break;
	case SYSTEM_MANUAL_MUTING_ON:
		m_ctrlManualMutingOn.SetValue(TRUE);
		m_ctrlManualMutingOff.SetValue(FALSE);
		break;
	default:

		break;
	}

	switch(m_iOptionData[SYSTEM_MANUAL_SOCKET])
	{
	case SYSTEM_MANUAL_SOCKET_OFF:
		m_ctrlManualSocketOn.SetValue(FALSE);
		m_ctrlManualSocketOff.SetValue(TRUE);
		break;
	case SYSTEM_MANUAL_SOCKET_ON:
		m_ctrlManualSocketOn.SetValue(TRUE);
		m_ctrlManualSocketOff.SetValue(FALSE);
		break;
	default:
		break;
	}

	switch(m_iOptionData[SYSTEM_MANUAL_SKIPFILMREMOVE])
	{
	case SYSTEM_MANUAL_SKIPFILMREMOVE_OFF:
		m_ctrlManualSkipFilmRemoveOn.SetValue(FALSE);
		m_ctrlManualSkipFilmRemoveOff.SetValue(TRUE);
		break;
	case SYSTEM_MANUAL_SKIPFILMREMOVE_ON:
		m_ctrlManualSkipFilmRemoveOn.SetValue(TRUE);
		m_ctrlManualSkipFilmRemoveOff.SetValue(FALSE);
		break;
	default:
		break;
	}
}
	


void CDialogMainData::DisplaySystemParam()
{
	CString strData;

	strData.Format(_T("%3.3f"),m_dParamData[SYSTEM_VEL_RATIO]);
	GetDlgItem(IDC_SYSTEM_PARAM_1)->SetWindowText(strData);
}

void CDialogMainData::OnClickDataItemOn()
{
	int nBtnID(0);

	CBtnEnh *pBtnEnh = (CBtnEnh*)GetFocus();
	nBtnID = pBtnEnh->GetDlgCtrlID();
	switch(nBtnID)
	{
	case IDC_DATA_MODE_AUTO:
		m_iOptionData[SYSTEM_OP_MODE] = SYSTEM_OP_MODE_AUTORUN;
		break;
	case IDC_DATA_MODE_DRYRUN:
		m_iOptionData[SYSTEM_OP_MODE] = SYSTEM_OP_MODE_DRYRUN;
		break;
	case IDC_DATA_MODE_BYPASS:
		break;
	case IDC_OPTION_LAN_KOREA:
		break;
	case IDC_OPTION_LAN_NONE:
		break;
	case IDC_OPTION_LAN_ENGLISH:
		break;
	case IDC_DATA_ITEM_ON_1:
		m_iOptionData[SYSTEM_MANUAL_MUTING] = SYSTEM_MANUAL_MUTING_ON;
		break;
	case IDC_DATA_ITEM_ON_2:
		m_iOptionData[SYSTEM_MANUAL_SOCKET] = SYSTEM_MANUAL_SOCKET_ON;
		break;
	case IDC_DATA_ITEM_ON_3:
		m_iOptionData[SYSTEM_MANUAL_SKIPFILMREMOVE] = SYSTEM_MANUAL_SKIPFILMREMOVE_ON;
		break;
	case IDC_DATA_ITEM_ON_4:
		break;
	case IDC_DATA_ITEM_ON_5:
		break;
	case IDC_DATA_ITEM_ON_6:
		break;
	case IDC_DATA_ITEM_ON_7:
		break;
	case IDC_DATA_ITEM_ON_8:
		break;
	case IDC_DATA_ITEM_ON_9:
		break;
	case IDC_DATA_ITEM_ON_10:
		break;
	case IDC_DATA_ITEM_ON_11:
		break;
	default:
		break;
	}		
}

void CDialogMainData::OnClickDataItemOff()
{
	int nBtnID(0);

	CBtnEnh *pBtnEnh = (CBtnEnh*)GetFocus();
	nBtnID = pBtnEnh->GetDlgCtrlID();
	
	switch(nBtnID)
	{
	case IDC_DATA_ITEM_OFF_1:
		m_iOptionData[SYSTEM_MANUAL_MUTING] = SYSTEM_MANUAL_MUTING_OFF;
		break;
	case IDC_DATA_ITEM_OFF_2:
		m_iOptionData[SYSTEM_MANUAL_SOCKET] = SYSTEM_MANUAL_SOCKET_OFF;
		break;
	case IDC_DATA_ITEM_OFF_3:
		m_iOptionData[SYSTEM_MANUAL_SKIPFILMREMOVE] = SYSTEM_MANUAL_SKIPFILMREMOVE_OFF;
		break;
	case IDC_DATA_ITEM_OFF_4:
		break;
	case IDC_DATA_ITEM_OFF_5:
		break;
	case IDC_DATA_ITEM_OFF_6:
		break;
	case IDC_DATA_ITEM_OFF_7:
		break;
	case IDC_DATA_ITEM_OFF_8:
		break;
	case IDC_DATA_ITEM_OFF_9:
		break;
	case IDC_DATA_ITEM_OFF_10:
		break;
	case IDC_DATA_ITEM_OFF_11:
		break;
	default:
		break;
	}
}

void CDialogMainData::OnClickSystemParam()
{
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
	//CSystemData* sys_data;
	//sys_data = pdlg->GetSystemData();

	int nBtnID(0);
	int nVal(0);
	int nPosLimit(0), nNegLimit(0);
	double dVal, dPoslimit, dNegLimit;

	CBtnEnh *pBtnEnh = (CBtnEnh*)GetFocus();
	nBtnID = pBtnEnh->GetDlgCtrlID();

	CString strVal, strMsg;
	CDialogNumberBox dlg;

	if(dlg.DoModal() != IDOK)
		return;

	strVal = dlg.GetstrNum();
	dVal = _ttof(strVal);
	nVal = (int)_ttof(strVal);

	switch(nBtnID)
	{
	case IDC_SYSTEM_PARAM_1:
		dPoslimit = 101;
		dNegLimit = 10;
		if(dVal > dPoslimit || dVal < dNegLimit)
		{
			strMsg.Format(_T("Data Input Range is %3.3f ~ %3.3f! Check Input Data!"),dNegLimit,dPoslimit);
			g_cMsgBox.OkMsgBox(strMsg);
			return;
		}
		m_dParamData[0] = dVal;
		strMsg.Format(_T("CDataView::OnClickSystemParam()-IDC_SYSTEM_PARAM_1[%s]"),strVal);
		//theApp.m_pSystemLog->LOG_INFO(strMsg); 
		WriteLog(_T("Program START"), SYSTEM_LOG);
		break;
		
	case IDC_SYSTEM_PARAM_2:
		
		break;
	case IDC_SYSTEM_PARAM_3:
		
		break;
	case IDC_SYSTEM_PARAM_4:
		
		break;
	case IDC_SYSTEM_PARAM_5:
		break;
	case IDC_SYSTEM_PARAM_6:
		break;
	case IDC_SYSTEM_PARAM_7:
		break;
	case IDC_SYSTEM_PARAM_8:
		break;
	case IDC_SYSTEM_PARAM_9:
		break;
	case IDC_SYSTEM_PARAM_10:
		break;
	case IDC_SYSTEM_PARAM_11:
		break;
	default:
		break;
	}		

	DisplaySystemParam();
}

void CDialogMainData::OnClickSystemDataLoad()
{
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	if(g_cMsgBox.YesNoMsgBox(_T("DATA 를 LOAD 하시겠습니까?"))==IDYES)
	{
		m_pSystemData->LoadSystemData();
		DisplaySystemData();
		
		//pdlg->m_pDlgManual->MotLoadAxisConfig();
	}
}
void CDialogMainData::OnClickSystemDataSave()
{
	if(g_cMsgBox.YesNoMsgBox(_T("DATA 를 저장 하시겠습니까?"))==IDYES)
	{
		CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

		m_pSystemData->SetSystemVelRatio(m_dParamData[SYSTEM_VEL_RATIO]);
		m_pSystemData->SetSystemOpMode(m_iOptionData[SYSTEM_OP_MODE]);
		m_pSystemData->SetSystemManualMuting(m_iOptionData[SYSTEM_MANUAL_MUTING]);
		m_pSystemData->SetSystemManualSocket(m_iOptionData[SYSTEM_MANUAL_SOCKET]);
		m_pSystemData->SetSystemManualSkipFilmRemove(m_iOptionData[SYSTEM_MANUAL_SKIPFILMREMOVE]);
		
		m_pSystemData->SaveSystemData();

		DisplaySystemData();
	}
}

void CDialogMainData::OnClickTowerLampManager() 
{
	//COpTowerDlg dlg;
	//dlg.DoModal();
}

void CDialogMainData::OnClickHistory() 
{
	CString strFile;

	strFile.Format(_T("%s\\History.txt"),DATA_SYSTEM_PATH);

	//	ShellExecute(NULL, "open", "notepad.exe", strFile, NULL, SW_SHOW);	
	ShellExecute(NULL, _T("open"), _T("notepad.exe"), strFile, NULL, SW_SHOW); //131011 JSPark

}

void CDialogMainData::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
