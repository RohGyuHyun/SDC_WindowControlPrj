// DialogMainCOM.cpp : ���� �����Դϴ�.
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


// CDialogMainCOM ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDialogMainCOM, CDialog)

CDialogMainCOM::CDialogMainCOM(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMainCOM::IDD, pParent)
	, m_strPacket(_T(""))
{

}

CDialogMainCOM::~CDialogMainCOM()
{
}

void CDialogMainCOM::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VISION1_IP, m_ctrlSocketVisionIP);
	DDX_Control(pDX, IDC_VISION1_PORT, m_ctrlSocketVisionPCPortNum);
	DDX_Control(pDX, IDC_VISION1_DISPLAY_CONNECT,	m_btnSocketVisionPCConnect);

	/*
	DDX_Control(pDX, IDC_VISION1_PORT,				m_lblSocketDataPCPortNum);
	DDX_Control(pDX, IDC_VISION2_PORT,				m_lblSocketAlignPortNum);
	DDX_Control(pDX, IDC_VISION1_IP,				m_ctrlSocketDataPCIP);
	DDX_Control(pDX, IDC_VISION2_IP,				m_ctrlSocketAlignPCIP);
	DDX_Control(pDX, IDC_VISION1_DISPLAY_CONNECT,	m_btnSocketDataPCConnect);
	DDX_Control(pDX, IDC_VISION2_DISPLAY_CONNECT,	m_btnSocketAlignConnect);
	
	DDX_Control(pDX, IDC_VISION1_PARAM1_NAME,		m_ctrlDataName[0]);
	DDX_Control(pDX, IDC_VISION1_PARAM2_NAME,		m_ctrlDataName[1]);
	DDX_Control(pDX, IDC_VISION1_PARAM3_NAME,		m_ctrlDataName[2]);
	DDX_Control(pDX, IDC_VISION1_PARAM4_NAME,		m_ctrlDataName[3]);
	DDX_Control(pDX, IDC_VISION1_PARAM5_NAME,		m_ctrlDataName[4]);
	DDX_Control(pDX, IDC_VISION1_PARAM6_NAME,		m_ctrlDataName[5]);
	DDX_Text(pDX, IDC_VISION1_PARAM1,				m_ctrlDataEdit[0]);
	DDX_Text(pDX, IDC_VISION1_PARAM2,				m_ctrlDataEdit[1]);
	DDX_Text(pDX, IDC_VISION1_PARAM3,				m_ctrlDataEdit[2]);
	DDX_Text(pDX, IDC_VISION1_PARAM4,				m_ctrlDataEdit[3]);
	DDX_Text(pDX, IDC_VISION1_PARAM5,				m_ctrlDataEdit[4]);
	DDX_Text(pDX, IDC_VISION1_PARAM6,				m_ctrlDataEdit[5]);

	*/
	DDX_Control(pDX, IDC_LIST_SEND, m_list_send);
	DDX_Control(pDX, IDC_LIST_RECEIVE, m_list_receive);
	DDX_Text(pDX, IDC_VISION1_PARAM1, m_strPacket);
	DDX_Control(pDX, IDC_VISION1_COMBO,	m_ctrlComboMsg);
}


BEGIN_MESSAGE_MAP(CDialogMainCOM, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_VISION1_COMBO, &CDialogMainCOM::OnCbnSelchangeVision1Combo)
END_MESSAGE_MAP()


// CDialogMainCOM �޽��� ó�����Դϴ�.


BOOL CDialogMainCOM::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	
	CString pack, strTemp;

	m_ctrlComboMsg.ResetContent();

	/*
		CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
		
	//��Ŷ �Ľ�
	//OTM.RECEIVE.GRAB.START.InnerID	�׷� ��ŸƮ ����
	//OTM.RECEIVE.GRAB.END.InnerID		�׷� �Ϸ� ����
	//OTM.SEND.INSPECT.RESULT.bin.InnerID �˻�Ϸ� ��Ŷ
	//	OTM.SEND.SWERROR.Message����	�˻翡�� ��Ŷ
	*/
	//strTemp.Format(_T("MTO.SEND.GRAB.START.123456789012"));
	strTemp.Format(_T("GRAB START"));
	//pack.Format(_T("%c0000%04d%s%c"), STX, strTemp.GetLength(), strTemp, ETX);
	m_ctrlComboMsg.AddString(strTemp);

	//strTemp.Format(_T("MTO.SEND.GRAB.END.123456789012"));
	strTemp.Format(_T("GRAB END"));
	//pack.Format(_T("%c0000%04d%s%c"), STX, strTemp.GetLength(), strTemp, ETX);
	m_ctrlComboMsg.AddString(strTemp);

	m_ctrlComboMsg.SetCurSel(0);

	LV_COLUMN m_lvColumn;

	m_lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	m_lvColumn.fmt=LVCFMT_LEFT;
	m_lvColumn.pszText=_T(" ");
	m_lvColumn.iSubItem=0;
	m_lvColumn.cx=700;
	m_list_send.InsertColumn(0, &m_lvColumn);
	m_list_receive.InsertColumn(0, &m_lvColumn);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CDialogMainCOM::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
	CString strTemp;
	//BYTE fi1, fi2, fi3, fi4;

	if(bShow)
	{
		m_ctrlSocketVisionIP.SetWindowTextW(pdlg->m_pDlgMainData->GetIPAddressVisionPC());
		strTemp.Format(_T("%d"), pdlg->m_pDlgMainData->GetPortNoVisionPC());
		m_ctrlSocketVisionPCPortNum.SetCaption(strTemp);

		if(pdlg->GetSocketState())
			m_btnSocketVisionPCConnect.SetValue(TRUE);
		else
			m_btnSocketVisionPCConnect.SetValue(FALSE);
	}
	else
	{
		/*
		m_ctrlSocketVisionIP.GetAddress(fi1,fi2,fi3,fi4);
		strTemp.Format(_T("%d.%d.%d.%d"),fi1,fi2,fi3,fi4);

		pdlg->m_pDlgMainData->SetIPAddressVisionPC(strTemp);
		pdlg->m_pDlgMainData->SetPortNoVisionPC(_ttoi(m_ctrlSocketVisionPCPortNum.GetCaption()));
		*/
	}

}


void CDialogMainCOM::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	


}

BEGIN_EVENTSINK_MAP(CDialogMainCOM, CDialog)
	ON_EVENT(CDialogMainCOM, IDB_VISION1_CONNECT, DISPID_CLICK, CDialogMainCOM::ClickVision1Connect, VTS_NONE)
	ON_EVENT(CDialogMainCOM, IDB_VISION1_DISCONNECT, DISPID_CLICK, CDialogMainCOM::ClickVision1Disconnect, VTS_NONE)
	ON_EVENT(CDialogMainCOM, IDB_VISION1_SEND, DISPID_CLICK, CDialogMainCOM::ClickVision1Send, VTS_NONE)
	ON_EVENT(CDialogMainCOM, IDB_VISION1_MSG_SEND_CLEAR, DISPID_CLICK, CDialogMainCOM::ClickVision1MsgSendClear, VTS_NONE)
	ON_EVENT(CDialogMainCOM, IDB_VISION1_MSG_RECV_CLEAR, DISPID_CLICK, CDialogMainCOM::ClickVision1MsgRecvClear, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogMainCOM::ClickVision1Connect()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	//InitPCSocket();
}



void CDialogMainCOM::ClickVision1Disconnect()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


void CDialogMainCOM::ClickVision1Send()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);	
	//��Ŷ �Ľ�
	//OTM.RECEIVE.GRAB.START.InnerID	�׷� ��ŸƮ ����
	//OTM.RECEIVE.GRAB.END.InnerID		�׷� �Ϸ� ����
	//OTM.SEND.INSPECT.RESULT.bin.InnerID �˻�Ϸ� ��Ŷ
	//	OTM.SEND.SWERROR.Message����	�˻翡�� ��Ŷ


	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
	CString strTemp;
	CTime time;
	time = CTime::GetCurrentTime();

 	int cursel = m_ctrlComboMsg.GetCurSel();
	//m_ctrlComboMsg.GetLBText(cursel, m_strPacket);
	switch(cursel)
	{
	case 0:
		strTemp = time.Format(_T("MTO.SEND.GRAB.START.%y%m%d%H%M%S"));
		break;
	case 1:
		strTemp = time.Format(_T("MTO.SEND.GRAB.END.%y%m%d%H%M%S"));
		break;
	}

	m_strPacket.Format(_T("%c0000%04d%s%c"), STX, strTemp.GetLength(), strTemp, ETX);
	pdlg->OnSend(m_strPacket);

	m_list_send.InsertItem(0, m_strPacket);
}

void CDialogMainCOM::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	switch(nIDEvent)
	{
	case TIMER_COM_VISION_CHECK:
		if( pdlg->GetSocketState() )
		{

		}
		else
		{

		}
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CDialogMainCOM::DisplayReceiveData(CString rcv_dat)
{
	m_list_receive.InsertItem(0, rcv_dat);
}


void CDialogMainCOM::ClickVision1MsgSendClear()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	m_list_send.DeleteAllItems();
}


void CDialogMainCOM::ClickVision1MsgRecvClear()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	m_list_receive.DeleteAllItems();
}


void CDialogMainCOM::OnCbnSelchangeVision1Combo()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

}
