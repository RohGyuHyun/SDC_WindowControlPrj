// DialogCheckIO.cpp : 구현 파일입니다.
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


// CDialogCheckIO 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogCheckIO, CDialog)

CDialogCheckIO::CDialogCheckIO(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogCheckIO::IDD, pParent)
{
	memset(m_bInputState, NULL, sizeof(m_bInputState));
	memset(m_bOutputState, NULL, sizeof(m_bOutputState));
}

CDialogCheckIO::~CDialogCheckIO()
{
}

void CDialogCheckIO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INPUT, m_listctrl_input);
	DDX_Control(pDX, IDC_LIST_OUTPUT, m_listctrl_output);
}


BEGIN_MESSAGE_MAP(CDialogCheckIO, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, IDC_LIST_OUTPUT, &CDialogCheckIO::OnNMClickListOutput)
END_MESSAGE_MAP()


// CDialogCheckIO 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CDialogCheckIO, CDialog)
	ON_EVENT(CDialogCheckIO, IDC_EXIT_BTN, DISPID_CLICK, CDialogCheckIO::ClickExitBtn, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogCheckIO::ClickExitBtn()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	EndDialog(0);
}


BOOL CDialogCheckIO::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	LoadIOLabelPara(DATA_SYSTEM_PATH);

	LV_COLUMN lvColumn;
	CFont font;
	CImageList *img_list_slct;
	CBitmap bitmap;
	CString str;
	int i;

	img_list_slct = new CImageList;
	img_list_slct->Create(16, 16, ILC_COLOR4, 3, 3);
	
	bitmap.LoadBitmap(IDB_BITMAP_IO_OFF);
	img_list_slct->Add(&bitmap, RGB(0, 0, 0));
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_BITMAP_IO_ON);
	img_list_slct->Add(&bitmap, RGB(0, 0, 0));
	bitmap.DeleteObject();
	m_listctrl_input.SetImageList(img_list_slct, LVSIL_SMALL);
	m_listctrl_output.SetImageList(img_list_slct, LVSIL_SMALL);
	
	lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt=LVCFMT_LEFT;
	lvColumn.pszText=_T("INPUT NO.");
	lvColumn.iSubItem=0;
	lvColumn.cx=120;
	m_listctrl_input.InsertColumn(0, &lvColumn);
	lvColumn.pszText=_T("OUTPUT NO.");
	m_listctrl_output.InsertColumn(0, &lvColumn);

	lvColumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt=LVCFMT_LEFT;
	lvColumn.pszText=_T("NAME");
	lvColumn.iSubItem=0;
	lvColumn.cx=900;
	m_listctrl_input.InsertColumn(1, &lvColumn);
	m_listctrl_output.InsertColumn(1, &lvColumn);

	font.CreatePointFont(120, _T("굴림"));
	m_listctrl_input.SetFont(&font, TRUE);
	m_listctrl_output.SetFont(&font, TRUE);

	m_listctrl_output.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listctrl_input.SetExtendedStyle(LVS_EX_FULLROWSELECT);


	for(i = 0; i < MAX_INPUT_NUM; i++)
	{
		str.Format(_T("X%03x"), i);
		m_listctrl_input.InsertItem(i, str);

		m_listctrl_input.SetItemText(i, 1, m_sDILabel[i]);
	}

	for(i = 0; i < MAX_OUTPUT_NUM; i++)
	{
		str.Format(_T("Y%03x"), i);
		m_listctrl_output.InsertItem(i, str);

		m_listctrl_output.SetItemText(i, 1, m_sDOLabel[i]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialogCheckIO::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(bShow)
	{
		SetTimer(TIMER_IO_READ, 100, NULL);
		
	}
	else
	{
		KillTimer(TIMER_IO_READ);
		
	}
		
}


void CDialogCheckIO::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case TIMER_IO_READ:
		if(IsWindowVisible())
		{
			DisplayReadIO();
		}
		break;
	}

	CDialog::OnTimer(nIDEvent);
}



BOOL CDialogCheckIO::LoadIOLabelPara(CString path)
{
	BOOL rslt = TRUE;
	CString key, dat;
	int i;
	CString file_path, pat_path, extention, master_path;
	
	file_path = path + _T("\\DIOLabel.ini");

	//file 존재 확인 
	HANDLE fd = CreateFile( file_path,
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL
							);
	if(((LONG)fd)<0)
	{
		rslt = FALSE;
		
		AfxMessageBox(_T("Not found I/O Label !"));
	}
	else
	{
		CloseHandle(fd);
		
		for(i = 0; i < MAX_INPUT_NUM; i++)
		{
			key.Empty();
			key.Format(_T("X%03x"), i);
			GetPrivateProfileString(_T("DIGITAL INPUT LABEL"), key, _T(""), m_sDILabel[i], sizeof(m_sDILabel), file_path);
		}

		for(i = 0; i < MAX_OUTPUT_NUM; i++)
		{
			key.Empty();
			key.Format(_T("Y%03x"), i);
			GetPrivateProfileString(_T("DIGITAL OUTPUT LABEL"), key, _T(""), m_sDOLabel[i], sizeof(m_sDOLabel), file_path);
		}
	}
	fd=0;
	
	return rslt;
}

void CDialogCheckIO::DisplayReadIO()
{
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	int i;
	LV_ITEM lvitem;

	CString str;
	BOOL state;

	for(i = 0; i < MAX_OUTPUT_NUM; i++)
	{
		state = pdlg->m_pDlgManual->ReadOutPut(i);

		if(m_bOutputState[i] == state)
			continue;

		m_bOutputState[i] = state;

		m_listctrl_output.GetItem(&lvitem);
		str.Format(_T("Y%03x"), i);

		lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
		lvitem.iItem=i;
		lvitem.iSubItem=0;
		lvitem.iImage = (state) ? 1:0;
		lvitem.pszText=(LPWSTR)(LPCWSTR)str;
		m_listctrl_output.SetItem(&lvitem);
	}

	for(i = 0; i < MAX_INPUT_NUM; i++)
	{
		//이전 값과 동일하면 갱신하지 않는다
		state = pdlg->m_pDlgManual->ReadInput(i);

		if(m_bInputState[i] == state)
			continue;

		m_bInputState[i] = state;

		m_listctrl_output.GetItem(&lvitem);
		str.Format(_T("X%03x"), i);

		lvitem.mask=LVIF_TEXT | LVIF_IMAGE;
		lvitem.iItem=i;
		lvitem.iSubItem=0;
		lvitem.iImage = (state) ? 1:0;
		lvitem.pszText=(LPWSTR)(LPCWSTR)str;
		m_listctrl_input.SetItem(&lvitem);
	}
}

void CDialogCheckIO::OnNMClickListOutput(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int idx = pNMListView->iItem;

	if(m_bOutputState[idx])
		pdlg->m_pDlgManual->WriteOutput(idx, FALSE);
	else
		pdlg->m_pDlgManual->WriteOutput(idx, TRUE);

	*pResult = 0;
}
