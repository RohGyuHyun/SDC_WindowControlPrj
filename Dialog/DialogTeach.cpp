// DialogTeach.cpp : 구현 파일입니다.
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


// CDialogTeach 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogTeach, CDialog)

CDialogTeach::CDialogTeach(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogTeach::IDD, pParent)
{
	m_pModelData			= NULL;
	m_pModelData			= new CModelData;

	m_nAxisID = AXIS_LD_PNP;
	m_nPosType = 0;

}

CDialogTeach::~CDialogTeach()
{

}

void CDialogTeach::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MODEL, m_listctrl_model);
	DDX_Control(pDX, IDC_SELECT_MODEL_NAME, m_ctrlSelectModelName);
	DDX_Control(pDX, IDC_CURRENT_MODEL_NAME, m_ctrlWorkModelName);

	DDX_Control(pDX, IDC_TEACH_SH_DATA_X0, m_ctrlTeachPos_X[0]);
	DDX_Control(pDX, IDC_TEACH_SH_DATA_X1, m_ctrlTeachPos_X[1]);
	DDX_Control(pDX, IDC_TEACH_SH_DATA_X2, m_ctrlTeachPos_X[2]);
	DDX_Control(pDX, IDC_TEACH_SH_DATA_X3, m_ctrlTeachPos_X[3]);

	DDX_Control(pDX, IDC_TEACH_SH_DATA_Y0, m_ctrlTeachPos_Y[0]);
	DDX_Control(pDX, IDC_TEACH_SH_DATA_Y1, m_ctrlTeachPos_Y[1]);
	DDX_Control(pDX, IDC_TEACH_SH_DATA_Y2, m_ctrlTeachPos_Y[2]);
	DDX_Control(pDX, IDC_TEACH_SH_DATA_Y3, m_ctrlTeachPos_Y[3]);

	DDX_Control(pDX, IDC_LABEL_AXIS_CURPOS_0, m_ctrlAxisCurrentName[0]);
	DDX_Control(pDX, IDC_LABEL_AXIS_CURPOS_1, m_ctrlAxisCurrentName[1]);

	DDX_Control(pDX, IDS_AXIS_CURPOS_0, m_ctrlAxisCurrentPos[0]);
	DDX_Control(pDX, IDS_AXIS_CURPOS_1, m_ctrlAxisCurrentPos[1]);
	DDX_Control(pDX, IDC_BTNENHCTRL_PROGRESS_MOVE, m_ctrl_progress);
}


BEGIN_MESSAGE_MAP(CDialogTeach, CDialog)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(NM_CLICK, IDC_LIST_MODEL, &CDialogTeach::OnNMClickListModel)
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDialogTeach 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CDialogTeach, CDialog)
	ON_EVENT(CDialogTeach, IDC_MODEL_ADD, DISPID_CLICK, CDialogTeach::ClickModelAdd, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_MODEL_DELETE, DISPID_CLICK, CDialogTeach::ClickModelDelete, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_MODEL_CHANGE, DISPID_CLICK, CDialogTeach::ClickModelChange, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_MODEL_DATA_SAVE, DISPID_CLICK, CDialogTeach::ClickModelDataSave, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_BTN_JOG, DISPID_CLICK, CDialogTeach::ClickBtnJog, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_TEACH_SET_TARGET_POS, DISPID_CLICK, CDialogTeach::ClickTeachSetTargetPos, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_TEACH_SH_AXIS_X, DISPID_CLICK, OnClickTeachSetAxis, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_TEACH_SH_AXIS_Y, DISPID_CLICK, OnClickTeachSetAxis, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_TEACH_SH_POS_0, DISPID_CLICK, OnClickTeachSetPos, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_TEACH_SH_POS_1, DISPID_CLICK, OnClickTeachSetPos, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_TEACH_SH_POS_2, DISPID_CLICK, OnClickTeachSetPos, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_TEACH_SH_POS_3, DISPID_CLICK, OnClickTeachSetPos, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_TEACH_SH_DATA_X0, DISPID_CLICK, OnClickTeachSetData, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_TEACH_SH_DATA_X1, DISPID_CLICK, OnClickTeachSetData, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_TEACH_SH_DATA_X2, DISPID_CLICK, OnClickTeachSetData, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_TEACH_SH_DATA_X3, DISPID_CLICK, OnClickTeachSetData, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_TEACH_SH_DATA_Y0, DISPID_CLICK, OnClickTeachSetData, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_TEACH_SH_DATA_Y1, DISPID_CLICK, OnClickTeachSetData, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_TEACH_SH_DATA_Y2, DISPID_CLICK, OnClickTeachSetData, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_TEACH_SH_DATA_Y3, DISPID_CLICK, OnClickTeachSetData, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_TEACH_MOVE_POS, DISPID_CLICK, CDialogTeach::ClickTeachMovePos, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_TEACH_MOVE_STOP, DISPID_CLICK, CDialogTeach::ClickTeachMoveStop, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_MODEL_LIST_UP, DISPID_CLICK, CDialogTeach::ClickModelListUp, VTS_NONE)
	ON_EVENT(CDialogTeach, IDC_MODEL_LIST_DOWN, DISPID_CLICK, CDialogTeach::ClickModelListDown, VTS_NONE)
END_EVENTSINK_MAP()


BOOL CDialogTeach::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
	
	if( !pdlg->m_pDlgMainData->GetWorkModelName().IsEmpty() )
	{
		m_ctrlWorkModelName.SetCaption(pdlg->m_pDlgMainData->GetWorkModelName());

		m_pModelData->LoadModelData(pdlg->m_pDlgMainData->GetWorkModelName());
	}

	InitListCtrlModel();
	DisplayModelList();
	DisplayModelData();

	((CButton*)GetDlgItem(IDC_TEACH_SH_POS_0))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_TEACH_SH_AXIS_X))->SetCheck(TRUE);
	
	m_ctrl_progress.SetProgressBarMin(0);
	m_ctrl_progress.SetProgressBarMax(100);
	m_ctrl_progress.SetProgressBarValue(m_nProcessMoveIdx);
	m_ctrl_progress.SetCaption(_T(" "));
	m_ctrl_progress.ShowWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialogTeach::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	if(bShow)
	{
		SetTimer(TIMER_TEACH_VIEW, 500, NULL);

		DisplayModelData();

		if(pdlg->m_pDlgMainData->GetCurUserName() == LOG_IN_NAME_OPERATOR)
		{
			GetDlgItem(IDC_MODEL_ADD)->EnableWindow(FALSE);
			GetDlgItem(IDC_MODEL_DELETE)->EnableWindow(FALSE);
			GetDlgItem(IDC_MODEL_DATA_SAVE)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_MODEL_ADD)->EnableWindow(TRUE);
			GetDlgItem(IDC_MODEL_DELETE)->EnableWindow(TRUE);
			GetDlgItem(IDC_MODEL_DATA_SAVE)->EnableWindow(TRUE);
		}
	}
	else
	{
		KillTimer(TIMER_TEACH_VIEW);	
	}
}



void CDialogTeach::InitListCtrlModel()
{
	//컬럼 설정
	m_listctrl_model.InsertColumn(0, _T("NO."), LVCFMT_LEFT, 50);
	m_listctrl_model.InsertColumn(1, _T("NAME"), LVCFMT_LEFT, 200);
	m_listctrl_model.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	m_listctrl_model.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_listctrl_model.DeleteAllItems();


	//아이콘 설정

}

void CDialogTeach::ClickModelDataSave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	BOOL bCheck(false);
	CString strMsg = _T("");	
	CString strUser;
	int iVal=0, i(0), j(0);		
	double dVal = 0.0;

	if(g_cMsgBox.YesNoMsgBox(_T("Data를 저장 하시겠습니까?")) != IDYES)
		return;

	dVal = _ttof(m_ctrlTeachPos_X[0].GetCaption());
	m_pModelData->SetLP_LoadPosX(dVal);
	dVal = _ttof(m_ctrlTeachPos_X[1].GetCaption());
	m_pModelData->SetLP_InspPosX(dVal);
	dVal = _ttof(m_ctrlTeachPos_X[2].GetCaption());
	m_pModelData->SetLP_UnloadPosX(dVal);
	dVal = _ttof(m_ctrlTeachPos_X[3].GetCaption());	// 20160425 현재 스페어 파라미터
	m_pModelData->SetLP_SparePosX(dVal);

	dVal = _ttof(m_ctrlTeachPos_Y[0].GetCaption());
	m_pModelData->SetIS_LoadPosY(dVal);
	dVal = _ttof(m_ctrlTeachPos_Y[1].GetCaption());
	m_pModelData->SetIS_InspPosY(dVal);
	dVal = _ttof(m_ctrlTeachPos_Y[2].GetCaption());
	m_pModelData->SetIS_UnloadPosY(dVal);
	dVal = _ttof(m_ctrlTeachPos_Y[3].GetCaption());
	m_pModelData->SetIS_CamCenterPosY(dVal);

	m_pModelData->SaveModelData(pdlg->m_pDlgMainData->GetWorkModelName());
}


//MODEL ADD
void CDialogTeach::ClickModelAdd()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CGetTextDlg	dlg;
	CString strModelName, strDestPath, strSourcePath;

	if(dlg.DoModal() != IDOK)
		return;

	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	strModelName = dlg.GetStringValue();
	
	strDestPath.Format(_T("%s\\%s"), DATA_MODEL_PATH, strModelName);
	strSourcePath.Format(_T("%s\\%s"), DATA_MODEL_PATH, pdlg->m_pDlgMainData->GetWorkModelName());//g_ModelData.GetModelName());
	
	CreateDirectory(strDestPath, NULL);

	DisplayModelList();
	
	CopyFolder(strSourcePath, pdlg->m_pDlgMainData->GetWorkModelName(), strDestPath, strModelName);

	//신규모델 추가 후 작업모델로 선택
	/*
	m_strSelectModelName = strModelName;
	
	bSearchModelName(m_strSelectModelName);
	DisplayModelName();

	OnClickModelChange() ;
	*/
}

//MODEL DELETE
void CDialogTeach::ClickModelDelete()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString strFilePath;

	if(IDNO == g_cMsgBox.YesNoMsgBox(_T("선택한 모델을 삭제하시겠습니까?")))
		return;

	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	//현재 작업중인 있는 모델인지 확인루틴 필요
	if( pdlg->m_pDlgMainData->GetWorkModelName() == m_ctrlSelectModelName.GetCaption() )
	{
		g_cMsgBox.OkMsgBox(_T("현재 작업 중인 모델은 삭제할 수 없습니다."), 1);
	
		return;
	}
	
	strFilePath.Format(_T("%s\\%s"), DATA_MODEL_PATH, m_ctrlSelectModelName.GetCaption());

	TRY
	{
		DeleteDirectoryFile(strFilePath);
	}
	CATCH( CFileException, e )
	{
		return;
	}
	END_CATCH
		
		DisplayModelList();	
}


void CDialogTeach::ClickModelChange()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	if(IDNO == g_cMsgBox.YesNoMsgBox(_T("선택한 모델로 변경합니까?")))
		return;

	if(m_ctrlSelectModelName.GetCaption() == pdlg->m_pDlgMainData->GetWorkModelName()) 
		return;

	pdlg->m_pDlgMainData->SetWorkModelName(m_ctrlSelectModelName.GetCaption());
	m_ctrlWorkModelName.SetCaption(pdlg->m_pDlgMainData->GetWorkModelName());
	pdlg->m_pDlgMainData->SaveSystemData();

	pdlg->SetModelName(pdlg->m_pDlgMainData->GetWorkModelName());

	m_pModelData->LoadModelData(pdlg->m_pDlgMainData->GetWorkModelName());

	DisplayModelData();

	g_cMsgBox.OkMsgBox(_T("모델 변경이 완료되었습니다."));
}

void CDialogTeach::DisplayModelList()
{
	CString strFilePath, strFileName, strName, strCount;
	strFilePath = DATA_MODEL_PATH;
	strFileName = _T("\\*.*");
	
	CFileFind cFFind;	
	int nCount = 0;	
	m_listctrl_model.DeleteAllItems();
	strFilePath += strFileName;
	
	TRY { 
		BOOL bIsFile = cFFind.FindFile(strFilePath, 0);
		while(bIsFile)
		{
			bIsFile = cFFind.FindNextFile();
			strName = cFFind.GetFileName();
			if(strName != "." && strName != ".." )
			{
				strCount.Format(_T("%d"), nCount + 1);

				m_listctrl_model.InsertItem(0, strCount);
				m_listctrl_model.SetItemText(nCount, 1, strName);

				nCount++;
			}
			//Delay(2,TRUE);
		}
	}
	
	CATCH (CException, e)
	{
		cFFind.Close();
		e->ReportError();
		e->Delete();
		return;
	}
    END_CATCH
		
		cFFind.Close();
}

void CDialogTeach::OnNMClickListModel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nIndex = pNMItemActivate->iItem;

	if( nIndex >= 0 && nIndex < m_listctrl_model.GetItemCount() )
	{
		m_ctrlSelectModelName.SetCaption( m_listctrl_model.GetItemText(nIndex, 1) );
	}


	*pResult = 0;
}

void CDialogTeach::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(NULL != m_pModelData){
		delete m_pModelData;
		m_pModelData = NULL;
	}

}


void CDialogTeach::ClickBtnJog()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	pdlg->ShowTeachAxisJog();
}


void CDialogTeach::ClickTeachSetTargetPos()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
	CString strSetPos = _T(""),  strLog=_T("");
	double dCurPos(0.0);

	CString strKMsg, strEMsg, strCMsg, strTmp;

	strKMsg.Format(_T("현재 위치 데이터를 \n적용 하시겠습니까?"));

	if(g_cMsgBox.YesNoMsgBox(strKMsg) != IDYES)
		return;

	pdlg->m_pDlgManual->AxisGetPosition(m_nAxisID, dCurPos);

	strSetPos.Format(_T("%.3f"), dCurPos);

	switch(m_nAxisID)
	{
	case AXIS_LD_PNP:
		if(!m_ctrlTeachPos_X[m_nPosType].IsWindowEnabled())
			return;
		m_ctrlTeachPos_X[m_nPosType].SetCaption(strSetPos);
		break;
	case AXIS_TRY_SHUTTLE:
		if(!m_ctrlTeachPos_Y[m_nPosType].IsWindowEnabled())
			return;
		m_ctrlTeachPos_Y[m_nPosType].SetCaption(strSetPos);
		break;
	}

	strTmp.Format(_T("Teach View --> OnClickTeachSetTargetPos [%s Axis %s Pos]"), pdlg->m_pDlgManual->AxisGetAxisName(m_nAxisID), strSetPos);
	WriteLog(strTmp, SYSTEM_LOG);

	
}

void CDialogTeach::OnClickTeachSetAxis()
{	
	CBtnEnh *pBtnEnh = (CBtnEnh*)GetFocus();
	CString strLog=_T("");

	switch(pBtnEnh->GetDlgCtrlID())
	{
	case IDC_TEACH_SH_AXIS_X:
		m_nAxisID = AXIS_LD_PNP;
		break;
	case IDC_TEACH_SH_AXIS_Y:
		m_nAxisID = AXIS_TRY_SHUTTLE;
		break;
	}
}

void CDialogTeach::OnClickTeachSetPos()
{
	CBtnEnh *pBtnEnh = (CBtnEnh*)GetFocus();
	CString strLog=_T("");
	int nPosType(0);

	switch(pBtnEnh->GetDlgCtrlID())
	{
	case IDC_TEACH_SH_POS_0:
		nPosType = m_nPosType = 0;
		break;
	case IDC_TEACH_SH_POS_1:
		nPosType = m_nPosType = 1;	
		break;
	case IDC_TEACH_SH_POS_2:
		nPosType = m_nPosType = 2;	
		break;
	case IDC_TEACH_SH_POS_3:
		nPosType = m_nPosType = 3;	
		break;
	}
}

void CDialogTeach::OnClickTeachSetData()
{
	CString strTmp=_T(""), strData=_T("");
	CDialogNumberBox dlg;
	CBtnEnh *pBtnEnh = (CBtnEnh*)GetFocus();

	GetDlgItemText(pBtnEnh->GetDlgCtrlID(), strTmp);
	dlg.SetstrNum(strTmp);

	if(dlg.DoModal() != IDOK) return;

	strTmp = dlg.GetstrNum();
	strData.Format(_T("%.3f"), _ttof(strTmp));
	GetDlgItem(pBtnEnh->GetDlgCtrlID())->SetWindowText(strData);
}


void CDialogTeach::ClickTeachMovePos()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;


	if(!pdlg->m_pDlgManual->AxisIsHomeFinished(m_nAxisID))
	{
		g_cMsgBox.OkMsgBox(_T("축 원점 작업이 완료되어 있지 않습니다."));
		return;
	}

	if(!pdlg->m_pDlgManual->ReadInput(LD_PNP_CYLINDER_UP_CHECK))
	{
		pdlg->m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_UP_SOL, TRUE);
		pdlg->m_pDlgManual->WriteOutput(LD_PNP_CYLINDER_DN_SOL, FALSE);
	}

	Delay(200);

	double dSetPos(0.0);
	CString strMsg = _T(""),  strLog=_T(""), strTemp;


	//X 축 3번 POS 은 현재 스페어 상태임.
	if( (AXIS_LD_PNP == m_nAxisID) && (3 == m_nPosType) )
		return;

	switch(m_nAxisID)
	{
	case AXIS_LD_PNP:
		dSetPos = _ttof(m_ctrlTeachPos_X[m_nPosType].GetCaption());
		break;
	case AXIS_TRY_SHUTTLE:
		dSetPos = _ttof(m_ctrlTeachPos_Y[m_nPosType].GetCaption());
		break;
	default:
		return;
	}
	
	strMsg.Format(_T("%s축을 %.3f 위치로 \n이동 하시겠습니까?"), pdlg->m_pDlgManual->AxisGetAxisName(m_nAxisID), dSetPos);
	if(g_cMsgBox.YesNoMsgBox(strMsg) != IDYES)
		return;
		
	strTemp.Format(_T("%s 축 이동 중.."), pdlg->m_pDlgManual->AxisGetAxisName(m_nAxisID));
	m_ctrl_progress.SetCaption(strTemp);
	m_ctrl_progress.ShowWindow(TRUE);

	SetTimer(TIMER_TEACH_PROGRESS, 200, NULL);

	pdlg->m_pDlgManual->AxisMove(m_nAxisID, dSetPos, 1, TRUE);
}


void CDialogTeach::DisplayModelData()
{
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	CString strData = _T("");
	
	strData.Format(_T("%3.3f"), m_pModelData->GetLP_LoadPosX());
	m_ctrlTeachPos_X[0].SetCaption(strData);
	strData.Format(_T("%3.3f"), m_pModelData->GetLP_InspPosX());
	m_ctrlTeachPos_X[1].SetCaption(strData);
	strData.Format(_T("%3.3f"), m_pModelData->GetLP_UnloadPosX());
	m_ctrlTeachPos_X[2].SetCaption(strData);
	strData.Format(_T("%3.3f"), m_pModelData->GetLP_SparePosX());
	m_ctrlTeachPos_X[3].SetCaption(strData);

	strData.Format(_T("%3.3f"), m_pModelData->GetIS_LoadPosY());
	m_ctrlTeachPos_Y[0].SetCaption(strData);
	strData.Format(_T("%3.3f"), m_pModelData->GetIS_InspPosY());
	m_ctrlTeachPos_Y[1].SetCaption(strData);
	strData.Format(_T("%3.3f"), m_pModelData->GetIS_UnloadPosY());
	m_ctrlTeachPos_Y[2].SetCaption(strData);
	strData.Format(_T("%3.3f"), m_pModelData->GetIS_CamCenterPosY());
	m_ctrlTeachPos_Y[3].SetCaption(strData);
}

void CDialogTeach::DisplayCurrentPos()
{
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	double dCurPos(0.0);
	CString strPos = _T("");
	int i(0), j(1), nStartAxis(0), nEndAxis(0);

	/*
		m_pAxis 배열은 Y축이 0으로 설정되어 있기 때문에 Teach U/I와 반대로 대입한다.
	*/	
	for(i = 0; i < MAX_AXIS_NUM; i++)
	{
		pdlg->m_pDlgManual->AxisGetPosition(i, dCurPos);

		strPos.Format(_T("%.3f"), dCurPos);
		m_ctrlAxisCurrentPos[j].SetCaption(strPos);
		j--;
	}
}

void CDialogTeach::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;
	DWORD Inpos;

	switch(nIDEvent)
	{
	case TIMER_TEACH_VIEW:
		DisplayCurrentPos();
		break;
	case TIMER_TEACH_PROGRESS:
		pdlg->m_pDlgManual->AxisReadInPosition(m_nAxisID, &Inpos);

		if(Inpos)
		{
			KillTimer(TIMER_TEACH_PROGRESS);
			m_nProcessMoveIdx = 0;
			m_ctrl_progress.SetProgressBarValue(m_nProcessMoveIdx);
			m_ctrl_progress.ShowWindow(FALSE);
		}
		else
		{
			if(m_nProcessMoveIdx >= 100)
				m_nProcessMoveIdx = 0;
			m_nProcessMoveIdx++;
			m_ctrl_progress.SetProgressBarValue(m_nProcessMoveIdx);
		}
		break;
	default:

		break;
	}

	CDialog::OnTimer(nIDEvent);
}


void CDialogTeach::ClickTeachMoveStop()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSDC_WindowControlPrjDlg * pdlg = (CSDC_WindowControlPrjDlg *)AfxGetApp()->m_pMainWnd;

	KillTimer(TIMER_TEACH_PROGRESS);
	m_nProcessMoveIdx = 0;
	m_ctrl_progress.SetProgressBarValue(m_nProcessMoveIdx);
	m_ctrl_progress.ShowWindow(FALSE);

	pdlg->m_pDlgManual->AxisStop(m_nAxisID);
	pdlg->m_pDlgManual->AxisEStop(m_nAxisID);
}


void CDialogTeach::ClickModelListUp()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

}


void CDialogTeach::ClickModelListDown()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
