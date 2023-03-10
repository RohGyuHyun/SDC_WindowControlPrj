// DialogManual.cpp : 구현 파일입니다.
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


// CDialogManual 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogManual, CDialog)

CDialogManual::CDialogManual(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogManual::IDD, pParent)
{
	m_pIO = NULL;
	m_pMotion = NULL;
	m_pIO = new CIO;
	m_pMotion = new CMotion;

	m_lStartTime = 0;
	m_lEndTime = 0;
}

CDialogManual::~CDialogManual()
{
}

void CDialogManual::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDS_UNIT1_AXIS1_POS, m_ctrlAxisPos[0]);
	DDX_Control(pDX, IDS_UNIT1_AXIS2_POS, m_ctrlAxisPos[1]);
	DDX_Control(pDX, IDC_UNIT1_AXIS1_POS_LIMIT, m_ctrlAxisPosLimit[0]);
	DDX_Control(pDX, IDC_UNIT1_AXIS2_POS_LIMIT, m_ctrlAxisPosLimit[1]);
	DDX_Control(pDX, IDC_UNIT1_AXIS1_HOME, m_ctrlAxisHomeLimit[0]);
	DDX_Control(pDX, IDC_UNIT1_AXIS2_HOME, m_ctrlAxisHomeLimit[1]);
	DDX_Control(pDX, IDC_UNIT1_AXIS1_NEG_LIMIT, m_ctrlAxisNegLimit[0]);
	DDX_Control(pDX, IDC_UNIT1_AXIS2_NEG_LIMIT, m_ctrlAxisNegLimit[1]);
	DDX_Control(pDX, IDS_UNIT1_AXIS1_AMP, m_ctrlAxisAmp[0]);
	DDX_Control(pDX, IDS_UNIT1_AXIS2_AMP, m_ctrlAxisAmp[1]);
	DDX_Control(pDX, IDB_JIG_A_VAC_ON, m_ctrlJigA_Vac);
	DDX_Control(pDX, IDB_JIG_A_MOVE_FORWARD, m_ctrlJigA_Cylinder);
	DDX_Control(pDX, IDB_JIG_B_VAC_ON, m_ctrlJigB_Vac);
	DDX_Control(pDX, IDB_JIG_B_CYLINDER_IN, m_ctrlJigB_Cylinder0);
	DDX_Control(pDX, IDB_JIG_B_CYLINDER_IN2, m_ctrlJigB_Cylinder1);
	DDX_Control(pDX, IDB_JIG_B_CYLINDER_IN3, m_ctrlJigB_Cylinder2);
	DDX_Control(pDX, IDB_JIG_B_CYLINDER_IN4, m_ctrlJigB_Cylinder3);
	DDX_Control(pDX, IDB_JIG_B_CYLINDER_IN5, m_ctrlJigB_Cylinder4);
	DDX_Control(pDX, IDB_PNP_VAC_ON, m_ctrlPnP_Vac);
	DDX_Control(pDX, IDB_PNP_UP, m_ctrlPnp_Cylinder);
	DDX_Control(pDX, IDB_SHUTTLE_VAC_ON, m_ctrlPnp_Shuttle_Vac);
	DDX_Control(pDX, IDB_JIG_B_AUTO_START, m_ctrlJigB_Auto);

	DDX_Control(pDX, IDC_BTN_INPUT_HANDLER_0, m_ctrlInputCheck[0]);
	DDX_Control(pDX, IDC_BTN_INPUT_HANDLER_1, m_ctrlInputCheck[1]);
	DDX_Control(pDX, IDC_BTN_INPUT_HANDLER_2, m_ctrlInputCheck[2]);
	DDX_Control(pDX, IDC_BTN_INPUT_HANDLER_3, m_ctrlInputCheck[3]);
	DDX_Control(pDX, IDC_BTN_INPUT_HANDLER_4, m_ctrlInputCheck[4]);
	DDX_Control(pDX, IDC_BTN_INPUT_HANDLER_5, m_ctrlInputCheck[5]);
	DDX_Control(pDX, IDC_BTN_INPUT_HANDLER_6, m_ctrlInputCheck[6]);
	DDX_Control(pDX, IDC_BTN_INPUT_HANDLER_7, m_ctrlInputCheck[7]);
	DDX_Control(pDX, IDC_BTN_INPUT_HANDLER_8, m_ctrlInputCheck[8]);
	DDX_Control(pDX, IDC_BTN_INPUT_HANDLER_9, m_ctrlInputCheck[9]);
	DDX_Control(pDX, IDC_BTN_INPUT_HANDLER_10, m_ctrlInputCheck[10]);
	DDX_Control(pDX, IDC_BTN_INPUT_HANDLER_11, m_ctrlInputCheck[11]);
	DDX_Control(pDX, IDC_BTN_INPUT_HANDLER_12, m_ctrlInputCheck[12]);
	DDX_Control(pDX, IDC_BTN_INPUT_HANDLER_13, m_ctrlInputCheck[13]);
	DDX_Control(pDX, IDC_BTN_INPUT_HANDLER_14, m_ctrlInputCheck[14]);
	DDX_Control(pDX, IDC_BTN_INPUT_HANDLER_15, m_ctrlInputCheck[15]);
	DDX_Control(pDX, IDC_BTN_INPUT_HANDLER_16, m_ctrlInputCheck[16]);
	DDX_Control(pDX, IDC_BTN_INPUT_HANDLER_17, m_ctrlInputCheck[17]);
	DDX_Control(pDX, IDC_BTN_INPUT_HANDLER_18, m_ctrlInputCheck[18]);
	DDX_Control(pDX, IDC_BTN_INPUT_HANDLER_19, m_ctrlInputCheck[19]);
	DDX_Control(pDX, IDB_MAIN_OP_DOOR2, m_ctrlOpDoor);
	
}


BEGIN_MESSAGE_MAP(CDialogManual, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDialogManual 메시지 처리기입니다.


BOOL CDialogManual::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	//아진 라이브러리 초기화
	InitializeAJINX();

	//모션 초기화
	if( MotInitialize() )
	{
		TRACE("MotionInitialize Success");
	}
	else
	{
		AfxMessageBox(_T("MotionInitialize Failed"));
	}
	
	//IO 초기화
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

//아진 I/O Motion 통합 라이브러리 초기화
BOOL CDialogManual::InitializeAJINX()
{
	BOOL bRet(TRUE);
	// Initialize library 
	DWORD Code = AxlOpenNoReset(7);

	if ( Code == AXT_RT_SUCCESS)
		TRACE("Library is initialized .\n");
	else
	{
		AfxMessageBox(_T("AJINX Failed initialization."));
		return FALSE;
	}

	return TRUE;
}

BOOL CDialogManual::DestroyAJINX()
{
	BOOL bRet(TRUE);

	MotAmpDisableAll(); 

	bRet = AxlClose();

	if(bRet)
		TRACE("Library is Close OK!! .\n");
	else
		TRACE("Failed AXL Library Close!!.");

	return bRet;
}

void CDialogManual::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	if(bShow)
	{
		SetTimer(TIMER_MANUAL_UNIT1, 300, NULL);

		if(!pdlg->GetDoorState())
		{
			m_ctrlOpDoor.SetBackColor(RED);
			m_ctrlOpDoor.SetCaption(_T("DOOR UNLOCK"));
		}
		else
		{
			m_ctrlOpDoor.SetBackColor(GREEN);
			m_ctrlOpDoor.SetCaption(_T("DOOR LOCK"));
		}
	}
	else
	{
		KillTimer(TIMER_MANUAL_UNIT1);
	}

}

void CDialogManual::DisplayUnitMoveTime()
{
	CString strTime;
	m_lEndTime = GetCurrentTime();
	strTime.Format(_T("%3.3f"), (m_lEndTime - m_lStartTime) * 0.001);
	GetDlgItem(IDC_UNIT_TACTTIME)->SetWindowText(strTime);
}

BEGIN_EVENTSINK_MAP(CDialogManual, CDialog)
	ON_EVENT(CDialogManual, IDB_IO_CHECK, DISPID_CLICK, CDialogManual::ClickIoCheck, VTS_NONE)
	ON_EVENT(CDialogManual, IDB_AXIS_ORIGIN, DISPID_CLICK, CDialogManual::ClickAxisOrigin, VTS_NONE)
	ON_EVENT(CDialogManual, IDB_JIG_A_MOVE_FORWARD, DISPID_CLICK, CDialogManual::ClickJigAMoveForward, VTS_NONE)
	ON_EVENT(CDialogManual, IDB_JIG_B_VAC_ON, DISPID_CLICK, CDialogManual::ClickJigBVacOn, VTS_NONE)
	ON_EVENT(CDialogManual, IDB_JIG_B_CYLINDER_IN, DISPID_CLICK, CDialogManual::ClickJigBCylinderIn, VTS_NONE)
	ON_EVENT(CDialogManual, IDB_JIG_B_CYLINDER_IN2, DISPID_CLICK, CDialogManual::ClickJigBCylinderIn2, VTS_NONE)
	ON_EVENT(CDialogManual, IDB_JIG_B_CYLINDER_IN3, DISPID_CLICK, CDialogManual::ClickJigBCylinderIn3, VTS_NONE)
	ON_EVENT(CDialogManual, IDB_JIG_B_CYLINDER_IN4, DISPID_CLICK, CDialogManual::ClickJigBCylinderIn4, VTS_NONE)
	ON_EVENT(CDialogManual, IDB_JIG_B_CYLINDER_IN5, DISPID_CLICK, CDialogManual::ClickJigBCylinderIn5, VTS_NONE)
	ON_EVENT(CDialogManual, IDB_JIG_A_VAC_ON, DISPID_CLICK, CDialogManual::ClickJigAVacOn, VTS_NONE)
	ON_EVENT(CDialogManual, IDB_PNP_VAC_ON, DISPID_CLICK, CDialogManual::ClickPnpVacOn, VTS_NONE)
	ON_EVENT(CDialogManual, IDB_PNP_UP, DISPID_CLICK, CDialogManual::ClickPnpUp, VTS_NONE)
	ON_EVENT(CDialogManual, IDB_SHUTTLE_VAC_ON, DISPID_CLICK, CDialogManual::ClickShuttleVacOn, VTS_NONE)
	ON_EVENT(CDialogManual, IDB_JIG_B_AUTO_START, DISPID_CLICK, CDialogManual::ClickJigBAutoStart, VTS_NONE)
	ON_EVENT(CDialogManual, IDB_GRAB_START_POS, DISPID_CLICK, CDialogManual::ClickGrabStartPos, VTS_NONE)
	ON_EVENT(CDialogManual, IDB_GRAB_END_POS, DISPID_CLICK, CDialogManual::ClickGrabEndPos, VTS_NONE)
	ON_EVENT(CDialogManual, IDB_INPUT_POS, DISPID_CLICK, CDialogManual::ClickInputPos, VTS_NONE)
	ON_EVENT(CDialogManual, IDB_CAMERA_CENTER_POS, DISPID_CLICK, CDialogManual::ClickCameraCenterPos, VTS_NONE)
	ON_EVENT(CDialogManual, IDB_MAIN_OP_DOOR2, DISPID_CLICK, CDialogManual::ClickMainOpDoor2, VTS_NONE)
END_EVENTSINK_MAP()

void CDialogManual::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	DestroyAJINX();

	if(NULL != m_pIO){
		delete m_pIO;
		m_pIO = NULL;
	}
	
	if(NULL != m_pMotion)
		delete m_pMotion;
		m_pMotion = NULL;

}

void CDialogManual::ClickIoCheck()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->ShowManualOCheck();

}

void CDialogManual::ClickAxisOrigin()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->ShowManualOrigin();

}




void CDialogManual::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{	
	case TIMER_MANUAL_UNIT1:
		DispCurPos();
		DispAxisState();
		DispUnitState();
		break;
	}

	CDialog::OnTimer(nIDEvent);
}


void CDialogManual::DispCurPos()
{
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	CString strPos = _T("");
	double dCurPos = 0.0;
	int j = 1;
		
	for(int axis = 0; axis < MAX_AXIS_NUM;axis++)
	{
		/*
		m_pAxis 배열은 Y축이 0으로 설정되어 있기 때문에 Teach U/I와 반대로 대입한다.
		*/	
		pdlg->m_pDlgManual->AxisGetPosition(axis, dCurPos);
		strPos.Format(_T("%.3f"), dCurPos);
		m_ctrlAxisPos[j].SetCaption(strPos);
		j--;
	}	
}


void CDialogManual::DispAxisState()
{
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
	int j = 1;
	long level; 

	for(int axis = 0;axis < MAX_AXIS_NUM; axis++)
	{
		/*
		m_pAxis 배열은 Y축이 0으로 설정되어 있기 때문에 Teach U/I와 반대로 대입한다.
		*/	

		level = 0;

		pdlg->m_pDlgManual->AxisGetPosLimitLevel(axis, level);

		if( pdlg->m_pDlgManual->AxisGetPosLimitSensor(axis) )
		{
			if(m_ctrlAxisPosLimit[j].GetBackColor() != GREEN)
				m_ctrlAxisPosLimit[j].SetBackColor(GREEN);
		}
		else
		{
			if(m_ctrlAxisPosLimit[j].GetBackColor() != LTYELLOW)
				m_ctrlAxisPosLimit[j].SetBackColor(LTYELLOW);
		}
		
		//NEG LIMIT
		pdlg->m_pDlgManual->AxisGetNegLimitLevel(axis, level);
		if(pdlg->m_pDlgManual->AxisGetNegLimitSensor(axis))
		{
			if(m_ctrlAxisNegLimit[j].GetBackColor() != GREEN)
				m_ctrlAxisNegLimit[j].SetBackColor(GREEN);
		}
		else
		{
			if(m_ctrlAxisNegLimit[j].GetBackColor() != LTYELLOW)
				m_ctrlAxisNegLimit[j].SetBackColor(LTYELLOW);
		}
		
		//HOME LIMIT
		pdlg->m_pDlgManual->AxisGetPosLimitLevel(axis, level);

		if(pdlg->m_pDlgManual->AxisGetHomeSensor(axis))
		{
			if(m_ctrlAxisHomeLimit[j].GetBackColor() != GREEN)
				m_ctrlAxisHomeLimit[j].SetBackColor(GREEN);
		}
		else
		{
			if(m_ctrlAxisHomeLimit[j].GetBackColor() != LTYELLOW)
				m_ctrlAxisHomeLimit[j].SetBackColor(LTYELLOW);
		}
		
		if(pdlg->m_pDlgManual->AxisGetAmpFaultState(axis))
		{
			if(m_ctrlAxisAmp[j].GetBackColor() != GREEN)
			{
				m_ctrlAxisAmp[j].SetCaption(_T("Normal"));
				m_ctrlAxisAmp[j].SetBackColor(GREEN);
				m_ctrlAxisAmp[j].SetForeColor(BLACK);
			}
		}
		else
		{
			if(m_ctrlAxisAmp[j].GetBackColor() != LTYELLOW)
			{
				m_ctrlAxisAmp[j].SetCaption(_T("Fault"));
				m_ctrlAxisAmp[j].SetBackColor(LTYELLOW);
				m_ctrlAxisAmp[j].SetForeColor(RED);
			}
		}

		j--;
	} 
}

void CDialogManual::SetLampWait()
{
	WriteOutput(TOWER_LAMP_RED, FALSE);
	WriteOutput(TOWER_LAMP_GREEN, FALSE);
	WriteOutput(TOWER_LAMP_YELLOW, TRUE);
	WriteOutput(BUZZER_ERROR_SIGNAL, FALSE);
}

void CDialogManual::SetLampRun()
{
	WriteOutput(TOWER_LAMP_RED, FALSE);
	WriteOutput(TOWER_LAMP_YELLOW, FALSE);
	WriteOutput(TOWER_LAMP_GREEN, TRUE);
	WriteOutput(BUZZER_ERROR_SIGNAL, FALSE);
}

void CDialogManual::SetLampError()
{
	WriteOutput(TOWER_LAMP_GREEN, FALSE);
	WriteOutput(TOWER_LAMP_YELLOW, FALSE);
	WriteOutput(TOWER_LAMP_RED, TRUE);
	WriteOutput(BUZZER_ERROR_SIGNAL, TRUE);
}

void CDialogManual::SetLampOff()
{
	WriteOutput(TOWER_LAMP_RED, FALSE);
	WriteOutput(TOWER_LAMP_YELLOW, FALSE);
	WriteOutput(TOWER_LAMP_GREEN, FALSE);
	WriteOutput(BUZZER_ERROR_SIGNAL, FALSE);
}

void CDialogManual::SetLampButton(BOOL bState)
{

	WriteOutput(START_SWITCH_LAMP_1, bState);
	WriteOutput(READY_SWITCH_LAMP_1, bState);
	WriteOutput(RESET_SWITCH_LAMP, bState);
	WriteOutput(START_SWITCH_LAMP_2, bState);
	WriteOutput(READY_SWITCH_LAMP_2, bState);

}

void CDialogManual::DispUnitState()
{
	long color;

	if( ReadInput(GLASS_JIG_VAC_ON_CHECK) )
	{
		color = m_ctrlInputCheck[0].GetBackColor();
		if(color != GREEN)
			m_ctrlInputCheck[0].SetBackColor(GREEN);
	}
	else
	{
		color = m_ctrlInputCheck[0].GetBackColor();
		if(color != LTGRAY)
			m_ctrlInputCheck[0].SetBackColor(LTGRAY);
	}

	if( ReadInput(GLASS_JIG_CYLINDER_IN_CHECK) )
	{
		color = m_ctrlInputCheck[1].GetBackColor();
		if(color != GREEN)
			m_ctrlInputCheck[1].SetBackColor(GREEN);
	}
	else
	{
		color = m_ctrlInputCheck[1].GetBackColor();
		if(color != LTGRAY)
			m_ctrlInputCheck[1].SetBackColor(LTGRAY);
	}

	if( ReadInput(GLASS_JIG_CYLINDER_OUT_CHECK) )
	{
		color = m_ctrlInputCheck[2].GetBackColor();
		if(color != GREEN)
			m_ctrlInputCheck[2].SetBackColor(GREEN);
	}
	else
	{
		color = m_ctrlInputCheck[2].GetBackColor();
		if(color != LTGRAY)
			m_ctrlInputCheck[2].SetBackColor(LTGRAY);
	}

	if( ReadInput(FILM_JIG_VAC_ON_CHECK) )
	{
		color = m_ctrlInputCheck[3].GetBackColor();
		if(color != GREEN)
			m_ctrlInputCheck[3].SetBackColor(GREEN);
	}
	else
	{
		color = m_ctrlInputCheck[3].GetBackColor();
		if(color != LTGRAY)
			m_ctrlInputCheck[3].SetBackColor(LTGRAY);
	}

	if( ReadInput(FILM_JIG_CYLINDER_IN_CHECK) )
	{
		color = m_ctrlInputCheck[4].GetBackColor();
		if(color != GREEN)
			m_ctrlInputCheck[4].SetBackColor(GREEN);
	}
	else
	{
		color = m_ctrlInputCheck[4].GetBackColor();
		if(color != LTGRAY)
			m_ctrlInputCheck[4].SetBackColor(LTGRAY);
	}

	if( ReadInput(FILM_JIG_CYLINDER_OUT_CHECK) )
	{
		color = m_ctrlInputCheck[5].GetBackColor();
		if(color != GREEN)
			m_ctrlInputCheck[5].SetBackColor(GREEN);
	}
	else
	{
		color = m_ctrlInputCheck[5].GetBackColor();
		if(color != LTGRAY)
			m_ctrlInputCheck[5].SetBackColor(LTGRAY);
	}

	if( ReadInput(FILM_JIG_GLASS_CLAMP_CYLINDER_BACK_CHECK) )
	{
		color = m_ctrlInputCheck[6].GetBackColor();
		if(color != GREEN)
			m_ctrlInputCheck[6].SetBackColor(GREEN);
	}
	else
	{
		color = m_ctrlInputCheck[6].GetBackColor();
		if(color != LTGRAY)
			m_ctrlInputCheck[6].SetBackColor(LTGRAY);
	}

	if( ReadInput(FILM_JIG_GLASS_CLAMP_CYLINDER_FEED_CHECK) )
	{
		color = m_ctrlInputCheck[7].GetBackColor();
		if(color != GREEN)
			m_ctrlInputCheck[7].SetBackColor(GREEN);
	}
	else
	{
		color = m_ctrlInputCheck[7].GetBackColor();
		if(color != LTGRAY)
			m_ctrlInputCheck[7].SetBackColor(LTGRAY);
	}

	if( ReadInput(FILM_JIG_GLASS_CLAMP_CYLINDER_CLAMP_CHECK) )
	{
		color = m_ctrlInputCheck[8].GetBackColor();
		if(color != GREEN)
			m_ctrlInputCheck[8].SetBackColor(GREEN);
	}
	else
	{
		color = m_ctrlInputCheck[8].GetBackColor();
		if(color != LTGRAY)
			m_ctrlInputCheck[8].SetBackColor(LTGRAY);
	}

	if( ReadInput(FILM_JIG_GLASS_CLAMP_CYLINDER_UNCLAMP_CHECK) )
	{
		color = m_ctrlInputCheck[9].GetBackColor();
		if(color != GREEN)
			m_ctrlInputCheck[9].SetBackColor(GREEN);
	}
	else
	{
		color = m_ctrlInputCheck[9].GetBackColor();
		if(color != LTGRAY)
			m_ctrlInputCheck[9].SetBackColor(LTGRAY);
	}

	if( ReadInput(FILM_JIG_TILTING_CYLINDER_TILT_UP_CHECK) )
	{
		color = m_ctrlInputCheck[10].GetBackColor();
		if(color != GREEN)
			m_ctrlInputCheck[10].SetBackColor(GREEN);
	}
	else
	{
		color = m_ctrlInputCheck[10].GetBackColor();
		if(color != LTGRAY)
			m_ctrlInputCheck[10].SetBackColor(LTGRAY);
	}

	if( ReadInput(FILM_JIG_TILTING_CYLINDER_TILT_DN_CHECK) )
	{
		color = m_ctrlInputCheck[11].GetBackColor();
		if(color != GREEN)
			m_ctrlInputCheck[11].SetBackColor(GREEN);
	}
	else
	{
		color = m_ctrlInputCheck[11].GetBackColor();
		if(color != LTGRAY)
			m_ctrlInputCheck[11].SetBackColor(LTGRAY);
	}

	if( ReadInput(FILM_JIG_VAC_PLATE_CYLINDER_UP_CHECK) )
	{
		color = m_ctrlInputCheck[12].GetBackColor();
		if(color != GREEN)
			m_ctrlInputCheck[12].SetBackColor(GREEN);
	}
	else
	{
		color = m_ctrlInputCheck[12].GetBackColor();
		if(color != LTGRAY)
			m_ctrlInputCheck[12].SetBackColor(LTGRAY);
	}

	if( ReadInput(FILM_JIG_VAC_PLATE_CYLINDER_DN_CHECK) )
	{
		color = m_ctrlInputCheck[13].GetBackColor();
		if(color != GREEN)
			m_ctrlInputCheck[13].SetBackColor(GREEN);
	}
	else
	{
		color = m_ctrlInputCheck[13].GetBackColor();
		if(color != LTGRAY)
			m_ctrlInputCheck[13].SetBackColor(LTGRAY);
	}

	if( ReadInput(LD_PNP_VAC_ON_CHECK) )
	{
		color = m_ctrlInputCheck[14].GetBackColor();
		if(color != GREEN)
			m_ctrlInputCheck[14].SetBackColor(GREEN);
	}
	else
	{
		color = m_ctrlInputCheck[14].GetBackColor();
		if(color != LTGRAY)
			m_ctrlInputCheck[14].SetBackColor(LTGRAY);
	}

	if( ReadInput(LD_PNP_CYLINDER_UP_CHECK) )
	{
		color = m_ctrlInputCheck[15].GetBackColor();
		if(color != GREEN)
			m_ctrlInputCheck[15].SetBackColor(GREEN);
	}
	else
	{
		color = m_ctrlInputCheck[15].GetBackColor();
		if(color != LTGRAY)
			m_ctrlInputCheck[15].SetBackColor(LTGRAY);
	}

	if( ReadInput(LD_PNP_CYLINDER_DN_CHECK) )
	{
		color = m_ctrlInputCheck[16].GetBackColor();
		if(color != GREEN)
			m_ctrlInputCheck[16].SetBackColor(GREEN);
	}
	else
	{
		color = m_ctrlInputCheck[16].GetBackColor();
		if(color != LTGRAY)
			m_ctrlInputCheck[16].SetBackColor(LTGRAY);
	}

	if( ReadInput(TRAY_SHUTTLE_GLASS_VAC_ON_CHECK) )
	{
		color = m_ctrlInputCheck[17].GetBackColor();
		if(color != GREEN)
			m_ctrlInputCheck[17].SetBackColor(GREEN);
	}
	else
	{
		color = m_ctrlInputCheck[17].GetBackColor();
		if(color != LTGRAY)
			m_ctrlInputCheck[17].SetBackColor(LTGRAY);
	}
}

void CDialogManual::ClickJigBVacOn()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	if(m_ctrlJigB_Vac.GetValue())
	{
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_VAC_OFF_SOL, FALSE);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_VAC_ON_SOL, TRUE);
	}
	else
	{
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_VAC_ON_SOL, FALSE);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_VAC_OFF_SOL, TRUE);
		Sleep(100);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_VAC_OFF_SOL, FALSE);
	}
}

//Film 분리 Jig Cylinder 투입/배출위치 SOL
void CDialogManual::ClickJigBCylinderIn()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	if(!pdlg->m_pDlgManual->ReadInput(LD_PNP_CYLINDER_UP_CHECK))
	{
		pdlg->m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_UP_SOL, TRUE);
		pdlg->m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_DN_SOL, FALSE);
	}

	Delay(200);

	if(m_ctrlJigB_Cylinder0.GetValue())
	{
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_CYLINDER_OUT_SOL, FALSE);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_CYLINDER_IN_SOL, TRUE);
	}
	else
	{
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_CYLINDER_OUT_SOL, TRUE);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_CYLINDER_IN_SOL, FALSE);
	}
}


void CDialogManual::ClickJigBCylinderIn2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	if(m_ctrlJigB_Cylinder1.GetValue())
	{
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_FEED_SOL, TRUE);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_BACK_SOL, FALSE);
	}
	else
	{
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_BACK_SOL, TRUE);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_FEED_SOL, FALSE);
	}
}


void CDialogManual::ClickJigBCylinderIn3()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	if(m_ctrlJigB_Cylinder2.GetValue())
	{
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_CLAMP_SOL, TRUE);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_UNCLAMP_SOL, FALSE);
	}
	else
	{
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_UNCLAMP_SOL, TRUE);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_CLAMP_SOL, FALSE);
	}
}


void CDialogManual::ClickJigBCylinderIn4()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	if(m_ctrlJigB_Cylinder3.GetValue())
	{
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_TILTING_CYLINDER_TILT_UP_SOL, FALSE);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_TILTING_CYLINDER_TILT_DN_SOL, TRUE);
	}
	else
	{
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_TILTING_CYLINDER_TILT_DN_SOL, FALSE);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_TILTING_CYLINDER_TILT_UP_SOL, TRUE);
	}
}


void CDialogManual::ClickJigBCylinderIn5()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	if(m_ctrlJigB_Cylinder4.GetValue())
	{
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_VAC_PLATE_CYLINDER_UP_SOL, FALSE);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_VAC_PLATE_CYLINDER_DN_SOL, TRUE);
	}
	else
	{
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_VAC_PLATE_CYLINDER_DN_SOL, FALSE);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_VAC_PLATE_CYLINDER_UP_SOL, TRUE);
	}
}


void CDialogManual::ClickJigAVacOn()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	if(m_ctrlJigA_Vac.GetValue())
	{
		pdlg->m_pDlgManual->WriteOutput(GLASS_JIG_VAC_OFF_SOL, FALSE);
		pdlg->m_pDlgManual->WriteOutput(GLASS_JIG_VAC_ON_SOL, TRUE);
	}
	else
	{
		pdlg->m_pDlgManual->WriteOutput(GLASS_JIG_VAC_OFF_SOL, TRUE);
		pdlg->m_pDlgManual->WriteOutput(GLASS_JIG_VAC_ON_SOL, FALSE);
		Delay(100);
		pdlg->m_pDlgManual->WriteOutput(GLASS_JIG_VAC_OFF_SOL, FALSE);

	}
}

void CDialogManual::ClickJigAMoveForward()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	if(!pdlg->m_pDlgManual->ReadInput(LD_PNP_CYLINDER_UP_CHECK))
	{
		pdlg->m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_UP_SOL, TRUE);
		pdlg->m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_DN_SOL, FALSE);
	}

	Delay(200);

	if(m_ctrlJigA_Cylinder.GetValue())
	{
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_CYLINDER_OUT_SOL, FALSE);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_CYLINDER_IN_SOL, TRUE);
	}
	else
	{
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_CYLINDER_OUT_SOL, TRUE);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_CYLINDER_IN_SOL, FALSE);
	}

}

void CDialogManual::ClickPnpVacOn()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	if(m_ctrlPnP_Vac.GetValue())
	{
		pdlg->m_pDlgManual->WriteOutput(LD_PNP_VAC_ON_SOL, TRUE);
		pdlg->m_pDlgManual->WriteOutput(LD_PNP_VAC_OFF_SOL, FALSE);
	}
	else
	{
		pdlg->m_pDlgManual->WriteOutput(LD_PNP_VAC_ON_SOL, FALSE);
		pdlg->m_pDlgManual->WriteOutput(LD_PNP_VAC_OFF_SOL, TRUE);
		Delay(30);
		pdlg->m_pDlgManual->WriteOutput(LD_PNP_VAC_OFF_SOL, FALSE);
	}
}

void CDialogManual::ClickPnpUp()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	if(m_ctrlPnp_Cylinder.GetValue())
	{
		pdlg->m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_UP_SOL, FALSE);
		pdlg->m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_DN_SOL, TRUE);
	}
	else
	{
		pdlg->m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_UP_SOL, TRUE);
		pdlg->m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_DN_SOL, FALSE);
	}
}


void CDialogManual::ClickShuttleVacOn()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	if(m_ctrlPnp_Shuttle_Vac.GetValue())
	{
		pdlg->m_pDlgManual->WriteOutput(TRAY_SHUTTLE_GLASS_VAC_ON_SOL, TRUE);
		pdlg->m_pDlgManual->WriteOutput(TRAY_SHUTTLE_GLASS_VAC_OFF_SOL, FALSE);
	}
	else
	{
		pdlg->m_pDlgManual->WriteOutput(TRAY_SHUTTLE_GLASS_VAC_ON_SOL, FALSE);
		pdlg->m_pDlgManual->WriteOutput(TRAY_SHUTTLE_GLASS_VAC_OFF_SOL, TRUE);
		Delay(100);
		pdlg->m_pDlgManual->WriteOutput(TRAY_SHUTTLE_GLASS_VAC_OFF_SOL, FALSE);
	}
}


void CDialogManual::ClickJigBAutoStart()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;


	if(m_ctrlJigB_Auto.GetValue())
	{
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_VAC_OFF_SOL, FALSE);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_VAC_ON_SOL, TRUE);
		Delay(200);

		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_CLAMP_SOL, TRUE);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_UNCLAMP_SOL, FALSE);
		Delay(200);

		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_FEED_SOL, TRUE);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_BACK_SOL, FALSE);
		Delay(200);

		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_TILTING_CYLINDER_TILT_UP_SOL, FALSE);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_TILTING_CYLINDER_TILT_DN_SOL, TRUE);

		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_VAC_PLATE_CYLINDER_UP_SOL, FALSE);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_VAC_PLATE_CYLINDER_DN_SOL, TRUE);
		Delay(500);

		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_VAC_OFF_SOL, TRUE);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_VAC_ON_SOL, FALSE);
		Delay(30);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_VAC_OFF_SOL, FALSE);

	}
	else
	{
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_VAC_PLATE_CYLINDER_UP_SOL, TRUE);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_VAC_PLATE_CYLINDER_DN_SOL, FALSE);

		//Delay(1000, TRUE);

		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_TILTING_CYLINDER_TILT_UP_SOL, TRUE);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_TILTING_CYLINDER_TILT_DN_SOL, FALSE);

		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_FEED_SOL, FALSE);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_BACK_SOL, TRUE);

		Delay(200, TRUE);
		
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_CLAMP_SOL, FALSE);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_UNCLAMP_SOL, TRUE);

		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_VAC_ON_SOL, FALSE);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_VAC_OFF_SOL, TRUE);
		Sleep(100);
		pdlg->m_pDlgManual->WriteOutput(FILM_JIG_VAC_OFF_SOL, FALSE);
	}
}


void CDialogManual::ClickGrabStartPos()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//픽커 위치 체크
	//원점 체크
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	if(!pdlg->m_pDlgManual->AxisIsHomeFinished(AXIS_TRY_SHUTTLE))
	{
		g_cMsgBox.OkMsgBox(_T("축 원점 작업이 완료되어 있지 않습니다."));
		return;
	}

	if(!pdlg->m_pDlgManual->ReadInput(LD_PNP_CYLINDER_UP_CHECK))
	{
		pdlg->m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_UP_SOL, TRUE);
		pdlg->m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_DN_SOL, FALSE);
	}

	AxisMove(AXIS_TRY_SHUTTLE, pdlg->m_pDlgTeach->GetInspPosY(), 3, FALSE);
}


void CDialogManual::ClickGrabEndPos()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//픽커 위치 체크
	//원점 체크
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	if(!pdlg->m_pDlgManual->ReadInput(SERVO_MC_ON_OFF_CHECK))
	{
		g_cMsgBox.OkMsgBox(_T("Servo MC Error! Reset 버튼을 1초간 눌러주세요"));
		return;
	}

	if(!pdlg->m_pDlgManual->AxisIsHomeFinished(AXIS_TRY_SHUTTLE))
	{
		g_cMsgBox.OkMsgBox(_T("축 원점 작업이 완료되어 있지 않습니다."));
		return;
	}
	
	double dStartPos, dEndPos, dPeriod;

	if(!pdlg->m_pDlgManual->ReadInput(LD_PNP_CYLINDER_UP_CHECK))
	{
		pdlg->m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_UP_SOL, TRUE);
		pdlg->m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_DN_SOL, FALSE);
	}

	dStartPos = pdlg->m_pDlgTeach->GetInspPosY() * 1000;
	dEndPos = pdlg->m_pDlgTeach->GetUnLoadPosY() * 1000;
	dPeriod = AxisGetTriggerPeriod(AXIS_TRY_SHUTTLE);

	AxisSetTriggerReset(AXIS_TRY_SHUTTLE);	//트리거 리셋
	Delay(300);
	AxisSetTriggerTimeLevel(AXIS_TRY_SHUTTLE);	//트리거 레벨
	AxisSetTriggerBlock(AXIS_TRY_SHUTTLE, dStartPos, dEndPos, dPeriod);//트리거 블럭셋
	Delay(100);
	AxisMove(AXIS_TRY_SHUTTLE, pdlg->m_pDlgTeach->GetUnLoadPosY(), 1, FALSE);
}


void CDialogManual::ClickInputPos()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	if(!pdlg->m_pDlgManual->ReadInput(SERVO_MC_ON_OFF_CHECK))
	{
		g_cMsgBox.OkMsgBox(_T("Servo MC Error! Reset 버튼을 1초간 눌러주세요"));
		return;
	}

	if(!pdlg->m_pDlgManual->AxisIsHomeFinished(AXIS_TRY_SHUTTLE))
	{
		g_cMsgBox.OkMsgBox(_T("축 원점 작업이 완료되어 있지 않습니다."));
		return;
	}

	if(!pdlg->m_pDlgManual->ReadInput(LD_PNP_CYLINDER_UP_CHECK))
	{
		pdlg->m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_UP_SOL, TRUE);
		pdlg->m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_DN_SOL, FALSE);
	}

	AxisMove(AXIS_TRY_SHUTTLE, pdlg->m_pDlgTeach->GetLoadPosY(), 3, FALSE);
}


void CDialogManual::ClickCameraCenterPos()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	if(!pdlg->m_pDlgManual->ReadInput(SERVO_MC_ON_OFF_CHECK))
	{
		g_cMsgBox.OkMsgBox(_T("Servo MC Error! Reset 버튼을 1초간 눌러주세요"));
		return;
	}

	if(!pdlg->m_pDlgManual->AxisIsHomeFinished(AXIS_TRY_SHUTTLE))
	{
		g_cMsgBox.OkMsgBox(_T("축 원점 작업이 완료되어 있지 않습니다."));
		return;
	}

	if(!pdlg->m_pDlgManual->ReadInput(LD_PNP_CYLINDER_UP_CHECK))
	{
		pdlg->m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_UP_SOL, TRUE);
		pdlg->m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_DN_SOL, FALSE);
	}

	AxisMove(AXIS_TRY_SHUTTLE, pdlg->m_pDlgTeach->GetCamCenterPosY(), 3, FALSE);
}


void CDialogManual::ClickMainOpDoor2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->ClickMainOpDoor();

	if(!pdlg->GetDoorState())
	{
		m_ctrlOpDoor.SetBackColor(RED);
		m_ctrlOpDoor.SetCaption(_T("DOOR UNLOCK"));
	}
	else
	{
		m_ctrlOpDoor.SetBackColor(GREEN);
		m_ctrlOpDoor.SetCaption(_T("DOOR LOCK"));
	}
}
