// DialogMainView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SDC_WindowControlPrj.h"
#include "DialogMainView.h"
#include "afxdialogex.h"


// CDialogMainView 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogMainView, CDialog)

CDialogMainView::CDialogMainView(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMainView::IDD, pParent)
{
	for(int i =0; i < DISPLAY_CELL_NUM; i++)
	{
		m_nStateCell[i] = 0;
		m_bViewUpdate[i] = FALSE;
	}

	m_nCurrentStateCell = 0;

	//m_bAutoClearMessageBox = FALSE;
}

CDialogMainView::~CDialogMainView()
{
}

void CDialogMainView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDS_BUTTON_READY, m_ctrlReadyButton);
	DDX_Control(pDX, IDS_BUTTON_START, m_ctrlStartButton);
	DDX_Control(pDX, IDS_BUTTON_RESET, m_ctrlResetButton);
	DDX_Control(pDX, IDS_JIGB_FILMMOVER_CELL, m_ctrlDisplayCell[DISPLAY_CELL_JIG_B_BACK]);
	DDX_Control(pDX, IDS_JIGB_FILMMOVER_CELL2, m_ctrlDisplayCell[DISPLAY_CELL_JIG_B_FORWARD]);
	DDX_Control(pDX, IDS_JIGA_GLASS_CELL, m_ctrlDisplayCell[DISPLAY_CELL_JIG_A_BACK]);
	DDX_Control(pDX, IDS_JIGA_GLASS_CELL2, m_ctrlDisplayCell[DISPLAY_CELL_JIG_A_FORWARD]);
	DDX_Control(pDX, IDS_SHUTTLE_CELL, m_ctrlDisplayCell[DISPLAY_CELL_TRAY_SHUTTLE]);
	DDX_Control(pDX, IDS_GRAB_CELL, m_ctrlDisplayCell[DISPLAY_CELL_GRAB_START]);
	DDX_Control(pDX, IDS_GRAB_CELL2, m_ctrlDisplayCell[DISPLAY_CELL_GRAB_END]);
	DDX_Control(pDX, IDC_CAMERA, m_ctrlDisplayCell[DISPLAY_CELL_CAMERA]);
}

/*
#define DISPLAY_CELL_JIG_B_BACK			0
#define DISPLAY_CELL_JIG_B_FORWARD		1
#define DISPLAY_CELL_JIG_A_BACK			2
#define DISPLAY_CELL_JIG_A_FORWARD		3
#define DISPLAY_CELL_TRAY_SHUTTLE		4
#define DISPLAY_CELL_GRAB_START			5
#define DISPLAY_CELL_GRAB_END			6
#define DISPLAY_CELL_CAMERA				7
*/

BEGIN_MESSAGE_MAP(CDialogMainView, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDialogMainView 메시지 처리기입니다.


BOOL CDialogMainView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	for(int i = 0; i < DISPLAY_CELL_NUM; i++)
	{
		m_ctrlDisplayCell[i].SetBackColor(LTGRAY);
	}

	SetTimer(MAIN_SEQUENCE_DISPALY, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialogMainView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(bShow)
	{
		SetTimer(MAIN_SEQUENCE_DISPALY, 500, NULL);
	}
	else
	{
		KillTimer(MAIN_SEQUENCE_DISPALY);
	}
}


void CDialogMainView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case MAIN_SEQUENCE_DISPALY:
		DisplayViewInfo();
		break;
/*
	case MAIN_MESSAGEBOX_DISPLAY:
		{
			if(m_bAutoClearMessageBox)
			{
				HWND wndDlg = ::GetLastActivePopup(m_hWnd);
	
				if(wndDlg && wndDlg != m_hWnd)
				{
					m_bAutoClearMessageBox = FALSE;
					KillTimer(MAIN_MESSAGEBOX_DISPLAY);

					wchar_t buffer[256] = {0};
					::GetClassName(wndDlg, buffer, 256);
					if(CString("#32770") == buffer) //메시지 박스는 분명히 다이얼로그이며 클래스명이 #32770
					{
						::EndDialog(wndDlg, IDOK);
					}
				}
			}
		}
		break;
*/
	default:

		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CDialogMainView::DisplayViewInfo()
{
	DWORD color;

	for(int i = 0; i < DISPLAY_CELL_NUM; i++)
	{
		if( IsViewUpdate(i) )
		{		
			switch( m_nStateCell[i] )
			{
			case CELL_STATE_OK:
				color = GREEN;
				break;
			case CELL_STATE_NG:
				color = RED;
				break;
			case CELL_STATE_EMPTY:
				color = LTGRAY;
				break;
			case CELL_STATE_WORK:
				color = BLACK;
				break;
			}

			m_ctrlDisplayCell[i].SetBackColor(color);

			SetVIewUpdate(i, FALSE);
		}
	}
}
void CDialogMainView::SetStateNG()
{
	//SetVIewUpdate(m_nCurrentStateCell, TRUE);
	m_nStateCell[m_nCurrentStateCell] = CELL_STATE_NG;
	SetVIewUpdate(m_nCurrentStateCell, TRUE);
}

void CDialogMainView::SetStateCell(int nType, int nState)
{
	if(DISPLAY_CELL_NUM < nType)
		return;

	for(int i = 0; i < DISPLAY_CELL_NUM; i++)
	{
		m_ctrlDisplayCell[i].SetBackColor(LTGRAY);
	}
	
	m_nCurrentStateCell = nType;
	m_nStateCell[nType] = nState;

	SetVIewUpdate(nType, TRUE);

}

BOOL CDialogMainView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) 
			return TRUE;
	}


	return CDialog::PreTranslateMessage(pMsg);
}

void CDialogMainView::SetAllEmpty()
{
	for(int i = 0; i < DISPLAY_CELL_NUM; i++)
	{
		m_ctrlDisplayCell[i].SetBackColor(LTGRAY);
	}
}
/*
void CDialogMainView::ShowAutoClearMessageBox()
{
	//SetTimer(MAIN_MESSAGEBOX_DISPLAY, 300, NULL);

	//AfxMessageBox(_T("VISION 으로 부터 결과 응답 대기 중..설비 내 조작 금지!"));
}
*/