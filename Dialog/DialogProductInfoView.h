#pragma once


// CDialogProductInfoView ��ȭ �����Դϴ�.

class CDialogProductInfoView : public CDialog
{
	DECLARE_DYNAMIC(CDialogProductInfoView)

public:
	CDialogProductInfoView(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogProductInfoView();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_TOP_PRODUCTINFO_VIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
