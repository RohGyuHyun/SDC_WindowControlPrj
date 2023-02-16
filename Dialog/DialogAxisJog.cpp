// DialogAxisJog.cpp : 구현 파일입니다.
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


// CDialogAxisJog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogAxisJog, CDialog)

CDialogAxisJog::CDialogAxisJog(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogAxisJog::IDD, pParent)
{
	m_bAxisOpDlgUpdate = FALSE;

	m_paxDlgX = NULL;
	m_paxDlgY = NULL;

	m_bTeachPosXFlag = FALSE;
	m_bTeachPosYFlag = FALSE;
	m_dTeachPosX = 0.;
	m_dTeachPosY = 0.;

	m_dStepPitch = 10.0;
}

CDialogAxisJog::~CDialogAxisJog()
{
}

void CDialogAxisJog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDS_AXIS_X_CUR_POS, m_dCurPosAxisX);
 	DDX_Control(pDX, IDS_AXIS_Y_CUR_POS, m_dCurPosAxisY);
	DDX_Control(pDX, IDS_POSITIVE_SENSOR, m_ctrlPosSensor);
	DDX_Control(pDX, IDS_HOME_SENSOR, m_ctrlHomeSensor);
	DDX_Control(pDX, IDS_NEGATIVE_SENSOR, m_ctrlNegSensor);
	DDX_Control(pDX, IDS_POSITIVE_SENSOR_Y, m_ctrlPosSensorY);
	DDX_Control(pDX, IDS_HOME_SENSOR_Y, m_ctrlHomeSensorY);
	DDX_Control(pDX, IDS_NEGATIVE_SENSOR_Y, m_ctrlNegSensorY);
}


BEGIN_MESSAGE_MAP(CDialogAxisJog, CDialog)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDialogAxisJog 메시지 처리기입니다.

/*
void CDialogTeach::InitJogDlgOpInit()
{
	
	int i(0);
	m_dStepPitch = 10.0;
	//IO Address Init
	for(i=0; i<MAX_DLG_INPUT_CNT; i++)
		m_iAxisOPCtrlInPut[i] = -1;
	for(i=0; i<MAX_DLG_OUTPUT_CNT; i++)
		m_iAxisOPCtrlOutPut[i] = -1;
		
}
*/

void CDialogAxisJog::AxisOpDispUpdate()
{
	CString strData;
	m_dStepPitch = 10.0;
	m_dJogSpeed = 10000;

	// Jog Button 초기화
	GetDlgItem(IDB_AXIS_JOG_RIGHT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_AXIS_JOG_LEFT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_AXIS_JOG_FRONT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_AXIS_JOG_BACK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_AXIS_JOG_CCW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_AXIS_JOG_CW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_AXIS_JOG_UP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_AXIS_JOG_DOWN)->ShowWindow(SW_HIDE);	
//>> 130331 JSPark
	if(m_paxDlgY != NULL) 
	{
		//m_paxSelectAxis = m_paxDlgY;
		m_dJogSpeed = m_paxDlgY->GetJogVel();
		GetDlgItem(IDB_AXIS_JOG_FRONT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_AXIS_JOG_BACK)->ShowWindow(SW_SHOW);
	}

	if(m_paxDlgX != NULL)
	{
		//m_paxSelectAxis = m_paxDlgX;
		m_dJogSpeed = m_paxDlgX->GetJogVel();
		GetDlgItem(IDB_AXIS_JOG_RIGHT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_AXIS_JOG_LEFT)->ShowWindow(SW_SHOW);
	}
//<<
	//DisplayOpInOutItemName();	

	m_iOpMode = STEP_MODE;
	((CButton*)GetDlgItem(IDS_SELECT_STEP))->SetCheck(TRUE);

	m_dJogRatio = 0.1;		
	((CButton*)GetDlgItem(IDB_AXIS_OP_JOG_SPEED_LOW))->SetCheck(TRUE);

	strData.Format(_T("%3.3f"), m_dStepPitch);
	GetDlgItem(IDS_STEP_MOVE_PITCH)->SetWindowText(strData);
	
	//if(m_strDlgTitle != "")
	//	GetDlgItem(IDC_SELECT_UNIT_NAME)->SetWindowText(m_strDlgTitle);

	m_bAxisOpDlgUpdate = FALSE; //130331 JSPark
}

BOOL CDialogAxisJog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	// Dlg에서 사용할 Axis, IO 정보 Setting
	//InitJogDlgOpInit();
	//InitJogDlgIoInfo();

	m_paxDlgX = pdlg->m_pDlgManual->MotGetAxis(AXIS_LD_PNP);
	m_paxDlgY = pdlg->m_pDlgManual->MotGetAxis(AXIS_TRY_SHUTTLE);

	AxisOpDispUpdate();
	SetTimer(TIMER_JOG_DLG, 500, NULL); 

	MoveWindow(245, 280, 690, 600, TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDialogAxisJog::DisplayAxisPos()
{
	double dPos(0.0);
	CString strPosX, strPosY;
	
	if(m_paxDlgX != NULL) 
	{
		m_paxDlgX->GetPosition(dPos);
		strPosX.Format(_T("%3.3f"),dPos);
	}
	if(m_paxDlgY != NULL) 
	{
		m_paxDlgY->GetPosition(dPos);
		strPosY.Format(_T("%3.3f"),dPos);
	}

	m_dCurPosAxisX.SetCaption(strPosX);
 	m_dCurPosAxisY.SetCaption(strPosY);

	if(m_paxDlgX != NULL)
	{
		if( (m_paxDlgX->GetAxisUse() == USE) )
		{
			// Pos Sensor
			if(!m_ctrlPosSensor.IsWindowEnabled())
			{
				m_ctrlPosSensor.SetCaption(_T("X Positive(+)"));
				m_ctrlPosSensor.SetBackColor(GREEN);
				m_ctrlPosSensor.SetForeColor(BLACK);
				m_ctrlPosSensor.EnableWindow(TRUE);
			}
			// Home Sensor
			if(!m_ctrlHomeSensor.IsWindowEnabled())
			{
				m_ctrlHomeSensor.SetCaption(_T("X Home"));
				m_ctrlHomeSensor.SetBackColor(GREEN);
				m_ctrlHomeSensor.SetForeColor(BLACK);
				m_ctrlHomeSensor.EnableWindow(TRUE);
			}
			// Neg Sensor
			if(!m_ctrlNegSensor.IsWindowEnabled())
			{
				m_ctrlNegSensor.SetCaption(_T("X Negative(-)"));
				m_ctrlNegSensor.SetBackColor(GREEN);
				m_ctrlNegSensor.SetForeColor(BLACK);
				m_ctrlNegSensor.EnableWindow(TRUE);
			}

			if( (m_paxDlgX->GetPosLimitSensor()) )
			{
				if(m_ctrlPosSensor.GetBackColor() != YELLOW)
					m_ctrlPosSensor.SetBackColor(YELLOW);
			}
			else
			{
				if(m_ctrlPosSensor.GetBackColor() != GREEN)
					m_ctrlPosSensor.SetBackColor(GREEN);
			}
			
			if( (m_paxDlgX->GetHomeSensor()) )
			{
				if(m_ctrlHomeSensor.GetBackColor() != YELLOW)
					m_ctrlHomeSensor.SetBackColor(YELLOW);
			}
			else
			{
				if(m_ctrlHomeSensor.GetBackColor() != GREEN)
					m_ctrlHomeSensor.SetBackColor(GREEN);
			}
			
			if(  (m_paxDlgX->GetNegLimitSensor()) )
			{
				if(m_ctrlNegSensor.GetBackColor() != YELLOW)
					m_ctrlNegSensor.SetBackColor(YELLOW);
			}
			else
			{
				if(m_ctrlNegSensor.GetBackColor() != GREEN)
					m_ctrlNegSensor.SetBackColor(GREEN);
			}
		}
		else
		{
			// Pos Sensor
			if(m_ctrlPosSensor.IsWindowEnabled())
			{
				m_ctrlPosSensor.SetCaption(_T("Not Use"));
				m_ctrlPosSensor.SetBackColor(LTGRAY);
				m_ctrlPosSensor.SetForeColor(RED);
				m_ctrlPosSensor.EnableWindow(FALSE);
			}
			// Home Sensor
			if(m_ctrlHomeSensor.IsWindowEnabled())
			{
				m_ctrlHomeSensor.SetCaption(_T("Not Use"));
				m_ctrlHomeSensor.SetBackColor(LTGRAY);
				m_ctrlHomeSensor.SetForeColor(RED);
				m_ctrlHomeSensor.EnableWindow(FALSE);
			}
			// Neg Sensor
			if(m_ctrlNegSensor.IsWindowEnabled())
			{
				m_ctrlNegSensor.SetCaption(_T("Not Use"));
				m_ctrlNegSensor.SetBackColor(LTGRAY);
				m_ctrlNegSensor.SetForeColor(RED);
				m_ctrlNegSensor.EnableWindow(FALSE);
			}
		}
	}

	if(m_paxDlgY != NULL)
	{
		if( (m_paxDlgY->GetAxisUse() == USE) )
		{
			// Pos Sensor
			if(!m_ctrlPosSensorY.IsWindowEnabled())
			{
				m_ctrlPosSensorY.SetCaption(_T("Y Positive(+)"));
				m_ctrlPosSensorY.SetBackColor(GREEN);
				m_ctrlPosSensorY.SetForeColor(BLACK);
				m_ctrlPosSensorY.EnableWindow(TRUE);
			}
			// Home Sensor
			if(!m_ctrlHomeSensorY.IsWindowEnabled())
			{
				m_ctrlHomeSensorY.SetCaption(_T("Y Home"));
				m_ctrlHomeSensorY.SetBackColor(GREEN);
				m_ctrlHomeSensorY.SetForeColor(BLACK);
				m_ctrlHomeSensorY.EnableWindow(TRUE);
			}
			// Neg Sensor
			if(!m_ctrlNegSensorY.IsWindowEnabled())
			{
				m_ctrlNegSensorY.SetCaption(_T("Y Negative(-)"));
				m_ctrlNegSensorY.SetBackColor(GREEN);
				m_ctrlNegSensorY.SetForeColor(BLACK);
				m_ctrlNegSensorY.EnableWindow(TRUE);
			}

			if( (m_paxDlgY->GetPosLimitSensor()) )
			{
				if(m_ctrlPosSensorY.GetBackColor() != YELLOW)
					m_ctrlPosSensorY.SetBackColor(YELLOW);
			}
			else
			{
				if(m_ctrlPosSensorY.GetBackColor() != GREEN)
					m_ctrlPosSensorY.SetBackColor(GREEN);
			}
			
			if( (m_paxDlgY->GetHomeSensor()) )
			{
				if(m_ctrlHomeSensorY.GetBackColor() != YELLOW)
					m_ctrlHomeSensorY.SetBackColor(YELLOW);
			}
			else
			{
				if(m_ctrlHomeSensorY.GetBackColor() != GREEN)
					m_ctrlHomeSensorY.SetBackColor(GREEN);
			}
			
			if(  (m_paxDlgY->GetNegLimitSensor()) )
			{
				if(m_ctrlNegSensorY.GetBackColor() != YELLOW)
					m_ctrlNegSensorY.SetBackColor(YELLOW);
			}
			else
			{
				if(m_ctrlNegSensorY.GetBackColor() != GREEN)
					m_ctrlNegSensorY.SetBackColor(GREEN);
			}
		}
		else
		{
			// Pos Sensor
			if(m_ctrlPosSensorY.IsWindowEnabled())
			{
				m_ctrlPosSensorY.SetCaption(_T("Not Use"));
				m_ctrlPosSensorY.SetBackColor(LTGRAY);
				m_ctrlPosSensorY.SetForeColor(RED);
				m_ctrlPosSensorY.EnableWindow(FALSE);
			}
			// Home Sensor
			if(m_ctrlHomeSensorY.IsWindowEnabled())
			{
				m_ctrlHomeSensorY.SetCaption(_T("Not Use"));
				m_ctrlHomeSensorY.SetBackColor(LTGRAY);
				m_ctrlHomeSensorY.SetForeColor(RED);
				m_ctrlHomeSensorY.EnableWindow(FALSE);
			}
			// Neg Sensor
			if(m_ctrlNegSensorY.IsWindowEnabled())
			{
				m_ctrlNegSensorY.SetCaption(_T("Not Use"));
				m_ctrlNegSensorY.SetBackColor(LTGRAY);
				m_ctrlNegSensorY.SetForeColor(RED);
				m_ctrlNegSensorY.EnableWindow(FALSE);
			}
		}
	}
}


void CDialogAxisJog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case TIMER_JOG_DLG:
		DisplayAxisPos();
		//DisplayOpIO(); //130401 JSPark
		break;
	default:

		break;
	}

	CDialog::OnTimer(nIDEvent);
}


void CDialogAxisJog::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
}
BEGIN_EVENTSINK_MAP(CDialogAxisJog, CDialog)
	ON_EVENT(CDialogAxisJog, IDB_BTN_DLG_CANCEL, DISPID_CLICK, CDialogAxisJog::ClickBtnDlgCancel, VTS_NONE)
	ON_EVENT(CDialogAxisJog, IDB_AXIS_JOG_RIGHT, DISPID_CLICK, CDialogAxisJog::ClickAxisJogRight, VTS_NONE)
	ON_EVENT(CDialogAxisJog, IDB_AXIS_JOG_LEFT, DISPID_CLICK, CDialogAxisJog::ClickAxisJogLeft, VTS_NONE)
	ON_EVENT(CDialogAxisJog, IDB_AXIS_JOG_BACK, DISPID_CLICK, CDialogAxisJog::ClickAxisJogBack, VTS_NONE)
	ON_EVENT(CDialogAxisJog, IDB_AXIS_JOG_FRONT, DISPID_CLICK, CDialogAxisJog::ClickAxisJogFront, VTS_NONE)
	ON_EVENT(CDialogAxisJog, IDB_AXIS_OP_JOG_SPEED_LOW, DISPID_CLICK, CDialogAxisJog::ClickAxisOpJogSpeedLow, VTS_NONE)
	ON_EVENT(CDialogAxisJog, IDB_AXIS_OP_JOG_SPEED_MID, DISPID_CLICK, CDialogAxisJog::ClickAxisOpJogSpeedMid, VTS_NONE)
	ON_EVENT(CDialogAxisJog, IDB_AXIS_OP_JOG_SPEED_HIGH, DISPID_CLICK, CDialogAxisJog::ClickAxisOpJogSpeedHigh, VTS_NONE)
	ON_EVENT(CDialogAxisJog, IDS_SELECT_JOG, DISPID_CLICK, CDialogAxisJog::ClickSelectJog, VTS_NONE)
	ON_EVENT(CDialogAxisJog, IDS_SELECT_STEP, DISPID_CLICK, CDialogAxisJog::ClickSelectStep, VTS_NONE)
	ON_EVENT(CDialogAxisJog, IDS_STEP_MOVE_PITCH, DISPID_CLICK, CDialogAxisJog::ClickStepMovePitch, VTS_NONE)
	ON_EVENT(CDialogAxisJog, IDS_EXTEND, DISPID_CLICK, CDialogAxisJog::ClickExtend, VTS_NONE)
	ON_EVENT(CDialogAxisJog, IDB_BTN_DLG_MOVING_POS, DISPID_CLICK, CDialogAxisJog::ClickBtnDlgMovingPos, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogAxisJog::ClickBtnDlgCancel()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_paxDlgX = NULL;
	m_paxDlgY = NULL;

	m_bTeachPosXFlag = false;
	m_bTeachPosYFlag = false;

	KillTimer(TIMER_JOG_DLG);

	CDialog::OnCancel();
}

void CDialogAxisJog::ClickAxisJogRight()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
		if(m_iOpMode != STEP_MODE) 
		return;

	if(m_paxDlgX != NULL)
	{
		if(!m_paxDlgX->isOriginReturn())
		{

			g_cMsgBox.OkMsgBox(_T("해당 축이 원점 복귀 되어 있지 않습니다."));
			return;
		}

		double dCurPos, dTragetPos;
		double dPosSWLimit = m_paxDlgX->GetPosSWLimit();
		double dRPos = 1.0*m_dStepPitch;
		m_paxDlgX->GetPosition(dCurPos);
		dTragetPos = dCurPos+dRPos;

		if(dTragetPos > dPosSWLimit)
		{
			//>>20130902 lee
			CString strKMsg;
			strKMsg.Format(_T("X축이 움직이고자 하는 목표 위치:%6.3f가 +SW Limit 설정 영역:%6.3f을 벗어났습니다.")
							,dTragetPos, dPosSWLimit);

			g_cMsgBox.OkMsgBox(strKMsg);
			//<<

			return;
		}
		m_paxDlgX->RMove(dRPos, true);
		m_bTeachPosXFlag = true;
		
		//m_paxSelectAxis = m_paxDlgX;
//<<
	}
}


void CDialogAxisJog::ClickAxisJogLeft()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(m_iOpMode != STEP_MODE) 
		return;

	if(m_paxDlgX != NULL)
	{
		if(!m_paxDlgX->isOriginReturn())
		{
			//g_cMsgView.OkMsgBox("해당 축이 원점 복귀 되어 있지 않습니다.","Axis Not Origin !!","菉轟랬릿貫 !!");
			g_cMsgBox.OkMsgBox(_T("해당 축이 원점 복귀 되어 있지 않습니다."));
			return;
		}
//>> 130331 JSPark
		double dCurPos, dTragetPos;
		double dNegSWLimit = m_paxDlgX->GetNegSWLimit();
		double dRPos = -1.0*m_dStepPitch;
		m_paxDlgX->GetPosition(dCurPos);
		dTragetPos = dCurPos+dRPos;
		if(dTragetPos < dNegSWLimit)
		{
			//>>20130902 lee
			CString strKMsg;
			strKMsg.Format(_T("X축이 움직이고자 하는 목표 위치:%6.3f가 -SW Limit 설정 영역:%6.3f을 벗어났습니다.")
							,dTragetPos, dNegSWLimit);

			g_cMsgBox.OkMsgBox(strKMsg);
			//<<
			return;
		}
		m_paxDlgX->RMove(dRPos, true);
		m_bTeachPosXFlag = true;
		//m_paxSelectAxis = m_paxDlgX;
//<<
	}
}


void CDialogAxisJog::ClickAxisJogBack()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(m_iOpMode != STEP_MODE) 
		return;

	if(m_paxDlgY != NULL)
	{
		if(!m_paxDlgY->isOriginReturn())
		{
			g_cMsgBox.OkMsgBox(_T("해당 축이 원점 복귀 되어 있지 않습니다."));
			return;
		}
//>> 130401 JSPark
		double dCurPos, dTragetPos;
		double dNegSWLimit = m_paxDlgY->GetNegSWLimit();
		double dRPos = -1.0*m_dStepPitch;
		m_paxDlgY->GetPosition(dCurPos);
		dTragetPos = dCurPos+dRPos;
		if(dTragetPos < dNegSWLimit)
		{
			//>>20130902 lee
			CString strKMsg;
			strKMsg.Format(_T("Y축이 움직이고자 하는 목표 위치:%6.3f가 -SW Limit 설정 영역:%6.3f을 벗어났습니다.")
							,dTragetPos, dNegSWLimit);

			g_cMsgBox.OkMsgBox(strKMsg);
			//<<

			return;
		}	
		m_paxDlgY->RMove(dRPos, true);
		m_bTeachPosYFlag = true;
		
		//m_paxSelectAxis = m_paxDlgY;
//<<
	}
}

void CDialogAxisJog::ClickAxisJogFront()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(m_iOpMode != STEP_MODE) 
		return;

	if(m_paxDlgY != NULL)
	{
		if(!m_paxDlgY->isOriginReturn())
		{
			g_cMsgBox.OkMsgBox(_T("해당 축이 원점 복귀 되어 있지 않습니다."));
			return;
		}

		double dCurPos, dTragetPos;
		double dPosSWLimit = m_paxDlgY->GetPosSWLimit();
		double dRPos = 1.0*m_dStepPitch;
		m_paxDlgY->GetPosition(dCurPos);
		dTragetPos = dCurPos+dRPos;
		if(dTragetPos > dPosSWLimit)
		{
			//>>20130902 lee
			CString strKMsg;
			strKMsg.Format(_T("Y축이 움직이고자 하는 목표 위치:%6.3f가 +SW Limit 설정 영역:%6.3f을 벗어났습니다.")
							,dTragetPos, dPosSWLimit);

			g_cMsgBox.OkMsgBox(strKMsg);
			//<<
			return;
		}
		m_paxDlgY->RMove(dRPos, true);
		m_bTeachPosYFlag = true;

		//m_paxSelectAxis = m_paxDlgY;
//<<
	}
}


void CDialogAxisJog::ClickAxisOpJogSpeedLow()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_dJogRatio	= 0.1;
}


void CDialogAxisJog::ClickAxisOpJogSpeedMid()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_dJogRatio	= 1;
}


void CDialogAxisJog::ClickAxisOpJogSpeedHigh()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_dJogRatio	= 2;
}


void CDialogAxisJog::ClickSelectJog()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_iOpMode = JOG_MODE;
}


void CDialogAxisJog::ClickSelectStep()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_iOpMode = STEP_MODE;
}


void CDialogAxisJog::ClickStepMovePitch()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString strData;
	CDialogNumberBox dlg;

	dlg.SetstrNum(m_dStepPitch);

	if(dlg.DoModal() != IDOK) return;

	strData = dlg.GetstrNum();

	if(_ttof(strData) > 360 || _ttof(strData) < 0) 
	{
		return;
	}

	m_dStepPitch = _ttof(strData);

	GetDlgItem(IDS_STEP_MOVE_PITCH)->SetWindowText(strData);
}


void CDialogAxisJog::ClickExtend()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(((CBtnEnh*)GetDlgItem(IDS_EXTEND))->GetValue())
		MoveWindow(40, 280, 1120, 600, TRUE);
	else
		MoveWindow(245, 280, 690, 600, TRUE);
}


BOOL CDialogAxisJog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->wParam != 0)
	{
		if(pMsg->wParam == VK_F4)
			return TRUE;
		
		if(pMsg->wParam == VK_ESCAPE)
			return TRUE;
		
		if(pMsg->wParam == VK_RETURN)
			return TRUE;
	}

	switch (pMsg->message)
	{
	case WM_LBUTTONDOWN:
		if (pMsg->hwnd == GetDlgItem(IDB_AXIS_JOG_RIGHT)->GetSafeHwnd())			// 120229 ytlee 비젼 좌표계에 맞춤
		{ 
			if(m_paxDlgX != NULL)
			{
				if(m_iOpMode == JOG_MODE)
				{
					if(!m_paxDlgX->isOriginReturn())
					{
						
						g_cMsgBox.OkMsgBox(_T("해당 축이 원점 복귀 되어 있지 않습니다."));
						return TRUE;
					}
					
					m_paxDlgX->VMove(m_paxDlgX->GetJogVel()*m_dJogRatio,m_paxDlgX->GetJogAcc(),m_paxDlgX->GetJogDec());	
				}
			}
		}
		else if (pMsg->hwnd == GetDlgItem(IDB_AXIS_JOG_LEFT)->GetSafeHwnd())			// 120229 ytlee 비젼 좌표계에 맞춤
		{
			if(m_paxDlgX != NULL)
			{
				if(m_iOpMode == JOG_MODE)
				{
					if(!m_paxDlgX->isOriginReturn())
					{
						g_cMsgBox.OkMsgBox(_T("해당 축이 원점 복귀 되어 있지 않습니다."));
						return TRUE;
					}

					m_paxDlgX->VMove(-1.0*m_paxDlgX->GetJogVel()*m_dJogRatio,m_paxDlgX->GetJogAcc(),m_paxDlgX->GetJogDec());		// 120221 ytlee
				}
			}
		}
		else if (pMsg->hwnd == GetDlgItem(IDB_AXIS_JOG_FRONT)->GetSafeHwnd())			// 120229 ytlee 비젼 좌표계에 맞춤
		{			
			if(m_paxDlgY != NULL)
			{
				if(m_iOpMode == JOG_MODE)
				{
					if(!m_paxDlgY->isOriginReturn())
					{
						
						g_cMsgBox.OkMsgBox(_T("해당 축이 원점 복귀 되어 있지 않습니다."));
						return TRUE;
					}

					m_paxDlgY->VMove(m_paxDlgY->GetJogVel()*m_dJogRatio,m_paxDlgY->GetJogAcc(),m_paxDlgY->GetJogDec());		//120221 ytlee
				}
			}
		}
		else if (pMsg->hwnd == GetDlgItem(IDB_AXIS_JOG_BACK)->GetSafeHwnd())			// 120229 ytlee 비젼 좌표계에 맞춤
		{ 		
			if(m_paxDlgY != NULL)
			{
				if(m_iOpMode == JOG_MODE)
				{
					if(!m_paxDlgY->isOriginReturn())
					{
						//g_cMsgView.OkMsgBox("해당 축이 원점 복귀 되어 있지 않습니다.", "Axis Not Origin !!", "菉轟랬릿貫!!");
						g_cMsgBox.OkMsgBox(_T("해당 축이 원점 복귀 되어 있지 않습니다."));
						return TRUE;
					}

					m_paxDlgY->VMove(-1.0*m_paxDlgY->GetJogVel()*m_dJogRatio,m_paxDlgY->GetJogAcc(),m_paxDlgY->GetJogDec());		//120221 ytlee
				}
			}
		}
		break;

	case WM_LBUTTONUP:
		if(pMsg->hwnd == GetDlgItem(IDB_AXIS_JOG_RIGHT)->GetSafeHwnd() || pMsg->hwnd == GetDlgItem(IDB_AXIS_JOG_LEFT)->GetSafeHwnd())
		{
			if(m_paxDlgX != NULL)			// 120229 ytlee 비젼 좌표계에 맞춤
			{
				if(m_iOpMode == JOG_MODE)
				{
					m_paxDlgX->Stop();

					m_bTeachPosXFlag = true;
				}
				else
				{
					Delay(10,TRUE);
				}
			}
		}
		else if(pMsg->hwnd == GetDlgItem(IDB_AXIS_JOG_FRONT)->GetSafeHwnd() || pMsg->hwnd == GetDlgItem(IDB_AXIS_JOG_BACK)->GetSafeHwnd())		
		{
			if(m_paxDlgY != NULL)			// 120229 ytlee 비젼 좌표계에 맞춤
			{
				if(m_iOpMode == JOG_MODE)
				{
					m_paxDlgY->Stop();

					m_bTeachPosYFlag = true;
				}
				else
				{
					Delay(10,TRUE);
				}
			}
		}
	
		break;
	}	

	return CDialog::PreTranslateMessage(pMsg);
}


void CDialogAxisJog::ClickBtnDlgMovingPos()	//Motor Stop
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_pDlgManual->AxisStop(m_paxDlgX->GetAxisNo());
	pdlg->m_pDlgManual->AxisStop(m_paxDlgY->GetAxisNo());

}
