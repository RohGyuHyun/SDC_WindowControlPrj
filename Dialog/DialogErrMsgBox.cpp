// DialogErrMsgBox.cpp : ���� �����Դϴ�.
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


// CDialogErrMsgBox ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDialogErrMsgBox, CDialog)

CDialogErrMsgBox::CDialogErrMsgBox(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogErrMsgBox::IDD, pParent)
{

}

CDialogErrMsgBox::~CDialogErrMsgBox()
{
}

void CDialogErrMsgBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDS_MESSAGE, m_sMessage);
	DDX_Control(pDX, IDS_ERR_CODE, m_sCode);
	
}


BEGIN_MESSAGE_MAP(CDialogErrMsgBox, CDialog)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDialogErrMsgBox �޽��� ó�����Դϴ�.


BOOL CDialogErrMsgBox::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if(pMsg->message ==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CDialogErrMsgBox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if(bShow)
	{
		//m_btn_msg.SetCaption(m_sMessage);
		MoveWindow(245, 280, 690, 450, TRUE);
	}
	else
	{

	}
	
}

BEGIN_EVENTSINK_MAP(CDialogErrMsgBox, CDialog)
	ON_EVENT(CDialogErrMsgBox, IDB_BUZZER_OFF, DISPID_CLICK, CDialogErrMsgBox::ClickBuzzerOff, VTS_NONE)
	ON_EVENT(CDialogErrMsgBox, IDB_OK, DISPID_CLICK, CDialogErrMsgBox::ClickOk, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogErrMsgBox::ClickBuzzerOff()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	//Buzzer off
	pdlg->m_pDlgManual->WriteOutput(BUZZER_ERROR_SIGNAL, FALSE);
}

void CDialogErrMsgBox::SetMessage(char *code, char *msg)
{
	TCHAR* sData = new TCHAR[512];

	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)msg, -1, sData, 512);
	m_sMessage.SetCaption(sData);

	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)code, -1, sData, 512);
	m_sCode.SetCaption(sData);

	delete []sData;
}

void CDialogErrMsgBox::ClickOk()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->AutoReset();

	CDialog::OnOK();
}


BOOL CDialogErrMsgBox::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	this->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
