// DialogErrMsgBox.cpp : 구현 파일입니다.
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


// CDialogErrMsgBox 대화 상자입니다.

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


// CDialogErrMsgBox 메시지 처리기입니다.


BOOL CDialogErrMsgBox::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->AutoReset();

	CDialog::OnOK();
}


BOOL CDialogErrMsgBox::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	this->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
