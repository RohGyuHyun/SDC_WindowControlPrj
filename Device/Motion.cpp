#include "StdAfx.h"

#include "Motion.h"

CMotion::CMotion(void)
{
	/*
		0 : Inspection_Shuttle (Y)
		1 : LD P&P (X)
	*/
	int axis;

	for(axis = 0; axis < MAX_AXIS_NUM; axis++)
	{
		m_pAxis[axis] = new CAxis;
	}

	SetAxis();
	LoadAxisConfig();
}


CMotion::~CMotion(void)
{
	int axis;
	for(axis = 0;axis<MAX_AXIS_NUM;axis++)
	{
		delete m_pAxis[axis];
		m_pAxis[axis] = NULL;
	}
}

BOOL CMotion::Initialize()
{
	// �� [CAUTION] �Ʒ��� �ٸ� Mot����(��� ��������)�� ����� ��� ��θ� �����Ͻʽÿ�.
	char szFilePath[MAX_CHAR];
	sprintf(szFilePath, "%S", SYSTEM_AXIS_AJINX_PATH);

	
	//++ ������ Mot������ ��������� ��Ǻ����� ���������� �ϰ����� �����մϴ�.
	if(AxmMotLoadParaAll(szFilePath) != AXT_RT_SUCCESS){
		AfxMessageBox(_T("Mot File Not Found."));
	}

	for (int axis_no = 0; axis_no < MAX_AXIS_NUM; axis_no++)
	{
		if (!SetAxisConfig(axis_no))
		{
			AfxMessageBox(_T("SetAxisConfig Failed."));
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CMotion::SaveAxisConfig()
{
	int ret;
	//CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	
	for(int i=0; i < MAX_AXIS_NUM; i++)
	{
		ret = m_pAxis[i]->SaveAxisConfig();
		if(!ret)
			return FALSE;
	}

	return TRUE;
}

BOOL CMotion::LoadAxisConfig()
{
	int ret;
	//CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());

	for(int i=0; i < MAX_AXIS_NUM; i++)
	{
		ret = m_pAxis[i]->LoadAxisConfig();

		m_pAxis[i]->SetAxisAccelUnit();

		if(!ret)
			return FALSE;
	}
	
	return TRUE;
}

void CMotion::SetAxis()
{
	CString strName[MAX_AXIS_NUM] = {_T("AXIS_TRY_SHUTTLE"), _T("AXIS_LD_PNP")};

	for(int i=0; i < MAX_AXIS_NUM; i++)
	{
		m_pAxis[i]->SetAxisNo(i);
		//m_pAxis[i]->SetAxisId(i+1);
		m_pAxis[i]->SetAxisName(strName[i]);
	}
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
		//m_pAxis[i]->ClearState();
		//m_pAxis[i]->ClearAxis();
		//m_pAxis[i]->ResetAmpFault();
		//m_pAxis[i]->SetThreadStarted(FALSE);
		//m_pAxis[i]->SetHomingErr(FALSE);
		//m_pAxis[i]->SetHomeFinished(FALSE);
		m_pAxis[i]->AmpDisable();
	}

	return TRUE;
}

BOOL CMotion::SetAxisConfig(int axis_no)
{
	switch(axis_no)
	{
	case AXIS_TRY_SHUTTLE:
		// for Motor Type
		//m_pAxis[axis_no]->AmpDisable();
		m_pAxis[axis_no]->SetAxisUse(USE);
		m_pAxis[axis_no]->SetMotorType(SERVO_MOTOR);
		m_pAxis[axis_no]->SetOriginMethod();		//���� ����
		m_pAxis[axis_no]->SetProfileMode(SYM_TRAPEZOIDE_MODE);
		m_pAxis[axis_no]->SetAbsMode();
		m_pAxis[axis_no]->SetAccUnit(UNIT_SEC2);

		// for Level/Event
		m_pAxis[axis_no]->SetPositiveLevel(HIGH);
		m_pAxis[axis_no]->SetNegativeLevel(HIGH);
		m_pAxis[axis_no]->SetHomeLevel(HIGH);
		m_pAxis[axis_no]->SetAmpEnableLevel(HIGH);
		m_pAxis[axis_no]->SetAmpfaultlevel(0);
		m_pAxis[axis_no]->SetAmpFaultResetLevel(0);
		m_pAxis[axis_no]->SetZPhase(0);

		// (Direction CW 0/CCW 1)
		m_pAxis[axis_no]->SetDirection(4);
		m_pAxis[axis_no]->SetElectricGear();
		m_pAxis[axis_no]->SetEncoderInput(7);

		// Software Gear
		m_pAxis[axis_no]->SetMm4Pulse(0.001);
		m_pAxis[axis_no]->SetPulse4Mm(1000);
		//m_pAxis[axis_no]->SetPosition(0.0);

		m_pAxis[axis_no]->ResetAmpFault();
		m_pAxis[axis_no]->AmpEnable();

		m_pAxis[axis_no]->SetInPosition(HIGH);

		break;
	case AXIS_LD_PNP:
		// for Motor Type
		//m_pAxis[axis_no]->AmpDisable();
		//m_pAxis[axis_no]->AmpEnable();
		m_pAxis[axis_no]->SetAxisUse(USE);
		m_pAxis[axis_no]->SetMotorType(SERVO_MOTOR);
		m_pAxis[axis_no]->SetOriginMethod();		//���� ����
		m_pAxis[axis_no]->SetProfileMode(SYM_TRAPEZOIDE_MODE);
		m_pAxis[axis_no]->SetAbsMode();
		m_pAxis[axis_no]->SetAccUnit(UNIT_SEC2);

		// for Level/Event
		m_pAxis[axis_no]->SetPositiveLevel(0);
		m_pAxis[axis_no]->SetNegativeLevel(0);
		m_pAxis[axis_no]->SetHomeLevel(HIGH);
		m_pAxis[axis_no]->SetAmpEnableLevel(HIGH);
		m_pAxis[axis_no]->SetAmpfaultlevel(0);
		m_pAxis[axis_no]->SetAmpFaultResetLevel(0);
		m_pAxis[axis_no]->SetZPhase(0);

		// (Direction CW 0/CCW 1)
		m_pAxis[axis_no]->SetDirection(4);
		m_pAxis[axis_no]->SetElectricGear();
		m_pAxis[axis_no]->SetEncoderInput(7);

		// Software Gear
		m_pAxis[axis_no]->SetMm4Pulse(0.001);
		m_pAxis[axis_no]->SetPulse4Mm(1000);
		//m_pAxis[axis_no]->SetPosition(0.0);
		m_pAxis[axis_no]->ResetAmpFault();
		m_pAxis[axis_no]->AmpEnable();

		m_pAxis[axis_no]->SetInPosition(HIGH);

		break;
	}
	return TRUE;
}
