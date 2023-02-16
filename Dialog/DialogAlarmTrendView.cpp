// DialogAlarmTrendView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SDC_WindowControlPrj.h"
#include "DialogAlarmTrendView.h"
#include "afxdialogex.h"

#include "MsgBox.h"									//����

// CDialogAlarmTrendView ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDialogAlarmTrendView, CDialog)

CDialogAlarmTrendView::CDialogAlarmTrendView(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogAlarmTrendView::IDD, pParent)
{
	m_bCheckDYTime = FALSE;
}

CDialogAlarmTrendView::~CDialogAlarmTrendView()
{
}

void CDialogAlarmTrendView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDS_ERROR_CODE_DY_1, m_ctrlErrorCode[0]);
	DDX_Control(pDX, IDS_ERROR_CODE_DY_2, m_ctrlErrorCode[1]);
	DDX_Control(pDX, IDS_ERROR_CODE_DY_3, m_ctrlErrorCode[2]);
	DDX_Control(pDX, IDS_ERROR_CODE_DY_4, m_ctrlErrorCode[3]);
	DDX_Control(pDX, IDS_ERROR_CODE_DY_5, m_ctrlErrorCode[4]);

	DDX_Control(pDX, IDS_ERROR_MSG_DY_1, m_ctrlErrorMsg[0]);
	DDX_Control(pDX, IDS_ERROR_MSG_DY_2, m_ctrlErrorMsg[1]);
	DDX_Control(pDX, IDS_ERROR_MSG_DY_3, m_ctrlErrorMsg[2]);
	DDX_Control(pDX, IDS_ERROR_MSG_DY_4, m_ctrlErrorMsg[3]);
	DDX_Control(pDX, IDS_ERROR_MSG_DY_5, m_ctrlErrorMsg[4]);
	
	/*
	DDX_Control(pDX, IDS_ERROR_UNIT_DY_1, m_ctrlErrorUnit[0]);
	DDX_Control(pDX, IDS_ERROR_UNIT_DY_2, m_ctrlErrorUnit[1]);
	DDX_Control(pDX, IDS_ERROR_UNIT_DY_3, m_ctrlErrorUnit[2]);
	DDX_Control(pDX, IDS_ERROR_UNIT_DY_4, m_ctrlErrorUnit[3]);
	DDX_Control(pDX, IDS_ERROR_UNIT_DY_5, m_ctrlErrorUnit[4]);
	*/

	DDX_Control(pDX, IDS_ERROR_CNT_DY_1, m_ctrlErrorCount[0]);
	DDX_Control(pDX, IDS_ERROR_CNT_DY_2, m_ctrlErrorCount[1]);
	DDX_Control(pDX, IDS_ERROR_CNT_DY_3, m_ctrlErrorCount[2]);
	DDX_Control(pDX, IDS_ERROR_CNT_DY_4, m_ctrlErrorCount[3]);
	DDX_Control(pDX, IDS_ERROR_CNT_DY_5, m_ctrlErrorCount[4]);
}


BEGIN_MESSAGE_MAP(CDialogAlarmTrendView, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDialogAlarmTrendView �޽��� ó�����Դϴ�.


BOOL CDialogAlarmTrendView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	InitAlarmWorstFive(); //130610 JSPark


	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


BOOL CDialogAlarmTrendView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return CDialog::PreTranslateMessage(pMsg);
}


void CDialogAlarmTrendView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if(bShow)
	{
		DisplayAlarmWorstFive();
	}
	else
	{

	}
}


void CDialogAlarmTrendView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.


	CDialog::OnTimer(nIDEvent);
}

BEGIN_EVENTSINK_MAP(CDialogAlarmTrendView, CDialog)
	ON_EVENT(CDialogAlarmTrendView, IDS_ALARM_TREND_DY_RESET, DISPID_CLICK, CDialogAlarmTrendView::ClickAlarmTrendDyReset, VTS_NONE)
	ON_EVENT(CDialogAlarmTrendView, IDS_ALARM_TREND_DY_SAVE, DISPID_CLICK, CDialogAlarmTrendView::ClickAlarmTrendDySave, VTS_NONE)
	END_EVENTSINK_MAP()


void CDialogAlarmTrendView::ClickAlarmTrendDyReset()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
		if(g_cMsgBox.YesNoMsgBox(_T("������ �˶� ������ �ʱ�ȭ �Ͻðڽ��ϱ�?")) == IDNO)
		return;

	m_vtAlarmTrendList.clear();
	//m_CtErrorTrend.ResetChart();
	//m_CtErrorTrend.m_dTotal = 0.0;
	Invalidate();
	GetAlarmWorstFive(); 
	DisplayAlarmWorstFive();
}


void CDialogAlarmTrendView::ClickAlarmTrendDySave()
{
	CString strTemp, strTemp2;

	strTemp.Format(_T("Err%03d"), rand()%1000);

	strTemp2.Format(_T("�˶�����"));

	AddMachineErrorChart(strTemp, strTemp2);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	/*
		if(g_cMsgView.YesNoMsgBox("������ ���������� �����Ͻðڽ��ϱ�?") == IDNO)
		return;

	CProfileDataIF cDataIf;
	CString strNode, strMsg[MAX_LAG_CNT], tempStr, strItem; //131103 JSPark
	CString strFilePath, strNamePath, strDayPath;
	CString tStr, strDate, strDay;
	DWORD dwRet;
	int iRet;
	
	CTime NowTime=CTime::GetCurrentTime();
	int hour = NowTime.GetHour();
	
	strDate = NowTime.Format(L"%Y%m%d%H%M%S");
	strFilePath = ALARM_DATA_PATH;
	strDayPath = strFilePath+strDate;
	
	dwRet = GetFileAttributes(strDayPath);
	if(dwRet != FILE_ATTRIBUTE_DIRECTORY)		// ������ �������� �ʴ� ���.
	{	
		iRet = CreateDirectory(strDayPath, NULL);
		if(iRet==0)	// �������� ���� ���. 
			return;
	}	
	//Shift 
	int nShift = DY_SHIFT;
	strNode.Format(L"AlarmTotalList_DY_Shift.dat");
	strNamePath = strDayPath+ _T("\\") +strNode;
	cDataIf.SetFilePath(strNamePath);

	int i, itotal;
	CString strSection, strKey, strData, strCode, strCount, strUnit; //131103 JSPark
	strSection = "Machine Alarm Total";
	strKey = "Total_Count";
	itotal = m_vtAlarmTrendList[nShift].size();
	strData.Format(L"%d", itotal);
	cDataIf.SetString(strSection, strKey, strData); 		
	
	strSection = "Total_Alarm_List";
	for(i=0; i<itotal; i++)
	{
		strKey.Format(L"%d",i);
		m_AlarmWorstTemp = m_vtAlarmTrendList[nShift][i];
		strCode = m_AlarmWorstTemp.sAlarmCode;
//>> 131103 JSPark
		strUnit = m_AlarmWorstTemp.sAlarmUnit;
		for(int j=0; j<MAX_LAG_CNT; j++)
			strMsg[j] = m_AlarmWorstTemp.sAlarmMsg[j];
		strCount.Format(L"%d", m_AlarmWorstTemp.iAlarmCount);
		strData.Format(L"%s,%s,%s,%s,%s,%s", strCode, strUnit,
			strMsg[KOREAN], strMsg[ENGLISH], strMsg[NONE], strCount);
//<<
		cDataIf.SetString(strSection, strKey, strData); 		
	}
	*/
}


//�߿����Լ�. �˶�Ʈ���� ��ŸƮ.
void CDialogAlarmTrendView::DispAlarmInfo() //130610 JSPark
{
	/*
	if(g_ErrData.IsAlarmTrendUpdate())
	{

		AddMachineErrorChart(g_ErrData.GetAlarmTrendCode(), g_ErrData.GetAlarmTrendMsg()));
	
		g_ErrData.SetAlarmTrendUpdate(false);
	}
	*/
}

void CDialogAlarmTrendView::InitAlarmWorstFive()
{
	//LoadAlarmData();

	GetAlarmWorstFive();
	DisplayAlarmWorstFive();
}

void CDialogAlarmTrendView::GetAlarmWorstFive()
{
	int nCnt;
	int total(0);

	ResetWorstFive();

	total = m_vtAlarmTrendList.size();
	
	for(int i=0; i<total; i++)
	{
		nCnt = m_vtAlarmTrendList[i].iAlarmCount;
		//�����Ͽ� ���� �Ұ�
		//index 0�� ���� ������?
		for(int j=WORST_LIST_CNT-1; j>-1; j--) 
		{
			if(m_AlarmWorstFive[j].iAlarmCount < nCnt)
			{
				m_AlarmWorstTemp = m_AlarmWorstFive[j];
				m_AlarmWorstFive[j] = m_vtAlarmTrendList[i];
				if(j != WORST_LIST_CNT-1)
					m_AlarmWorstFive[j+1] = m_AlarmWorstTemp;	
			}
		}
	}
}

void CDialogAlarmTrendView::ResetWorstFive()
{
	for(int i=0; i<WORST_LIST_CNT; i++)
	{
		m_AlarmWorstFive[i].iAlarmCount = 0; 
		m_AlarmWorstFive[i].sAlarmCode = _T(""); 
		//m_AlarmWorstFive[i].sAlarmUnit = _T("");
		m_AlarmWorstFive[i].sAlarmMsg = _T("");
	}
}


void CDialogAlarmTrendView::AddAlarmItem(CString strAlarmCode, /*CString strAlarmUnit,*/ CString strAlarmMsg)
{
	//�켱 List���� ã�ƺ��� ������ Count�� 1
	int hour;
	CTime NowTime=CTime::GetCurrentTime();
	hour = NowTime.GetHour();

	int nIndex(-1);
	int nCnt(0);
	/*
	//Shift 
	if(hour>=DY_TIME_START && hour<DY_TIME_END)   //DY_SHIFT 
		nShift = DY_SHIFT;
#ifdef _3SHIFT_DEFINE
	else if(hour>=SW_TIME_START && hour<SW_TIME_END)   //SW_SHIFT 
		nShift = SW_SHIFT;
#endif
	else
		nShift = GY_SHIFT;

	*/
	for(int i=0; i<(int)(m_vtAlarmTrendList.size()); i++)
	{
		if(m_vtAlarmTrendList[i].sAlarmCode == strAlarmCode)
		{
			nIndex = i;
			break;
		}
	}
	//������ ������ �űԷ� �ְ� 
	if(nIndex < 0)
	{
		m_AlarmWorstTemp.sAlarmCode = strAlarmCode;
		//m_AlarmWorstTemp.sAlarmUnit = strAlarmUnit; //130619 JSPark
		m_AlarmWorstTemp.sAlarmMsg = strAlarmMsg; 
		//m_AlarmWorstTemp.sAlarmMsg[ENGLISH] = strAlarmEMsg; 
		//m_AlarmWorstTemp.sAlarmMsg[NONE] = strAlarmCMsg; 

		m_AlarmWorstTemp.iAlarmCount = 1;
		m_vtAlarmTrendList.push_back(m_AlarmWorstTemp);
	}
	else //0����~~
	{
		//Count Up
		m_vtAlarmTrendList[nIndex].iAlarmCount = m_vtAlarmTrendList[nIndex].iAlarmCount+1;
	}
}

void CDialogAlarmTrendView::DisplayAlarmWorstFive() 
{
	CString strData;

	for(int i=0; i<WORST_LIST_CNT; i++)
	{
		m_ctrlErrorCode[i].SetWindowText(m_AlarmWorstFive[i].sAlarmCode);   
		//m_ctrlErrorUnit[i].SetWindowText(m_AlarmWorstFive[i].sAlarmUnit); 
		m_ctrlErrorMsg[i].SetWindowText(m_AlarmWorstFive[i].sAlarmMsg); 
		
		if(m_AlarmWorstFive[i].iAlarmCount > 4)
		{
			if(m_ctrlErrorCount[i].GetForeColor() != RED)
				m_ctrlErrorCount[i].SetForeColor(RED);
		}
		else
		{
			if(m_ctrlErrorCount[i].GetForeColor() != BLACK) 
				m_ctrlErrorCount[i].SetForeColor(BLACK);
		}
		strData.Format(_T("%d"), m_AlarmWorstFive[i].iAlarmCount);
		m_ctrlErrorCount[i].SetWindowText(strData);  
	}
}

void CDialogAlarmTrendView::AddMachineErrorChart(CString strAlCode, /*CString strAlUnit,*/ CString strAlMsg)
{
	UpdateData();

	if(strAlCode==_T("")) 
		return;

	AddAlarmItem(strAlCode, /*strAlUnit,*/ strAlMsg);

	GetAlarmWorstFive();
	DisplayAlarmWorstFive();
	SaveAlarmData(TRUE);
}

void CDialogAlarmTrendView::LoadAlarmData() 
{
	/*
	int i(0), j(0);
	CString strSection, strKey;
	CString strPath, strFilePath;
	CString strtemp, strData;
	CString strCode, strMsg, strCount; //131101 JSPark
	// File Path Setting
	strPath = ALARM_DATA_PATH;
	strFilePath = strPath+"\\AlarmData.ini";

	CProfileDataIF cDataIf;
	cDataIf.SetFilePath(strFilePath);
	// GY Save Day Load
	CTime ctTime;
	ctTime = CTime::GetCurrentTime();
	// GY_SHIFT ���� ��¥�� �ҷ��´�.(������ ���ڰ� ���� ��� ���� ��¥�� �ҷ��´�.)
	m_strAlarmGYSaveDay = cDataIf.GetString("GY Save Day Info", "SAVE DAY", ctTime.Format(L"%Y%m%d"));

	for(int nShift=0; nShift<SHIFT_TOTAL; nShift++)
	{
		// Total Count Load
		if(nShift == DY_SHIFT)
			strSection = "DY_Shift Alarm Total";
#ifdef _3SHIFT_DEFINE
		else if(nShift == SW_SHIFT)
			strSection = "SW_Shift Alarm Total";
#endif
		else
			strSection = "GY_Shift Alarm Total";

		strKey = "Total_Count";
		int total = cDataIf.GetInt(strSection, strKey, 0); 

		// Alarm List Load
		if(nShift == DY_SHIFT)
			strSection = "DY_Shift Total_Alarm_List";
#ifdef _3SHIFT_DEFINE
		else if(nShift == SW_SHIFT)
			strSection = "SW_Shift Total_Alarm_List";
#endif
		else
			strSection = "GY_Shift Total_Alarm_List";

		for(i=0; i<total; i++)
		{
			strKey.Format(L"%d", i);
			strData = cDataIf.GetString(strSection, strKey, ""); 
			
			if(strData != "")
			{
				strCode = GetExtractionCommaMsg(strData);
				strUnit = GetExtractionCommaMsg(strData);
//>> 131103 JSPark
				for(j=0; j<MAX_LAG_CNT; j++)
					strMsg[j] = GetExtractionCommaMsg(strData);
//<<
				strCount = GetExtractionCommaMsg(strData);			
				
				m_AlarmWorstTemp.sAlarmCode = strCode;
				m_AlarmWorstTemp.sAlarmUnit = strUnit;
//>> 131103 JSPark
				for(j=0; j<MAX_LAG_CNT; j++)
					m_AlarmWorstTemp.sAlarmMsg[j] = strMsg[j];
//<<
				m_AlarmWorstTemp.iAlarmCount = _ttoi(strCount);
				m_vtAlarmTrendList[nShift].push_back(m_AlarmWorstTemp);
				
				AddMachineErrorChartLoadCnt(nShift, strCode, m_AlarmWorstTemp.iAlarmCount);
			}
		}
	}
	*/
}

void CDialogAlarmTrendView::SaveAlarmData(BOOL bDeleteFile) 
{
	CProfileDataIF cDataIf;

	CString strKey, strCode, strMsg, strCount, strData, strSection, strPath;
	int itotal = m_vtAlarmTrendList.size();

	strPath.Format(_T("%s\\AlarmTrend.csv"), ALARM_LOG_PATH);
	cDataIf.SetFilePath(strPath);

	if(bDeleteFile)
		DeleteFile(strPath.GetBuffer(strPath.GetLength()));

	strSection = _T("Alarm Total");

	for(int  i = 0; i < itotal; i++)
	{
		strKey.Format(_T("%d"), i);
		m_AlarmWorstTemp = m_vtAlarmTrendList[i];
		strCode = m_AlarmWorstTemp.sAlarmCode;

		strMsg = m_AlarmWorstTemp.sAlarmMsg;
		strCount.Format(_T("%d"), m_AlarmWorstTemp.iAlarmCount);
		strData.Format(_T("%s,%s,%s"), strCode, strMsg, strCount);

		cDataIf.SetString(strSection, strKey, strData); 		
	}

	/*
	m_csAlarmDataSaveInterlock.Lock();

	CString strSection, strFileName, strKey;
	CString strPath, strFilePath;
	CString strtemp, strData;
	CString strCode, strUnit, strMsg[MAX_LAG_CNT], strCount; //131103 JSPark
	// File Path Setting
	strPath = ALARM_DATA_PATH;
	strFilePath = strPath+"\\AlarmData.ini";
	// ini ���� ���� ������ ���� �� �ٽ� �����Ѵ�.
	if(bDeleteFile)
		DeleteFile(strFilePath.GetBuffer(strFilePath.GetLength()));

	CProfileDataIF cDataIf;
	cDataIf.SetFilePath(strFilePath);
	// GY Save Day Save
	cDataIf.SetString("GY Save Day Info", "SAVE DAY", m_strAlarmGYSaveDay);

	if(bDeleteFile)
	{	// ���� ���� ���� �� ���� ���θ� �ٽ� ����.
		for(int nShift=0; nShift<SHIFT_TOTAL; nShift++)
		{
			// Total Count Save
			if(nShift == DY_SHIFT)
				strSection = "DY_Shift Alarm Total";
#ifdef _3SHIFT_DEFINE
			else if(nShift == SW_SHIFT)
				strSection = "SW_Shift Alarm Total";
#endif
			else
				strSection = "GY_Shift Alarm Total";

			strKey = "Total_Count";
			int itotal = m_vtAlarmTrendList[nShift].size();
			strData.Format(L"%d", itotal);
			cDataIf.SetString(strSection, strKey, strData);
			
			// Alarm List Save
			if(nShift == DY_SHIFT)
				strSection = "DY_Shift Total_Alarm_List";
#ifdef _3SHIFT_DEFINE
			else if(nShift == SW_SHIFT)
				strSection = "SW_Shift Total_Alarm_List";
#endif
			else
				strSection = "GY_Shift Total_Alarm_List";

			for(int i=0; i<itotal; i++)
			{
				strKey.Format(L"%d",i);
				m_AlarmWorstTemp = m_vtAlarmTrendList[nShift][i];
				strCode = m_AlarmWorstTemp.sAlarmCode;
				strUnit = m_AlarmWorstTemp.sAlarmUnit;
//>> 131103 JSPark
				for(int j=0; j<MAX_LAG_CNT; j++)
					strMsg[j] = m_AlarmWorstTemp.sAlarmMsg[j];
				strCount.Format(L"%d", m_AlarmWorstTemp.iAlarmCount);
				strData.Format(L"%s,%s,%s,%s,%s,%s", strCode, strUnit, 
					strMsg[KOREAN], strMsg[ENGLISH], strMsg[NONE], strCount);
//<<
				cDataIf.SetString(strSection, strKey, strData); 		
			}
		}
	}
	else
	{	// ���� ������ �״�� �ΰ� �ش� Shift ���븸 �����Ѵ�.
		// Total Count Save
		if(nShift == DY_SHIFT)
			strSection = "DY_Shift Alarm Total";
#ifdef _3SHIFT_DEFINE
		else if(nShift == SW_SHIFT)
			strSection = "SW_Shift Alarm Total";
#endif
		else
			strSection = "GY_Shift Alarm Total";
		strKey = "Total_Count";
		int itotal = m_vtAlarmTrendList[nShift].size();
		strData.Format(L"%d", itotal);
		cDataIf.SetString(strSection, strKey, strData);
		
		// Alarm List Save
		if(nShift == DY_SHIFT)
			strSection = "DY_Shift Total_Alarm_List";
#ifdef _3SHIFT_DEFINE
		else if(nShift == SW_SHIFT)
			strSection = "SW_Shift Total_Alarm_List";
#endif
		else
			strSection = "GY_Shift Total_Alarm_List";
		for(int i=0; i<itotal; i++)
		{
			strKey.Format(L"%d",i);
			m_AlarmWorstTemp = m_vtAlarmTrendList[nShift][i];
			strCode = m_AlarmWorstTemp.sAlarmCode;
			strUnit = m_AlarmWorstTemp.sAlarmUnit;
//>> 131103 JSPark
			for(int j=0; j<MAX_LAG_CNT; j++)
				strMsg[j] = m_AlarmWorstTemp.sAlarmMsg[j];
			strCount.Format(L"%d", m_AlarmWorstTemp.iAlarmCount);
			strData.Format(L"%s,%s,%s,%s,%s,%s", strCode, strUnit, 
				strMsg[KOREAN], strMsg[ENGLISH], strMsg[NONE], strCount);
//<<
			cDataIf.SetString(strSection, strKey, strData); 		
		}
	}

	m_csAlarmDataSaveInterlock.Unlock();
	*/
}