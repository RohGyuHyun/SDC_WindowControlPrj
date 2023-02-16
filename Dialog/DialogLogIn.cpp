// DialogLogIn.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SDC_WindowControlPrj.h"


#include "MsgBox.h"

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


// CDialogLogIn ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDialogLogIn, CDialog)

CDialogLogIn::CDialogLogIn(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogLogIn::IDD, pParent)
{
	m_nSelUser = OPERATOR;
	m_stLogInData.UserName = LOG_IN_NAME_OPERATOR;
	m_stLogInData.Password = "";
}

CDialogLogIn::~CDialogLogIn()
{
}

void CDialogLogIn::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDS_PASSWORD, m_ctrlPassword);
	DDX_Control(pDX, IDB_CANCEL, m_ctrlCancel);
	DDX_Control(pDX, IDB_OK, m_ctrlOk);
	DDX_Control(pDX, IDB_CHANGE_PASSWORD, m_ctrlChangePW);
	DDX_Control(pDX, IDC_USER_NAME, m_ctrlUserName);
	DDX_Control(pDX, IDC_PASSWORD, m_ctrlPW);
	DDX_Control(pDX, IDB_USER_OP, m_ctrlUserOp);
	DDX_Control(pDX, IDB_USER_MAKER, m_ctrlUserMaker);
	DDX_Control(pDX, IDB_USER_ENGINEER, m_ctrlUserEngineer);
}


BEGIN_MESSAGE_MAP(CDialogLogIn, CDialog)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDialogLogIn �޽��� ó�����Դϴ�.
BEGIN_EVENTSINK_MAP(CDialogLogIn, CDialog)
    //{{AFX_EVENTSINK_MAP(CLogInDlg)
	ON_EVENT(CDialogLogIn, IDB_OK, -600 /* Click */, OnClickOk, VTS_NONE)
	ON_EVENT(CDialogLogIn, IDB_CANCEL, -600 /* Click */, OnClickCancel, VTS_NONE)
	ON_EVENT(CDialogLogIn, IDB_CHANGE_PASSWORD, -600 /* Click */, OnClickChangePassword, VTS_NONE)
	ON_EVENT(CDialogLogIn, IDS_PASSWORD, -600 /* Click */, OnClickPassword, VTS_NONE)
	ON_EVENT(CDialogLogIn, IDB_USER_OP, -600 /* Click */, OnClickUserOp, VTS_NONE)
	ON_EVENT(CDialogLogIn, IDB_USER_ENGINEER, -600 /* Click */, OnClickUserEngineer, VTS_NONE)
	ON_EVENT(CDialogLogIn, IDB_USER_MAKER, -600 /* Click */, OnClickUserMaker, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


void CDialogLogIn::OnClickUserMaker()
{
	m_nSelUser = MAKER;
	m_stLogInData.UserName = LOG_IN_NAME_MAKER;	
}


void CDialogLogIn::OnClickUserEngineer() 
{
	m_nSelUser = ENGINEER;
	m_stLogInData.UserName = LOG_IN_NAME_ENGINEER;	
}

void CDialogLogIn::OnClickUserOp() 
{
	m_nSelUser = OPERATOR;
	m_stLogInData.UserName = LOG_IN_NAME_OPERATOR;
}

void CDialogLogIn::OnClickPassword() 
{
	CGetTextDlg dlg(10, "", "Input the Value.", NULL, TRUE);

	if(dlg.DoModal() == IDOK)
	{
		CString strVal = dlg.GetStringValue();

		m_stLogInData.Password = strVal;

		int len = strVal.GetLength();
		
		strVal = _T("");
		for(int i=0; i<len; i++) 
			strVal += _T("*");
		
		m_ctrlPassword.SetCaption(strVal);
	}
}

void CDialogLogIn::OnClickChangePassword() 
{
	if(g_cMsgBox.YesNoMsgBox(_T("��� ��ȣ�� �����Ͻðڽ��ϱ�?")) == IDNO)
		return;

	WriteLog(_T("��й�ȣ ���� ����)"), LOGIN_LOG); 

	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
	
	g_cMsgBox.OkMsgBox(_T("��й�ȣ�� �Է� �Ͻʽÿ�."));
	
	CGetTextDlg dlg(10, "", "Input the Value.", NULL, TRUE);
	if(dlg.DoModal() == IDOK)
	{
		CString strPassword;
		switch(m_nSelUser)
		{
		case MAKER:
			strPassword = pdlg->m_pDlgMainData->GetMakerPW();
			break;
		case ENGINEER:
			strPassword = pdlg->m_pDlgMainData->GetEngineerPW();
			break;
		case OPERATOR:
		default:
			strPassword = pdlg->m_pDlgMainData->GetOperatorPW();
		}

		if(strPassword != dlg.GetStringValue())
		{
			CString strMsg;
			strMsg = m_stLogInData.UserName + _T("Password is wrong.");
			g_cMsgBox.OkMsgBox(strMsg);
			WriteLog(_T("Password change failed. (���� ��й�ȣ ����)"), LOGIN_LOG); 

			return;
		}

		g_cMsgBox.OkMsgBox(_T("���ο� ��й�ȣ�� �Է� �Ͻʽÿ�."));

		CGetTextDlg dlg(10, "", "Input the Value.", NULL, TRUE);
		if(dlg.DoModal() == IDOK)
		{
			CString strPW = dlg.GetStringValue();

			g_cMsgBox.OkMsgBox(_T("��й�ȣ�� Ȯ���� ���� �ٽ� �ѹ� �Է��� �ֽʽÿ�."));

			CGetTextDlg dlg(10, "", "Input the Value.", NULL, TRUE);
			if(dlg.DoModal() == IDOK)
			{
				if(strPW == dlg.GetStringValue())
				{	
					switch(m_nSelUser)
					{
					case MAKER:
						pdlg->m_pDlgMainData->SetMakerPW(dlg.GetStringValue());
						break;
					case ENGINEER:
						pdlg->m_pDlgMainData->SetEngineerPW(dlg.GetStringValue());
						break;
					case OPERATOR:
					default:
						pdlg->m_pDlgMainData->SetOperatorPW(dlg.GetStringValue());
					}

					g_cMsgBox.OkMsgBox(_T("��й�ȣ�� ����Ǿ����ϴ�."));
					WriteLog(_T("��й�ȣ ���� ����"), LOGIN_LOG); 

					pdlg->m_pDlgMainData->SaveSystemData();
				}
				else
				{
					g_cMsgBox.OkMsgBox(_T("��й�ȣ�� ������ �ֽ��ϴ�.!!"));

					WriteLog(_T("��й�ȣ ���� ����(�ű� ��й�ȣ ����"), LOGIN_LOG); 
				}
			}
		}
	}
	
}

void CDialogLogIn::LogInOp()
{
	OnClickUserOp();
	OnClickOk();
}


void CDialogLogIn::OnClickOk()
{
	CSDC_WindowControlPrjDlg *pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	CString strMsg;

	if(GetLogInData().UserName == LOG_IN_NAME_MAKER)
	{
		if(pdlg->m_pDlgMainData->GetMakerPW() != GetLogInData().Password)
		{
			g_cMsgBox.OkMsgBox(_T("Maker ��ȣ ����"));
			WriteLog(_T("Maker ��ȣ ����"), LOGIN_LOG); 
			return;
		}
		
		pdlg->SetOperator(false);
		pdlg->SetEngineer(false);
		pdlg->SetMaker(true);

		WriteLog(_T("Maker �α��� ����"), LOGIN_LOG); 
	}
	else if(GetLogInData().UserName == LOG_IN_NAME_ENGINEER)
	{
		if(pdlg->m_pDlgMainData->GetEngineerPW() != GetLogInData().Password)
		{
			g_cMsgBox.OkMsgBox(_T("Engineer ��ȣ ����."));
			WriteLog(_T("Engineer ��ȣ ����"), LOGIN_LOG); 
			return;
		}
		
		pdlg->SetOperator(false);
		pdlg->SetEngineer(true);
		pdlg->SetMaker(false);
		
		WriteLog(_T("Engineer �α��� ����"), LOGIN_LOG); 
	}
	else
	{
		
		pdlg->SetOperator(true);
		pdlg->SetEngineer(false);
		pdlg->SetMaker(false);
		
		WriteLog(_T("Operator �α��� ����"), LOGIN_LOG); 
	}

	pdlg->m_pDlgMainData->SetCurUserName(GetLogInData().UserName);
	pdlg->m_pDlgMainData->SetCurUserPW(GetLogInData().Password);
	pdlg->SetLogIn();

	if(pdlg->m_pDlgTeach->IsWindowVisible())
		pdlg->SwitchingView(TEACHVIEW);

	ShowWindow(SW_HIDE);
}

void CDialogLogIn::OnClickCancel()
{
	ShowWindow(SW_HIDE);
}

void CDialogLogIn::DispUserName()
{
	CSDC_WindowControlPrjDlg *pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	m_ctrlPassword.SetCaption(_T(""));
	
	if(pdlg->IsMaker())
	{
		m_nSelUser = MAKER;
		m_ctrlUserMaker.SetValue(TRUE);
	}
	else if(pdlg->IsEngineer())
	{
		m_nSelUser = ENGINEER;
		m_ctrlUserEngineer.SetValue(TRUE);
	}
	else
	{
		m_nSelUser = OPERATOR;
		m_ctrlUserOp.SetValue(TRUE);
	}
	
}

void CDialogLogIn::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if(bShow && nStatus == 0)	
	{
		m_stLogInData.Password = "";
		m_stLogInData.ID = "";

		m_ctrlOk.SetCaption(_T("����"));
		m_ctrlCancel.SetCaption(_T("���"));
		m_ctrlChangePW.SetCaption(_T("PW ����"));
		m_ctrlPW.SetCaption(_T("��ȣ"));
		m_ctrlUserName.SetCaption(_T("�����"));
	
	}

	DispUserName();	
}
