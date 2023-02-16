// DialogAxisManager.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SDC_WindowControlPrj.h"

#include "MsgBox.h"									//전역

//DIALOG
#include "Clientsok.h"
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


// CDialogAxisManager 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogAxisManager, CDialog)

CDialogAxisManager::CDialogAxisManager(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogAxisManager::IDD, pParent)
{
	m_iAxis = 0;
}

CDialogAxisManager::~CDialogAxisManager()
{
}

void CDialogAxisManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDS_MAX_VEL, m_ctrlMaxVel);
	DDX_Control(pDX, IDS_MAX_ACC, m_ctrlMaxAcc);
	DDX_Control(pDX, IDS_MIN_ACC, m_ctrlMinAcc);
	DDX_Control(pDX, IDS_MIN_VEL, m_ctrlMinVel);
	DDX_Control(pDX, IDC_AXIS_NAME, m_ctrlAxisName);
	DDX_Control(pDX, IDS_CUR_POS, m_ctrlCurPos);
	DDX_Control(pDX, IDS_AMP_STATE, m_ctrlAmpState);
	DDX_Control(pDX, IDS_POSITIVE_SENSOR, m_ctrlPosSensor);
	DDX_Control(pDX, IDS_NEGATIVE_SENSOR, m_ctrlNegSensor);
	DDX_Control(pDX, IDS_HOME_SENSOR, m_ctrlHomeSensor);
	DDX_Control(pDX, IDS_VELOCITY, m_ctrlVel);
	DDX_Control(pDX, IDS_ACC_TIME, m_ctrlAcc);
	DDX_Control(pDX, IDS_ORIGIN_VEL, m_ctrlOriginVel);
	DDX_Control(pDX, IDS_ORIGIN_VEL2, m_ctrlOriginVel2);
	DDX_Control(pDX, IDS_ORIGIN_VEL3, m_ctrlOriginVel3);
	DDX_Control(pDX, IDS_JOG_VEL, m_ctrlJogVel);
	DDX_Control(pDX, IDS_ORIGIN_ACC, m_ctrlOriginAcc);
	DDX_Control(pDX, IDS_JOG_ACC, m_ctrlJogAcc);
	DDX_Control(pDX, IDS_POS_SW_LIMIT, m_ctrlPosSWLimit);
	DDX_Control(pDX, IDS_NEG_SW_LIMIT, m_ctrlNegSWLimit);
	DDX_Control(pDX, IDS_TRIGGER_PERIOD, m_ctrlTriggerPeriod);
}


BEGIN_MESSAGE_MAP(CDialogAxisManager, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDialogAxisManager 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CDialogAxisManager, CDialog)
	ON_EVENT(CDialogAxisManager, IDS_VELOCITY, DISPID_CLICK, OnClickVelocity, VTS_NONE)
	ON_EVENT(CDialogAxisManager, IDS_ACC_TIME, DISPID_CLICK, OnClickAccTime, VTS_NONE)
	ON_EVENT(CDialogAxisManager, IDS_ORIGIN_VEL, DISPID_CLICK, OnClickOriginVel, VTS_NONE)
	ON_EVENT(CDialogAxisManager, IDS_ORIGIN_ACC, DISPID_CLICK, OnClickOriginAcc, VTS_NONE)
	ON_EVENT(CDialogAxisManager, IDS_JOG_VEL, DISPID_CLICK, OnClickJogVel, VTS_NONE)
	ON_EVENT(CDialogAxisManager, IDS_JOG_ACC, DISPID_CLICK, OnClickJogAcc, VTS_NONE)
	ON_EVENT(CDialogAxisManager, IDS_MAX_VEL, DISPID_CLICK, OnClickMaxVel, VTS_NONE)
	ON_EVENT(CDialogAxisManager, IDS_MIN_VEL, DISPID_CLICK, OnClickMinVel, VTS_NONE)
	ON_EVENT(CDialogAxisManager, IDS_MAX_ACC, DISPID_CLICK, OnClickMaxAcc, VTS_NONE)
	ON_EVENT(CDialogAxisManager, IDS_MIN_ACC, DISPID_CLICK, OnClickMinAcc, VTS_NONE)
	ON_EVENT(CDialogAxisManager, IDS_POS_SW_LIMIT, DISPID_CLICK, OnClickPosSwLimit, VTS_NONE)
	ON_EVENT(CDialogAxisManager, IDS_NEG_SW_LIMIT, DISPID_CLICK, OnClickNegSwLimit, VTS_NONE)
	ON_EVENT(CDialogAxisManager, IDB_OK, DISPID_CLICK, CDialogAxisManager::ClickOk, VTS_NONE)
	ON_EVENT(CDialogAxisManager, IDB_SAVE_AXIS, DISPID_CLICK, CDialogAxisManager::ClickSaveAxis, VTS_NONE)
	ON_EVENT(CDialogAxisManager, IDB_AMP_ON, DISPID_CLICK, CDialogAxisManager::ClickAmpOn, VTS_NONE)
	ON_EVENT(CDialogAxisManager, IDB_AMP_OFF, DISPID_CLICK, CDialogAxisManager::ClickAmpOff, VTS_NONE)
	ON_EVENT(CDialogAxisManager, IDB_AMP_RESET, DISPID_CLICK, CDialogAxisManager::ClickAmpReset, VTS_NONE)
	ON_EVENT(CDialogAxisManager, IDB_MOTOR_PARAM_PREV, DISPID_CLICK, CDialogAxisManager::ClickMotorParamPrev, VTS_NONE)
	ON_EVENT(CDialogAxisManager, IDB_MOTOR_PARAM_NEXT, DISPID_CLICK, CDialogAxisManager::ClickMotorParamNext, VTS_NONE)
	ON_EVENT(CDialogAxisManager, IDS_TRIGGER_PERIOD, DISPID_CLICK, CDialogAxisManager::ClickTriggerPeriod, VTS_NONE)
	ON_EVENT(CDialogAxisManager, IDS_ORIGIN_VEL2, DISPID_CLICK, CDialogAxisManager::ClickOriginVel2, VTS_NONE)
	ON_EVENT(CDialogAxisManager, IDS_ORIGIN_VEL3, DISPID_CLICK, CDialogAxisManager::ClickOriginVel3, VTS_NONE)
END_EVENTSINK_MAP()

void CDialogAxisManager::DispMotorParam()
{
	CString strVal;
	CString tmpAxisName;

	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	if(AXIS_TRY_SHUTTLE == m_iAxis)
	{
		GetDlgItem(IDS_TRIGGER_PERIOD)->ShowWindow(TRUE);
		GetDlgItem(IDB_TRIGGER_PERIOD)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDS_TRIGGER_PERIOD)->ShowWindow(FALSE);
		GetDlgItem(IDB_TRIGGER_PERIOD)->ShowWindow(FALSE);
	}

	tmpAxisName = pdlg->m_pDlgManual->AxisGetAxisName(m_iAxis);
	strVal.Format(_T("%d. %s"), m_iAxis, tmpAxisName);
	m_ctrlAxisName.SetCaption(strVal);
	
	strVal.Format(_T("%d"), (int)pdlg->m_pDlgManual->AxisGetNormVel(m_iAxis));
	m_ctrlVel.SetCaption(strVal);
	
	strVal.Format(_T("%d"), (int)pdlg->m_pDlgManual->AxisGetHomeVel(m_iAxis));
	m_ctrlOriginVel.SetCaption(strVal);

	strVal.Format(_T("%d"), (int)pdlg->m_pDlgManual->AxisGetHomeVel2(m_iAxis));
	m_ctrlOriginVel2.SetCaption(strVal);

	strVal.Format(_T("%d"), (int)pdlg->m_pDlgManual->AxisGetHomeVel3(m_iAxis));
	m_ctrlOriginVel3.SetCaption(strVal);
	
	strVal.Format(_T("%d"), (int)pdlg->m_pDlgManual->AxisGetJogVel(m_iAxis));
	m_ctrlJogVel.SetCaption(strVal);

	strVal.Format(_T("%3.2f"), pdlg->m_pDlgManual->AxisGetNormAcc(m_iAxis));
	m_ctrlAcc.SetCaption(strVal);
	
	strVal.Format(_T("%3.2f"), pdlg->m_pDlgManual->AxisGetHomeAcc(m_iAxis));
	m_ctrlOriginAcc.SetCaption(strVal);
	
	strVal.Format(_T("%3.2f"), pdlg->m_pDlgManual->AxisGetJogAcc(m_iAxis));
	m_ctrlJogAcc.SetCaption(strVal);
	
	strVal.Format(_T("%d"), (int)pdlg->m_pDlgManual->AxisGetMaxVel(m_iAxis));
	m_ctrlMaxVel.SetCaption(strVal);
	
	strVal.Format(_T("%d"), (int)pdlg->m_pDlgManual->AxisGetMinVel(m_iAxis));
	m_ctrlMinVel.SetCaption(strVal);
	
	strVal.Format(_T("%3.2f"), pdlg->m_pDlgManual->AxisGetMaxAcc(m_iAxis));
	m_ctrlMaxAcc.SetCaption(strVal);
	
	strVal.Format(_T("%3.2f"), pdlg->m_pDlgManual->AxisGetMinAcc(m_iAxis));
	m_ctrlMinAcc.SetCaption(strVal);

	strVal.Format(_T("%6.3f"), pdlg->m_pDlgManual->AxisGetPosSWLimit(m_iAxis));
	m_ctrlPosSWLimit.SetCaption(strVal);
	
	strVal.Format(_T("%6.3f"), pdlg->m_pDlgManual->AxisGetNegSWLimit(m_iAxis));
	m_ctrlNegSWLimit.SetCaption(strVal);

	strVal.Format(_T("%3.2f"), pdlg->m_pDlgManual->AxisGetTriggerPeriod(m_iAxis));
	m_ctrlTriggerPeriod.SetCaption(strVal);

//>> 130401 JSPark
	if(pdlg->m_pDlgManual->AxisGetAxisUse(m_iAxis) == USE)
	{
		// Pos Sensor
		m_ctrlPosSensor.SetCaption(_T("Off"));
		m_ctrlPosSensor.SetBackColor(GREEN);
		m_ctrlPosSensor.SetForeColor(BLACK);
		m_ctrlPosSensor.EnableWindow(TRUE);
		// Home Sensor
		m_ctrlHomeSensor.SetCaption(_T("Off"));
		m_ctrlHomeSensor.SetBackColor(GREEN);
		m_ctrlHomeSensor.SetForeColor(BLACK);
		m_ctrlHomeSensor.EnableWindow(TRUE);
		// Neg Sensor
		m_ctrlNegSensor.SetCaption(_T("Off"));
		m_ctrlNegSensor.SetBackColor(GREEN);
		m_ctrlNegSensor.SetForeColor(BLACK);
		m_ctrlNegSensor.EnableWindow(TRUE);
	}
	else
	{
		// Pos Sensor
		m_ctrlPosSensor.SetCaption(_T("Not Use"));
		m_ctrlPosSensor.SetBackColor(LTGRAY);
		m_ctrlPosSensor.SetForeColor(RED);
		m_ctrlPosSensor.EnableWindow(FALSE);
		// Home Sensor
		m_ctrlHomeSensor.SetCaption(_T("Not Use"));
		m_ctrlHomeSensor.SetBackColor(LTGRAY);
		m_ctrlHomeSensor.SetForeColor(RED);
		m_ctrlHomeSensor.EnableWindow(FALSE);
		// Neg Sensor
		m_ctrlNegSensor.SetCaption(_T("Not Use"));
		m_ctrlNegSensor.SetBackColor(LTGRAY);
		m_ctrlNegSensor.SetForeColor(RED);
		m_ctrlNegSensor.EnableWindow(FALSE);
	}
}

void CDialogAxisManager::DispUnitState()
{
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	double dCurPos = 0;
	CString strValue;

	pdlg->m_pDlgManual->AxisGetPosition(m_iAxis, dCurPos);
	strValue.Format(_T("%6.3f"),dCurPos);
	m_ctrlCurPos.SetCaption(strValue);

//>> 130401 JSPark
	if(pdlg->m_pDlgManual->AxisGetAxisUse(m_iAxis) == USE)
	{
		if(pdlg->m_pDlgManual->AxisGetPosLimitSensor(m_iAxis))
		{
			if(m_ctrlPosSensor.GetBackColor() != YELLOW)
			{
				m_ctrlPosSensor.SetCaption(_T("On"));
				m_ctrlPosSensor.SetBackColor(YELLOW);
				m_ctrlPosSensor.SetForeColor(RED);
			}
		}
		else
		{
			if(m_ctrlPosSensor.GetBackColor() != GREEN)
			{
				m_ctrlPosSensor.SetCaption(_T("Off"));
				m_ctrlPosSensor.SetBackColor(GREEN);
				m_ctrlPosSensor.SetForeColor(BLACK);
			}
		}
		
		if(pdlg->m_pDlgManual->AxisGetNegLimitSensor(m_iAxis))
		{
			if(m_ctrlNegSensor.GetBackColor() != YELLOW)
			{
				m_ctrlNegSensor.SetCaption(_T("On"));
				m_ctrlNegSensor.SetBackColor(YELLOW);
				m_ctrlNegSensor.SetForeColor(RED);
			}
		}
		else
		{
			if(m_ctrlNegSensor.GetBackColor() != GREEN)
			{
				m_ctrlNegSensor.SetCaption(_T("Off"));
				m_ctrlNegSensor.SetBackColor(GREEN);
				m_ctrlNegSensor.SetForeColor(BLACK);
			}
		}
		
		if(pdlg->m_pDlgManual->AxisGetHomeSensor(m_iAxis))
		{
			if(m_ctrlHomeSensor.GetBackColor() != YELLOW)
			{
				m_ctrlHomeSensor.SetCaption(_T("On"));
				m_ctrlHomeSensor.SetBackColor(YELLOW);
				m_ctrlHomeSensor.SetForeColor(RED);
			}
		}
		else
		{
			if(m_ctrlHomeSensor.GetBackColor() != GREEN)
			{
				m_ctrlHomeSensor.SetCaption(_T("Off"));
				m_ctrlHomeSensor.SetBackColor(GREEN);
				m_ctrlHomeSensor.SetForeColor(BLACK);
			}
		}
	}

	if(pdlg->m_pDlgManual->AxisGetAmpFaultState(m_iAxis)) //Amp Fault 상태 체크 함수 수정
	{
		if(m_ctrlAmpState.GetBackColor() != GREEN)
		{
			m_ctrlAmpState.SetCaption(_T("Normal"));
			m_ctrlAmpState.SetBackColor(GREEN);
			m_ctrlAmpState.SetForeColor(BLACK);
		}
	}
	else
	{
		if(m_ctrlAmpState.GetBackColor() != YELLOW)
		{
			m_ctrlAmpState.SetCaption(_T("Fault"));
			m_ctrlAmpState.SetBackColor(YELLOW);
			m_ctrlAmpState.SetForeColor(RED);
		}
	}
}

void CDialogAxisManager::ClickOk()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	KillTimer(TIMER_AXIS_MANAGE);
	CDialog::OnOK();
}


BOOL CDialogAxisManager::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	DispMotorParam();
	SetTimer(TIMER_AXIS_MANAGE, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialogAxisManager::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case TIMER_AXIS_MANAGE:
		DispUnitState();
		break;
	}

	CDialog::OnTimer(nIDEvent);
}


void CDialogAxisManager::ClickSaveAxis()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	WriteLog(_T("CAxisManageDlg::OnClickSaveAxis()-Start"), SYSTEM_LOG);

	if(g_cMsgBox.YesNoMsgBox(_T("Really Save Motor Parameter?")) == IDNO)
	{
		WriteLog(_T("CAxisManageDlg::OnClickSaveAxis()-Cancel"), SYSTEM_LOG);
		
		return;
	}
	
	CString strVal;
	pdlg->m_pDlgManual->AxisSetNormVel(m_iAxis, _ttof(m_ctrlVel.GetCaption()) );
	pdlg->m_pDlgManual->AxisSetHomeVel(m_iAxis, _ttof(m_ctrlOriginVel.GetCaption()));
	pdlg->m_pDlgManual->AxisSetHomeVel2(m_iAxis, _ttof(m_ctrlOriginVel2.GetCaption()));
	pdlg->m_pDlgManual->AxisSetHomeVel3(m_iAxis, _ttof(m_ctrlOriginVel3.GetCaption()));
	pdlg->m_pDlgManual->AxisSetJogVel(m_iAxis, _ttof(m_ctrlJogVel.GetCaption()));

	strVal = m_ctrlAcc.GetCaption();
	pdlg->m_pDlgManual->AxisSetNormAcc(m_iAxis, _ttof(strVal));
	pdlg->m_pDlgManual->AxisSetNormDec(m_iAxis, _ttof(strVal));
	
	strVal = m_ctrlOriginAcc.GetCaption();
	pdlg->m_pDlgManual->AxisSetHomeAcc(m_iAxis, _ttof(strVal));
	pdlg->m_pDlgManual->AxisSetHomeDec(m_iAxis, _ttof(strVal));
	
	strVal = m_ctrlJogAcc.GetCaption();
	pdlg->m_pDlgManual->AxisSetJogAcc(m_iAxis, _ttof(strVal));
	pdlg->m_pDlgManual->AxisSetJogDec(m_iAxis, _ttof(strVal));
	
	strVal = m_ctrlPosSWLimit.GetCaption();
	pdlg->m_pDlgManual->AxisSetPosSWLimit(m_iAxis, _ttof(strVal));
	
	strVal = m_ctrlNegSWLimit.GetCaption();
	pdlg->m_pDlgManual->AxisSetNegSWLimit(m_iAxis, _ttof(strVal));

	strVal = m_ctrlMaxVel.GetCaption();
	pdlg->m_pDlgManual->AxisSetMaxVel(m_iAxis, _ttof(strVal));
	pdlg->m_pDlgManual->AxisSetMaxHomeVel(m_iAxis, _ttof(strVal));
	pdlg->m_pDlgManual->AxisSetMaxJogVel(m_iAxis, _ttof(strVal));
	
	strVal = m_ctrlMinVel.GetCaption();
	pdlg->m_pDlgManual->AxisSetMinVel(m_iAxis, _ttof(strVal));
	pdlg->m_pDlgManual->AxisSetMinHomeVel(m_iAxis, _ttof(strVal));
	pdlg->m_pDlgManual->AxisSetMinJogVel(m_iAxis, _ttof(strVal));
	
	strVal = m_ctrlMaxAcc.GetCaption();
	pdlg->m_pDlgManual->AxisSetMaxAcc(m_iAxis, _ttof(strVal));
	pdlg->m_pDlgManual->AxisSetMaxHomeAcc(m_iAxis, _ttof(strVal));
	pdlg->m_pDlgManual->AxisSetMaxJogAcc(m_iAxis, _ttof(strVal));
	
	strVal = m_ctrlMinAcc.GetCaption();
	pdlg->m_pDlgManual->AxisSetMinAcc(m_iAxis, _ttof(strVal));
	pdlg->m_pDlgManual->AxisSetMinHomeAcc(m_iAxis, _ttof(strVal));
	pdlg->m_pDlgManual->AxisSetMinJogAcc(m_iAxis, _ttof(strVal));

	strVal = m_ctrlTriggerPeriod.GetCaption();
	pdlg->m_pDlgManual->AxisSetTriggerPeriod(m_iAxis, _ttof(strVal));
	
	pdlg->m_pDlgManual->SaveAxisConfig(m_iAxis);	

	WriteLog(_T("CAxisManageDlg::OnClickSaveAxis()-End"), SYSTEM_LOG);
}


void CDialogAxisManager::ClickAmpOn()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	BOOL rslt = pdlg->m_pDlgManual->AxisAmpEnable(m_iAxis);
}


void CDialogAxisManager::ClickAmpOff()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_pDlgManual->AxisAmpDisable(m_iAxis);
	pdlg->m_pDlgManual->AxisSetHomeFinished(m_iAxis, FALSE);
	pdlg->m_pDlgManual->AxisSetOriginReturn(m_iAxis, FALSE);	
}


void CDialogAxisManager::ClickAmpReset()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	//pdlg->m_pDlgManual->AxisClearState();
	//pdlg->m_pDlgManual->AxisClearAxis(m_iAxis);
	pdlg->m_pDlgManual->AxisResetAmpFault(m_iAxis);
}


void CDialogAxisManager::ClickMotorParamPrev()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_iAxis--;
	
	if(0 > m_iAxis )
		m_iAxis = MAX_AXIS_NUM-1;
	
	DispMotorParam();	
}


void CDialogAxisManager::ClickMotorParamNext()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_iAxis++;
	
	if( MAX_AXIS_NUM == m_iAxis )
		m_iAxis = 0;
	
	DispMotorParam();		
}

void CDialogAxisManager::OnClickVelocity()
{
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	WriteLog(_T("CDialogAxisManager::OnClickVelocity()-Start"), SYSTEM_LOG);

	CString strVal = m_ctrlVel.GetCaption();
	CDialogNumberBox dlg;
	
	if(dlg.DoModal() == IDOK)
	{
		int iVal = _ttoi(dlg.GetstrNum());
		int iMin = (int)pdlg->m_pDlgManual->AxisGetMinVel(m_iAxis);
		int iMax = (int)pdlg->m_pDlgManual->AxisGetMaxVel(m_iAxis);
		
		if(iVal < iMin || iVal > iMax)
		{
			CString strMsg;
			strMsg.Format(_T("Only %d ~ %d is available"), iMin, iMax);
			g_cMsgBox.OkMsgBox(strMsg);
			WriteLog(_T("CCDialogAxisManager::OnClickVelocity(F)-End"), SYSTEM_LOG);
			return;
		}
		strVal.Format(_T("%d"), iVal);
		m_ctrlVel.SetCaption(strVal);
		
		WriteLog(strVal, SYSTEM_LOG);
	}	

		WriteLog(_T("CDialogAxisManager::OnClickVelocity(C)-End"), SYSTEM_LOG); 
}

void CDialogAxisManager::OnClickAccTime()
{
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
	WriteLog(_T("CDialogAxisManager::OnClickAccTime()-Start"), SYSTEM_LOG); 

	CString strVal = m_ctrlAcc.GetCaption();
	CDialogNumberBox dlg;
	
	if(dlg.DoModal() == IDOK)
	{
		double dVal = _ttof(dlg.GetstrNum());
		double iMin = pdlg->m_pDlgManual->AxisGetMinAcc(m_iAxis);
		double iMax = pdlg->m_pDlgManual->AxisGetMaxAcc(m_iAxis);
		
		if(dVal < iMin || dVal > iMax)
		{
			CString strMsg;
			strMsg.Format(_T("Only %3.2f ~ %3.2f is available"), iMin, iMax);
			g_cMsgBox.OkMsgBox(strMsg);
			WriteLog(_T("CDialogAxisManager::OnClickAccTime(F)-End"), SYSTEM_LOG); 
			return;
		}
		strVal.Format(_T("%3.2f"), dVal);
		m_ctrlAcc.SetCaption(strVal);
		
		WriteLog(strVal, SYSTEM_LOG); 
	}	

	WriteLog(_T("CDialogAxisManager::OnClickAccTime(C)-End"), SYSTEM_LOG); 
}

void CDialogAxisManager::OnClickOriginVel()
{
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
	WriteLog(_T("CDialogAxisManager::OnClickOriginVel()-Start"), SYSTEM_LOG); 

	CString strVal = m_ctrlOriginVel.GetCaption();

	CDialogNumberBox dlg;
	
	if(dlg.DoModal() == IDOK)
	{
		int iVal = _ttoi(dlg.GetstrNum());
		int iMin = (int)pdlg->m_pDlgManual->AxisGetMinHomeVel(m_iAxis);
		int iMax = (int)pdlg->m_pDlgManual->AxisGetMaxHomeVel(m_iAxis);
		
		if(iVal < iMin || iVal > iMax)
		{
			CString strMsg;
			strMsg.Format(_T("Only %d ~ %d is available"), iMin, iMax);
			g_cMsgBox.OkMsgBox(strMsg);
			WriteLog(_T("CDialogAxisManager::OnClickOriginVel(F)-End"), SYSTEM_LOG); 
			return;
		}
		strVal.Format(_T("%d"), iVal);
		m_ctrlOriginVel.SetCaption(strVal);
		
		WriteLog(strVal, SYSTEM_LOG); 
	}	

	WriteLog(_T("CDialogAxisManager::OnClickOriginVel(C)-End"), SYSTEM_LOG); 
}

void CDialogAxisManager::OnClickOriginAcc()
{
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	WriteLog(_T("CDialogAxisManager::OnClickOriginAcc()-Start"), SYSTEM_LOG); 

	CString strVal = m_ctrlOriginAcc.GetCaption();
	CDialogNumberBox dlg;
	
	if(dlg.DoModal() == IDOK)
	{
		double dVal = _ttof(dlg.GetstrNum());
		double dMin = pdlg->m_pDlgManual->AxisGetMinHomeAcc(m_iAxis);
		double dMax = pdlg->m_pDlgManual->AxisGetMaxHomeAcc(m_iAxis);
		
		if(dVal < dMin || dVal > dMax)
		{
			CString strMsg;
			strMsg.Format(_T("Only %3.2f ~ %3.2f is available"), dMin, dMax);
			g_cMsgBox.OkMsgBox(strMsg);
			WriteLog(_T("CDialogAxisManager::OnClickOriginAcc(F)-End"), SYSTEM_LOG); 
			return;
		}
		
		strVal.Format(_T("%3.2f"), dVal);
		m_ctrlOriginAcc.SetCaption(strVal);
		WriteLog(strVal, SYSTEM_LOG); 
	}	

	WriteLog(_T("CDialogAxisManager::OnClickOriginAcc(C)-End"), SYSTEM_LOG); 
}

void CDialogAxisManager::OnClickJogVel()
{
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	WriteLog(_T("CDialogAxisManager::OnClickJogVel()-Start"), SYSTEM_LOG); 

	CString strVal = m_ctrlJogVel.GetCaption();
	CDialogNumberBox dlg;

	if(dlg.DoModal() == IDOK)
	{
		int iVal = _ttoi(dlg.GetstrNum());
		int iMin, iMax;
		iMin = (int)pdlg->m_pDlgManual->AxisGetMinJogVel(m_iAxis);
		iMax = (int)pdlg->m_pDlgManual->AxisGetMaxJogVel(m_iAxis);
		
		if(iVal < iMin || iVal > iMax)
		{
			CString strMsg;
			strMsg.Format(_T("Only %d ~ %d is available"), iMin, iMax);
			g_cMsgBox.OkMsgBox(strMsg);
			WriteLog(_T("CDialogAxisManager::OnClickJogVel(F)-End"), SYSTEM_LOG); 
			return;
		}
		
		strVal.Format(_T("%d"), iVal);
		m_ctrlJogVel.SetCaption(strVal);
		WriteLog(strVal, SYSTEM_LOG); 
	}	
	WriteLog(_T("CDialogAxisManager::OnClickJogVel(C)-End"), SYSTEM_LOG); 
}

void CDialogAxisManager::OnClickJogAcc()
{
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	WriteLog(_T("CDialogAxisManager::OnClickJogAcc()-Start"), SYSTEM_LOG); 

	CString strVal = m_ctrlJogAcc.GetCaption();
	CDialogNumberBox dlg;

	if(dlg.DoModal() == IDOK)
	{
		double dVal = _ttof(dlg.GetstrNum());
		double dMin = pdlg->m_pDlgManual->AxisGetMinJogAcc(m_iAxis);
		double dMax = pdlg->m_pDlgManual->AxisGetMaxJogAcc(m_iAxis);
		
		if(dVal < dMin || dVal > dMax)
		{
			CString strMsg;
			strMsg.Format(_T("Only %3.2f ~ %3.2f is available"), dMin, dMax);
			g_cMsgBox.OkMsgBox(strMsg);
			WriteLog(_T("CDialogAxisManager::OnClickJogAcc(F)-End"), SYSTEM_LOG); 
			return;
		}
		
		strVal.Format(_T("%3.2f"), dVal);
		m_ctrlJogAcc.SetCaption(strVal);
		WriteLog(strVal, SYSTEM_LOG); 
	}	
	WriteLog(_T("CDialogAxisManager::OnClickJogAcc(C)-End"), SYSTEM_LOG); 
}

void CDialogAxisManager::OnClickMaxVel()
{
	WriteLog(_T("CDialogAxisManager::OnClickMaxVel()-Start"), SYSTEM_LOG); 

	CString strVal = m_ctrlMaxVel.GetCaption();
	CDialogNumberBox dlg;

	if(dlg.DoModal() == IDOK)
	{
		int iVal = _ttoi(dlg.GetstrNum());
		
		strVal.Format(_T("%d"), iVal);
		m_ctrlMaxVel.SetCaption(strVal);
		WriteLog(strVal, SYSTEM_LOG); 
	}	

	WriteLog(_T("CDialogAxisManager::OnClickMaxVel()-End"), SYSTEM_LOG); 
}

void CDialogAxisManager::OnClickMinVel()
{
	WriteLog(_T("CDialogAxisManager::OnClickMinVel()-Start"), SYSTEM_LOG); 

	CString strVal = m_ctrlMinVel.GetCaption();
	CDialogNumberBox dlg;

	if(dlg.DoModal() == IDOK)
	{
		int iVal = _ttoi(dlg.GetstrNum());
		
		strVal.Format(_T("%d"), iVal);
		m_ctrlMinVel.SetCaption(strVal);
		WriteLog(strVal, SYSTEM_LOG); 
	}	
	WriteLog(_T("CDialogAxisManager::OnClickMinVel()-End"), SYSTEM_LOG); 
}

void CDialogAxisManager::OnClickMaxAcc()
{
	WriteLog(_T("CDialogAxisManager::OnClickMaxAcc()-Start"), SYSTEM_LOG); 

	CString strVal = m_ctrlMaxAcc.GetCaption();
	CDialogNumberBox dlg;
	if(dlg.DoModal() == IDOK)
	{
		double dVal = _ttof(dlg.GetstrNum());
		
		strVal.Format(_T("%3.2f"), dVal);
		m_ctrlMaxAcc.SetCaption(strVal);
		WriteLog(strVal, SYSTEM_LOG); 
	}	
	WriteLog(_T("CDialogAxisManager::OnClickMaxAcc()-End"), SYSTEM_LOG); 
}

void CDialogAxisManager::OnClickMinAcc()
{
	WriteLog(_T("CDialogAxisManager::OnClickMinAcc()-Start"), SYSTEM_LOG); 
	CString strVal = m_ctrlMinAcc.GetCaption();
	CDialogNumberBox dlg;
	if(dlg.DoModal() == IDOK)
	{
		double dVal = _ttof(dlg.GetstrNum());
		
		strVal.Format(_T("%3.2f"), dVal);
		m_ctrlMinAcc.SetCaption(strVal);
		WriteLog(strVal, SYSTEM_LOG); 
	}	
	WriteLog(_T("CDialogAxisManager::OnClickMaxAcc()-End"), SYSTEM_LOG); 
}

void CDialogAxisManager::OnClickPosSwLimit()
{
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
	WriteLog(_T("CDialogAxisManager::OnClickPosSwLimit()-Start"), SYSTEM_LOG); 

	CString strVal = m_ctrlPosSWLimit.GetCaption();
	CDialogNumberBox dlg;
	
	if(dlg.DoModal() == IDOK)
	{
		strVal = dlg.GetstrNum();
		double dVal = _ttof(strVal);
		double dMin = pdlg->m_pDlgManual->AxisGetNegSWLimit(m_iAxis);
		if(dVal <= dMin)
		{
			CString strMsg;
			strMsg.Format(_T("Must be more than %6.3f"), dMin);
			g_cMsgBox.OkMsgBox(strMsg);
			WriteLog(_T("CDialogAxisManager::OnClickPosSwLimit(F)-End"), SYSTEM_LOG); 
			return;
		}
		m_ctrlPosSWLimit.SetCaption(strVal);
		WriteLog(strVal, SYSTEM_LOG); 
	}	
	WriteLog(_T("CDialogAxisManager::OnClickPosSwLimit(C)-End"), SYSTEM_LOG); 

}

void CDialogAxisManager::OnClickNegSwLimit()
{
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
	WriteLog(_T("CDialogAxisManager::OnClickNegSwLimit()-Start"), SYSTEM_LOG); 

	CString strVal = m_ctrlNegSWLimit.GetCaption();
	CDialogNumberBox dlg;
	
	if(dlg.DoModal() == IDOK)
	{
		strVal = dlg.GetstrNum();
		double dVal = _ttof(strVal);
		double dMax = pdlg->m_pDlgManual->AxisGetPosSWLimit(m_iAxis);
		if(dVal >= dMax)
		{
			CString strMsg;
			strMsg.Format(_T("Must be less than %6.3f"), dMax);
			g_cMsgBox.OkMsgBox(strMsg);
			WriteLog(_T("CDialogAxisManager::OnClickNegSwLimit(F)-End"), SYSTEM_LOG); 
			return;
		}
		m_ctrlNegSWLimit.SetCaption(strVal);
		WriteLog(strVal, SYSTEM_LOG); 
	}	

	WriteLog(_T("CDialogAxisManager::OnClickNegSwLimit(C)-End"), SYSTEM_LOG); 
}


void CDialogAxisManager::ClickTriggerPeriod()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	WriteLog(_T("CDialogAxisManager::ClickTriggerPeriod()-Start"), SYSTEM_LOG); 

	CString strVal = m_ctrlTriggerPeriod.GetCaption();
	CDialogNumberBox dlg;

	int min, max;
	min = 1;
	max = 3000;

	if(dlg.DoModal() == IDOK)
	{
		int dVal = _ttoi(dlg.GetstrNum());
		
		if(dVal < min || dVal > max)
		{
			CString strMsg;
			strMsg.Format(_T("Only %d ~ %d is available"), min, max);
			g_cMsgBox.OkMsgBox(strMsg);
			WriteLog(_T("CDialogAxisManager::ClickTriggerPeriod(F)-End"), SYSTEM_LOG); 
			return;
		}
		
		strVal.Format(_T("%d"), dVal);
		m_ctrlTriggerPeriod.SetCaption(strVal);
		WriteLog(strVal, SYSTEM_LOG); 
	}	
	WriteLog(_T("CDialogAxisManager::ClickTriggerPeriod(C)-End"), SYSTEM_LOG); 
}


void CDialogAxisManager::ClickOriginVel2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
	WriteLog(_T("CDialogAxisManager::OnClickOriginVel2()-Start"), SYSTEM_LOG); 

	CString strVal = m_ctrlOriginVel2.GetCaption();

	CDialogNumberBox dlg;
	
	if(dlg.DoModal() == IDOK)
	{
		int iVal = _ttoi(dlg.GetstrNum());
		int iMin = (int)pdlg->m_pDlgManual->AxisGetMinHomeVel(m_iAxis);
		int iMax = (int)pdlg->m_pDlgManual->AxisGetMaxHomeVel(m_iAxis);
		
		if(iVal < iMin || iVal > iMax)
		{
			CString strMsg;
			strMsg.Format(_T("Only %d ~ %d is available"), iMin, iMax);
			g_cMsgBox.OkMsgBox(strMsg);
			WriteLog(_T("CDialogAxisManager::OnClickOriginVel2(F)-End"), SYSTEM_LOG); 
			return;
		}
		strVal.Format(_T("%d"), iVal);
		m_ctrlOriginVel2.SetCaption(strVal);
		
		WriteLog(strVal, SYSTEM_LOG); 
	}	

	WriteLog(_T("CDialogAxisManager::OnClickOriginVel2(C)-End"), SYSTEM_LOG); 
}


void CDialogAxisManager::ClickOriginVel3()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
	WriteLog(_T("CDialogAxisManager::OnClickOriginVel3()-Start"), SYSTEM_LOG); 

	CString strVal = m_ctrlOriginVel3.GetCaption();

	CDialogNumberBox dlg;
	
	if(dlg.DoModal() == IDOK)
	{
		int iVal = _ttoi(dlg.GetstrNum());
		int iMin = (int)pdlg->m_pDlgManual->AxisGetMinHomeVel(m_iAxis);
		int iMax = (int)pdlg->m_pDlgManual->AxisGetMaxHomeVel(m_iAxis);
		
		if(iVal < iMin || iVal > iMax)
		{
			CString strMsg;
			strMsg.Format(_T("Only %d ~ %d is available"), iMin, iMax);
			g_cMsgBox.OkMsgBox(strMsg);
			WriteLog(_T("CDialogAxisManager::OnClickOriginVel3(F)-End"), SYSTEM_LOG); 
			return;
		}
		strVal.Format(_T("%d"), iVal);
		m_ctrlOriginVel3.SetCaption(strVal);
		
		WriteLog(strVal, SYSTEM_LOG); 
	}	

	WriteLog(_T("CDialogAxisManager::OnClickOriginVel3(C)-End"), SYSTEM_LOG); 
}
