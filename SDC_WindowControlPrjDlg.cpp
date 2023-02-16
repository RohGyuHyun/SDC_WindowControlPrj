
// SDC_WindowControlPrjDlg.cpp : 구현 파일
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
#include "DialogMainData.h"r
#include "DialogMainCOM.h"
#include "DialogLogIn.h"
#include "DialogCheckIO.h"
#include "DialogAxisOrigin.h"
#include "DialogAxisManager.h"
#include "DialogAxisJog.h"
#include "DialogAlarmTrendView.h"

#include "SDC_WindowControlPrjDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD _TimerCounter;

//Time
UINT _TimerID;
TIMECAPS _Caps;
BOOL _TimerPost;
void CALLBACK TimerReadFunc(UINT wID,UINT wUser,DWORD dwUser,DWORD dw1,DWORD dw2);

static void CALLBACK TimerReadFunc(UINT wID,UINT wUser,DWORD dwUser,DWORD dw1,DWORD dw2)
{
	if(_TimerPost==FALSE)
	{
		_TimerCounter++;
	}
	else
	{
		AfxMessageBox(_T("Error : high resolution timer event loss!"));
	}	
}



// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSDC_WindowControlPrjDlg 대화 상자


//////////////////////////////////////////////////////////////////////
//Motion Data
//////////////////////////////////////////////////////////////////////
CMotionData::CMotionData()
{
	memset(&m_ProSts, NULL, sizeof(typeMotionProcessSts) * MOT_STATUS_MAX_NUM * MOT_STATUS_SUB_MAX_NUM);
	memset(&m_Seq, NULL, sizeof(typeMotionSeq) * MOT_STATUS_MAX_NUM);

}

CMotionData::~CMotionData()
{

}

void CMotionData::ResetSeq()
{
	memset(&m_Seq, NULL, sizeof(typeMotionSeq) * MOT_STATUS_MAX_NUM);

}

CSDC_WindowControlPrjDlg::CSDC_WindowControlPrjDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSDC_WindowControlPrjDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME1);
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pDlgMainView				= NULL;
	m_pDlgTackTimeView			= NULL;
	m_pDlgProductInfoView		= NULL;
	m_pDlgAlarmTrendView		= NULL;
	m_pDlgLogIn					= NULL;
	m_pDlgManual				= NULL;
	m_pDlgTeach					= NULL;
	m_pDlgMainCOM				= NULL;
	m_pDlgMainData				= NULL;
	
	m_pClient				= NULL;
	m_pClient				= new CClientSock;

	m_nSelectTopTab			= MAIN;
	
	memset(&m_Flag, NULL, sizeof(typeMotionFlag));
	m_Flag.m_bProcessThread = TRUE;

	m_bGrabStart = FALSE;
	m_bGrabEnd	= FALSE;
	m_bInspectEnd = FALSE;
	m_strInnerID.Empty();
	m_nVisionResult = BIN_NONE;

	m_bIsVisionConected = FALSE;

	m_lStartTime = 0;
	m_lEndTime = 0;

	m_lTotalCell = 0;
	m_lPassCell = 0;
	m_lFailCell = 0;
	m_dFailRatioCell = 0.;

	m_bClickDoorLock = FALSE;
}

void CSDC_WindowControlPrjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATE_LABEL, m_ctrlDateLabel);
	DDX_Control(pDX, IDC_TITLE_LABEL, m_ctrlTitleLabel);
	DDX_Control(pDX, IDC_WORK_MODEL_NAME_LABEL, m_ctrlModelNameLabel);
	DDX_Control(pDX, IDC_VERSION_LABEL, m_ctrlVersionLabel);
	DDX_Control(pDX, IDC_BTN_PC_VISION_1, m_ctrlVisionPCStage);
	DDX_Control(pDX, IDC_BTNENHCTRL1, m_ctrlSystemInfo);
	DDX_Control(pDX, IDC_LOG_IN, m_ctrlLogIn);
	DDX_Control(pDX, IDC_MAIN_OP_EQPID, m_ctrlMainOP_EQPID);
	DDX_Control(pDX, IDC_MAIN_OP_MODE, m_ctrlMainOP_Mode);
	DDX_Control(pDX, IDC_MAIN_OP_STATE, m_ctrlMainOP_State);
	DDX_Control(pDX, IDC_MAIN_PRODUCT_TOTAL, m_ctrlMainProduct_Total);
	DDX_Control(pDX, IDC_MAIN_PRODUCT_PASS, m_ctrlMainProduct_Pass);
	DDX_Control(pDX, IDC_MAIN_PRODUCT_FAIL, m_ctrlMainProduct_Fail);
	DDX_Control(pDX, IDC_MAIN_PRODUCT_FAIL_RATIO, m_ctrlMainProduct_Fail_Ratio);
	DDX_Control(pDX, IDB_MAIN_OP_START, m_ctrlOpStart);
	DDX_Control(pDX, IDB_MAIN_OP_STOP, m_ctrlOpStop);
	DDX_Control(pDX, IDB_MAIN_OP_ESTOP, m_ctrlOpReset);
	DDX_Control(pDX, IDB_MAIN_OP_DOOR, m_ctrlOpDoor);
}

BEGIN_MESSAGE_MAP(CSDC_WindowControlPrjDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_MESSAGE(WM_ONCONNECT, OnConnect)
	ON_MESSAGE(WM_RECEIVE_DATA, OnReceive)
	ON_MESSAGE(WM_ONCLOSE, OnClose)
	ON_MESSAGE(USER_MSG_MOT, OnMotMessage)
END_MESSAGE_MAP()


// CSDC_WindowControlPrjDlg 메시지 처리기

BOOL CSDC_WindowControlPrjDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetWindowText(_T("Window Inspection Control"));

	MakeDefaultDir();	//기본 디렉토리 생성


	CreateSheet();//Create SubDialog 

	//Vision연결
	//CreateClientSocket(m_pDlgMainData->GetIPAddressVisionPC(), m_pDlgMainData->GetPortNoVisionPC());
	 CreateClientSocket(_T("127.0.0.1"), 2000);

	//LOGIN

		//high resolution timer for IO reading & PLC init
	//(timer)
	_TimerPost=FALSE;
	timeGetDevCaps(&_Caps,sizeof(_Caps));
	_Caps.wPeriodMin = 1;
	timeBeginPeriod(_Caps.wPeriodMin);
	_TimerID = timeSetEvent(_Caps.wPeriodMin, _Caps.wPeriodMin, (LPTIMECALLBACK)&TimerReadFunc, 0, (UINT)TIME_PERIODIC);
	if(!_TimerID)
	{
		AfxMessageBox(_T("Error : high resolution timer set fail!"));
	}

	//MAIN SEQUENCE THREAD
	m_pThread = AfxBeginThread(MotionThread, this, THREAD_PRIORITY_NORMAL);
	if (m_pThread) m_pThread->m_bAutoDelete = TRUE;
	
	//Main Timer
	SetTimer(TIMER_TOP_DATE, 500, NULL);
	SetTimer(TIMER_MAIN_VIEW, 200, NULL);

	//Set Label
	m_ctrlTitleLabel.SetCaption(SYSTEM_TITLE);
	m_ctrlModelNameLabel.SetCaption(m_pDlgMainData->GetWorkModelName());
	m_ctrlVersionLabel.SetCaption(SYSTEM_VERSION);


	//Set Button
	m_ctrlOpStart.SetEnabled(TRUE);
	m_ctrlOpStart.SetValue(TRUE);
	m_ctrlOpStop.SetEnabled(FALSE);
	m_ctrlOpStop.SetValue(FALSE);
	m_ctrlOpReset.SetEnabled(FALSE);
	m_ctrlOpReset.SetValue(FALSE);

	//Set I/O
	m_pDlgManual->SetLampWait();
	m_pDlgManual->SetLampButton(FALSE);

	if( SYSTEM_MANUAL_MUTING_ON == m_pDlgMainData->GetSystemManualMuting() )
	{
		m_pDlgManual->WriteOutput(LIGHT_CURTAIN_MUTING_ON, FALSE);
	}
	else if( SYSTEM_MANUAL_MUTING_OFF == m_pDlgMainData->GetSystemManualMuting() )
	{
		m_pDlgManual->WriteOutput(LIGHT_CURTAIN_MUTING_ON, TRUE);
	}

	if(m_pDlgManual->ReadOutPut(SAFETY_DOOR_UNLOCK))
	{
		m_bClickDoorLock = FALSE;
		m_pDlgManual->WriteOutput(SAFETY_KEY_LOCK_ON_OFF, FALSE);	// Safety key 잠금	
		m_ctrlOpDoor.SetBackColor(RED);
		m_ctrlOpDoor.SetCaption(_T("DOOR UNLOCK"));
	}
	else
	{
		m_bClickDoorLock = TRUE;

		m_pDlgManual->WriteOutput(SAFETY_KEY_LOCK_ON_OFF, TRUE);	  // Safety key 잠금	
		m_ctrlOpDoor.SetBackColor(GREEN);
		m_ctrlOpDoor.SetCaption(_T("DOOR LOCK"));
	}

	
	WriteLog(_T("Program START"), SYSTEM_LOG);
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CSDC_WindowControlPrjDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSDC_WindowControlPrjDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


//SubDialog Management START//////////////////////////////////////////////////////
//Create Window
void CSDC_WindowControlPrjDlg::CreateSheet()
{
	CRect top_rect, main_rect;
	//SystemParametersInfo(SPI_GETWORKAREA, NULL, &sys_rect, NULL);					//화면 작업영역 사이즈 얻기 (작업표시줄 제외)
	MoveWindow(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)); //화면 최대 해상도로 맞추기

	//TOP MENU
	GetDlgItem(IDC_MAIN_FRM_DISP)->GetWindowRect(&top_rect);

	m_pDlgMainView = new CDialogMainView;
	m_pDlgMainView->Create(IDD_TOP_MAIN_VIEW, this);
	m_pDlgMainView->MoveWindow(&top_rect);
	m_pDlgMainView->ShowWindow(SW_SHOW);

	m_pDlgTackTimeView = new CDialogTackTimeView;
	m_pDlgTackTimeView->Create(IDD_TOP_TACTTIME_VIEW, this);
	m_pDlgTackTimeView->MoveWindow(&top_rect);
	m_pDlgTackTimeView->ShowWindow(SW_HIDE);

	m_pDlgProductInfoView = new CDialogProductInfoView;
	m_pDlgProductInfoView->Create(IDD_TOP_PRODUCTINFO_VIEW, this);
	m_pDlgProductInfoView->MoveWindow(&top_rect);
	m_pDlgProductInfoView->ShowWindow(SW_HIDE);
	
	m_pDlgAlarmTrendView = new CDialogAlarmTrendView;
	m_pDlgAlarmTrendView->Create(IDD_TOP_ALARMTREND_VIEW, this);
	m_pDlgAlarmTrendView->MoveWindow(&top_rect);
	m_pDlgAlarmTrendView->ShowWindow(SW_HIDE);

	m_pDlgLogIn = new CDialogLogIn;
	m_pDlgLogIn->Create(IDD_LOGIN, this);
	m_pDlgLogIn->ShowWindow(SW_HIDE);

	m_dlgLogAlarm = new CDialogErrMsgBox;
	m_dlgLogAlarm->Create(IDD_MSG_ERROR, this);
	m_dlgLogAlarm->ShowWindow(SW_HIDE);


	
	//BOTTOM MENU
	GetDlgItem(IDC_MAIN_FRM_DISP2)->GetWindowRect(&main_rect);

	//DATA
	m_pDlgMainData = new CDialogMainData;
	m_pDlgMainData->Create(IDD_MAIN_DATA, this);
	m_pDlgMainData->MoveWindow(&main_rect);
	m_pDlgMainData->ShowWindow(SW_HIDE);

	//MANUAL
	m_pDlgManual = new CDialogManual;
	m_pDlgManual->Create(IDD_MAIN_MANUAL, this);
	m_pDlgManual->MoveWindow(&main_rect);
	m_pDlgManual->ShowWindow(SW_HIDE);

	//TEACH
	m_pDlgTeach = new CDialogTeach;
	m_pDlgTeach->Create(IDD_MAIN_TEACH, this);
	m_pDlgTeach->MoveWindow(&main_rect);
	m_pDlgTeach->ShowWindow(SW_HIDE);
	
	//COM
	m_pDlgMainCOM = new CDialogMainCOM;
	m_pDlgMainCOM->Create(IDD_MAIN_COM, this);
	m_pDlgMainCOM->MoveWindow(&main_rect);
	m_pDlgMainCOM->ShowWindow(SW_HIDE);


}

//Destroy Window
void CSDC_WindowControlPrjDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

}

//20180904 by
//첫 접속시도를 한다. OnConnect() 메시지를 통해서 접속결과를 알수 있다. 
BOOL CSDC_WindowControlPrjDlg::CreateClientSocket(CString strIP, int nPort)
{
	BOOL rslt = TRUE;

	if( m_pClient->Create() )
	{
		m_pClient->SetWnd(this->m_hWnd);
		m_pClient->Connect(strIP, nPort);
	}
	else
	 rslt = FALSE;
	
	return rslt;
}

void CSDC_WindowControlPrjDlg::OnSend(CString data)
{
	int rslt = 1;
	int size = 0;
	char temp[PACKET_MAX_NUM] = {0, };
	
	sprintf(temp, "%S", data);
	size = strlen(temp);

	//WriteTCPList(temp, FALSE);

	m_pClient->Send((LPSTR)temp, size);

}

//20180904 by
//오버라이딩 된 OnConnect함수를 통해서 접속시도 후 에러코드를 돌려받아 그에 따른 처리를 해준다. 현재는 접속 성공, 연결 거부 등 2가지만 구현되어있음.
//인터넷에 찾아보면 많이 나옴.
LRESULT CSDC_WindowControlPrjDlg::OnConnect(WPARAM wParam, LPARAM lParam)
{
	int err = (int)wParam;

	switch(err)
	{
	case NULL:				//연결 성공
		KillTimer(TIMER_VISION_CONNECT);
		m_bIsVisionConected = TRUE;
		m_ctrlVisionPCStage.SetValue(TRUE);

		WriteLog(_T("Vision connected!"), SYSTEM_LOG);
		break;
	case WSAECONNREFUSED:	//연결 시도가 강제로 거부되었습니다. TIMER 를 동작시켜 재접속시도 한다. 
		SetTimer(TIMER_VISION_CONNECT, 500, NULL);
		
		break;
	}

	return 0;
}

LRESULT CSDC_WindowControlPrjDlg::OnReceive(WPARAM wParam, LPARAM lParam)
{
	char temp[MAX_CHAR];
	CString strRcvData, strTemp, strSend;
	CString strMsgLength, strMsg, strName, strCom, strMethod, strAction, strBin, strInnerID, strErrMsg;
	int nBin = 0;
	wchar_t chETX = ETX;
	
	m_pClient->Receive((LPSTR)temp, MAX_CHAR);

	strRcvData = temp;

	//패킷 파싱
	//OTM.RECEIVE.GRAB.START.InnerID	그랩 스타트 응답
	//OTM.RECEIVE.GRAB.END.InnerID		그랩 완료 응답
	//OTM.SEND.INSPECT.RESULT.bin.InnerID 검사완료 패킷
	//	OTM.SEND.SWERROR.Message내용	검사에러 패킷

	strRcvData.Delete(0, 1);								// STX 제거
	
	strRcvData = strRcvData.Left(strRcvData.Find(chETX));	// 쓰레기 값 처리

	//SOCKET LOG
	WriteLog(strRcvData, PACKET_LOG);
	if(m_pDlgMainCOM->IsWindowVisible())
		m_pDlgMainCOM->DisplayReceiveData(strRcvData);


	strRcvData.Delete(0, 2);								// 예비1 제거
	strRcvData.Delete(0, 2);								// 예비2 제거

	strMsgLength = strRcvData.Left(4);
	strRcvData.Delete(0, 4);								// Message 길이 제거

	strMsg = strRcvData.Left(_ttoi(strMsgLength));		//Message

	AfxExtractSubString(strName, strMsg, 0, '.');
	AfxExtractSubString(strCom, strMsg, 1, '.');
	AfxExtractSubString(strMethod, strMsg, 2, '.');
	
	//From Vision
	if( !strName.Compare(_T("OTM")) )
	{
		if( !strCom.Compare(_T("SEND")) )
		{
			if( !strMethod.Compare(_T("INSPECT")) )		//검사결과
			{
				AfxExtractSubString(strAction, strMsg, 3, '.');

				if( !strAction.Compare(_T("RESULT")) )
				{
					AfxExtractSubString(strBin, strMsg, 4, '.');
					AfxExtractSubString(strInnerID, strMsg, 5, '.');
				
					nBin = _ttoi(strBin);
					m_nVisionResult = nBin;

					//Product result Update
					switch(m_nVisionResult)
					{
					case BIN_OK:
						m_lPassCell++;
						break;
					case BIN_REJECT:
							
						break;
					case BIN_NG:
						m_lFailCell++;
						break;
					}

					m_lTotalCell++;
					m_dFailRatioCell = (m_lFailCell / m_lTotalCell) * 100;

					strTemp.Format(_T("%s,%s,%s,%d"), m_strStartTime, m_strEndTime, m_strInnerID, m_nVisionResult);
					WriteLog(strTemp, RESULT_LOG);

					m_bInspectEnd = TRUE;

					SendMessage(USER_MSG_MOT, 3, 0);
				}
			}
			else if( !strMethod.Compare(_T("SWERROR")) )		//Vision Error
			{
				AfxExtractSubString(strErrMsg, strMsg, 3, '.');
			
				//처리

			}
		}
		else if( !strCom.Compare(_T("RECEIVE")) )
		{
			if( !strMethod.Compare(_T("GRAB")) )
			{
				AfxExtractSubString(strAction, strMsg, 3, '.');
				AfxExtractSubString(strInnerID, strMsg, 4, '.');

				if( !strAction.Compare(_T("START")) )
				{
					//트리거 시작.
					m_bGrabStart = TRUE;

				}
				else if( !strAction.Compare(_T("END")) )
				{
					//응답을 받으면 다음 동작을 수행한다. (T/T 때문에 무시할 수도 있음)
					m_bGrabEnd = TRUE;
				}
			}
		}
	}

	return 0;
}

LRESULT CSDC_WindowControlPrjDlg::OnClose(WPARAM wParam, LPARAM lParam)
{
	m_bIsVisionConected = FALSE;
	m_ctrlVisionPCStage.SetValue(FALSE);

	delete m_pClient;
	m_pClient = NULL;

	WriteLog(_T("Vision disconnected!"), SYSTEM_LOG);

	m_pClient = new CClientSock();
	m_pClient->Create();
	m_pClient->SetWnd(this->m_hWnd);
	SetTimer(TIMER_VISION_CONNECT, 500, NULL);

	return 0;
}

void CSDC_WindowControlPrjDlg::FreeDialog()
{
	m_pDlgMainView->DestroyWindow();
	m_pDlgTackTimeView->DestroyWindow();
	m_pDlgProductInfoView->DestroyWindow();
	m_pDlgAlarmTrendView->DestroyWindow();
	m_pDlgLogIn->DestroyWindow();
	m_pDlgManual->DestroyWindow();
	m_pDlgTeach->DestroyWindow();
	m_pDlgMainCOM->DestroyWindow();
	m_pDlgMainData->DestroyWindow();
}

void CSDC_WindowControlPrjDlg::FreePointerVar()
{
	if(NULL != m_pClient){
		delete m_pClient;
		m_pClient = NULL;
	}

	if(NULL != m_pDlgMainView){
		delete m_pDlgMainView;
		m_pDlgMainView = NULL;
	}

	if(NULL != m_pDlgTackTimeView){
		delete m_pDlgTackTimeView;
		m_pDlgTackTimeView = NULL;
	}

	if(NULL != m_pDlgProductInfoView){
		delete m_pDlgProductInfoView;
		m_pDlgProductInfoView = NULL;
	}

	if(NULL != m_pDlgAlarmTrendView){
		delete m_pDlgAlarmTrendView;
		m_pDlgAlarmTrendView = NULL;
	}

	if(NULL != m_pDlgLogIn){
		delete m_pDlgLogIn;
		m_pDlgLogIn = NULL;
	}

	if(NULL != m_dlgLogAlarm){
		delete m_dlgLogAlarm;
		m_dlgLogAlarm = NULL;
	}

	/*
		m_pDlgManual가 종료된 후에 I/O MOTION 접근에 주의.
	*/
	if(NULL != m_pDlgManual){
		delete m_pDlgManual;
		m_pDlgManual = NULL;
	}

	if(NULL != m_pDlgTeach){
		delete m_pDlgTeach;
		m_pDlgTeach = NULL;
	}

	if(NULL != m_pDlgMainCOM){
		delete m_pDlgMainCOM;
		m_pDlgMainCOM = NULL;
	}

	if(NULL != m_pDlgMainData){
		delete m_pDlgMainData;
		m_pDlgMainData = NULL;	
	}
}
//SubDialog Management END
////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Show Window START
void CSDC_WindowControlPrjDlg::ShowManualOCheck()
{
	CDialogCheckIO dlg;
	dlg.DoModal();
}

void CSDC_WindowControlPrjDlg::ShowManualOrigin()
{
	CDialogAxisOrigin dlg;
	dlg.DoModal();
}

void CSDC_WindowControlPrjDlg::ShowTeachAxisJog()
{
	CDialogAxisJog dlg;
	dlg.DoModal();
}

void CSDC_WindowControlPrjDlg::ShowDataAxisManager()
{
	CDialogAxisManager dlg;
	dlg.DoModal();
}

//Show Window END
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSDC_WindowControlPrjDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BEGIN_EVENTSINK_MAP(CSDC_WindowControlPrjDlg, CDialogEx)
	ON_EVENT(CSDC_WindowControlPrjDlg, IDC_TAB_WORK_INFO, DISPID_CLICK, CSDC_WindowControlPrjDlg::ClickTabWorkInfo, VTS_NONE)
	ON_EVENT(CSDC_WindowControlPrjDlg, IDC_TAB_TACT_INFO, DISPID_CLICK, CSDC_WindowControlPrjDlg::ClickTabWorkInfo, VTS_NONE)
	ON_EVENT(CSDC_WindowControlPrjDlg, IDC_TAB_UI_INFO, DISPID_CLICK, CSDC_WindowControlPrjDlg::ClickTabWorkInfo, VTS_NONE)
	ON_EVENT(CSDC_WindowControlPrjDlg, IDC_TAB_ALARM_TREND, DISPID_CLICK, CSDC_WindowControlPrjDlg::ClickTabWorkInfo, VTS_NONE)
	ON_EVENT(CSDC_WindowControlPrjDlg, IDC_MAINVIEW_BTN, DISPID_CLICK, CSDC_WindowControlPrjDlg::OnClickViewBtn, VTS_NONE)
	ON_EVENT(CSDC_WindowControlPrjDlg, IDC_MANUALMVIEW_BTN, DISPID_CLICK, CSDC_WindowControlPrjDlg::OnClickViewBtn, VTS_NONE)
	ON_EVENT(CSDC_WindowControlPrjDlg, IDC_TEACHVIEW_BTN, DISPID_CLICK, CSDC_WindowControlPrjDlg::OnClickViewBtn, VTS_NONE)
	ON_EVENT(CSDC_WindowControlPrjDlg, IDC_COMVIEW_BTN, DISPID_CLICK, CSDC_WindowControlPrjDlg::OnClickViewBtn, VTS_NONE)
	ON_EVENT(CSDC_WindowControlPrjDlg, IDC_DATAVIEW_BTN, DISPID_CLICK, CSDC_WindowControlPrjDlg::OnClickViewBtn, VTS_NONE)
	ON_EVENT(CSDC_WindowControlPrjDlg, IDC_EXITVIEW_BTN, DISPID_CLICK, CSDC_WindowControlPrjDlg::OnClickViewBtn, VTS_NONE)
	ON_EVENT(CSDC_WindowControlPrjDlg, IDC_LOG_IN, DISPID_CLICK, CSDC_WindowControlPrjDlg::ClickLogIn, VTS_NONE)
	ON_EVENT(CSDC_WindowControlPrjDlg, IDB_MAIN_OP_START, -600 /* Click */, OnClickAutoRun, VTS_NONE)
	ON_EVENT(CSDC_WindowControlPrjDlg, IDB_MAIN_OP_STOP, -600 /* Click */, OnClickAutoRun, VTS_NONE)
	ON_EVENT(CSDC_WindowControlPrjDlg, IDB_MAIN_OP_ESTOP, -600 /* Click */, OnClickAutoRun, VTS_NONE)
	ON_EVENT(CSDC_WindowControlPrjDlg, IDB_MAIN_OP_INIT, DISPID_CLICK, CSDC_WindowControlPrjDlg::ClickMainOpInit, VTS_NONE)
	ON_EVENT(CSDC_WindowControlPrjDlg, IDB_MAIN_OP_DOOR, DISPID_CLICK, CSDC_WindowControlPrjDlg::ClickMainOpDoor, VTS_NONE)
	ON_EVENT(CSDC_WindowControlPrjDlg, IDC_LOGVIEW_BTN, DISPID_CLICK, CSDC_WindowControlPrjDlg::ClickLogviewBtn, VTS_NONE)
	ON_EVENT(CSDC_WindowControlPrjDlg, IDB_MAIN_PRODUCT_RESET, DISPID_CLICK, CSDC_WindowControlPrjDlg::ClickMainProductReset, VTS_NONE)
END_EVENTSINK_MAP()

void CSDC_WindowControlPrjDlg::DisplayPCSoket()
{

}

void CSDC_WindowControlPrjDlg::OnClickSoketRetry()
{
	m_pClient->Connect(m_pDlgMainData->GetIPAddressVisionPC(), m_pDlgMainData->GetPortNoVisionPC());
}

//하단부 메뉴관리 #1
void CSDC_WindowControlPrjDlg::OnClickViewBtn() 
{
	if( m_Flag.m_bAutoStart )
		return;

	CString strData;
	CGetTextDlg dlg;

	int nBtnID, nViewID = 0;
	CBtnEnh *pBtnEnh = (CBtnEnh*)GetFocus();
	nBtnID = pBtnEnh->GetDlgCtrlID();


	if( (m_nOldViewCtrl == nBtnID) && 
		(IDC_EXITVIEW_BTN != m_nOldViewCtrl) && 
		(IDC_LOGVIEW_BTN != m_nOldViewCtrl)	)
	{
		return; 
	}

	m_nOldViewCtrl = nBtnID;

	switch(nBtnID)
	{
	case IDC_MAINVIEW_BTN:
		nViewID = MAINVIEW;
		break;
	case IDC_MANUALMVIEW_BTN:
		nViewID = MANUALVIEW;
		SetFirstStart(FALSE);
		break;
	case IDC_TEACHVIEW_BTN:
		nViewID = TEACHVIEW;
		SetFirstStart(FALSE);
		break;
	case IDC_DATAVIEW_BTN:
		nViewID = DATAVIEW;
		SetFirstStart(FALSE);
		break;
	case IDC_COMVIEW_BTN:
		nViewID = COMVIEW;
		SetFirstStart(FALSE);
		break;
	case IDC_EXITVIEW_BTN:
		if(!m_pDlgManual->ReadInput(SAFETY_KEY_AUTO_CHECK))	//Teach 모드가 아니라면 리턴
		{
			g_cMsgBox.OkMsgBox(_T("Safety Mode 를 Teach Mode 로 전환 후 종료해주세요."));
			return;
		}

 		if(g_cMsgBox.YesNoMsgBox(_T("프로그램을 종료 하시겠습니까? ")) == IDNO)
 			return;

		KillTimer(TIMER_TOP_DATE);
		KillTimer(TIMER_MAIN_VIEW);
		
		m_Flag.m_bProcessThread = FALSE;
		m_pClient->Close();

		m_pDlgManual->SetLampOff();
		m_pDlgManual->SetLampButton(FALSE);

		m_pDlgManual->WriteOutput(SAFETY_KEY_LOCK_ON_OFF, FALSE);
		m_pDlgManual->WriteOutput(SAFETY_DOOR_UNLOCK, TRUE);

		m_pDlgManual->WriteOutput(LIGHT_CURTAIN_MUTING_ON, TRUE);

		m_pDlgManual->WriteOutput(LD_PNP_VAC_ON_SOL, FALSE);
		m_pDlgManual->WriteOutput(LD_PNP_VAC_OFF_SOL, TRUE);
		Delay(30);
		m_pDlgManual->WriteOutput(LD_PNP_VAC_OFF_SOL, FALSE);

		m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_UP_SOL, TRUE);
		m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_DN_SOL, FALSE);

		m_pDlgManual->WriteOutput(FILM_JIG_VAC_ON_SOL, FALSE);
		m_pDlgManual->WriteOutput(FILM_JIG_VAC_OFF_SOL, TRUE);
		Delay(30);
		m_pDlgManual->WriteOutput(FILM_JIG_VAC_OFF_SOL, FALSE);

		m_pDlgManual->WriteOutput(FILM_JIG_CYLINDER_IN_SOL, FALSE);
		m_pDlgManual->WriteOutput(FILM_JIG_CYLINDER_OUT_SOL, TRUE);

		m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_FEED_SOL, FALSE);
		m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_BACK_SOL, TRUE);

		m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_CLAMP_SOL, FALSE);
		m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_UNCLAMP_SOL, TRUE);

		m_pDlgManual->WriteOutput(FILM_JIG_TILTING_CYLINDER_TILT_UP_SOL, TRUE);
		m_pDlgManual->WriteOutput(FILM_JIG_TILTING_CYLINDER_TILT_DN_SOL, FALSE);

		m_pDlgManual->WriteOutput(FILM_JIG_VAC_PLATE_CYLINDER_UP_SOL, TRUE);
		m_pDlgManual->WriteOutput(FILM_JIG_VAC_PLATE_CYLINDER_DN_SOL, FALSE);

		m_pDlgManual->WriteOutput(GLASS_JIG_VAC_ON_SOL, FALSE);
		m_pDlgManual->WriteOutput(GLASS_JIG_VAC_OFF_SOL, TRUE);
		Delay(30);
		m_pDlgManual->WriteOutput(GLASS_JIG_VAC_OFF_SOL, FALSE);

		m_pDlgManual->WriteOutput(TRAY_SHUTTLE_GLASS_VAC_ON_SOL, FALSE);
		m_pDlgManual->WriteOutput(TRAY_SHUTTLE_GLASS_VAC_OFF_SOL, TRUE);
		Delay(30);
		m_pDlgManual->WriteOutput(TRAY_SHUTTLE_GLASS_VAC_OFF_SOL, FALSE);

	
		FreeDialog();
	
		FreePointerVar();

		WriteLog(_T("Program EXIT"), SYSTEM_LOG);
		
		EndDialog(NULL);			// Exit program
		return;
	default:
		g_cMsgBox.OkMsgBox(_T("Linked View don't exit"));
		return;
	}

	SwitchingView(nViewID);

}

//하단부 메뉴관리 #2
void CSDC_WindowControlPrjDlg::SwitchingView(UINT nID)
{
	if(m_Flag.m_bAutoStart)
		return;

	if(!m_pDlgManual->ReadInput(SAFETY_KEY_AUTO_CHECK))	//Teach 모드가 아니라면 리턴
	{
		g_cMsgBox.OkMsgBox(_T("Safety key is Auto mode."));

		return;
	}
		

	if(m_pDlgManual->IsWindowVisible())
 		m_pDlgManual->ShowWindow(SW_HIDE);
	if(m_pDlgTeach->IsWindowVisible())
		m_pDlgTeach->ShowWindow(SW_HIDE);
	if(m_pDlgMainCOM->IsWindowVisible())
		m_pDlgMainCOM->ShowWindow(SW_HIDE);
	if(m_pDlgMainData->IsWindowVisible())
		m_pDlgMainData->ShowWindow(SW_HIDE);

	switch(nID)
	{
	case MAINVIEW:
		
		break;	
	case MANUALVIEW:
		m_pDlgManual->ShowWindow(SW_SHOW);
		break;
	case TEACHVIEW:
		m_pDlgTeach->ShowWindow(SW_SHOW);
		break;
	case COMVIEW:
		m_pDlgMainCOM->ShowWindow(SW_SHOW);
		break;
	case DATAVIEW:
		m_pDlgMainData->ShowWindow(SW_SHOW);
		break;

	default:
		ASSERT(0);
		return;
	}
}

//상단부 메뉴관리 #1
void CSDC_WindowControlPrjDlg::ClickTabWorkInfo()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CBtnEnh *pBtnEnh = (CBtnEnh*)GetFocus();
	int nID = pBtnEnh->GetDlgCtrlID();

	switch(nID)
	{
	case IDC_TAB_WORK_INFO:
		m_nSelectTopTab = MAIN;
		break;
	case IDC_TAB_TACT_INFO:
		m_nSelectTopTab = TACTTIME;
		break;
	case IDC_TAB_UI_INFO:
		m_nSelectTopTab = PRODUCT;
		break; 
	case IDC_TAB_ALARM_TREND:
		m_nSelectTopTab = ALARM;
		break; 

	default:

		return;
	}
	
	if(m_nOldSelectTopTab == m_nSelectTopTab) 
		return; //이전 Tab과 동일 

	m_nOldSelectTopTab = m_nSelectTopTab;
	
	ChangeTopTabStatus(TRUE);
}

//상단부 메뉴관리 #2
void CSDC_WindowControlPrjDlg::ChangeTopTabStatus(BOOL bShow)
{
	if(m_pDlgMainView->IsWindowVisible())
 		m_pDlgMainView->ShowWindow(SW_HIDE);
	if(m_pDlgTackTimeView->IsWindowVisible())
		m_pDlgTackTimeView->ShowWindow(SW_HIDE);
	if(m_pDlgProductInfoView->IsWindowVisible())
		m_pDlgProductInfoView->ShowWindow(SW_HIDE);
	if(m_pDlgAlarmTrendView->IsWindowVisible())
		m_pDlgAlarmTrendView->ShowWindow(SW_HIDE);

	if(bShow)
	{
		switch(m_nSelectTopTab)
		{
		case MAIN:
			if(m_pDlgMainView)
				m_pDlgMainView->ShowWindow(SW_SHOW);
			break;
		case TACTTIME:
			if(m_pDlgTackTimeView)
 	 			m_pDlgTackTimeView->ShowWindow(SW_SHOW);
			break;
		case PRODUCT:
			if(m_pDlgProductInfoView)
				m_pDlgProductInfoView->ShowWindow(SW_SHOW);
			break;
		case ALARM:
			if(m_pDlgAlarmTrendView)
				m_pDlgAlarmTrendView->ShowWindow(SW_SHOW);
			break;
		default:
			if(m_pDlgMainView)
		 		m_pDlgMainView->ShowWindow(SW_SHOW);
			break;
		}
	}
}

void CSDC_WindowControlPrjDlg::MakeDefaultDir()
{
	CString strPath;

	CreateDirectory(DATA_SYSTEM_PATH, NULL);
	CreateDirectory(DATA_MODEL_PATH, NULL);
	CreateDirectory(SYSYEM_LOG_PATH, NULL);
	CreateDirectory(LOGIN_LOG_PATH, NULL);
	CreateDirectory(ALARM_LOG_PATH, NULL);
	CreateDirectory(PACKET_LOG_PATH, NULL);
	CreateDirectory(RESULT_LOG_PATH, NULL);
	CreateDirectory(TACTTIME_LOG_PATH, NULL);
}

void CSDC_WindowControlPrjDlg::DisplayProductInfo()
{
	CString strTemp;

	//if(m_Flag.m_bAutoStart  == TRUE)
	//{
		if(_ttoi(m_ctrlMainProduct_Total.GetCaption()) != m_lTotalCell)
		{
			strTemp.Format(_T("%d"), m_lTotalCell);
			m_ctrlMainProduct_Total.SetCaption(strTemp);

		}

		if(_ttoi(m_ctrlMainProduct_Pass.GetCaption()) != m_lPassCell)
		{
			strTemp.Format(_T("%d"), m_lPassCell);
			m_ctrlMainProduct_Pass.SetCaption(strTemp);

		}

		if(_ttoi(m_ctrlMainProduct_Fail.GetCaption()) != m_lFailCell)
		{
			strTemp.Format(_T("%d"), m_lFailCell);
			m_ctrlMainProduct_Fail.SetCaption(strTemp);

		}

		if(_ttof(m_ctrlMainProduct_Total.GetCaption()) != m_dFailRatioCell)
		{
			strTemp.Format(_T("%.2f%%"), m_dFailRatioCell);
			m_ctrlMainProduct_Fail_Ratio.SetCaption(strTemp);

		}
	//}

}

void CSDC_WindowControlPrjDlg::DisplayEQPStatus()
{

	if( SYSTEM_OP_MODE_AUTORUN == m_pDlgMainData->GetSystemOpMode() )
	{
		if(m_ctrlMainOP_Mode.GetCaption() != _T("AUTO RUN"))
			m_ctrlMainOP_Mode.SetCaption(_T("AUTO RUN"));
	}
	else if( SYSTEM_OP_MODE_DRYRUN == m_pDlgMainData->GetSystemOpMode() )
	{
		if(m_ctrlMainOP_Mode.GetCaption() != _T("DRY RUN"))
			m_ctrlMainOP_Mode.SetCaption(_T("DRY RUN"));
	}

	if(m_Flag.m_bAutoStart  == TRUE)
	{
		if( m_Flag.m_bAlarm == TRUE )
		{
			if(m_ctrlMainOP_State.GetCaption() != _T("AUTO ERROR"))
				m_ctrlMainOP_State.SetCaption(_T("AUTO ERROR"));
		}
		else
		{
			if(m_ctrlMainOP_State.GetCaption() != _T("RUN"))
				m_ctrlMainOP_State.SetCaption(_T("RUN"));
		}
	}
	else
	{
		if( m_Flag.m_bAlarm == TRUE )
		{
			if(m_ctrlMainOP_State.GetCaption() != _T("STOP ERROR"))
				m_ctrlMainOP_State.SetCaption(_T("STOP ERROR"));
		}
		else
		{
			if(m_ctrlMainOP_State.GetCaption() != _T("STEP STOP"))
				m_ctrlMainOP_State.SetCaption(_T("STEP STOP"));
		}
	}
}


void CSDC_WindowControlPrjDlg::OnClickAutoRun() 
{
	CBtnEnh *pBtnEnh = (CBtnEnh*)GetFocus();
	BOOL rslt = TRUE; 

	switch(pBtnEnh->GetDlgCtrlID())
	{
	case IDB_MAIN_OP_START:
		if(g_cMsgBox.YesNoMsgBox(_T("Auto Run Start?")) == IDYES)
		{
			m_pDlgLogIn->LogInOp();
			
			for(int i = 0; i < MAX_AXIS_NUM; i++)
			{
				if(!m_pDlgManual->AxisIsHomeFinished(i))
				{
					rslt = FALSE;
				}
			}

			if(!rslt)
			{
				g_cMsgBox.OkMsgBox(_T("축 원점 작업이 완료되어 있지 않습니다."));
				return;
			}

			if( m_pDlgManual->ReadInput(SAFETY_KEY_AUTO_CHECK) )
			{
				g_cMsgBox.OkMsgBox(_T("Safety key is Teach Mode."));
				return;
			}

			AutoStart();

			//Button
			m_ctrlOpStart.SetEnabled(FALSE);
			m_ctrlOpStart.SetValue(FALSE);
			m_ctrlOpStop.SetEnabled(TRUE);
			m_ctrlOpStop.SetValue(TRUE);
			m_ctrlOpReset.SetEnabled(FALSE);
			m_ctrlOpReset.SetValue(FALSE);
			m_ctrlOpDoor.SetEnabled(FALSE);
			//m_ctrlOpDoor.SetValue(FALSE);

			m_pDlgManual->SetLampRun();
		}
		break;
	case IDB_MAIN_OP_STOP:
		{
			AutoStop();

			m_ctrlOpStart.SetEnabled(TRUE);
			m_ctrlOpStart.SetValue(TRUE);
			m_ctrlOpStop.SetEnabled(FALSE);
			m_ctrlOpStop.SetValue(FALSE);
			m_ctrlOpReset.SetEnabled(FALSE);
			m_ctrlOpReset.SetValue(FALSE);
			m_ctrlOpDoor.SetEnabled(TRUE);

			m_pDlgManual->SetLampWait();
			
			break;
		}
	case IDB_MAIN_OP_ESTOP://RESET
		{

			break;
		}
	default:
		return;
	}
}

void CSDC_WindowControlPrjDlg::ClickMainOpDoor()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(!m_pDlgManual->ReadInput(SAFETY_KEY_AUTO_CHECK))	//Teach 모드가 아니라면 리턴
	{
		g_cMsgBox.OkMsgBox(_T("Safety key is Auto mode."));
		return;
	}
		
	if(m_bClickDoorLock)
	{
		if(g_cMsgBox.YesNoMsgBox(_T("DOOR 잠금을 해제합니까?")) == IDYES)
		{
			m_bClickDoorLock = FALSE;

			m_pDlgManual->WriteOutput(SAFETY_DOOR_UNLOCK, TRUE);
			m_pDlgManual->WriteOutput(SAFETY_KEY_LOCK_ON_OFF, FALSE);	// Safety key 잠금	
			m_ctrlOpDoor.SetBackColor(RED);
			m_ctrlOpDoor.SetCaption(_T("DOOR UNLOCK"));
		}
	}
	else
	{
		if(m_pDlgManual->ReadInput(FRONT_LEFT_DOOR_CHECK))
		{
			g_cMsgBox.OkMsgBox(_T("전면 좌측 문 열림!"));
			return;
		}

		if(m_pDlgManual->ReadInput(FRONT_RIGHT_DOOR_CHECK))
		{
			g_cMsgBox.OkMsgBox(_T("전면 우측 문 열림!"));
			return;
		}

		if(m_pDlgManual->ReadInput(REAR_LEFT_DOOR_CHECK))
		{
			g_cMsgBox.OkMsgBox(_T("후면 좌측 문 열림!"));
			return;
		}

		if(m_pDlgManual->ReadInput(REAR_RIGHT_DOOR_CHECK))
		{
			g_cMsgBox.OkMsgBox(_T("후면 우측 문 열림!"));
			return;
		}

		m_bClickDoorLock = TRUE;

		m_pDlgManual->WriteOutput(SAFETY_DOOR_UNLOCK, FALSE);
		m_pDlgManual->WriteOutput(SAFETY_KEY_LOCK_ON_OFF, TRUE);	// Safety key 잠금	
		m_ctrlOpDoor.SetBackColor(GREEN);
		m_ctrlOpDoor.SetCaption(_T("DOOR LOCK"));
	}
}

void CSDC_WindowControlPrjDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//CString strVersion, strSysInfo;
	CTime CurTime;
	CString strCurTime;
	static CTime OldTime;

	switch(nIDEvent)
	{
	case TIMER_TOP_DATE:
		CurTime = CTime::GetCurrentTime();
		if(CurTime.GetSecond() == OldTime.GetSecond())
			break;
		OldTime = CurTime;
		strCurTime = CurTime.Format(_T("[%Y-%m-%d]\n %H:%M:%S %p"));

		m_ctrlDateLabel.SetCaption(strCurTime);
		//DisplayPCSoket();
		break;
	case TIMER_VISION_CONNECT:		//20180904 by 재접속 시도.
		if(!m_bIsVisionConected)
		{
			m_pClient->Connect(m_pDlgMainData->GetIPAddressVisionPC(), m_pDlgMainData->GetPortNoVisionPC());
		}
	case TIMER_MAIN_VIEW:
		DisplayEQPStatus();		// 시퀀스 상태를 실시간 표시
		DisplayProductInfo();	// 생산 정보를 실시간 표시
		break;
	} 


	CDialogEx::OnTimer(nIDEvent);
}

void CSDC_WindowControlPrjDlg::ClickLogIn()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(TRUE == m_Flag.m_bAutoStart)
		return

	SwitchingView(MAINVIEW);

	CWnd *test;
	test = GetDesktopWindow();
	CRect rc;
	test->GetWindowRect(rc);
	CRect Dlgrc;
	m_pDlgLogIn->GetWindowRect(Dlgrc);
	m_pDlgLogIn->SetWindowPos(STATIC_DOWNCAST(CSDC_WindowControlPrjDlg, AfxGetMainWnd()), (rc.Width()-Dlgrc.Width())/2, 
		(rc.Height()-Dlgrc.Height())/2, 0, 0, SWP_HIDEWINDOW|SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);

	m_pDlgLogIn->ShowWindow(SW_SHOW);
}

// 전체 초기화 작업을 진행한다.
void CSDC_WindowControlPrjDlg::ClickMainOpInit()
{	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if( g_cMsgBox.YesNoMsgBox(_T("시퀀스를 초기화 합니까?")) == IDNO )
		return;

	m_pDlgManual->WriteOutput(LD_PNP_VAC_ON_SOL, FALSE);
	m_pDlgManual->WriteOutput(LD_PNP_VAC_OFF_SOL, TRUE);
	Delay(100);
	m_pDlgManual->WriteOutput(LD_PNP_VAC_OFF_SOL, FALSE);

	m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_UP_SOL, TRUE);
	m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_DN_SOL, FALSE);

	m_pDlgManual->WriteOutput(FILM_JIG_VAC_ON_SOL, FALSE);
	m_pDlgManual->WriteOutput(FILM_JIG_VAC_OFF_SOL, TRUE);
	Delay(100);
	m_pDlgManual->WriteOutput(FILM_JIG_VAC_OFF_SOL, FALSE);

	m_pDlgManual->WriteOutput(FILM_JIG_CYLINDER_IN_SOL, FALSE);
	m_pDlgManual->WriteOutput(FILM_JIG_CYLINDER_OUT_SOL, TRUE);

	m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_FEED_SOL, FALSE);
	m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_BACK_SOL, TRUE);

	Delay(200);

	m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_CLAMP_SOL, FALSE);
	m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_UNCLAMP_SOL, TRUE);

	m_pDlgManual->WriteOutput(FILM_JIG_TILTING_CYLINDER_TILT_UP_SOL, TRUE);
	m_pDlgManual->WriteOutput(FILM_JIG_TILTING_CYLINDER_TILT_DN_SOL, FALSE);

	m_pDlgManual->WriteOutput(FILM_JIG_VAC_PLATE_CYLINDER_UP_SOL, TRUE);
	m_pDlgManual->WriteOutput(FILM_JIG_VAC_PLATE_CYLINDER_DN_SOL, FALSE);

	m_pDlgManual->WriteOutput(GLASS_JIG_VAC_ON_SOL, FALSE);
	m_pDlgManual->WriteOutput(GLASS_JIG_VAC_OFF_SOL, TRUE);
	Delay(100);
	m_pDlgManual->WriteOutput(GLASS_JIG_VAC_OFF_SOL, FALSE);
	
	m_pDlgManual->WriteOutput(TRAY_SHUTTLE_GLASS_VAC_ON_SOL, FALSE);
	m_pDlgManual->WriteOutput(TRAY_SHUTTLE_GLASS_VAC_OFF_SOL, TRUE);
	Sleep(100);
	m_pDlgManual->WriteOutput(TRAY_SHUTTLE_GLASS_VAC_OFF_SOL, FALSE);

	m_pDlgMainView->SetAllEmpty();

	m_pDlgManual->SetLampButton(FALSE);

	m_MDat[0].m_Seq[2].m_iSubSeqIdx = 100;
}

void CSDC_WindowControlPrjDlg::SetModelName(CString strModelName) 
{
	CString strTmp;
	strTmp.Format(_T("%s"), strModelName);
	m_ctrlModelNameLabel.SetCaption(strTmp);
}

void CSDC_WindowControlPrjDlg::SetLogIn()
{
	m_strLogIn.Format(_T("Log In : %s"), m_pDlgMainData->GetCurUserName());
	m_ctrlLogIn.SetCaption(m_strLogIn); 
	UpdateData(FALSE);
}

void CSDC_WindowControlPrjDlg::SetLogInOperator()
{
	m_strLogIn.Format(_T("Log In : %s"), LOG_IN_NAME_OPERATOR);
	m_bOperatorFlag = true;
	m_bEngineerFlag = false;
	m_bMakerFlag = false;
	
	m_ctrlLogIn.SetCaption(m_strLogIn);
}

////////////////////////////////////////////////////////////////////////////////////
//Main Sequence
UINT CSDC_WindowControlPrjDlg::MotionThread(LPVOID pParam)
{
	CSDC_WindowControlPrjDlg *pdlg = (CSDC_WindowControlPrjDlg *)pParam;

	pdlg->MotionThread();

	return 0;
}

void CSDC_WindowControlPrjDlg::MotionThread()
{
	int rslt[MOT_PROCESS_MAX_NUM][MOT_STATUS_MAX_NUM];
	int i, j, prc_idx, seq_idx;

	BOOL chk = FALSE;

	while(m_Flag.m_bProcessThread)
	{ 		
		//쓰레드 루틴이 돌 때마다 m_bAutoStart를 체크한다.
		chk = TRUE;
		for(i = 0; i < MOT_PROCESS_MAX_NUM; i++)
		{
			for(j = 0; j < MOT_STATUS_MAX_NUM; j++)
			{
				if(!m_MDat[i].m_Seq[j].m_bAutoStart)
				{
					chk = FALSE;
				}
			}
		}
	
		if(chk)		//Auto mode
		{
			//Proccessing
			for(i = 0; i < MOT_PROCESS_MAX_NUM; i++)
			{
				for(j = 0; j < MOT_STATUS_MAX_NUM; j++)
				{
					rslt[i][j] = MotionProcess(i, j);
					
					if(1 == rslt[i][j])
					{
						//Auto status
						
					}
					else if(0 == rslt[i][j])
					{
						
						//Error
						
						//정지
						AutoStop();

						//Alarm동작
						//AlarmOperation(i, j, m_MDat[i].m_Seq[j].m_iSubSeqIdx);
						
						m_MDat[i].m_Seq[j].m_bAlarm = TRUE;
						m_Flag.m_bAlarm	 = TRUE; // Display Status 참조용

						//먼져 에러가 나는쪽 먼져 처리한다.
						::SendMessage(this->m_hWnd, USER_MSG_MOT, 0, 0);
							
						//리셋시 다음번 동작예약
						m_MDat[i].m_Seq[j].m_iSubSeqIdx = m_MDat[i].m_Seq[j].m_iNextSubSeqIdx;
						break;
					}
				}
			}
		}
		else		//Stop Mode
		{
			if( 1 == StopModeOperation(&prc_idx, &seq_idx) )
			{

			}
			else
			{
				//정지
				AutoStop();

				//Alarm동작
				//AlarmOperation(prc_idx, seq_idx, m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx);

				//Error
				m_MDat[prc_idx].m_Seq[seq_idx].m_bAlarm = TRUE;
				m_Flag.m_bAlarm	 = TRUE; // Display Status 참조용

				//먼져 에러가 나는쪽 먼져 처리한다.
				::SendMessage(this->m_hWnd, USER_MSG_MOT, 0, 0);
			}
			Sleep(1);
		}

		Sleep(1);
	}

	AfxEndThread(0);
}

int CSDC_WindowControlPrjDlg::MotionProcess(int prc_idx, int seq_idx)
{
	int rslt = 1;
	int sub_seq_idx;
	BOOL chk_timeout = TRUE;
	BOOL not_delay = TRUE;
	double dCurPos = 0.;
	CString SendPacket, strTemp;

	char status_type[20];
	memset(status_type, NULL, sizeof(status_type));
	CTime CurTime;
	CurTime = CTime::GetCurrentTime();

	double dStartPos, dEndPos, dPeriod;

	switch(seq_idx)
	{
	//Stop mode check
	case 0:
		chk_timeout = FALSE;

		break;

	//CASE 1: Safe and limit sensor(inter lock)
	case 1:
		sub_seq_idx = m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx;
		switch(sub_seq_idx)
		{
			case 100:	//OP EMS 체크
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "EMO 버튼 누름 감지! = X008 (1:100)");	
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_EMO);

				if(!m_pDlgManual->ReadInput(OP_EMS_CHECK))
				{
					SetSeqIdx(prc_idx, seq_idx, 105);
				}
				break;
			case 105:	//Safety key Auto check
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Safety Auto Mode 이상 감지 = X000 (1:105)");	
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_SAFETYKEY);

				if(!m_pDlgManual->ReadInput(SAFETY_KEY_AUTO_CHECK))
				{
					SetSeqIdx(prc_idx, seq_idx, 110);
				}
				break;
			case 110:	//GPS Power Check
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "GPS Power 이상 감지! = X002 (1:110)");	
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_GPSPOWER);

				if(m_pDlgManual->ReadInput(GPS_POWER_FAILURE_ALARM_CHECK))
				{
					SetSeqIdx(prc_idx, seq_idx, 115);
				}
				break;
			case 115:	//Servo MC Check
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "SERVO MC OFF. 리셋버튼을 1초간 눌러 초기화해주세요. = X003 (1:115)");	
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_SERVOMC);

				if(m_pDlgManual->ReadInput(SERVO_MC_ON_OFF_CHECK))
				{
					SetSeqIdx(prc_idx, seq_idx, 120);
				}
				break;
			case 120:	//Front Left door Check
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "정면 좌측 문 열림! = X004(1:120)");	
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_DOOR_FL);

				if(!m_pDlgManual->ReadInput(FRONT_LEFT_DOOR_CHECK))
				{
					SetSeqIdx(prc_idx, seq_idx, 125);
				}
				break;
			case 125:	//Front Right door Check
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "정면 우측 문 열림! = X005 (1:125)");	
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_DOOR_FR);

				if(!m_pDlgManual->ReadInput(FRONT_RIGHT_DOOR_CHECK))
				{
					SetSeqIdx(prc_idx, seq_idx, 130);
				}
				break;
			case 130:	//Rear Left door Check
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "후면 좌측 문 열림! = X006 (1:130)");	
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_DOOR_RL);

				if(!m_pDlgManual->ReadInput(REAR_LEFT_DOOR_CHECK))
				{
					SetSeqIdx(prc_idx, seq_idx, 135);
				}
				break;
			case 135:	//Rear Right door Check
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "후면 우측 문 열림! = X007 (1:135)");
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_DOOR_RR);

				if(!m_pDlgManual->ReadInput(REAR_RIGHT_DOOR_CHECK))
				{
					SetSeqIdx(prc_idx, seq_idx, 140);
				}
				break;
			case 140:		//Main AIr Check
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Main Air 센서 감지 이상 = X025 (1:140)");	
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_MAINAIR);
									
				if(m_pDlgManual->ReadInput(MAIN_AIR_CHECK))
				{
					SetSeqIdx(prc_idx, seq_idx, 150);
				}
				break;			
			case 150:	// Light Curtain Check
				//Manual Muting 
				if( SYSTEM_MANUAL_MUTING_ON == m_pDlgMainData->GetSystemManualMuting() )
				{
					//Manual Muting 완료 상태
					if( !m_pDlgManual->ReadInput(LIGHT_CURTAIN_MUTING_START_CHECK_MANUAL) &&
						m_pDlgManual->ReadInput(LIGHT_CURTAIN_MUTING_END_CHECK) )
					{
						m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
						sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "라이트 커튼 감지! = X009 (1:150)");
						sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_LIGHTCURTAIN);

						if(m_pDlgManual->ReadInput(LIGHT_CURTAIN_DETECT_CHECK))
						{
							SetSeqIdx(prc_idx, seq_idx, 160);
						}
					}
					//Manual Muting 시작 상태
					else if( m_pDlgManual->ReadInput(LIGHT_CURTAIN_MUTING_START_CHECK_MANUAL) &&
							!m_pDlgManual->ReadInput(LIGHT_CURTAIN_MUTING_END_CHECK) )
					{
						SetSeqIdx(prc_idx, seq_idx, 160);
					}
				}
				//Auto Muting 
				else if( SYSTEM_MANUAL_MUTING_OFF == m_pDlgMainData->GetSystemManualMuting() )
				{

					//m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
					//sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Light Curtain Detected Error=X009 : 라이트 커튼 감지!");	

					//if( m_pDlgManual->ReadInput(LIGHT_CURTAIN_MUTING_START_CHECK_AUTO) )
					//{
						SetSeqIdx(prc_idx, seq_idx, 160);
					//}
				}
				break;
			case 160:
				SetSeqIdx(prc_idx, seq_idx, 100);
				break;
		}

		break;
	//CASE 2: Main sequence
	case 2:
		sub_seq_idx = m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx;
		switch(sub_seq_idx)
		{
		case 100:
			m_pDlgManual->SetLampButton(FALSE);

		//	m_pDlgTackTimeView->InitialTotalTactTime();

			SetSeqIdx(prc_idx, seq_idx, 105);
			break;
		case 105:	// 제품 Check
			if( SYSTEM_OP_MODE_AUTORUN == m_pDlgMainData->GetSystemOpMode() )
			{
				/*
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "Film Jig Check Error=X030 : 제품 있음!");	

				if(!m_pDlgManual->ReadInput(FILM_JIG_GLASS_CHECK))
				{
					SetSeqIdx(prc_idx, seq_idx, 110);
				}
				*/
				SetSeqIdx(prc_idx, seq_idx, 110);
			}
			else if( SYSTEM_OP_MODE_DRYRUN == m_pDlgMainData->GetSystemOpMode() )
				SetSeqIdx(prc_idx, seq_idx, 110);
			break;
		case 110:
			m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_UP_SOL, TRUE);
			m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_DN_SOL, FALSE);

			SetSeqIdx(prc_idx, seq_idx, 115);
			break;
		case 115:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "PICKER UP 실린더 센서 감지 이상 = X00C (2:115)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_PICKERUP);

			if(m_pDlgManual->ReadInput(LD_PNP_CYLINDER_UP_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 116);
			}

			break;
		case 116: 
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "PICKER DOWN 실린더 센서 감지 이상 = X00D (2:116)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_PICKERDOWN);

			if(!m_pDlgManual->ReadInput(LD_PNP_CYLINDER_DN_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 117);
			}
			break;
		case 117:
			not_delay = CheckDelay(prc_idx, seq_idx, 300);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 120);
			}
			break;
		case 120:
			// LOT START
			m_pDlgManual->WriteOutput(LD_PNP_VAC_ON_SOL, FALSE);
			m_pDlgManual->WriteOutput(LD_PNP_VAC_OFF_SOL, TRUE);
			Delay(30);
			m_pDlgManual->WriteOutput(LD_PNP_VAC_OFF_SOL, FALSE);

			m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_UP_SOL, TRUE);
			m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_DN_SOL, FALSE);

			m_pDlgManual->WriteOutput(FILM_JIG_VAC_ON_SOL, FALSE);
			m_pDlgManual->WriteOutput(FILM_JIG_VAC_OFF_SOL, TRUE);
			Delay(30);
			m_pDlgManual->WriteOutput(FILM_JIG_VAC_OFF_SOL, FALSE);

			m_pDlgManual->WriteOutput(FILM_JIG_CYLINDER_IN_SOL, FALSE);
			m_pDlgManual->WriteOutput(FILM_JIG_CYLINDER_OUT_SOL, TRUE);

			m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_FEED_SOL, FALSE);
			m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_BACK_SOL, TRUE);

			m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_CLAMP_SOL, FALSE);
			m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_UNCLAMP_SOL, TRUE);

			m_pDlgManual->WriteOutput(FILM_JIG_TILTING_CYLINDER_TILT_UP_SOL, TRUE);
			m_pDlgManual->WriteOutput(FILM_JIG_TILTING_CYLINDER_TILT_DN_SOL, FALSE);

			m_pDlgManual->WriteOutput(FILM_JIG_VAC_PLATE_CYLINDER_UP_SOL, TRUE);
			m_pDlgManual->WriteOutput(FILM_JIG_VAC_PLATE_CYLINDER_DN_SOL, FALSE);

			m_pDlgManual->WriteOutput(GLASS_JIG_VAC_ON_SOL, FALSE);
			m_pDlgManual->WriteOutput(GLASS_JIG_VAC_OFF_SOL, TRUE);
			Delay(30);
			m_pDlgManual->WriteOutput(GLASS_JIG_VAC_OFF_SOL, FALSE);
	
			m_pDlgManual->WriteOutput(TRAY_SHUTTLE_GLASS_VAC_ON_SOL, FALSE);
			m_pDlgManual->WriteOutput(TRAY_SHUTTLE_GLASS_VAC_OFF_SOL, TRUE);
			Delay(30);
			m_pDlgManual->WriteOutput(TRAY_SHUTTLE_GLASS_VAC_OFF_SOL, FALSE);

			SetSeqIdx(prc_idx, seq_idx, 130);
			break;
		case 130:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B 투입 실린더 센서 감지 이상 = X033 (2:130)");
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_IN);

			if(!m_pDlgManual->ReadInput(FILM_JIG_CYLINDER_IN_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 140);
			}
			break;
		case 140:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B 배출 실린더 센서 감지 이상 = X032 (2:140)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_OUT);

			if(m_pDlgManual->ReadInput(FILM_JIG_CYLINDER_OUT_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 150);
			}
			break;
		case 150:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B 클램프 전진 센서 감지 이상 = X034 (2:150)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_CLAMP_FORWARD);

			if(!m_pDlgManual->ReadInput(FILM_JIG_GLASS_CLAMP_CYLINDER_FEED_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 160);
			}
			break;
		case 160:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B 클램프 후진 센서 감지 이상 = X035 (2:160)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_CLAMP_BACK);

			if(m_pDlgManual->ReadInput(FILM_JIG_GLASS_CLAMP_CYLINDER_BACK_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 170);
			}
			break;
		case 170:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B 클램프 하강 센서 감지 이상 = X036 (2:170)");
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_CLAMP_DOWN);

			if(!m_pDlgManual->ReadInput(FILM_JIG_GLASS_CLAMP_CYLINDER_CLAMP_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 180);
			}
			break;
		case 180:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B  클램프 상승 센서 감지 이상 = X037 (2:180)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_CLAMP_UP);

			if(m_pDlgManual->ReadInput(FILM_JIG_GLASS_CLAMP_CYLINDER_UNCLAMP_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 190);
			}
			break;
		case 190:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B 틸트 UP 센서 감지 이상 = X038 (2:190)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_TILT_UP);

			if(m_pDlgManual->ReadInput(FILM_JIG_TILTING_CYLINDER_TILT_UP_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 200);
			}
			break;
		case 200:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B 틸트 DOWN 센서 감지 이상 = X039 (2:200)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_TILT_DOWN);

			if(!m_pDlgManual->ReadInput(FILM_JIG_TILTING_CYLINDER_TILT_DN_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 210);
			}
			break;
		case 210:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B VAC PLATE UP 센서 감지 이상 = X03A (2:210)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_PLATE_UP);

			if(m_pDlgManual->ReadInput(FILM_JIG_VAC_PLATE_CYLINDER_UP_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 220);
			}
			break;
		case 220:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B VAC PLATE DOWN 센서 감지 이상 = X03B (2:220)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_PLATE_DOWN);

			if(!m_pDlgManual->ReadInput(FILM_JIG_VAC_PLATE_CYLINDER_DN_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 230);
			}
			break;
		case 230:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG A 투입 실린더 센서 감지 이상 = X03E (2:230)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGA_IN);

			if(!m_pDlgManual->ReadInput(GLASS_JIG_CYLINDER_IN_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 240);
			}
			break;
		case 240:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG A 배출 실린더 센서 감지 이상 = X03D (2:240)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGA_OUT);

			if(m_pDlgManual->ReadInput(GLASS_JIG_CYLINDER_OUT_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 245);
			}
			break;
		case 245:	// 모든 모터 초기 위치로 이동
			m_pDlgManual->AxisMove(AXIS_TRY_SHUTTLE, m_pDlgTeach->GetLoadPosY(), 7, FALSE);
			m_pDlgManual->AxisMove(AXIS_LD_PNP, m_pDlgTeach->GetLoadPosX(), 7, FALSE);

			SetSeqIdx(prc_idx, seq_idx, 246);
			break;
		case 246:	// 모터 0 Move done
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "TRAY SHUTTLE(Y축) LOAD POS 이동 실패 = (2:246)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_AXIS_Y_MOVEFAIL);

			if(m_pDlgManual->AxisMoveDone(AXIS_TRY_SHUTTLE))
			{
				SetSeqIdx(prc_idx, seq_idx, 247);
			}
			break;
		case 247:	// 모터 1 Move done
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "AXIS_LD_PNP(X축) LOAD POS 이동 실패 = (2:247)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_AXIS_X_MOVEFAIL);

			if(m_pDlgManual->AxisMoveDone(AXIS_LD_PNP))
			{
				SetSeqIdx(prc_idx, seq_idx, 250);
			}
			break;
		case 250:	//Ready ON
			//if( SYSTEM_MANUAL_MUTING_OFF == m_pDlgMainData->GetSystemManualMuting()) //Auto Muting
			//	m_pDlgManual->WriteOutput(LIGHT_CURTAIN_MUTING_ON, TRUE);

			m_strInnerID = CurTime.Format(_T("%y%m%d%H%M%S"));
			m_bGrabStart = FALSE;
			m_bGrabEnd = FALSE;
			m_bInspectEnd = FALSE;
			m_nVisionResult = -1;

			if( SYSTEM_MANUAL_SKIPFILMREMOVE_OFF == m_pDlgMainData->GetSystemManualSkipFilmRemove())
			{
				m_pDlgManual->WriteOutput(READY_SWITCH_LAMP_1, TRUE);
				m_pDlgManual->WriteOutput(READY_SWITCH_LAMP_2, TRUE);

				if( SYSTEM_MANUAL_MUTING_OFF == m_pDlgMainData->GetSystemManualMuting() )	//Auto MUTING ON
					m_pDlgManual->WriteOutput(LIGHT_CURTAIN_MUTING_ON, TRUE);

				SetSeqIdx(prc_idx, seq_idx, 252);
			}
			else
			{
				SetSeqIdx(prc_idx, seq_idx, 260);
			}
			break;
		case 252:
			if( SYSTEM_OP_MODE_AUTORUN == m_pDlgMainData->GetSystemOpMode() )
			{
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = 0;//ms
				if( (m_pDlgManual->ReadInput(READY_SWITCH_LAMP_1)) || (m_pDlgManual->ReadInput(READY_SWITCH_LAMP_2)) )
				{
					SetSeqIdx(prc_idx, seq_idx, 253);
				}
			}
			else if( SYSTEM_OP_MODE_DRYRUN == m_pDlgMainData->GetSystemOpMode() )
			{
				SetSeqIdx(prc_idx, seq_idx, 253);
			}

			break;
		case 253:
			if( SYSTEM_OP_MODE_AUTORUN == m_pDlgMainData->GetSystemOpMode() )
			{
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_INPUT_BUTTON;//ms
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "READY 양수 버튼 입력 시간 초과 (2:253)");
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_READY_TIMEOUT);

				if( (m_pDlgManual->ReadInput(READY_SWITCH_LAMP_1)) && (m_pDlgManual->ReadInput(READY_SWITCH_LAMP_2)) )
				{
					SetSeqIdx(prc_idx, seq_idx, 255);
				}
			}
			else if( SYSTEM_OP_MODE_DRYRUN == m_pDlgMainData->GetSystemOpMode() )
			{
				SetSeqIdx(prc_idx, seq_idx, 255);
			}
			break;
		case 255:
			m_pDlgMainView->SetStateCell(DISPLAY_CELL_JIG_B_BACK, CELL_STATE_OK);	//DISPLAY MAIN VIEW 

			if( SYSTEM_OP_MODE_AUTORUN == m_pDlgMainData->GetSystemOpMode() )
			{
				//Manual Muting
				if( SYSTEM_MANUAL_MUTING_ON == m_pDlgMainData->GetSystemManualMuting() )
				{
					m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
					sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "뮤팅 완료버튼을 누른 후 계속 진행해주세요. (2:255)");	
					sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_MUTE_PUSH_COMPLETE);
					
					if( !m_pDlgManual->ReadInput(LIGHT_CURTAIN_MUTING_START_CHECK_MANUAL) && 
						m_pDlgManual->ReadInput(LIGHT_CURTAIN_MUTING_END_CHECK) )
					{
						SetSeqIdx(prc_idx, seq_idx, 1000);	// FILM 벗기기 동작

						m_Rslt.m_iFilmRemoveSeq = 260;

						m_pDlgManual->WriteOutput(READY_SWITCH_LAMP_1, FALSE);
						m_pDlgManual->WriteOutput(READY_SWITCH_LAMP_2, FALSE);
					}
				}
				//Auto Muting
				else if( SYSTEM_MANUAL_MUTING_OFF == m_pDlgMainData->GetSystemManualMuting() )
				{
					m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
					sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "자동 뮤팅 감지 이상 (2:255)");
					sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_MUTE_AUTO_FAIL);

					if( m_pDlgManual->ReadInput(LIGHT_CURTAIN_MUTING_START_CHECK_AUTO) )
					{
						SetSeqIdx(prc_idx, seq_idx, 1000);	// FILM 벗기기 동작

						m_Rslt.m_iFilmRemoveSeq = 260;

						m_pDlgManual->WriteOutput(READY_SWITCH_LAMP_1, FALSE);
						m_pDlgManual->WriteOutput(READY_SWITCH_LAMP_2, FALSE);

						if( SYSTEM_MANUAL_MUTING_OFF == m_pDlgMainData->GetSystemManualMuting() )	// Auto Muting
							m_pDlgManual->WriteOutput(LIGHT_CURTAIN_MUTING_ON, FALSE);	
					}
				}
			}
			else if( SYSTEM_OP_MODE_DRYRUN == m_pDlgMainData->GetSystemOpMode() )
			{
				Delay(1000);

				SetSeqIdx(prc_idx, seq_idx, 1000);	// FILM 벗기기 동작

				m_Rslt.m_iFilmRemoveSeq = 260;

				m_pDlgManual->WriteOutput(READY_SWITCH_LAMP_1, FALSE);
				m_pDlgManual->WriteOutput(READY_SWITCH_LAMP_2, FALSE);
			}

			m_strStartTime = CurTime.Format(_T("%y%m%d%H%M%S"));

			break;
		case 260:	//FILM Remove 완료 후 시퀀스
			SetSeqIdx(prc_idx, seq_idx, 265);
			break;
		case 265:
			m_lStartTime = GetCurrentTime();//INPUT_START_BUTTON TACT START 20160412

			m_pDlgManual->WriteOutput(START_SWITCH_LAMP_1, TRUE);
			m_pDlgManual->WriteOutput(START_SWITCH_LAMP_2, TRUE);

			if( SYSTEM_MANUAL_MUTING_OFF == m_pDlgMainData->GetSystemManualMuting() )	//Auto Muting
				m_pDlgManual->WriteOutput(LIGHT_CURTAIN_MUTING_ON, TRUE);	

			SetSeqIdx(prc_idx, seq_idx, 267);
			break;
		case 267:
			if( SYSTEM_OP_MODE_AUTORUN == m_pDlgMainData->GetSystemOpMode() )
			{
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = 0;//ms
				if( (m_pDlgManual->ReadInput(START_SWITCH_1)) || (m_pDlgManual->ReadInput(START_SWITCH_2)) )
				{
					SetSeqIdx(prc_idx, seq_idx, 268);
				}
			}
			else if( SYSTEM_OP_MODE_DRYRUN == m_pDlgMainData->GetSystemOpMode() )
			{
				SetSeqIdx(prc_idx, seq_idx, 268);
			}

			break;
		case 268:
			if( SYSTEM_OP_MODE_AUTORUN == m_pDlgMainData->GetSystemOpMode() )
			{
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_INPUT_BUTTON;//ms
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "START 양수버튼 입력시간 초과 (2:268)");	
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_START_TIMEOUT);

				if( (m_pDlgManual->ReadInput(START_SWITCH_1)) && (m_pDlgManual->ReadInput(START_SWITCH_2)) )
				{
					SetSeqIdx(prc_idx, seq_idx, 270);
				}
			}
			else if( SYSTEM_OP_MODE_DRYRUN == m_pDlgMainData->GetSystemOpMode() )
			{
				SetSeqIdx(prc_idx, seq_idx, 270);
			}
			break;
		case 270:	//Start ON
			m_pDlgMainView->SetStateCell(DISPLAY_CELL_JIG_A_BACK, CELL_STATE_OK);	//DISPLAY MAIN VIEW 
			
			if( SYSTEM_OP_MODE_AUTORUN == m_pDlgMainData->GetSystemOpMode() )
			{
				//Manual Muting
				if( SYSTEM_MANUAL_MUTING_ON == m_pDlgMainData->GetSystemManualMuting() )
				{
					m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
					sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "뮤팅 완료버튼을 누른 후 계속 진행해주세요. (2:270)");
					sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_MUTE_PUSH_COMPLETE);

					if( !m_pDlgManual->ReadInput(LIGHT_CURTAIN_MUTING_START_CHECK_MANUAL) && 
						m_pDlgManual->ReadInput(LIGHT_CURTAIN_MUTING_END_CHECK) )
					{
						SetSeqIdx(prc_idx, seq_idx, 273);

						m_pDlgManual->WriteOutput(START_SWITCH_LAMP_1, FALSE);
						m_pDlgManual->WriteOutput(START_SWITCH_LAMP_2, FALSE);

						m_lEndTime = GetCurrentTime();//INPUT_START_BUTTON TACT END 20160412
						m_pDlgTackTimeView->SetTactTime(INPUT_START_BUTTON, m_lEndTime - m_lStartTime);
						m_pDlgTackTimeView->SetTTUpdate(INPUT_START_BUTTON, TRUE);
					}
				}
				//Auto Muting
				else if( SYSTEM_MANUAL_MUTING_OFF == m_pDlgMainData->GetSystemOpMode() )
				{
					m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
					sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "자동 뮤팅 감지 이상 (2:270)");
					sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_MUTE_AUTO_FAIL);

					if( m_pDlgManual->ReadInput(LIGHT_CURTAIN_MUTING_START_CHECK_AUTO) )
					{
						SetSeqIdx(prc_idx, seq_idx, 273);

						m_pDlgManual->WriteOutput(START_SWITCH_LAMP_1, FALSE);
						m_pDlgManual->WriteOutput(START_SWITCH_LAMP_2, FALSE);

						if( SYSTEM_MANUAL_MUTING_OFF == m_pDlgMainData->GetSystemManualMuting() )	//Auto Muting
							m_pDlgManual->WriteOutput(LIGHT_CURTAIN_MUTING_ON, FALSE);	

						m_lEndTime = GetCurrentTime();//INPUT_START_BUTTON TACT END 20160412
						m_pDlgTackTimeView->SetTactTime(INPUT_START_BUTTON, m_lEndTime - m_lStartTime);
						m_pDlgTackTimeView->SetTTUpdate(INPUT_START_BUTTON, TRUE);
					}
				}
			}
			else if( SYSTEM_OP_MODE_DRYRUN == m_pDlgMainData->GetSystemOpMode() )
			{
				Delay(1000);
				SetSeqIdx(prc_idx, seq_idx, 273);

				m_pDlgManual->WriteOutput(START_SWITCH_LAMP_1, FALSE);
				m_pDlgManual->WriteOutput(START_SWITCH_LAMP_2, FALSE);
			}

			break;
		case 273:	//Film Jig TILT 위치 원복
			m_lStartTime = GetCurrentTime();//INSERT_JIG TACT START 20160412

			m_pDlgManual->WriteOutput(FILM_JIG_TILTING_CYLINDER_TILT_DN_SOL, FALSE);
			m_pDlgManual->WriteOutput(FILM_JIG_TILTING_CYLINDER_TILT_UP_SOL, TRUE);
			//Delay(200);
			m_pDlgManual->WriteOutput(FILM_JIG_VAC_PLATE_CYLINDER_DN_SOL, FALSE);
			m_pDlgManual->WriteOutput(FILM_JIG_VAC_PLATE_CYLINDER_UP_SOL, TRUE);

			SetSeqIdx(prc_idx, seq_idx, 274);
			break;
		case 274:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B 틸트 DOWN 센서 감지 이상 = X039 (2:274)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_TILT_DOWN);

			if(!m_pDlgManual->ReadInput(FILM_JIG_TILTING_CYLINDER_TILT_DN_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 275);
			}
			break;
		case 275:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B 틸트 UP 센서 감지 이상= X038 (2:275)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_TILT_UP);

			if(m_pDlgManual->ReadInput(FILM_JIG_TILTING_CYLINDER_TILT_UP_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 276);
			}
			break;
		case 276:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B VAC PLATE DOWN 센서 감지 이상 = X03B (2:276)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_PLATE_DOWN);

			if(!m_pDlgManual->ReadInput(FILM_JIG_VAC_PLATE_CYLINDER_DN_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 277);
			}
			break;
		case 277:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B VAC PLATE UP 센서 감지 이상 = X03A (2:277)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_PLATE_UP);

			if(m_pDlgManual->ReadInput(FILM_JIG_VAC_PLATE_CYLINDER_UP_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 280);
			}
			break;
		case 280:		//GLASS Vac ON / FILM JIG 전진
			m_pDlgMainView->SetStateCell(DISPLAY_CELL_JIG_A_FORWARD, CELL_STATE_OK);	//DISPLAY MAIN VIEW 

			m_pDlgManual->WriteOutput(FILM_JIG_CYLINDER_IN_SOL, TRUE);
			m_pDlgManual->WriteOutput(FILM_JIG_CYLINDER_OUT_SOL, FALSE);
			
			SetSeqIdx(prc_idx, seq_idx, 283);
			break;
		case 283: //JIG A Vac On	
			m_pDlgManual->WriteOutput(GLASS_JIG_VAC_ON_SOL, TRUE);
			m_pDlgManual->WriteOutput(GLASS_JIG_VAC_OFF_SOL, FALSE);

			SetSeqIdx(prc_idx, seq_idx, 285);
			break;
		case 285: //JIG A Vac Check
			if( SYSTEM_OP_MODE_AUTORUN == m_pDlgMainData->GetSystemOpMode() )
			{
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = 1000;//ms
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG A VAC 센서 감지 이상 = X03C (2:285)");	
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGA_VAC_FAIL);

				if(m_pDlgManual->ReadInput(GLASS_JIG_VAC_ON_CHECK))
				{
					SetSeqIdx(prc_idx, seq_idx, 290);
				}
			}
			else if( SYSTEM_OP_MODE_DRYRUN == m_pDlgMainData->GetSystemOpMode() )
				SetSeqIdx(prc_idx, seq_idx, 290);
			break;
		case 290: //FILM JIG 전진 체크
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B 투입 실린더 센서 감지 이상 = X033 (2:290)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_IN);

			if(m_pDlgManual->ReadInput(FILM_JIG_CYLINDER_IN_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 291);
			}
			break;
		case 291: //FILM JIG 전진 체크
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B 배출 실린더 센서 감지 이상 = X032 (2:291)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_OUT);

			if(!m_pDlgManual->ReadInput(FILM_JIG_CYLINDER_OUT_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 292);
			}
			break;
		case 292: //GLASS JIG 전진 체크
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG A 투입 실린더 센서 감지 이상 = X03E (2:292)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGA_IN);

			if(m_pDlgManual->ReadInput(GLASS_JIG_CYLINDER_IN_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 293);
			}
			break;
		case 293://GLASS JIG 전진 체크
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG A 배출 실린더 센서 감지 이상 = X03D (2:293)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGA_OUT);

			if(!m_pDlgManual->ReadInput(GLASS_JIG_CYLINDER_OUT_CHECK))
			{
				m_lEndTime = GetCurrentTime();//INSERT_JIG TACT END 20160412
				m_pDlgTackTimeView->SetTactTime(INSERT_JIG, m_lEndTime - m_lStartTime);
				m_pDlgTackTimeView->SetTTUpdate(INSERT_JIG, TRUE);

				SetSeqIdx(prc_idx, seq_idx, 310);
			}
			break;
		case 310:	//Picker DOWN
			m_lStartTime = GetCurrentTime();//IN_SHUTTLE TACT START 20160412

			m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_UP_SOL, FALSE);
			m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_DN_SOL, TRUE);
			
			SetSeqIdx(prc_idx, seq_idx, 315);
			break;
		case 315:	//Picker Up Check
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "PICKER UP 실린더 센서 감지 이상 = X00C (2:315)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_PICKERUP);

			if(!m_pDlgManual->ReadInput(LD_PNP_CYLINDER_UP_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 320);
			}

			break;
		case 320://Picker Dn Check
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "PICKER DOWN 실린더 센서 감지 이상 = X00D (2:320)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_PICKERDOWN);

			if(m_pDlgManual->ReadInput(LD_PNP_CYLINDER_DN_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 330);
			}
			break;
		case 330:	//Picker VAC ON
			m_pDlgManual->WriteOutput(LD_PNP_VAC_ON_SOL, TRUE);
			m_pDlgManual->WriteOutput(LD_PNP_VAC_OFF_SOL, FALSE);

			SetSeqIdx(prc_idx, seq_idx, 335);
			break;
		case 335:	
			not_delay = CheckDelay(prc_idx, seq_idx, 300);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 340);
			}
			break;
		case 340://Picker VAC CHECK
			if( SYSTEM_OP_MODE_AUTORUN == m_pDlgMainData->GetSystemOpMode() )
			{
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = 1000;//ms
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "PICKER VAC 센서 감지 이상 = X02F (2:340)");
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_PICKER_VAC);

				if(m_pDlgManual->ReadInput(LD_PNP_VAC_ON_CHECK))
				{
					SetSeqIdx(prc_idx, seq_idx, 343);
				}
			}
			else if( SYSTEM_OP_MODE_DRYRUN == m_pDlgMainData->GetSystemOpMode() )
				SetSeqIdx(prc_idx, seq_idx, 343);
			break;
		case 343:	//JIG A Vac OFF
			
			m_pDlgManual->WriteOutput(GLASS_JIG_VAC_ON_SOL, FALSE);
			m_pDlgManual->WriteOutput(GLASS_JIG_VAC_OFF_SOL, TRUE);
			Delay(100);
			m_pDlgManual->WriteOutput(GLASS_JIG_VAC_OFF_SOL, FALSE);

			SetSeqIdx(prc_idx, seq_idx, 345);
			break;
		case 345:	// JIG A Vac Check
			if( SYSTEM_OP_MODE_AUTORUN == m_pDlgMainData->GetSystemOpMode() )
			{
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = 1000;//ms
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG A VAC 센서 감지 이상 = X03C (2:345)");
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGA_VAC_FAIL);

				if(!m_pDlgManual->ReadInput(GLASS_JIG_VAC_ON_CHECK))
				{
					SetSeqIdx(prc_idx, seq_idx, 350);
				}
			}
			else if( SYSTEM_OP_MODE_DRYRUN == m_pDlgMainData->GetSystemOpMode() )
				SetSeqIdx(prc_idx, seq_idx, 350);
			break;
		case 350:	// Picker UP
			m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_UP_SOL, TRUE);
			m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_DN_SOL, FALSE);

			SetSeqIdx(prc_idx, seq_idx, 355);
			break;
		case 355:	// Picker UP Check
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "PICKER UP 실린더 센서 감지 이상 = X00C (2:355)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_PICKERUP);

			if(m_pDlgManual->ReadInput(LD_PNP_CYLINDER_UP_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 360);
			}
			break;
		case 360:// Picker DN Check
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "PICKER DOWN 실린더 센서 감지 이상 = X00D(2:360)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_PICKERDOWN);

			if(!m_pDlgManual->ReadInput(LD_PNP_CYLINDER_DN_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 370);
			}
			break;
		case 370:	// LD P&P AXIS : INSP MOVE
			m_pDlgManual->AxisMove(AXIS_LD_PNP, m_pDlgTeach->GetInspPosX(), 8,  FALSE);

			SetSeqIdx(prc_idx, seq_idx, 375);
			break;
		case 375:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "AXIS_LD_PNP(X축) INSP POS 이동 실패 (2:375)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_AXIS_Y_MOVEFAIL);

			if(m_pDlgManual->AxisMoveDone(AXIS_LD_PNP))
			{
				SetSeqIdx(prc_idx, seq_idx, 380);
			}
			break;
		case 380:	// Picker DOWN
			m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_UP_SOL, FALSE);
			m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_DN_SOL, TRUE);

			SetSeqIdx(prc_idx, seq_idx, 385);
			break;
		case 385:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "PICKER UP 실린더 센서 감지 이상 = X00C (2:385)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_PICKERUP);

			if(!m_pDlgManual->ReadInput(LD_PNP_CYLINDER_UP_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 390);
			}
			break;
		case 390:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "PICKER DOWN 실린더 센서 감지 이상 = X00D (2:390)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_PICKERDOWN);

			if(m_pDlgManual->ReadInput(LD_PNP_CYLINDER_DN_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 395);
			}
			break;
		case 395: //PNP VAC OFF 20160430
			m_pDlgManual->WriteOutput(LD_PNP_VAC_ON_SOL, FALSE);
			m_pDlgManual->WriteOutput(LD_PNP_VAC_OFF_SOL, TRUE);
			Delay(100);
			m_pDlgManual->WriteOutput(LD_PNP_VAC_OFF_SOL, FALSE);

			SetSeqIdx(prc_idx, seq_idx, 400);
			break;
		case 400:	//PNP VAC CHECK
			if( SYSTEM_OP_MODE_AUTORUN == m_pDlgMainData->GetSystemOpMode() )
			{
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = 1000;//ms
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "PICKER VAC 센서 감지 이상 = X02F (2:400)");	
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_PICKER_VAC);

				if(!m_pDlgManual->ReadInput(LD_PNP_VAC_ON_CHECK))
				{
					SetSeqIdx(prc_idx, seq_idx, 410);
				}
			}
			else if( SYSTEM_OP_MODE_DRYRUN == m_pDlgMainData->GetSystemOpMode() )
				SetSeqIdx(prc_idx, seq_idx, 410);
			break;
		case 410: //트레이 셔틀 VAC ON
			m_pDlgMainView->SetStateCell(DISPLAY_CELL_TRAY_SHUTTLE, CELL_STATE_OK);	//DISPLAY MAIN VIEW 

			m_pDlgManual->WriteOutput(TRAY_SHUTTLE_GLASS_VAC_ON_SOL, TRUE);
			m_pDlgManual->WriteOutput(TRAY_SHUTTLE_GLASS_VAC_OFF_SOL, FALSE);

			SetSeqIdx(prc_idx, seq_idx, 415);
			break;
		case 415: 
			not_delay = CheckDelay(prc_idx, seq_idx, 300);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 420);
			}
			break;
		case 420:	//트레이 셔틀 VAC ON CHECK
			if( SYSTEM_OP_MODE_AUTORUN == m_pDlgMainData->GetSystemOpMode() )
			{
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = 1000;//ms
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "TRAY SHUTTLE VAC 센서 감지 이상 = X03F (2:420)");
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_SHUTTLE_VAC);

				if(m_pDlgManual->ReadInput(TRAY_SHUTTLE_GLASS_VAC_ON_CHECK))
				{
					m_lEndTime = GetCurrentTime();//IN_SHUTTLE TACT END 20160412
					m_pDlgTackTimeView->SetTactTime(IN_SHUTTLE, m_lEndTime - m_lStartTime);
					m_pDlgTackTimeView->SetTTUpdate(IN_SHUTTLE, TRUE);

					SetSeqIdx(prc_idx, seq_idx, 425);
				}
			}
			if( SYSTEM_OP_MODE_DRYRUN == m_pDlgMainData->GetSystemOpMode() )
				SetSeqIdx(prc_idx, seq_idx, 425);
			break;
		case 425:	// Picker UP
			m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_UP_SOL, TRUE);
			m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_DN_SOL, FALSE);

			SetSeqIdx(prc_idx, seq_idx, 426);
			break;
		case 426:	// Picker UP Check
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "PICKER UP 실린더 센서 감지 이상 = X00C (2:426)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_PICKERUP);

			if(m_pDlgManual->ReadInput(LD_PNP_CYLINDER_UP_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 427);
			}
			break;
		case 427:// Picker UP Check
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "PICKER DOWN 실린더 센서 감지 이상 = X00D (2:427)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_PICKERDOWN);

			if(!m_pDlgManual->ReadInput(LD_PNP_CYLINDER_DN_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 428);
			}
			break;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//그랩 통신
		case 428://그랩스타트 패킷 전송
			if( SYSTEM_OP_MODE_AUTORUN == m_pDlgMainData->GetSystemOpMode() )
			{
				if( SYSTEM_MANUAL_SOCKET_ON == m_pDlgMainData->GetSystemManualSocket() )	//
					SendMessage(USER_MSG_MOT, 1, 0);

				SetSeqIdx(prc_idx, seq_idx, 430);
			}
			else if( SYSTEM_OP_MODE_DRYRUN == m_pDlgMainData->GetSystemOpMode() )
			{
				SetSeqIdx(prc_idx, seq_idx, 430);
			}
			break;
		case 430://그랩 스타트 위치로 이동	
			m_pDlgMainView->SetStateCell(DISPLAY_CELL_GRAB_START, CELL_STATE_OK);	//DISPLAY MAIN VIEW 

			m_lStartTime = GetCurrentTime();//SHUTTLE MOVE GRAB START

			m_pDlgManual->AxisMove(AXIS_TRY_SHUTTLE, m_pDlgTeach->GetInspPosY(), 8, FALSE);
			SetSeqIdx(prc_idx, seq_idx, 440);
			break;
		case 440: 
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "TRAY SHUTTLE(Y축) INSP POS 이동 실패 (2:440)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_AXIS_Y_MOVEFAIL);

			if(m_pDlgManual->AxisMoveDone(AXIS_TRY_SHUTTLE))
			{
				m_lEndTime = GetCurrentTime();//SHUTTLE_MOVE_GRAB_START TACT END
				m_pDlgTackTimeView->SetTactTime(SHUTTLE_MOVE_GRAB_START, m_lEndTime - m_lStartTime);
				m_pDlgTackTimeView->SetTTUpdate(SHUTTLE_MOVE_GRAB_START, TRUE);

				SetSeqIdx(prc_idx, seq_idx, 450);
			}
			break;
		case 450://그랩스타트 패킷 응답
			if( SYSTEM_OP_MODE_AUTORUN == m_pDlgMainData->GetSystemOpMode() )
			{
				if( SYSTEM_MANUAL_SOCKET_ON == m_pDlgMainData->GetSystemManualSocket() )	
				{
					m_MDat[prc_idx	].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_TCPIP_RCV;//ms
					sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "비젼 GRAB START 응답 없음 (2:450)");
					sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_VISION_GRAB_START_TIMEOUT);

					if(m_bGrabStart)	// 응답체크
					{
						m_bGrabStart = FALSE;

						SetSeqIdx(prc_idx, seq_idx, 460);
					}
				}
				else if( SYSTEM_MANUAL_SOCKET_OFF == m_pDlgMainData->GetSystemManualSocket() )
					SetSeqIdx(prc_idx, seq_idx, 460);
			}
			else if( SYSTEM_OP_MODE_DRYRUN == m_pDlgMainData->GetSystemOpMode() )
			{
				SetSeqIdx(prc_idx, seq_idx, 460);
			}
			break;
		case 460:	//그랩 종료 위치 이동
			m_pDlgMainView->SetStateCell(DISPLAY_CELL_CAMERA, CELL_STATE_OK);	//DISPLAY MAIN VIEW 
			m_pDlgMainView->SetStateCell(DISPLAY_CELL_GRAB_END, CELL_STATE_OK);	//DISPLAY MAIN VIEW 

			m_lStartTime = GetCurrentTime();//SHUTTLE MOVE GRAB END

			dStartPos = m_pDlgTeach->GetInspPosY() * 1000;
			dEndPos = m_pDlgTeach->GetUnLoadPosY() * 1000;
			dPeriod = m_pDlgManual->AxisGetTriggerPeriod(AXIS_TRY_SHUTTLE);
			
			m_pDlgManual->AxisSetTriggerReset(AXIS_TRY_SHUTTLE);	//트리거 리셋
			Delay(100);
			m_pDlgManual->AxisSetTriggerTimeLevel(AXIS_TRY_SHUTTLE);	//트리거 레벨
			m_pDlgManual->AxisSetTriggerBlock(AXIS_TRY_SHUTTLE, dStartPos, dEndPos, dPeriod);//트리거 블럭
			Delay(100);
			m_pDlgManual->AxisMove(AXIS_TRY_SHUTTLE, m_pDlgTeach->GetUnLoadPosY(), 1, FALSE);
			
			SetSeqIdx(prc_idx, seq_idx, 465);
		case 465: 
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "TRAY SHUTTLE(Y축) UNLOAD POS 이동 실패 (2:465)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_AXIS_Y_MOVEFAIL);

			if(m_pDlgManual->AxisMoveDone(AXIS_TRY_SHUTTLE))
			{
				m_lEndTime = GetCurrentTime();//SHUTTLE_MOVE_GRAB_END TACT END
				m_pDlgTackTimeView->SetTactTime(SHUTTLE_MOVE_GRAB_END, m_lEndTime - m_lStartTime);
				m_pDlgTackTimeView->SetTTUpdate(SHUTTLE_MOVE_GRAB_END, TRUE);

				SetSeqIdx(prc_idx, seq_idx, 470);
			}
			break;
		case 470:	//그랩 종료 패킷 전송
			if( SYSTEM_OP_MODE_AUTORUN == m_pDlgMainData->GetSystemOpMode() )
			{
				if( SYSTEM_MANUAL_SOCKET_ON == m_pDlgMainData->GetSystemManualSocket() )	
					SendMessage(USER_MSG_MOT, 2, 0);
			
				SetSeqIdx(prc_idx, seq_idx, 480);
			}
			else if( SYSTEM_OP_MODE_DRYRUN == m_pDlgMainData->GetSystemOpMode() )
			{
				SetSeqIdx(prc_idx, seq_idx, 480);
			}
			break;
		case 480:	//그랩 종료 응답 체크	//무시하고 진행할 수도 있음
			if( SYSTEM_OP_MODE_AUTORUN == m_pDlgMainData->GetSystemOpMode() )
			{
				if( SYSTEM_MANUAL_SOCKET_ON == m_pDlgMainData->GetSystemManualSocket() )	
				{
					m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_TCPIP_RCV;//ms
					sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "비젼 GRAB END 응답 없음 (2:480)");
					sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_VISION_GRAB_END_TIMEOUT);

					if(m_bGrabEnd)
					{
						m_bGrabEnd = FALSE;

						SetSeqIdx(prc_idx, seq_idx, 490);

					}
				}
				else if( SYSTEM_MANUAL_SOCKET_OFF == m_pDlgMainData->GetSystemManualSocket() )
					SetSeqIdx(prc_idx, seq_idx, 490);
			}
			else if( SYSTEM_OP_MODE_DRYRUN == m_pDlgMainData->GetSystemOpMode() )
			{
				SetSeqIdx(prc_idx, seq_idx, 490);
			}
			break;
		case 490:	// AXIS Y LOAD 위치로 이동
			m_pDlgMainView->SetStateCell(DISPLAY_CELL_TRAY_SHUTTLE, CELL_STATE_OK);	//DISPLAY MAIN VIEW 

			m_lStartTime = GetCurrentTime();//COMPLETE GRAB END

			m_pDlgManual->AxisMove(AXIS_TRY_SHUTTLE, m_pDlgTeach->GetLoadPosY(), 8, FALSE);
			SetSeqIdx(prc_idx, seq_idx, 500);
			break;
		case 500: 
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "TRAY SHUTTLE(Y축) LOAD POS 이동 실패 = (2:500)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_AXIS_Y_MOVEFAIL);

			if(m_pDlgManual->AxisMoveDone(AXIS_TRY_SHUTTLE))
			{
				m_lEndTime = GetCurrentTime();//COMPLETE_GRAB TACT END 20160412
				m_pDlgTackTimeView->SetTactTime(COMPLETE_GRAB, m_lEndTime - m_lStartTime);
				m_pDlgTackTimeView->SetTTUpdate(COMPLETE_GRAB, TRUE);

				SetSeqIdx(prc_idx, seq_idx, 510);
			}
			break;
		case 510:
			m_lStartTime = GetCurrentTime();//IN_JIG_B TACT START 20160412

			m_pDlgManual->WriteOutput(TRAY_SHUTTLE_GLASS_VAC_ON_SOL, FALSE);
			m_pDlgManual->WriteOutput(TRAY_SHUTTLE_GLASS_VAC_OFF_SOL, TRUE);
			Delay(30);
			m_pDlgManual->WriteOutput(TRAY_SHUTTLE_GLASS_VAC_OFF_SOL, FALSE);

			SetSeqIdx(prc_idx, seq_idx, 520);
			break;
		case 520:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = 1000;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "TRAY SHUTTLE VAC 센서 감지 이상 = X02F (2:520)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_SHUTTLE_VAC);

			if(!m_pDlgManual->ReadInput(TRAY_SHUTTLE_GLASS_VAC_OFF_SOL))
			{
				SetSeqIdx(prc_idx, seq_idx, 530);
			}
			break;
		case 530:	// Picker DOWN
			m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_UP_SOL, FALSE);
			m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_DN_SOL, TRUE);

			SetSeqIdx(prc_idx, seq_idx, 535);
			break;
		case 535:	//Picker Down Check
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "PICKER UP 실린더 센서 감지 이상=X00C(2:535)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_PICKERUP);

			if(!m_pDlgManual->ReadInput(LD_PNP_CYLINDER_UP_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 536);
			}
			break;
		case 536:	//Picker Down Check
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "PICKER DOWN 실린더 센서 감지 이상 = X00D(2:536)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_PICKERDOWN);

			if(m_pDlgManual->ReadInput(LD_PNP_CYLINDER_DN_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 540);
			}
			break;
		case 540://Picker VAC ON
			m_pDlgManual->WriteOutput(LD_PNP_VAC_ON_SOL, TRUE);
			m_pDlgManual->WriteOutput(LD_PNP_VAC_OFF_SOL, FALSE);

			SetSeqIdx(prc_idx, seq_idx, 550);
			break;
		case 550:	
			not_delay = CheckDelay(prc_idx, seq_idx, 300);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 560);
			}
			break;
		case 560://Picker VAC CHECK
			if( SYSTEM_OP_MODE_AUTORUN == m_pDlgMainData->GetSystemOpMode() )
			{
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = 1000;//ms
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "PICKER VAC 센서 감지 이상 = X02F (2:560)");	
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_PICKER_VAC);

				if(m_pDlgManual->ReadInput(LD_PNP_VAC_ON_CHECK))
				{
					SetSeqIdx(prc_idx, seq_idx, 570);
				}
			}
			else if( SYSTEM_OP_MODE_DRYRUN == m_pDlgMainData->GetSystemOpMode() )
				SetSeqIdx(prc_idx, seq_idx, 570);
			break;
		case 570: //Picker UP
			m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_UP_SOL, TRUE);
			m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_DN_SOL, FALSE);

			SetSeqIdx(prc_idx, seq_idx, 580);
			break;
		case 580:	// Picker UP Check
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "PICKER UP 실린더 센서 감지 이상=X00C(2:580)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_PICKERUP);

			if(m_pDlgManual->ReadInput(LD_PNP_CYLINDER_UP_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 590);
			}
			break;
		case 590:// Picker UP Check
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "PICKER DOWN 실린더 센서 감지 이상 = X00D (2:590)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_PICKERDOWN);

			if(!m_pDlgManual->ReadInput(LD_PNP_CYLINDER_DN_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 600);
			}
			break;
		case 600://AXIS LD P&P MOVE UNLOAD POSITION
			m_pDlgManual->AxisMove(AXIS_LD_PNP, m_pDlgTeach->GetUnLoadPosX(), 8, FALSE);

			SetSeqIdx(prc_idx, seq_idx, 610);
			break;
		case 610:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "AXIS_LD_PNP(X축) UNLOAD POS 이동 실패 (2:610)");
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_AXIS_X_MOVEFAIL);

			if(m_pDlgManual->AxisMoveDone(AXIS_LD_PNP))
			{
				SetSeqIdx(prc_idx, seq_idx, 680);
			}
			break;
		case 680://Picker Down
			m_pDlgMainView->SetStateCell(DISPLAY_CELL_JIG_B_FORWARD, CELL_STATE_OK);	//DISPLAY MAIN VIEW 

			m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_UP_SOL, FALSE);
			m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_DN_SOL, TRUE);

			SetSeqIdx(prc_idx, seq_idx, 690);
			break;
		case 690:	//Picker Down Check
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "PICKER UP 실린더 센서 감지 이상 = X00C (2:690)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_PICKERUP);

			if(!m_pDlgManual->ReadInput(LD_PNP_CYLINDER_UP_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 700);
			}
			break;
		case 700:	//Picker Down Check
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "PICKER DOWN 실린더 센서 감지 이상 = X00D (2:700)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_PICKERDOWN);

			if(m_pDlgManual->ReadInput(LD_PNP_CYLINDER_DN_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 710);
			}
			break;
		case 710: //PNP VAC OFF
			m_pDlgManual->WriteOutput(LD_PNP_VAC_ON_SOL, FALSE);
			m_pDlgManual->WriteOutput(LD_PNP_VAC_OFF_SOL, TRUE);
			Delay(100);
			m_pDlgManual->WriteOutput(LD_PNP_VAC_OFF_SOL, FALSE);

			SetSeqIdx(prc_idx, seq_idx, 720);
			break;
		case 720:
			not_delay = CheckDelay(prc_idx, seq_idx, 200);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 730);
			}
			break;
		case 730:	//PNP VAC CHECK
			if( SYSTEM_OP_MODE_AUTORUN == m_pDlgMainData->GetSystemOpMode() )
			{
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = 1000;//ms
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "PICKER VAC 센서 감지 이상 = X02F (2:730)");	
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_PICKER_VAC);

				if(!m_pDlgManual->ReadInput(LD_PNP_VAC_ON_CHECK))
				{
					SetSeqIdx(prc_idx, seq_idx, 740);
				}
			}
			else if( SYSTEM_OP_MODE_DRYRUN == m_pDlgMainData->GetSystemOpMode() )
				SetSeqIdx(prc_idx, seq_idx, 740);
			break;
		case 740:
			m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_UP_SOL, TRUE);
			m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_DN_SOL, FALSE);

			SetSeqIdx(prc_idx, seq_idx, 750);
			break;
		case 750:	// Picker UP Check
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "PICKER UP 실린더 센서 감지 이상 = X00C (2:750)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_PICKERUP); 

			if(m_pDlgManual->ReadInput(LD_PNP_CYLINDER_UP_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 760);
			}
			break;
		case 760:// Picker DN Check
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "PICKER DOWN 실린더 센서 감지 이상 = X00D (2:760)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_PICKERDOWN);

			if(!m_pDlgManual->ReadInput(LD_PNP_CYLINDER_DN_CHECK))
			{
				m_lEndTime = GetCurrentTime();//IN_JIG_B TACT END 20160412
				m_pDlgTackTimeView->SetTactTime(IN_JIG_B, m_lEndTime - m_lStartTime);
				m_pDlgTackTimeView->SetTTUpdate(IN_JIG_B, TRUE);

				SetSeqIdx(prc_idx, seq_idx, 765);
			}
			break;
		case 765:
			m_pDlgManual->AxisMove(AXIS_LD_PNP, m_pDlgTeach->GetLoadPosX(), 8, FALSE);

			SetSeqIdx(prc_idx, seq_idx, 770);
			break;
		case 770:	//GLASS / FILM JIG 후진
			m_pDlgMainView->SetStateCell(DISPLAY_CELL_JIG_B_BACK, CELL_STATE_OK);	//DISPLAY MAIN VIEW 

			m_lStartTime = GetCurrentTime();//PASS_JIG TACT START 20160412

			m_pDlgManual->WriteOutput(FILM_JIG_CYLINDER_IN_SOL, FALSE);
			m_pDlgManual->WriteOutput(FILM_JIG_CYLINDER_OUT_SOL, TRUE);
			
			SetSeqIdx(prc_idx, seq_idx, 780);
			break;
		case 780: //FILM JIG 후진 체크
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B 투입 실린더 센서 감지 이상 = X033 (2:780)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_IN); 

			if(!m_pDlgManual->ReadInput(FILM_JIG_CYLINDER_IN_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 790);
			}
			break;
		case 790: //FILM JIG 후진 체크
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B 배출 실린더 센서 감지 이상 = X032 (2:790)");
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_OUT); 

			if(m_pDlgManual->ReadInput(FILM_JIG_CYLINDER_OUT_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 800);
			}
			break;
		case 800: //GLASS JIG 후진 체크
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG A 투입 실린더 센서 감지 이상 = X03E (2:800)");
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGA_IN); 

			if(!m_pDlgManual->ReadInput(GLASS_JIG_CYLINDER_IN_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 810);
			}
			break;
		case 810://GLASS JIG 후진 체크
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG A 배출 실린더 센서 감지 이상 =X 03D (2:810)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGA_OUT); 

			if(m_pDlgManual->ReadInput(GLASS_JIG_CYLINDER_OUT_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 820);
			}
			break;
		case 820:
			m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_FEED_SOL, FALSE);
			m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_BACK_SOL, TRUE);

			SetSeqIdx(prc_idx, seq_idx, 830);
			break;
		case 830:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B 클램프 전진 센서 감지 이상 = X034 (2:830)");
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_CLAMP_FORWARD); 

			if(!m_pDlgManual->ReadInput(FILM_JIG_GLASS_CLAMP_CYLINDER_FEED_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 840);
			}
			break;
		case 840:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B 클램프 후진 센서 감지 이상 = X035 (2:840)");
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_CLAMP_BACK); 

			if(m_pDlgManual->ReadInput(FILM_JIG_GLASS_CLAMP_CYLINDER_BACK_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 845);
			}
			break;
		case 845:
			not_delay = CheckDelay(prc_idx, seq_idx, 300);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 850);
			}
			break;
		case 850:
			m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_CLAMP_SOL, FALSE);
			m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_UNCLAMP_SOL, TRUE);

			SetSeqIdx(prc_idx, seq_idx, 860);
			break;
		case 860:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B 클램프 하강 센서 감지 이상 = X036 (2:860)");
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_CLAMP_DOWN); 

			if(!m_pDlgManual->ReadInput(FILM_JIG_GLASS_CLAMP_CYLINDER_CLAMP_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 870);
			}
			break;
		case 870:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B 클램프 상승 센서 감지 이상 = X037 (2:870)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_CLAMP_UP); 

			if(m_pDlgManual->ReadInput(FILM_JIG_GLASS_CLAMP_CYLINDER_UNCLAMP_CHECK))
			{
				m_lEndTime = GetCurrentTime();//PASS_JIG TACT END 20160412
				m_pDlgTackTimeView->SetTactTime(PASS_JIG, m_lEndTime - m_lStartTime);
				m_pDlgTackTimeView->SetTTUpdate(PASS_JIG, TRUE);

				m_pDlgTackTimeView->SetTTUpdate(TOTAL_TACT_TIME, TRUE);
				m_pDlgTackTimeView->WriteTactLog();

				m_strEndTime = CurTime.Format(_T("%y%m%d%H%M%S"));

				SetSeqIdx(prc_idx, seq_idx, 875);
			}
			break;
		case 875:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MOTOR_MOVE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "AXIS_LD_PNP(X축) UNLOAD POS 이동 실패 (2:875)");
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_AXIS_X_MOVEFAIL);

			if(m_pDlgManual->AxisMoveDone(AXIS_LD_PNP))
			{
				if( SYSTEM_MANUAL_MUTING_OFF == m_pDlgMainData->GetSystemManualMuting() )	//Auto Muting
							m_pDlgManual->WriteOutput(LIGHT_CURTAIN_MUTING_ON, TRUE);	

				SetSeqIdx(prc_idx, seq_idx, 880);
			}
			break;
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 시퀀스 끝 종료 처리 : 임시로 무시함.20160511
		/*
		case 875:	//그랩 종료 응답 체크	//검사 결과 디스플레이
			if( SYSTEM_OP_MODE_AUTORUN == m_pDlgMainData->GetSystemOpMode() )
			{
				if( SYSTEM_MANUAL_SOCKET_ON == m_pDlgMainData->GetSystemManualSocket() )	
				{
					m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_TCPIP_RCV;//ms
					sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "비젼 INSPECT RESULT 응답 없음 (2:875)");
					sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_VISION_RESULT_TIMEOUT); 

					//Product result Update
					if(m_bInspectEnd)
					{
						m_bInspectEnd = FALSE;

						switch(m_nVisionResult)
						{
						case BIN_OK:
							m_lPassCell++;
							break;
						case BIN_REJECT:
							
							break;
						case BIN_NG:
							m_lFailCell++;
							break;
						}

						m_lTotalCell++;
						m_dFailRatioCell = (m_lFailCell / m_lTotalCell) * 100;

						strTemp.Format(_T("%s,%s,%s,%d"), m_strStartTime, m_strEndTime, m_strInnerID, m_nVisionResult);
						WriteLog(strTemp, RESULT_LOG);

						SetSeqIdx(prc_idx, seq_idx, 880);
					}

					SetSeqIdx(prc_idx, seq_idx, 880);
				}
				else if( SYSTEM_MANUAL_SOCKET_OFF == m_pDlgMainData->GetSystemManualSocket() )
					SetSeqIdx(prc_idx, seq_idx, 880);
			}
			else if( SYSTEM_OP_MODE_DRYRUN == m_pDlgMainData->GetSystemOpMode() )
			{
				SetSeqIdx(prc_idx, seq_idx, 880);
			}
			break;
		*/
		case 880:
			if( SYSTEM_OP_MODE_AUTORUN == m_pDlgMainData->GetSystemOpMode() )
				//SetSeqIdx(prc_idx, seq_idx, 890, 891, 1);
				SetSeqIdx(prc_idx, seq_idx, 891);
			else if( SYSTEM_OP_MODE_DRYRUN == m_pDlgMainData->GetSystemOpMode() )
				SetSeqIdx(prc_idx, seq_idx, 891);
			break;
		/*
		case 890:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_MESSAGE;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "검사가 완료 되었습니다.");	
			break;
		*/
		case 891:
			SetSeqIdx(prc_idx, seq_idx, 100);
			break;

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Film 제거 동작
		case 1000:
			if( SYSTEM_OP_MODE_AUTORUN == m_pDlgMainData->GetSystemOpMode() )
			{
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B 제품이 없습니다. = X030 (2:1000)");	
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_EMPTY); 

				if(m_pDlgManual->ReadInput(FILM_JIG_GLASS_CHECK))
				{
					m_lStartTime = GetCurrentTime();//FILM_MOVE TACT START 20160412
					m_pDlgTackTimeView->InitialTotalTactTime();	// TOTAL TACT 초기화

					m_pDlgManual->WriteOutput(FILM_JIG_VAC_ON_SOL, TRUE);
					m_pDlgManual->WriteOutput(FILM_JIG_VAC_OFF_SOL, FALSE);

					m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_CLAMP_SOL, TRUE);
					m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_UNCLAMP_SOL, FALSE);

					SetSeqIdx(prc_idx, seq_idx, 1010);
				}
			}
			else if( SYSTEM_OP_MODE_DRYRUN == m_pDlgMainData->GetSystemOpMode() )
			{
				m_pDlgTackTimeView->InitialTotalTactTime();	// TOTAL TACT 초기화

				// FILM MOVE T/T 측정 시작
				m_pDlgManual->WriteOutput(FILM_JIG_VAC_ON_SOL, TRUE);
				m_pDlgManual->WriteOutput(FILM_JIG_VAC_OFF_SOL, FALSE);

				m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_CLAMP_SOL, TRUE);
				m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_UNCLAMP_SOL, FALSE);

				SetSeqIdx(prc_idx, seq_idx, 1010);
			}
			break;
		case 1010:
			if( SYSTEM_OP_MODE_AUTORUN == m_pDlgMainData->GetSystemOpMode() )
			{
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B VAC 센서 감지 이상 = X031 (2:1010)");	
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_VAC_FAIL); 

				if(m_pDlgManual->ReadInput(FILM_JIG_VAC_ON_CHECK))
				{
					SetSeqIdx(prc_idx, seq_idx, 1020);
				}
			}
			else if( SYSTEM_OP_MODE_DRYRUN == m_pDlgMainData->GetSystemOpMode() )
				SetSeqIdx(prc_idx, seq_idx, 1020);
			break;
		case 1020:
			not_delay = CheckDelay(prc_idx, seq_idx, 200);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 1030);
			}
			break;
		case 1030:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B 클램프 하강 센서 감지 이상 = X036 (2:1030)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_CLAMP_DOWN); 

			if(m_pDlgManual->ReadInput(FILM_JIG_GLASS_CLAMP_CYLINDER_CLAMP_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 1040);
			}
			break;
		case 1040:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B 클램프 상승 센서 감지 이상 = X037 (2:1040)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_CLAMP_UP); 

			if(!m_pDlgManual->ReadInput(FILM_JIG_GLASS_CLAMP_CYLINDER_UNCLAMP_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 1050);
			}
			break;
		case 1050:
			not_delay = CheckDelay(prc_idx, seq_idx, 100);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 1060);
			}
			break;
		case 1060:
			m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_FEED_SOL, TRUE);
			m_pDlgManual->WriteOutput(FILM_JIG_GLASS_CLAMP_CYLINDER_BACK_SOL, FALSE);

			SetSeqIdx(prc_idx, seq_idx, 1070);
			break;
		case 1070:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B 클램프 전진 센서 감지 이상 = X034 (2:1070)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_CLAMP_FORWARD); 

			if(m_pDlgManual->ReadInput(FILM_JIG_GLASS_CLAMP_CYLINDER_FEED_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 1080);
			}
			break;
		case 1080:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B 클램프 후진 센서 감지 이상 = X035 (2:1080)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_CLAMP_BACK); 

			if(!m_pDlgManual->ReadInput(FILM_JIG_GLASS_CLAMP_CYLINDER_BACK_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 1090);
			}
			break;
		case 1090:
			not_delay = CheckDelay(prc_idx, seq_idx, 100);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 1100);
			}
			break;
		case 1100:
			m_pDlgManual->WriteOutput(FILM_JIG_TILTING_CYLINDER_TILT_DN_SOL, TRUE);
			m_pDlgManual->WriteOutput(FILM_JIG_TILTING_CYLINDER_TILT_UP_SOL, FALSE);

			m_pDlgManual->WriteOutput(FILM_JIG_VAC_PLATE_CYLINDER_DN_SOL, TRUE);
			m_pDlgManual->WriteOutput(FILM_JIG_VAC_PLATE_CYLINDER_UP_SOL, FALSE);

			SetSeqIdx(prc_idx, seq_idx, 1110);
			break;
		case 1110:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B 틸트 DOWN 센서 감지 이상 = X039 (2:1110)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_TILT_DOWN); 

			if(m_pDlgManual->ReadInput(FILM_JIG_TILTING_CYLINDER_TILT_DN_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 1120);
			}
			break;
		case 1120:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B 틸트 UP 센서 감지 이상 = X038 (2:1120)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_TILT_UP); 

			if(!m_pDlgManual->ReadInput(FILM_JIG_TILTING_CYLINDER_TILT_UP_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 1130);
			}
			break;
		case 1130:
				SetSeqIdx(prc_idx, seq_idx, 1140);			
			break;
		case 1140:
			//m_pDlgManual->WriteOutput(FILM_JIG_VAC_PLATE_CYLINDER_DN_SOL, TRUE);
			//m_pDlgManual->WriteOutput(FILM_JIG_VAC_PLATE_CYLINDER_UP_SOL, FALSE);

			SetSeqIdx(prc_idx, seq_idx, 1150);
			break;
		case 1150:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B VAC PLATE DOWN 센서 감지 이상 = X03B (2:1150)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_PLATE_DOWN); 

			if(m_pDlgManual->ReadInput(FILM_JIG_VAC_PLATE_CYLINDER_DN_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 1160);
			}
			break;
		case 1160:
			m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B VAC PLATE UP 센서 감지 이상 = X03A (2:1160)");	
			sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_PLATE_UP); 

			if(!m_pDlgManual->ReadInput(FILM_JIG_VAC_PLATE_CYLINDER_UP_CHECK))
			{
				SetSeqIdx(prc_idx, seq_idx, 1170);
			}
			break;
		case 1170:
			not_delay = CheckDelay(prc_idx, seq_idx, 100);
			if(not_delay)
			{
				SetSeqIdx(prc_idx, seq_idx, 1180);
			}
			break;
		case 1180:
			m_pDlgManual->WriteOutput(FILM_JIG_VAC_ON_SOL, FALSE);
			m_pDlgManual->WriteOutput(FILM_JIG_VAC_OFF_SOL, TRUE);
			Delay(100);
			m_pDlgManual->WriteOutput(FILM_JIG_VAC_OFF_SOL, FALSE);
			
			SetSeqIdx(prc_idx, seq_idx, 1190);
			break;
		case 1190:
			if( SYSTEM_OP_MODE_AUTORUN == m_pDlgMainData->GetSystemOpMode() )
			{
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_CYL_SENSOR;//ms
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, "JIG B VAC 센서 감지 이상 = X031 (1190)");	
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, ALARM_CODE_JIGB_VAC_FAIL); 

				if(!m_pDlgManual->ReadInput(FILM_JIG_VAC_ON_CHECK))
				{
					SetSeqIdx(prc_idx, seq_idx, 1700);
				}
			}
			else if( SYSTEM_OP_MODE_DRYRUN == m_pDlgMainData->GetSystemOpMode() )
				SetSeqIdx(prc_idx, seq_idx, 1700);
			break;
		case 1700:		//FILM Remove 완료
			m_lEndTime = GetCurrentTime();//FILM_MOVE TACT END 20160412
			m_pDlgTackTimeView->SetTactTime(COMPLETE_FILM_REMOVE, m_lEndTime - m_lStartTime);
			m_pDlgTackTimeView->SetTTUpdate(COMPLETE_FILM_REMOVE, TRUE);

			SetSeqIdx(prc_idx, seq_idx, m_Rslt.m_iFilmRemoveSeq);
			break;
		}
		break;
	}

	if(chk_timeout)
	{
		rslt = CheckTimeout(prc_idx, seq_idx, m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx, status_type);
	}


	return rslt;
}

//시퀀스 인덱스 넣기(리셋후 다음동작 않함)
void CSDC_WindowControlPrjDlg::SetSeqIdx(int prc_idx, int seq_idx, int seq_sub_idx)
{
	m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_iNGSeqIdx = 0;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx = seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iDispSubSeqIdx = seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iNextSubSeqIdx = seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_bOpSts = 0;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iDeviceIdx = 0;
}

//시퀀스 인덱스 넣기(리셋하며 다음동작으로 넘어감)
void CSDC_WindowControlPrjDlg::SetSeqIdx(int prc_idx, int seq_idx, int seq_sub_idx, int next_seq_sub_idx, int type)
{
	m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_iNGSeqIdx = 0;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx = seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iDispSubSeqIdx = seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iNextSubSeqIdx = next_seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_bOpSts = type;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iDeviceIdx = 0;
}

void CSDC_WindowControlPrjDlg::SetSeqIdx(int prc_idx, int seq_idx, int seq_sub_idx, int next_seq_sub_idx, int type, int device_idx)
{
	m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_iNGSeqIdx = 0;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx = seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iDispSubSeqIdx = seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iNextSubSeqIdx = next_seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_bOpSts = type;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iDeviceIdx = device_idx;
}

//Check timeout
int CSDC_WindowControlPrjDlg::CheckTimeout(int prc_idx, int seq_idx, int seq_sub_idx, char* status_type)
{

	if(0 == m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_iNGSeqIdx)
	{
		m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_iNGSeqIdx++;
		m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_dTimeoutS = _TimerCounter;
	}
	else
	{
		//Compare timeout
		if(0 <m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_dTimeout)
		{
			m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_dTimeoutE = _TimerCounter;
			if( m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_dTimeout < (m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_dTimeoutE - m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_dTimeoutS) )
			{
				return 0;
			}
		}
	}

	sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_iTypeStatus, "%s", status_type);//축번호

	return 1;
}

//Check timeout
int CSDC_WindowControlPrjDlg::CheckDelay(int prc_idx, int seq_idx, int delay)
{
	if(0 == m_MDat[prc_idx].m_ProSts[seq_idx][m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx].m_iNGSeqIdx)
	{
		m_MDat[prc_idx].m_ProSts[seq_idx][m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx].m_iNGSeqIdx++;
		m_MDat[prc_idx].m_ProSts[seq_idx][m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx].m_dTimeoutS = _TimerCounter;
	}
	else
	{
		//Compare timeout
		if(0 < delay)
		{
			m_MDat[prc_idx].m_ProSts[seq_idx][m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx].m_dTimeoutE = _TimerCounter;
			if( delay < (m_MDat[prc_idx].m_ProSts[seq_idx][m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx].m_dTimeoutE - m_MDat[prc_idx].m_ProSts[seq_idx][m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx].m_dTimeoutS) )
			{
				return 1;
			}
		}
		else
		{
			return 1;
		}
	}

	return 0;
}


int CSDC_WindowControlPrjDlg::StopModeOperation(int *prc_idx, int *seq_idx)
{
	BOOL rslt = TRUE;;

	*prc_idx = 0;
	*seq_idx = 0;

	if(m_MDat[*prc_idx].m_Seq[*seq_idx].m_bAlarm)
	{
		return 1;
	}

	//Safe and limit sensor check
	if(m_pDlgManual->ReadInput(OP_EMS_CHECK))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = OP_EMS_CHECK;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "EMO 버튼 누름 감지! = X008");
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGCode, ALARM_CODE_EMO);

		return 0;
	}

	if(!m_pDlgManual->ReadInput(GPS_POWER_FAILURE_ALARM_CHECK))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = GPS_POWER_FAILURE_ALARM_CHECK;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "GPS Power 이상 감지! = X002");
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGCode, ALARM_CODE_GPSPOWER);
		

		return 0;
	}

	if(!m_pDlgManual->ReadInput(SERVO_MC_ON_OFF_CHECK))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = SERVO_MC_ON_OFF_CHECK;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "SERVO MC OFF. 리셋버튼을 1초간 눌러 초기화해주세요. = X003");	
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGCode, ALARM_CODE_SERVOMC);

		return 0;
	}

	if(!m_pDlgManual->ReadInput(MAIN_AIR_CHECK))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = MAIN_AIR_CHECK;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "Main Air 센서 감지 이상 = X025");
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGCode, ALARM_CODE_MAINAIR);

		return 0;
	}

	if( SYSTEM_MANUAL_MUTING_ON == m_pDlgMainData->GetSystemManualMuting() )
	{
		if(m_pDlgManual->ReadOutPut(LIGHT_CURTAIN_MUTING_ON))
			m_pDlgManual->WriteOutput(LIGHT_CURTAIN_MUTING_ON, FALSE);
	}
	else if( SYSTEM_MANUAL_MUTING_OFF == m_pDlgMainData->GetSystemManualMuting() )
	{
		if(!m_pDlgManual->ReadOutPut(LIGHT_CURTAIN_MUTING_ON))
			m_pDlgManual->WriteOutput(LIGHT_CURTAIN_MUTING_ON, TRUE);
	}
	/*
	if(m_bClickDoorLock)
	{
		if(!m_pDlgManual->ReadInput(FRONT_RIGHT_DOOR_CHECK))
		{
			m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = FRONT_RIGHT_DOOR_CHECK;
			m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "정면 우측 문 열림! = X005");
			sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGCode, ALARM_CODE_DOOR_FR);
		}

		if(!m_pDlgManual->ReadInput(FRONT_LEFT_DOOR_CHECK))
		{
			m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = FRONT_LEFT_DOOR_CHECK;
			m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "정면 좌측 문 열림! = X004");
			sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGCode, ALARM_CODE_DOOR_FL);
		}

		if(!m_pDlgManual->ReadInput(REAR_RIGHT_DOOR_CHECK))
		{
			m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = REAR_RIGHT_DOOR_CHECK;
			m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "후면 우측 문 열림! = X007");
			sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGCode, ALARM_CODE_DOOR_RR);
		}

		if(!m_pDlgManual->ReadInput(REAR_LEFT_DOOR_CHECK))
		{
			m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = REAR_LEFT_DOOR_CHECK;
			m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
			sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, "후면 좌측 문 열림! = X007");
			sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGCode, ALARM_CODE_DOOR_RL);
		}
	}
	*/
	return TRUE;
}

void CSDC_WindowControlPrjDlg::SetLotInit()
{
	m_MDat[0].m_Seq[1].m_iSubSeqIdx = 100;
	m_MDat[0].m_Seq[2].m_iSubSeqIdx = 100;

	//Data 초기화
	memset(&m_Rslt, NULL, sizeof(typeProcessResult));

}


int CSDC_WindowControlPrjDlg::AutoStart()
{
	int i, j;

	//First start
	if(!m_Flag.m_bFirstStart)
	{
		m_Flag.m_bFirstStart = TRUE;

		//검사시작
		SetLotInit();	//재시작
		ClickMainOpInit();	//IMSI
	}

	for(i = 0; i < MOT_PROCESS_MAX_NUM; i++)
	{
		for(j = 0; j < MOT_STATUS_MAX_NUM; j++)
		{
			m_Flag.m_bAutoStart = TRUE;
			m_MDat[i].m_Seq[j].m_bAutoStart = m_Flag.m_bAutoStart;
		}
	}

	((CButton*)GetDlgItem(IDB_MAIN_OP_INIT))->EnableWindow(FALSE);

	return 0;
}


void CSDC_WindowControlPrjDlg::AutoStop()
{
	int i, j;	

	for(i = 0; i < MOT_PROCESS_MAX_NUM; i++)
	{
		for(j = 0; j < MOT_STATUS_MAX_NUM; j++)
		{
			m_Flag.m_bAutoStart = FALSE;
			m_MDat[i].m_Seq[j].m_bAutoStart = m_Flag.m_bAutoStart;
		}
	}

	((CButton*)GetDlgItem(IDB_MAIN_OP_INIT))->EnableWindow(TRUE);
}

int CSDC_WindowControlPrjDlg::AutoReset()
{
	int i, j;	
	int prc_idx, seq_idx, sub_seq_idx;
	prc_idx = 0;
	seq_idx = 0;
	sub_seq_idx = 0;

	for(i = 0; i < MOT_PROCESS_MAX_NUM; i++)
	{
		for(j = 0; j < MOT_STATUS_MAX_NUM; j++)
		{
			if(m_MDat[i].m_Seq[j].m_bAlarm)
			{
				prc_idx = i;
				seq_idx = j;
				sub_seq_idx =  m_MDat[i].m_Seq[j].m_iSubSeqIdx;

			}
			m_MDat[i].m_Seq[j].m_bAlarm = FALSE;
		}
	}

	m_Flag.m_bAlarm = FALSE;	//Display Status 참조용

	//int category, code, level;
	//MakeAlarmCode(prc_idx, seq_idx, sub_seq_idx, &category, &code, &level);
	
	//비젼 관련 에러는 여기서 처리한다. 
	switch(seq_idx)
	{
	case 0:

		break;
	case 1:

		break;
	case 2:
		switch(sub_seq_idx)
		{
		case 253:
		case 255:
			m_MDat[0].m_Seq[seq_idx].m_iSubSeqIdx = 250;		//레디 버튼 입력 대기로 이동
			break;
		case 268:
		case 270:
			m_MDat[0].m_Seq[seq_idx].m_iSubSeqIdx = 265;		//스타트 버튼 입력 대기로 이동
			break;
		case 450: //그랩 스타트 전송
		case 480:	//그랩 종료 전송
			m_MDat[0].m_Seq[seq_idx].m_iSubSeqIdx = 430;		//그랩 시작 위치로 이동
			break;

		default:

			break;
		}
		break;
	}

	m_dlgLogAlarm->ShowWindow(SW_HIDE);

	//Button
	m_ctrlOpStart.SetEnabled(TRUE);
	m_ctrlOpStart.SetValue(TRUE);
	m_ctrlOpStop.SetEnabled(FALSE);
	m_ctrlOpStop.SetValue(FALSE);
	m_ctrlOpReset.SetEnabled(FALSE);
	m_ctrlOpReset.SetValue(FALSE);
	m_ctrlOpDoor.SetEnabled(TRUE);

			
	m_Flag.m_bAutoStart = FALSE;

	m_pDlgManual->SetLampWait();
	m_pDlgManual->WriteOutput(RESET_SWITCH_LAMP, FALSE);

	//Buzzer off
	//m_pDlgManual->WriteOutput(BUZZER_ERROR_SIGNAL, FALSE);

	return 0;
}

void CSDC_WindowControlPrjDlg::AlarmOperation(int pro_idx, int sts_idx, int sub_seq_idx)
{
	/*
	int category, code, level;

	MakeAlarmCode(pro_idx, sts_idx, sub_seq_idx, &category, &code, &level);

	//ECS/////////////////////////////
	//Alarm

	if(0 == level)
	{
		m_Inline->SetLightAlarmReport(TRUE, code);
	}
	else
	{
		m_Inline->SetDownAlarmCode(code);
		m_Inline->SetHeavyAlarmReport(TRUE, code);
	}
	
	//Opeation mode
	m_Inline->SetBitOModeUnitAuto(FALSE);
	m_Inline->SetBitOModeOperation_AutoOrManual(FALSE);

	//Equipment status->Alarm일때만 ECS내부에서 처리
	*/
}

LRESULT CSDC_WindowControlPrjDlg::OnMotMessage(WPARAM para0, LPARAM para1)
{
	// TODO: Add your control notification handler code here
	int i, j;
	CString strPacket, strTemp, strTemp2;
	
	switch(para0)
	{
	//Alarm
	case 0:
		for(i = 0; i < MOT_PROCESS_MAX_NUM; i++)
		{
			for(j = 0; j < MOT_STATUS_MAX_NUM; j++)
			{
				if(m_MDat[i].m_Seq[j].m_bAlarm)
				{
					if(0 < m_MDat[i].m_Seq[j].m_bOpSts)
					{
						switch(m_MDat[i].m_Seq[j].m_bOpSts)
						{
						case 1:			// 검사완료 IMSI
							AutoReset();
							AutoStart();
							break;
						case 2:

							break;
						default:

							break;
						}
						break;
					}
					else
					{
						if(SYSTEM_MANUAL_MUTING_OFF == m_pDlgMainData->GetSystemManualMuting())
						{
							if(!m_pDlgManual->ReadInput(LIGHT_CURTAIN_MUTING_START_CHECK_AUTO))
								m_pDlgManual->WriteOutput(LIGHT_CURTAIN_MUTING_ON, TRUE); // STOP 상태에서는 뮤팅 무효화 ON
						}

						m_dlgLogAlarm->SetMessage(m_MDat[i].m_ProSts[j][m_MDat[i].m_Seq[j].m_iSubSeqIdx].m_cNGCode, 
																		m_MDat[i].m_ProSts[j][m_MDat[i].m_Seq[j].m_iSubSeqIdx].m_cNGStatus);

						m_dlgLogAlarm->ShowWindow(SW_SHOW);

						m_pDlgMainView->SetStateNG();

						//ALARM LOG
						strTemp.Format(_T("%s"), CA2T(m_MDat[i].m_ProSts[j][m_MDat[i].m_Seq[j].m_iSubSeqIdx].m_cNGStatus));
						WriteLog(strTemp, ALARM_LOG);


						strTemp.Format(_T("%s"), CA2T(m_MDat[i].m_ProSts[j][m_MDat[i].m_Seq[j].m_iSubSeqIdx].m_cNGStatus));
						strTemp2.Format(_T("%s"), CA2T(m_MDat[i].m_ProSts[j][m_MDat[i].m_Seq[j].m_iSubSeqIdx].m_cNGCode));
						m_pDlgAlarmTrendView->AddMachineErrorChart(strTemp2, strTemp);

						m_ctrlOpStart.SetEnabled(FALSE);
						m_ctrlOpStart.SetValue(FALSE);
						m_ctrlOpStop.SetEnabled(FALSE);
						m_ctrlOpStop.SetValue(FALSE);
						m_ctrlOpReset.SetEnabled(TRUE);
						m_ctrlOpReset.SetValue(TRUE);

						m_pDlgManual->SetLampError();
						m_pDlgManual->WriteOutput(RESET_SWITCH_LAMP, TRUE);
						break;
					}
				}
			}
		}
		break;
	case 1:	// Grab Start
		//패킷 파싱
		//OTM.RECEIVE.GRAB.START.InnerID	그랩 스타트 응답
		//OTM.RECEIVE.GRAB.END.InnerID		그랩 완료 응답
		//OTM.SEND.INSPECT.RESULT.bin.InnerID 검사완료 패킷
		//	OTM.SEND.SWERROR.Message내용	검사에러 패킷

		strTemp.Format(_T("MTO.SEND.GRAB.START.%s"), m_strInnerID);
		strPacket.Format(_T("%c0000%04d%s%c"), STX, strTemp.GetLength(), strTemp, ETX);
		OnSend(strPacket);
		strPacket.Delete(0, 1);
		strPacket.Delete(strPacket.GetLength()-1, 1);
		WriteLog(strPacket, PACKET_LOG);
		break;
	case 2:	// Grab End
		strTemp.Format(_T("MTO.SEND.GRAB.END.%s"),  m_strInnerID);
		strPacket.Format(_T("%c0000%04d%s%c"), STX, strTemp.GetLength(), strTemp, ETX);
		OnSend(strPacket);
		strPacket.Delete(0, 1);
		strPacket.Delete(strPacket.GetLength()-1, 1);
		WriteLog(strPacket, PACKET_LOG);
		break;
	case 3:	//Result Ack
		strTemp.Format(_T("MTO.RECEIVE.INSPECT.RESULT.%d.%s"), m_nVisionResult, m_strInnerID);
		strPacket.Format(_T("%c0000%04d%s%c"), STX, strTemp.GetLength(), strTemp, ETX);
		OnSend(strPacket);
		strPacket.Delete(0, 1);
		strPacket.Delete(strPacket.GetLength()-1, 1);
		WriteLog(strPacket, PACKET_LOG);
		break;
	default:
		AfxMessageBox(_T("Error : invalid index!(CEMIControlDlg::OnMotMessage)"));
		break;
	}

	return 0;
}


////////////////////////////////////////////////////////////////////////////////////


void CSDC_WindowControlPrjDlg::ClickLogviewBtn()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString strPath;
	strPath.Format(_T("%s"), SYSYEM_LOG_PATH); // 여기에 경로를.. 
	ShellExecute(NULL, _T("open"), _T("explorer"), _T("/select,") + strPath, NULL, SW_SHOW);
}


void CSDC_WindowControlPrjDlg::ClickMainProductReset()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(g_cMsgBox.YesNoMsgBox(_T("생산 정보를 정말 초기화 합니까?")) == IDYES)
	{
		m_lTotalCell = 0;
		m_lPassCell = 0;
		m_lFailCell = 0;
		m_dFailRatioCell = 0.;
	}
}
