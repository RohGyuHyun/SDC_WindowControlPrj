// DialogTackTimeView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SDC_WindowControlPrj.h"
#include "DialogTackTimeView.h"
#include "afxdialogex.h"


// CDialogTackTimeView 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogTackTimeView, CDialog)

CDialogTackTimeView::CDialogTackTimeView(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogTackTimeView::IDD, pParent)
{
	for(int i = 0; i < MAX_TT_UNIT_CNT; i++)
	{
		m_bTTUpdate[i] = FALSE;

		m_lTactTime[i] = 0;
		m_lMinTactTime[i] = 0;
		m_lMaxTactTime[i] = 0;
	}
}

CDialogTackTimeView::~CDialogTackTimeView()
{
}

void CDialogTackTimeView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TITLE_UNIT_NAME,   m_lblUnitName[COMPLETE_FILM_REMOVE]);
	DDX_Control(pDX, IDC_TITLE_UNIT_NAME2,  m_lblUnitName[INPUT_START_BUTTON]);
	DDX_Control(pDX, IDC_TITLE_UNIT_NAME3,  m_lblUnitName[INSERT_JIG]);
	DDX_Control(pDX, IDC_TITLE_UNIT_NAME4,  m_lblUnitName[IN_SHUTTLE]);
	DDX_Control(pDX, IDC_TITLE_UNIT_NAME5,  m_lblUnitName[SHUTTLE_MOVE_GRAB_START]);
	DDX_Control(pDX, IDC_TITLE_UNIT_NAME6,  m_lblUnitName[SHUTTLE_MOVE_GRAB_END]);
	DDX_Control(pDX, IDC_TITLE_UNIT_NAME7,  m_lblUnitName[COMPLETE_GRAB]);
	DDX_Control(pDX, IDC_TITLE_UNIT_NAME8,  m_lblUnitName[IN_JIG_B]);
	DDX_Control(pDX, IDC_TITLE_UNIT_NAME9,  m_lblUnitName[PASS_JIG]);
	DDX_Control(pDX, IDC_TITLE_UNIT_NAME10,  m_lblUnitName[TOTAL_TACT_TIME]);
	DDX_Control(pDX, IDS_UNIT_TACTTIME,   m_lblUnitTactTime[COMPLETE_FILM_REMOVE]);
	DDX_Control(pDX, IDS_UNIT_TACTTIME2,  m_lblUnitTactTime[INPUT_START_BUTTON]);
	DDX_Control(pDX, IDS_UNIT_TACTTIME3,  m_lblUnitTactTime[INSERT_JIG]);
	DDX_Control(pDX, IDS_UNIT_TACTTIME4,  m_lblUnitTactTime[IN_SHUTTLE]);
	DDX_Control(pDX, IDS_UNIT_TACTTIME5,  m_lblUnitTactTime[SHUTTLE_MOVE_GRAB_START]);
	DDX_Control(pDX, IDS_UNIT_TACTTIME6,  m_lblUnitTactTime[SHUTTLE_MOVE_GRAB_END]);
	DDX_Control(pDX, IDS_UNIT_TACTTIME7,  m_lblUnitTactTime[COMPLETE_GRAB]);
	DDX_Control(pDX, IDS_UNIT_TACTTIME8,  m_lblUnitTactTime[IN_JIG_B]);
	DDX_Control(pDX, IDS_UNIT_TACTTIME9,  m_lblUnitTactTime[PASS_JIG]);
	DDX_Control(pDX, IDS_UNIT_TACTTIME10,  m_lblUnitTactTime[TOTAL_TACT_TIME]);
	DDX_Control(pDX, IDS_UNIT_MIN_TACTTIME,   m_lblUnitMinTactTime[COMPLETE_FILM_REMOVE]);
	DDX_Control(pDX, IDS_UNIT_MIN_TACTTIME2,  m_lblUnitMinTactTime[INPUT_START_BUTTON]);
	DDX_Control(pDX, IDS_UNIT_MIN_TACTTIME3,  m_lblUnitMinTactTime[INSERT_JIG]);
	DDX_Control(pDX, IDS_UNIT_MIN_TACTTIME4,  m_lblUnitMinTactTime[IN_SHUTTLE]);
	DDX_Control(pDX, IDS_UNIT_MIN_TACTTIME5,  m_lblUnitMinTactTime[SHUTTLE_MOVE_GRAB_START]);
	DDX_Control(pDX, IDS_UNIT_MIN_TACTTIME6,  m_lblUnitMinTactTime[SHUTTLE_MOVE_GRAB_END]);
	DDX_Control(pDX, IDS_UNIT_MIN_TACTTIME7,  m_lblUnitMinTactTime[COMPLETE_GRAB]);
	DDX_Control(pDX, IDS_UNIT_MIN_TACTTIME8,  m_lblUnitMinTactTime[IN_JIG_B]);
	DDX_Control(pDX, IDS_UNIT_MIN_TACTTIME9,  m_lblUnitMinTactTime[PASS_JIG]);
	DDX_Control(pDX, IDS_UNIT_MIN_TACTTIME10,  m_lblUnitMinTactTime[TOTAL_TACT_TIME]);
	DDX_Control(pDX, IDS_UNIT_MAX_TACTTIME,   m_lblUnitMaxTactTime[COMPLETE_FILM_REMOVE]);
	DDX_Control(pDX, IDS_UNIT_MAX_TACTTIME2,  m_lblUnitMaxTactTime[INPUT_START_BUTTON]);
	DDX_Control(pDX, IDS_UNIT_MAX_TACTTIME3,  m_lblUnitMaxTactTime[INSERT_JIG]);
	DDX_Control(pDX, IDS_UNIT_MAX_TACTTIME4,  m_lblUnitMaxTactTime[IN_SHUTTLE]);
	DDX_Control(pDX, IDS_UNIT_MAX_TACTTIME5,  m_lblUnitMaxTactTime[SHUTTLE_MOVE_GRAB_START]);
	DDX_Control(pDX, IDS_UNIT_MAX_TACTTIME6,  m_lblUnitMaxTactTime[SHUTTLE_MOVE_GRAB_END]);
	DDX_Control(pDX, IDS_UNIT_MAX_TACTTIME7,  m_lblUnitMaxTactTime[COMPLETE_GRAB]);
	DDX_Control(pDX, IDS_UNIT_MAX_TACTTIME8,  m_lblUnitMaxTactTime[IN_JIG_B]);
	DDX_Control(pDX, IDS_UNIT_MAX_TACTTIME9,  m_lblUnitMaxTactTime[PASS_JIG]);
	DDX_Control(pDX, IDS_UNIT_MAX_TACTTIME10,  m_lblUnitMaxTactTime[TOTAL_TACT_TIME]);
}


BEGIN_MESSAGE_MAP(CDialogTackTimeView, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDialogTackTimeView 메시지 처리기입니다.

BOOL CDialogTackTimeView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_lblUnitName[COMPLETE_FILM_REMOVE].SetCaption(_T("COMPLETE_FILM_REMOVE"));
	m_lblUnitName[INPUT_START_BUTTON].SetCaption(_T("INPUT_START_BUTTON"));
	m_lblUnitName[INSERT_JIG].SetCaption(_T("INSERT_JIG")); 
	m_lblUnitName[IN_SHUTTLE].SetCaption(_T("IN_SHUTTLE"));
	m_lblUnitName[SHUTTLE_MOVE_GRAB_START].SetCaption(_T("SHUTTLE_MOVE_GRAB_START"));
	m_lblUnitName[SHUTTLE_MOVE_GRAB_END].SetCaption(_T("SHUTTLE_MOVE_GRAB_END"));
	m_lblUnitName[COMPLETE_GRAB].SetCaption(_T("COMPLETE_GRAB"));
	m_lblUnitName[IN_JIG_B].SetCaption(_T("IN_JIG_B"));
	m_lblUnitName[PASS_JIG].SetCaption(_T("PASS_JIG"));
	m_lblUnitName[TOTAL_TACT_TIME].SetCaption(_T("TOTAL_TACT_TIME"));
	
	for(int i = 0; i < MAX_TT_UNIT_CNT; i++)
	{
		// Last
		m_lblUnitTactTime[i].SetCaption(_T("0.000s"));
		// Max
		m_lblUnitMaxTactTime[i].SetCaption(_T("0.000s"));
		// Min
		m_lblUnitMinTactTime[i].SetCaption(_T("0.000s"));
	}

	SetTimer(TIMER_MAIN_TACT_TIME_INFO, 100, NULL); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialogTackTimeView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(bShow)
	{
		SetTimer(TIMER_MAIN_TACT_TIME_INFO,100,NULL);
	}
	else
	{
		KillTimer(TIMER_MAIN_TACT_TIME_INFO);
	}

}


void CDialogTackTimeView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case TIMER_MAIN_TACT_TIME_INFO:
		DispTactTimeInfo();
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CDialogTackTimeView::DispTactTimeInfo()
{
	CString strTemp, strMinTemp, strMaxTemp;

	for(int i=0; i<MAX_TT_UNIT_CNT; i++)
	{
		if(IsTTUpdate(i))
		{
			// Last TactTime
			strTemp.Format(_T("%3.3fs"), GetTactTime(i));
			m_lblUnitTactTime[i].SetCaption(strTemp);

			// Min TactTime
			strMinTemp.Format(_T("%3.3fs"), GetMinTactTime(i));
			m_lblUnitMinTactTime[i].SetCaption(strMinTemp);

			// Max TactTime
			strMaxTemp.Format(_T("%3.3fs"), GetMaxTactTime(i));
			m_lblUnitMaxTactTime[i].SetCaption(strMaxTemp);

			SetTTUpdate(i, FALSE);
		}
	}
}

void CDialogTackTimeView::WriteTactLog()
{
	CString strTact;


	strTact.Format(_T("%3.3f,%3.3f,%3.3f,%3.3f,%3.3f,%3.3f,%3.3f,%3.3f,%3.3f,%3.3f"), GetTactTime(COMPLETE_FILM_REMOVE), 
																																GetTactTime(INPUT_START_BUTTON), 
																																GetTactTime(INSERT_JIG), 
																																GetTactTime(IN_SHUTTLE), 
																																GetTactTime(SHUTTLE_MOVE_GRAB_START), 
																																GetTactTime(SHUTTLE_MOVE_GRAB_END), 
																																GetTactTime(COMPLETE_GRAB), 
																																GetTactTime(IN_JIG_B), 
																																GetTactTime(PASS_JIG),
																																GetTactTime(TOTAL_TACT_TIME));

	WriteLog(strTact, TACTTIME_LOG);

	//InitialTotalTactTime();
}

BOOL CDialogTackTimeView::IsTTUpdate(int nType)
{
	return m_bTTUpdate[nType];
}

void CDialogTackTimeView::SetTTUpdate(int nType, BOOL bState)
{
	m_bTTUpdate[nType] = bState;
}

double CDialogTackTimeView::GetTactTime(int nType)
{
	return m_lTactTime[nType] * 0.001;
}

double CDialogTackTimeView::GetMinTactTime(int nType)
{
	return m_lMinTactTime[nType] * 0.001;
}

double CDialogTackTimeView::GetMaxTactTime(int nType)
{
	return m_lMaxTactTime[nType] * 0.001;
}

void CDialogTackTimeView::SetTactTime(int nType, long time)
{
	m_lTactTime[nType] = time;

	m_lTactTime[TOTAL_TACT_TIME] += time;

	SetMinTactTime(nType, time);
	SetMaxTactTime(nType, time);
}

void CDialogTackTimeView::SetMinTactTime(int nType, long time)
{
	//MIN 갱신
	if(m_lMinTactTime[nType] < time)
	{
		m_lMinTactTime[nType] =  time;
	}

	m_lMinTactTime[TOTAL_TACT_TIME] += m_lMinTactTime[nType];
}

void CDialogTackTimeView::SetMaxTactTime(int nType, long time)
{
	//MAX 갱신
	if( 0 >= m_lMaxTactTime[nType] )
		m_lMaxTactTime[nType] = time;

	if( m_lMaxTactTime[nType] > time )
	{
		m_lMaxTactTime[nType] = time;
	}

	m_lMaxTactTime[TOTAL_TACT_TIME] += m_lMaxTactTime[nType];
}

void CDialogTackTimeView::InitialTotalTactTime()
{
	m_lTactTime[TOTAL_TACT_TIME] = 0;
	m_lMinTactTime[TOTAL_TACT_TIME] = 0;
	m_lMaxTactTime[TOTAL_TACT_TIME] = 0;

}

BOOL CDialogTackTimeView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) 
			return TRUE;
	}


	return CDialog::PreTranslateMessage(pMsg);
}
