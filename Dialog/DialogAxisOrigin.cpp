// DialogAxisOrigin.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SDC_WindowControlPrj.h"

#include "MsgBox.h"									//����

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


// CDialogAxisOrigin ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDialogAxisOrigin, CDialog)

CDialogAxisOrigin::CDialogAxisOrigin(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogAxisOrigin::IDD, pParent)
{
	m_nProcessMoveIdx = 0;
	m_iAxisNum = 0;
	m_bClickSelectAll = FALSE;

	for(int i = 0; i < MAX_AXIS_NUM; i++)
		m_bClickAxis[i] = FALSE;
}

CDialogAxisOrigin::~CDialogAxisOrigin()
{
}

void CDialogAxisOrigin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDB_OK, m_ctrlOk);
	DDX_Control(pDX, IDB_CANCEL, m_ctrlCancel);
	DDX_Control(pDX, IDB_BTN_AXIS0, m_ctrlAxis[AXIS_TRY_SHUTTLE]);
	DDX_Control(pDX, IDB_BTN_AXIS1,  m_ctrlAxis[AXIS_LD_PNP]);
	DDX_Control(pDX, IDB_ORIGIN_RUN, m_ctrlOriginRun);
	DDX_Control(pDX, IDB_AMP_ALL_ON, m_ctrlAmpAllOn);
	DDX_Control(pDX, IDB_AMP_ALL_OFF, m_ctrlAmpAllOff);
	DDX_Control(pDX, IDC_BTNENHCTRL_PROGRESS_ORIGIN, m_ctrl_progress);
	DDX_Control(pDX, IDB_SELECT_ALL_AXIS, m_ctrlSelectAll);
	
}


BEGIN_MESSAGE_MAP(CDialogAxisOrigin, CDialog)
	ON_WM_TIMER()
//	ON_WM_SHOWWINDOW()
ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDialogAxisOrigin �޽��� ó�����Դϴ�.
BEGIN_EVENTSINK_MAP(CDialogAxisOrigin, CDialog)
	ON_EVENT(CDialogAxisOrigin, IDB_ORIGIN_RUN, DISPID_CLICK, OnClickOriginRun, VTS_NONE)
	ON_EVENT(CDialogAxisOrigin, IDB_OK, DISPID_CLICK, CDialogAxisOrigin::ClickOk, VTS_NONE)
	ON_EVENT(CDialogAxisOrigin, IDB_AMP_ALL_OFF, DISPID_CLICK, OnClickAmpAllOff, VTS_NONE)
	ON_EVENT(CDialogAxisOrigin, IDB_AMP_ALL_ON, DISPID_CLICK, OnClickAmpAllOn, VTS_NONE)
	ON_EVENT(CDialogAxisOrigin, IDB_CANCEL, DISPID_CLICK, CDialogAxisOrigin::ClickCancel, VTS_NONE)
	ON_EVENT(CDialogAxisOrigin, IDB_SELECT_ALL_AXIS, DISPID_CLICK, CDialogAxisOrigin::ClickSelectAllAxis, VTS_NONE)
END_EVENTSINK_MAP()


BOOL CDialogAxisOrigin::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
	
	m_ctrlAxis[AXIS_TRY_SHUTTLE].SetCaption(_T("AXIS_TRY_SHUTTLE"));
	m_ctrlAxis[AXIS_LD_PNP].SetCaption(_T("AXIS_LD_PNP"));

	int i;
	for(i=0; i<MAX_AXIS_NUM; i++)
	{
		if(pdlg->m_pDlgManual->AxisIsHomeFinished(i))
		{
			if(pdlg->m_pDlgManual->AxisGetAmpFaultState(i))
			{
				m_ctrlAxis[i].SetBackColorInterior(GREEN);
			}
			else
			{
				// Amp Fault �����̸� ��ư ���� Red�� ����
				//m_ctrlAxis[i].SetBackColorInterior(RED);
				m_ctrlAxis[i].SetValue(FALSE);		
			}
		}
		else
		{
			m_ctrlAxis[i].SetBackColorInterior(0x00FFFFFF);
			m_ctrlAxis[i].SetValue(FALSE);		
		}
	}

	for(i=0; i<MAX_AXIS_NUM; i++)
	{
		if(!pdlg->m_pDlgManual->AxisGetAmpOnState(i))
		{
			m_bAmpOnFlag = FALSE;
			break;
		}
		else
			m_bAmpOnFlag = TRUE;
	}

	
	m_ctrl_progress.SetProgressBarMin(0);
	m_ctrl_progress.SetProgressBarMax(100);
	m_ctrl_progress.SetProgressBarValue(m_nProcessMoveIdx);
	m_ctrl_progress.SetCaption(_T(" "));
	m_ctrl_progress.ShowWindow(FALSE);
	
	SetTimer(TIMER_AXIS_STATE, 500, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CDialogAxisOrigin::OnClickAmpAllOff()
{
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	if(g_cMsgBox.YesNoMsgBox(_T("Really Off The All Axis Amp?")) == IDNO)
		return;

	pdlg->m_pDlgManual->MotAmpDisableAll();

	m_ctrlOk.SetEnabled(TRUE);
	m_ctrlOriginRun.SetEnabled(TRUE);
	m_ctrlCancel.SetEnabled(FALSE);

	for(int i=0; i < MAX_AXIS_NUM; i++)
	{
		pdlg->m_pDlgManual->AxisSetHomeFinished(i, FALSE);
		pdlg->m_pDlgManual->AxisSetOriginReturn(i, FALSE);
		m_ctrlAxis[i].SetValue(FALSE);
		m_bClickAxis[i] = FALSE;
	}

	m_bAmpOnFlag = FALSE;
}

void CDialogAxisOrigin::OnClickAmpAllOn() 
{
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->m_pDlgManual->MotAmpEnableAll();
	pdlg->m_pDlgManual->MotInitialize();
	
	m_bAmpOnFlag = TRUE;
}

void CDialogAxisOrigin::OnClickOriginRun()
{
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
	int i;
	CString strTemp;
	CString strMsg;

	if(!m_bAmpOnFlag)
	{
		g_cMsgBox.OkMsgBox(_T("AMP�� OFF �����Դϴ�."));
		return;
	}

	if(!pdlg->m_pDlgManual->ReadInput(SERVO_MC_ON_OFF_CHECK))
	{
		g_cMsgBox.OkMsgBox(_T("Servo MC Error! Reset ��ư�� 1�ʰ� �����ּ���"));
		return;
	}

	if(!pdlg->m_pDlgManual->ReadInput(LD_PNP_CYLINDER_UP_CHECK))
	{
		pdlg->m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_UP_SOL, TRUE);
		pdlg->m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_DN_SOL, FALSE);
	}

	//���� ����
	if(g_cMsgBox.YesNoMsgBox(_T("���� �۾��� ���� �մϱ�?")) == IDNO)
		return;
	
	for( i=0; i<MAX_AXIS_NUM; i++ )
	{
		if( m_ctrlAxis[i].GetValue() )
		{
			m_bClickAxis[i] = TRUE;

			m_iAxisNum++;
			// Amp Fault ���� Msg ǥ�� �� �� �� ��ȣ�� ǥ�� �߰�
			if( !pdlg->m_pDlgManual->AxisGetAmpFaultState(i) )
			{
				strMsg.Format(_T("The %d-axis is amp fault"), i);
				g_cMsgBox.OkMsgBox(strMsg);
				return;
			}
			/*
			if(pdlg->m_pDlgManual->AxisIsHomeFinished(i))
			{
				strMsg.Format(_T("%s �� ���� �Ϸ��."),  pdlg->m_pDlgManual->AxisGetAxisName(i));
				g_cMsgBox.OkMsgBox(strMsg);
				return;
			}
			*/
			pdlg->m_pDlgManual->MotSetOriginMethod(i);
			pdlg->m_pDlgManual->MotStartOrigin(i);

		}
	}
	
	// Select Axis Check
	if( m_iAxisNum <= 0 )
	{
		g_cMsgBox.OkMsgBox(_T("���� ���õǾ� ���� �ʽ��ϴ�."));
		return;
	}

	SetTimer(TIMER_ORIGIN_DLG, 200, NULL);

	m_ctrlOk.SetEnabled(FALSE);
	m_ctrlOriginRun.SetEnabled(FALSE);
	m_ctrlCancel.SetEnabled(TRUE);

	//strTemp.Format(_T("%s ���� �۾� ��.."), pdlg->m_pDlgManual->AxisGetAxisName(m_iAxisNum));
	strTemp.Format(_T("���� �۾� ��.."));
	m_ctrl_progress.SetCaption(strTemp);
	m_ctrl_progress.ShowWindow(TRUE);

}


void CDialogAxisOrigin::ClickOk()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	KillTimer(TIMER_AXIS_STATE);

	CDialog::OnOK();
}

void CDialogAxisOrigin::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
	DWORD dwRslt;
	CString strKMsg;
	int i;

	switch(nIDEvent)
	{
	case TIMER_AXIS_STATE:
		
		for(i=0; i < MAX_AXIS_NUM; i++)
		{
			// Amp Fault �����̸� ��ư ���� Red�� ����
			if(!pdlg->m_pDlgManual->AxisGetAmpFaultState(i))
			{
				if(m_ctrlAxis[i].GetBackColorInterior() != RED)
					m_ctrlAxis[i].SetBackColorInterior(RED);
			}
			// Amp Off �����̸� ��ư ���� Normal�� ����
			
			if(!pdlg->m_pDlgManual->AxisGetAmpOnState(i))
			{
				if(m_ctrlAxis[i].GetBackColorInterior() != 0x00FFFFFF)
					m_ctrlAxis[i].SetBackColorInterior(0x00FFFFFF);
			}
			else
			{
				// Amp On �����̸� ��ư ���� Blue�� ����
				if(m_ctrlAxis[i].GetBackColorInterior() != LTSKYBLUE)
					m_ctrlAxis[i].SetBackColorInterior(LTSKYBLUE);
			}
			// Origin Check -> Green
			if(!pdlg->m_pDlgManual->AxisIsHomeFinished(i))
			{
				if(m_ctrlAxis[i].GetBackColorInterior() != RED)
					m_ctrlAxis[i].SetBackColorInterior(RED);
			}
			// ETC
			else
			{
				if(m_ctrlAxis[i].GetBackColorInterior() != GREEN)
					m_ctrlAxis[i].SetBackColorInterior(GREEN);
			}
		}

		if(m_bAmpOnFlag)
		{
			if(m_ctrlAmpAllOn.GetBackColorInterior() != YELLOW)
			{
				m_ctrlAmpAllOn.SetBackColorInterior(YELLOW);
				m_ctrlAmpAllOff.SetBackColorInterior(0x00FFFFFF);
			}
		}
		else
		{
			if(m_ctrlAmpAllOff.GetBackColorInterior() != YELLOW)
			{
				m_ctrlAmpAllOn.SetBackColorInterior(0x00FFFFFF);
				m_ctrlAmpAllOff.SetBackColorInterior(YELLOW);
			}
		}
		break;
	case TIMER_ORIGIN_DLG:		
		for(i = 0; i < MAX_AXIS_NUM; i++)
		{
			if(m_bClickAxis[i])
			{
				if(!pdlg->m_pDlgManual->AxisIsHomeFinished(i))
				{
					dwRslt = pdlg->m_pDlgManual->MotReadOriginResult(i);

					if(dwRslt == HOME_SUCCESS)
					{
						pdlg->m_pDlgManual->AxisSetOriginReturn(i, TRUE);
						pdlg->m_pDlgManual->AxisSetPosition(i, 0.0);

						if(m_ctrlAxis[i].GetBackColorInterior() != GREEN)
							m_ctrlAxis[i].SetBackColorInterior(GREEN);

						if(1 == m_iAxisNum)
						{
							KillTimer(TIMER_ORIGIN_DLG);

							m_ctrlOk.SetEnabled(TRUE);
							m_ctrlOriginRun.SetEnabled(TRUE);
							m_ctrlCancel.SetEnabled(FALSE);

							m_nProcessMoveIdx = 0;
							m_ctrl_progress.SetProgressBarValue(m_nProcessMoveIdx);
							m_ctrl_progress.ShowWindow(FALSE);

							strKMsg.Format(_T("%s�� ���� �Ϸ�"), pdlg->m_pDlgManual->AxisGetAxisName(i));
							g_cMsgBox.OkMsgBox(strKMsg);
						}
						break;
					}
					else if(dwRslt == HOME_SEARCHING)
					{
						if(m_nProcessMoveIdx >= 100)
							m_nProcessMoveIdx = 0;
						m_nProcessMoveIdx++;
						m_ctrl_progress.SetProgressBarValue(m_nProcessMoveIdx);
			
					}
					else if(dwRslt == HOME_ERR_NOT_DETECT)
					{
						KillTimer(TIMER_ORIGIN_DLG);

						m_ctrlOk.SetEnabled(TRUE);
						m_ctrlOriginRun.SetEnabled(TRUE);
						m_ctrlCancel.SetEnabled(FALSE);

						m_nProcessMoveIdx = 0;
						m_ctrl_progress.SetProgressBarValue(m_nProcessMoveIdx);
						m_ctrl_progress.ShowWindow(FALSE);

						pdlg->m_pDlgManual->AxisSetOriginReturn(i, FALSE);
						strKMsg.Format(_T("%s�� HOME_ERR_NOT_DETECT"), pdlg->m_pDlgManual->AxisGetAxisName(i));
						g_cMsgBox.OkMsgBox(strKMsg);
						break;
					}
					else if(dwRslt == HOME_ERR_NEG_LIMIT)
					{
						KillTimer(TIMER_ORIGIN_DLG);

						m_ctrlOk.SetEnabled(TRUE);
						m_ctrlOriginRun.SetEnabled(TRUE);
						m_ctrlCancel.SetEnabled(FALSE);

						m_nProcessMoveIdx = 0;
						m_ctrl_progress.SetProgressBarValue(m_nProcessMoveIdx);
						m_ctrl_progress.ShowWindow(FALSE);

						pdlg->m_pDlgManual->AxisSetOriginReturn(i, FALSE);
						strKMsg.Format(_T("%s�� HOME_ERR_NEG_LIMIT"), pdlg->m_pDlgManual->AxisGetAxisName(i));
						g_cMsgBox.OkMsgBox(strKMsg);
						break;
					}
					else if(dwRslt == HOME_ERR_POS_LIMIT)
					{
						KillTimer(TIMER_ORIGIN_DLG);

						m_ctrlOk.SetEnabled(TRUE);
						m_ctrlOriginRun.SetEnabled(TRUE);
						m_ctrlCancel.SetEnabled(FALSE);

						m_nProcessMoveIdx = 0;
						m_ctrl_progress.SetProgressBarValue(m_nProcessMoveIdx);
						m_ctrl_progress.ShowWindow(FALSE);

						pdlg->m_pDlgManual->AxisSetOriginReturn(i, FALSE);
						strKMsg.Format(_T("%s�� HOME_ERR_POS_LIMIT"), pdlg->m_pDlgManual->AxisGetAxisName(i));
						g_cMsgBox.OkMsgBox(strKMsg);
						break;			
					}
					else if(dwRslt == HOME_ERR_AMP_FAULT)
					{
						KillTimer(TIMER_ORIGIN_DLG);

						m_ctrlOk.SetEnabled(TRUE);
						m_ctrlOriginRun.SetEnabled(TRUE);
						m_ctrlCancel.SetEnabled(FALSE);

						m_nProcessMoveIdx = 0;
						m_ctrl_progress.SetProgressBarValue(m_nProcessMoveIdx);
						m_ctrl_progress.ShowWindow(FALSE);

						pdlg->m_pDlgManual->AxisSetOriginReturn(i, FALSE);
						strKMsg.Format(_T("%s�� HOME_ERR_AMP_FAULT"), pdlg->m_pDlgManual->AxisGetAxisName(i));
						g_cMsgBox.OkMsgBox(strKMsg);
						break;
					}
					else if(dwRslt == HOME_ERR_TIMEOUT)
					{
						KillTimer(TIMER_ORIGIN_DLG);

						m_ctrlOk.SetEnabled(TRUE);
						m_ctrlOriginRun.SetEnabled(TRUE);
						m_ctrlCancel.SetEnabled(FALSE);

						m_nProcessMoveIdx = 0;
						m_ctrl_progress.SetProgressBarValue(m_nProcessMoveIdx);
						m_ctrl_progress.ShowWindow(FALSE);

						pdlg->m_pDlgManual->AxisSetOriginReturn(i, FALSE);
						strKMsg.Format(_T("%s�� HOME_ERR_TIMEOUT"), pdlg->m_pDlgManual->AxisGetAxisName(i));
						g_cMsgBox.OkMsgBox(strKMsg);
						break;
					}
					else if(dwRslt == HOME_ERR_USER_BREAK)
					{
						KillTimer(TIMER_ORIGIN_DLG);

						m_ctrlOk.SetEnabled(TRUE);
						m_ctrlOriginRun.SetEnabled(TRUE);
						m_ctrlCancel.SetEnabled(FALSE);

						m_nProcessMoveIdx = 0;
						m_ctrl_progress.SetProgressBarValue(m_nProcessMoveIdx);
						m_ctrl_progress.ShowWindow(FALSE);

						pdlg->m_pDlgManual->AxisSetOriginReturn(i, FALSE);
						strKMsg.Format(_T("%s�� HOME_ERR_USER_BREAK"), pdlg->m_pDlgManual->AxisGetAxisName(i));
						g_cMsgBox.OkMsgBox(strKMsg);
						break;
					}
					else if(dwRslt == HOME_ERR_UNKNOWN)
					{
						KillTimer(TIMER_ORIGIN_DLG);

						m_ctrlOk.SetEnabled(TRUE);
						m_ctrlOriginRun.SetEnabled(TRUE);
						m_ctrlCancel.SetEnabled(FALSE);

						m_nProcessMoveIdx = 0;
						m_ctrl_progress.SetProgressBarValue(m_nProcessMoveIdx);
						m_ctrl_progress.ShowWindow(FALSE);

						pdlg->m_pDlgManual->AxisSetOriginReturn(i, FALSE);
						strKMsg.Format(_T("%s�� HOME_ERR_UNKNOWN"), pdlg->m_pDlgManual->AxisGetAxisName(i));
						g_cMsgBox.OkMsgBox(strKMsg);
						break;
					}
				}
			}
		}

		if(pdlg->m_pDlgManual->AxisIsHomeFinished(0) && pdlg->m_pDlgManual->AxisIsHomeFinished(1))
		{
			KillTimer(TIMER_ORIGIN_DLG);

			m_ctrlOk.SetEnabled(TRUE);
			m_ctrlOriginRun.SetEnabled(TRUE);
			m_ctrlCancel.SetEnabled(FALSE);

			m_nProcessMoveIdx = 0;
			m_ctrl_progress.SetProgressBarValue(m_nProcessMoveIdx);
			m_ctrl_progress.ShowWindow(FALSE);

			strKMsg.Format(_T("��� �� ���� �Ϸ�"));
			g_cMsgBox.OkMsgBox(strKMsg);
		}
		break;
	default:

		break;
	}
	
	CDialog::OnTimer(nIDEvent);
}


void CDialogAxisOrigin::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	KillTimer(TIMER_AXIS_STATE);
}

void CDialogAxisOrigin::ClickCancel()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
	KillTimer(TIMER_ORIGIN_DLG);

	m_ctrlOk.SetEnabled(TRUE);
	m_ctrlOriginRun.SetEnabled(TRUE);
	m_ctrlCancel.SetEnabled(FALSE);

	m_nProcessMoveIdx = 0;
	m_ctrl_progress.SetProgressBarValue(m_nProcessMoveIdx);
	m_ctrl_progress.ShowWindow(FALSE);
	
	for(int i = 0; i < MAX_AXIS_NUM; i++)
	{
		pdlg->m_pDlgManual->AxisEStop(i);
		m_bClickAxis[i] = FALSE;
	}

	//strKMsg.Format(_T("%s�� ���� �Ϸ�"), pdlg->m_pDlgManual->AxisGetAxisName(m_iAxisNum));
	//g_cMsgBox.OkMsgBox(strKMsg);
}


void CDialogAxisOrigin::ClickSelectAllAxis()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	int i;
	m_ctrlOriginRun.SetEnabled(FALSE);

	if(!m_bClickSelectAll)
	{
		m_bClickSelectAll = TRUE;

		for(i = 0; i < MAX_AXIS_NUM; i++)
		{
			m_ctrlAxis[i].SetBackColor(RGB(0, 255, 255));
			m_ctrlAxis[i].SetValue(true);
		}

		m_ctrlSelectAll.SetCaption(_T("Select Free"));
		m_ctrlSelectAll.SetBackColorInterior(YELLOW);
	}
	else
	{
		m_bClickSelectAll = FALSE;

		for(i = 0; i < MAX_AXIS_NUM; i++)
		{
			m_ctrlAxis[i].SetBackColor(RGB(255, 255, 255));
			m_ctrlAxis[i].SetValue(false);
		}

		m_ctrlSelectAll.SetCaption(_T("Select All"));
		m_ctrlSelectAll.SetBackColorInterior(0x00FFFFFF);
	}

	m_ctrlOriginRun.SetEnabled(TRUE);
}
