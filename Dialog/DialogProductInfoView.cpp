// DialogProductInfoView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SDC_WindowControlPrj.h"
#include "DialogProductInfoView.h"
#include "afxdialogex.h"


// CDialogProductInfoView ��ȭ �����Դϴ�.

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


// CDialogProductInfoView �޽��� ó�����Դϴ�.


BOOL CDialogProductInfoView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) 
			return TRUE;
	}


	return CDialog::PreTranslateMessage(pMsg);
}
