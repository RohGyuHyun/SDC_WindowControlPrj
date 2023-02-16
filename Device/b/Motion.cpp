// Motion.cpp: implementation of the CMotion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "SDC_WindowInspPrj.h"
#include "Motion.h"
//#include "MainFrm.h"
//#include "MessageView.h"
#include "SystemData.h"
//#include "AniUtil.h"
#include "TimeCheck.h"
#include "Define.h"
#include "Axis.h"
#ifdef _USE_AJINXL
#include "AXL.h"
#include "AXM.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMotion g_Motion;

CMotion::CMotion()
{
	/*
	MMC_INITIALZED = FALSE;
#ifdef _USE_DNET
	mAxisNumCount = 0;
	portID = 0;
#endif
	CreateMotion();

	LoadAxisConfig();
	SetAxis();
	SetAxisOrder();

#ifdef _USE_FASTECH
	m_iMaxInputCnt = 0;
	m_iMaxOutputCnt = 0;
	m_IoInputList = m_IoOutputList = NULL; 
#endif
#ifdef _USE_NMC
	m_bMNCInitFlage = false;
#endif
	g_hMutex = CreateMutex(NULL, FALSE, NULL);	
	*/
}

CMotion::~CMotion()
{
	/*
	DestroyMotion();
	CloseHandle(g_hMutex);

#ifdef _USE_FASTECH
	if(g_Motion.m_IoInputList != NULL)
	{
		delete [] g_Motion.m_IoInputList;
		g_Motion.m_IoInputList = NULL;
	}
	if(g_Motion.m_IoOutputList != NULL)
	{
		delete [] g_Motion.m_IoOutputList;
		g_Motion.m_IoOutputList = NULL;
	}
#endif

#ifdef _USE_AJINXL
	AxlClose();  
#endif
	*/
}

/*
bool CMotion::SaveAxisConfig()
{
	int ret;
	CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	
	for(int i=0; i < MAX_AXIS_NUM; i++)
	{
		ret = m_pAxis[i]->SaveAxisConfig();
		if(!ret)
			return false;
	}

	return true;
}

bool CMotion::LoadAxisConfig()
{
	int ret;
	CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());

	for(int i=0; i < MAX_AXIS_NUM; i++)
	{
		ret = m_pAxis[i]->LoadAxisConfig();

		m_pAxis[i]->SetAxisAccelUnit();

		if(!ret)
			return false;
// #ifdef _USE_DNET //151029 JSLee
// 		ret = m_pAxis[i]->SetProfileSetup();
// 
// 		if(!ret)
// 			return false;
// #endif
	}
#ifdef _USE_DNET
//	m_pAxis[AXIS_TRANSFER_X]->SetHomingSetup();
// 	// Set.Motion.Group1
// 	m_pAxis[AXIS_SHUTTLE_Y1]->SetMultiGroup(SHUTTLE1_MULTY_GROUP);
// 	m_pAxis[AXIS_FILM_T1]->SetMultiGroup(SHUTTLE1_MULTY_GROUP);
// 	// Set.Motion.Group2
// 	m_pAxis[AXIS_SHUTTLE_Y2]->SetMultiGroup(SHUTTLE2_MULTY_GROUP);
// 	m_pAxis[AXIS_FILM_T2]->SetMultiGroup(SHUTTLE2_MULTY_GROUP);

#endif

	return true;
}
void CMotion::SafeOperation()
{
#ifndef _SYSTEM_SIMULATION //151102 JSLee
	dnetSafeOperation(portID);
#endif
}
void CMotion::SetAxis()
{
	//MMC Or ML2
	CString strName[MAX_AXIS_NUM] = {_T("INSPECTION_SHUTTLE_X"),_T("LD_PNP_Y")};

	for(int i=0; i < MAX_AXIS_NUM; i++)
	{
		m_pAxis[i]->SetAxisNo(i);
		m_pAxis[i]->SetAxisId(i+1);
		m_pAxis[i]->SetAxisName(strName[i]);
	}
}

void CMotion::SetAxisOrder()
{
	//>> 물류 Axis
	
	m_paxAlignX				= m_pAxis[AXIS_ALIGN_X];
	m_paxAlignY				= m_pAxis[AXIS_ALIGN_Y];
	m_paxAlignZ				= m_pAxis[AXIS_ALIGN_Z];
	m_paxAlignT				= m_pAxis[AXIS_ALIGN_T];
	m_paxShuttleX			= m_pAxis[AXIS_SHUTTLE_X];
	m_paxShuttleZ			= m_pAxis[AXIS_SHUTTLE_Z];
	m_paxUnloadX			= m_pAxis[AXIS_UNLOAD_X];
	m_paxUnloadZ			= m_pAxis[AXIS_UNLOAD_Z];
	m_paxLoadCV				= m_pAxis[AXIS_LOADING_CONVEYOR];

}

bool CMotion::CheckOrigin(int nAxis)
{
#ifdef NOT_MACHINE
	return false;
#endif

	if(nAxis == MAX_AXIS_NUM)
	{
		for(int i=0; i<MAX_AXIS_NUM; i++)
		{
			if(!m_pAxis[i]->CheckHomingDone())
				return false;
		}
	}
	else
	{
		if(!m_pAxis[nAxis]->CheckHomingDone())
			return false;
	}

	return true;
}
//JMHWANG 130826
bool CMotion::CheckAmp()
{
#ifdef NOT_MACHINE
	return false;
#endif

	BOOL bstate;
	
	for(int j = 0; j< MAX_AXIS_NUM; j++)
	{
		bstate = g_Motion.m_pAxis[j]->GetAmpState(TRUE);
		if(!bstate)
		{
			return false;
		}
	}
	return true;
}


void CMotion::SetHomingOrder()
{
#ifdef NOT_MACHINE
	return;
#endif
	//Z축과 인터록 우선 순위 인 놈들 부터 잡자~~
	//   Interlock은 Group당 동시에 동작하고 Check 해서 사전에 알람 발생 후 진행 한다. 
	// - Group 1 :
	//   Stage Shift X축	[1 Axis]

	// - Group 2 :     
	//   Main Stage U/V축	[2 Axis]
 
	// - Group 3 :
	//   Main Stage W축	[1 Axis] 

	// 두축은 간섭으로 인해 Senosr 확인후 순번 정하기!


	m_paxAlignX->SetHomingOrder(0);	
	m_paxAlignY->SetHomingOrder(0);		
	m_paxAlignZ->SetHomingOrder(0);		
	m_paxAlignT->SetHomingOrder(0);		
	m_paxShuttleX->SetHomingOrder(0);	
	m_paxShuttleZ->SetHomingOrder(0);	
	m_paxUnloadX->SetHomingOrder(0);	
	m_paxUnloadZ->SetHomingOrder(0);	
	m_paxLoadCV->SetHomingOrder(0);	
}

BOOL CMotion::AmpEnableAll()
{ 
	for(int i=0; i<MAX_AXIS_NUM; i++)
	{
		if(!m_pAxis[i]->AmpEnable())
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CMotion::AmpDisableAll()
{
	for(int i=0; i<MAX_AXIS_NUM; i++)
	{
		m_pAxis[i]->ClearState();
		m_pAxis[i]->ClearAxis();
		m_pAxis[i]->ResetAmpFault();
		m_pAxis[i]->SetThreadStarted(FALSE);
		m_pAxis[i]->SetHomingErr(FALSE);
		m_pAxis[i]->SetHomeFinished(FALSE);
		m_pAxis[i]->AmpDisable();
	}

	return TRUE;
}

void CMotion::ClearHomingErr()
{
	for(int i=0; i<MAX_AXIS_NUM; i++)
	{
		m_pAxis[i]->SetHomingErr(FALSE);
	}
}

void CMotion::BrakeHoming()
{
	for(int i=0; i<MAX_AXIS_NUM; i++)
	{
		m_pAxis[i]->SetHomingErr(TRUE);
	}
}

BOOL CMotion::IsHoming()
{
	for(int i=0; i<MAX_AXIS_NUM; i++)
	{
		if(m_pAxis[i]->IsThreadStarted())
			return TRUE;
	}

	return FALSE;
}

void CMotion::ResetHomingAll()
{
	for(int i=0; i<MAX_AXIS_NUM; i++)
	{
		m_pAxis[i]->ClearState();
		m_pAxis[i]->SetThreadStarted(FALSE);
		m_pAxis[i]->SetHomingErr(FALSE);
		m_pAxis[i]->SetHomeFinished(FALSE);
	}
}

void CMotion::ResetHoming(int iAxis) //131013 JSPark
{
	m_pAxis[iAxis]->ClearState();
	m_pAxis[iAxis]->SetThreadStarted(FALSE);
	m_pAxis[iAxis]->SetHomingErr(FALSE);
	m_pAxis[iAxis]->SetHomeFinished(FALSE);
}

void CMotion::MoveEstopAll()
{
	for(int i=0; i<MAX_AXIS_NUM; i++)
	{
		m_pAxis[i]->MoveEStop();
		
	}
}

void CMotion::MoveStopAll()
{
	for(int i=0; i<MAX_AXIS_NUM; i++)
	{
		m_pAxis[i]->MoveStop();
	}
}

bool CMotion::Initialize(CAxisConfig* axisConfig)
{
	CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	// set Config INI File
	BOOL BRet(false);
	m_pAxisConfig = axisConfig;
	int axis_no = 0;

	for (axis_no=0; axis_no<MAX_AXIS_NUM; axis_no++)
		ReadAxisConfig(axis_no);
//>> 130219 JSPark
#ifdef _USE_MMC
	// Init MMC Board
	long addr[NO_OF_MMCBD] = { 0xD8000000 };
	int err = mmc_initx(NO_OF_MMCBD, addr);
	if (err)
	{
		if (err == MMC_TIMEOUT_ERR)
			set_dpram_addr(0, 0xD8000000);
		else
			return FALSE;
	}
#elif _USE_AJINXL
	DWORD Code = AXT_RT_SUCCESS;
//	if(!pMainFrame->IsAXLInitFlag) // Ajin Init시 다시 할 필요 없음...
//		 Code = AxlOpen(7); 

	if (Code == AXT_RT_SUCCESS)
	{
		CString strInfo;
		//Module 확인 
		DWORD dwStatus;
		Code = AxmInfoIsMotionModule(&dwStatus);
		if(Code == AXT_RT_SUCCESS)
		{
			if(dwStatus == STATUS_EXIST)
			{
				// 축 정보를 읽어 온다. 기본 설정...이후 공통 항목으로 다시 설정 한다. 
 				m_pAxis[0]->bAjinMotorParamLoadAll();
			}
			else
				AfxMessageBox(_T("Ajin Motion Module 이 확인되지 않았습니다."));
		}
	}
	else
	{
		AfxMessageBox(_T("Ajin B/D 초기화 실패!!! "));
	}
#endif
//<<
	SetHomingOrder();
// 	for (axis_no = 0; axis_no < MAX_AXIS_NUM; axis_no++)
// 	{
// 		if (!SetAxisConfig(axis_no))
// 		{
// 			return FALSE;
// 		}
// 	}

#ifdef _USE_AJINXL
	m_pAxis[0]->bAjinMotorParamLoadAll();
#endif
#ifdef _USE_FASTECH 
	for(axis_no = 0; axis_no < MAX_AXIS_NUM; axis_no++)
	{
		if(m_pAxis[axis_no]->GetControlType() == _FASTECH)
		{
			if(!m_pAxis[axis_no]->bConnectFastechDriver())
				return FALSE;
		}
	}
#endif
#ifdef _USE_DNET
	//////////////////////////////////////////////////////////////////////////
	CString strPort = _T("");
	int nRet = 0;
	
	// 포트 열기 : 
	
	nRet = dnetOpenPort("COM1", &portID);
	
	if (nRet != DNET_ERR_NO){
		//ErrorException
		AfxMessageBox(_T("Init Fail"));
		BRet = false;
	}
	
	dnetGetDeviceNum(portID, &mAxisNumCount);
	//////////////////////////////////////////////////////////////////////////
	// Driver에서 가져온 Device Count와 설정 Count가 다르면..
	if(mAxisNumCount != (MAX_AXIS_NUM*2))
	{
		AfxMessageBox(_T("dnetGetDeviceNum Failed.."));
	}
#endif
#ifdef _USE_NMC
	MC_STATUS ms;
	// 초기화
	ms = MC_Init();
	if(ms == MC_OK)
	{
		ms = MC_MasterRUN(0);
		if(ms != MC_OK)
		{
			AfxMessageBox(L"NMC Board Run Mode Fail");
		}

		if(!BRet)
			BRet = FALSE;
	}
	else
	{
		if(ms ==MC_MASTERID_DUPLICATION_ERR)
		{
			AfxMessageBox(L"NMC Board Run Mode Fail");
		}
		AfxMessageBox(L"Initialization NMC Board Fail");
		m_bMNCInitFlage = false;
			BRet = FALSE;
	}

	if(ms == MC_OK) //NMC ONOFF Flag
		m_bMNCInitFlage = true;
	else
		m_bMNCInitFlage = false;
#endif

	// Motor 종류별 파라메타 할당
	for (axis_no = 0; axis_no < MAX_AXIS_NUM; axis_no++)
	{
		if (!SetAxisConfig(axis_no))
		{
			return FALSE;
		}
	}
	if(!BRet)
	{
		return FALSE;
	}
	else
	{
		MMC_INITIALZED = TRUE;
		return TRUE;
	}
}

void CMotion::ReadAxisConfig(int axis_no)
{
	// 기본 설정  -------------------------------------------------------------
	// (Hard Cording Parameter)
	// for Motor Type
	m_pAxis[axis_no]->SetParasMotor(MICRO_STEPPER);		// (SERVO_MOTOR 0/STEPPER 1/MICRO_STEPPER 2)
	m_pAxis[axis_no]->SetParasLoopMode(FALSE);			// (ColsedLoop TRUE/OpenLoop FALSE)
	m_pAxis[axis_no]->SetParasStepMode(TWO_PULSE);		// CW+CCW(TWO_PULSE) 0/Sign+Pulse(SIGN_PULSE) 1 
	// for Level/Event
	m_pAxis[axis_no]->SetParasErrorLimit(1000);
	m_pAxis[axis_no]->SetParasErrorLimitEvent(NO_EVENT);
	
	// Motor 설정 -------------------------------------------------------------
	// from INI (read Only Parameter)
	CString strFilePath = _T("");
	strFilePath.Format(L"%sAxisDefine.ini",DATA_SYSTEM_PATH);

	m_pAxisConfig->SetFilePath(strFilePath);
	// Axis 정의
	CString AxisKeyName = _T("");
	
	// HardWare Limit Sensor & amp 관련된 Level/Event
	AxisKeyName = m_pAxis[axis_no]->GetAxisName();
	m_pAxis[axis_no]->SetParasAxisUse(m_pAxisConfig->GetInt(AxisKeyName, _T("AxisUse"), 1)); //130220 JSPark
	//>> 130317 JSPark
	m_pAxis[axis_no]->SetParasMotorType(m_pAxisConfig->GetInt(AxisKeyName, _T("MotorType"), 0));
	m_pAxis[axis_no]->SetParasPulseMethod(m_pAxisConfig->GetInt(AxisKeyName, _T("PulseOutMethod"), 8));

	// Motion Controller Setting
	m_pAxis[axis_no]->SetParasPortNo(m_pAxisConfig->GetInt(AxisKeyName, _T("ComPort"), 1));
	m_pAxis[axis_no]->SetParasControlType(m_pAxisConfig->GetInt(AxisKeyName, _T("ControlType"), 3));
	m_pAxis[axis_no]->SetParasLoadRatioType(m_pAxisConfig->GetInt(AxisKeyName, _T("LoadRatioType"), 1));
	//<<
	// Origin Method Select
	//>> 130325 JSPark
#ifdef _USE_AJINXL
	m_pAxis[axis_no]->SetParasOriginMethod(m_pAxisConfig->GetInt(AxisKeyName, _T("OriginMethod"), 3));
#else
	m_pAxis[axis_no]->SetParasOriginMethod(m_pAxisConfig->GetInt(AxisKeyName, _T("OriginMethod"), 0));
#endif
	//<<
	// Limit Sensor Setting
	m_pAxis[axis_no]->SetParasPositiveLimitLevel(m_pAxisConfig->GetInt(AxisKeyName, _T("PositiveLimitLevel"), 0));
	m_pAxis[axis_no]->SetParasHomeLimitLevel(m_pAxisConfig->GetInt(AxisKeyName, _T("HomeLevel"), 0));
	m_pAxis[axis_no]->SetParasNegativeLimitLevel(m_pAxisConfig->GetInt(AxisKeyName, _T("NegativeLimitLevel"), 0));
	// Amp Setting
	m_pAxis[axis_no]->SetParasAmpFaultLevel(m_pAxisConfig->GetInt(AxisKeyName, _T("AmpFaultLevel"), 0));
	m_pAxis[axis_no]->SetParasAmpEnableLevel(m_pAxisConfig->GetInt(AxisKeyName, _T("AmpEnableLevel"), 1));
	m_pAxis[axis_no]->SetParasAmpFaultResetLevel(m_pAxisConfig->GetInt(AxisKeyName, _T("AmpFaultResetLevel"), 0));
	// Axis Direction Setting
	m_pAxis[axis_no]->SetParasCordinateDirection(m_pAxisConfig->GetInt(AxisKeyName, _T("Direction"), 0));
	// Driver Electronic Gear Setting
	m_pAxis[axis_no]->SetParasGearRatio(m_pAxisConfig->GetDouble(AxisKeyName, _T("E_Gear"), 1.0));
	// Software Gear Setting
	m_pAxis[axis_no]->SetParasMm4Pulse(m_pAxisConfig->GetDouble(AxisKeyName, _T("Mm4Pulse"), 0.001));
	m_pAxis[axis_no]->SetParasPulse4Mm(m_pAxisConfig->GetDouble(AxisKeyName, _T("Pulse4Mm"), 1000));
//<<
}	

//_____________________________________________________________________________
//
bool CMotion::SetAxisConfig(int axis_no)
{
	m_pAxis[axis_no]->AmpDisable();
	// for Motor Type
	m_pAxis[axis_no]->SetLoopModeType();
	m_pAxis[axis_no]->SetStepMode();
	// General parameter
	m_pAxis[axis_no]->SetStopRate();
	m_pAxis[axis_no]->SetEStopRate();
	m_pAxis[axis_no]->SetErrLimit();
//>> 130219 JSPark
	// Motion Controller
	m_pAxis[axis_no]->SetAxisUse(); //130220 JSPark
	m_pAxis[axis_no]->SetPortNo();
	m_pAxis[axis_no]->SetControlType();
//>> 130317 JSPark
	m_pAxis[axis_no]->SetMotorType();
	m_pAxis[axis_no]->SetPulseMethod();
	m_pAxis[axis_no]->SetLoadRatioType();
//<
	m_pAxis[axis_no]->SetOriginMethod();
	// for Level/Event
	m_pAxis[axis_no]->SetPositiveLevel();
	m_pAxis[axis_no]->SetNegativeLevel();
	m_pAxis[axis_no]->SetHomeLevel();
	m_pAxis[axis_no]->SetAmpEnableLevel();
	m_pAxis[axis_no]->SetAmpfaultlevel();
	m_pAxis[axis_no]->SetAmpFaultResetLevel();
	// (Direction CW 0/CCW 1)
	m_pAxis[axis_no]->SetDirection();
	m_pAxis[axis_no]->SetElectricGear();
	// Software Gear
	m_pAxis[axis_no]->SetMm4Pulse();
	m_pAxis[axis_no]->SetPulse4Mm();
//<<
	m_pAxis[axis_no]->SetPosition(0.0);
	m_pAxis[axis_no]->ResetAmpFault();
	m_pAxis[axis_no]->SetAmpFault();

#ifdef _USE_FASTECH
	m_pAxis[axis_no]->bAllParamSaveRom();
#endif
	return TRUE;
}

bool CMotion::Close()
{
	// Server OFF
	AmpDisableAll();
#ifdef _USE_DNET
	//>> 151102 JSLee
	dnetClosePort(portID);
	dnetClose(portID);
	//<<
#endif
	MMC_INITIALZED = FALSE;
	return TRUE;
}

BOOL CMotion::SetSyncControl(int master, int slave, long gain)
{
	BOOL ret=true;
	ret = m_pAxis[master]->SetSyncControl(master,slave);
	ret = m_pAxis[master]->SetSyncGain(gain);
	return ret;
}

BOOL CMotion::GetSyncControl(int master)
{
	BOOL ret=true;
	ret = m_pAxis[master]->GetSyncControl();
	return ret;
}

BOOL CMotion::RetSyncControl(int master, int slave)
{
	BOOL ret=true;
	ret = m_pAxis[master]->RetSyncControl(master, slave);
	return ret;
}

void CMotion::CreateMotion()
{
	int axis;
	for(axis = 0;axis<MAX_AXIS_NUM;axis++)
	{
		m_pAxis[axis] = new CAxis;
	}
}

void CMotion::DestroyMotion()
{
	int axis;
	for(axis = 0;axis<MAX_AXIS_NUM;axis++)
	{
		delete m_pAxis[axis];
		m_pAxis[axis] = NULL;
	}
}
*/