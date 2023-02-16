// DialogProductInfoView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SDC_WindowControlPrj.h"
#include "DialogProductInfoView.h"
#include "afxdialogex.h"


// CDialogProductInfoView 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogProductInfoView, CDialog)

CDialogProductInfoView::CDialogProductInfoView(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogProductInfoView::IDD, pParent)
{

}

CDialogProductInfoView::~CDialogProductInfoView()
{
}

void CDialogProductInfoView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogProductInfoView, CDialog)
END_MESSAGE_MAP()


// CDialogProductInfoView 메시지 처리기입니다.


BOOL CDialogProductInfoView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) 
			return TRUE;
	}


	return CDialog::PreTranslateMessage(pMsg);
}
