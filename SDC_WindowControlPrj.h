
// SDC_WindowControlPrj.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CSDC_WindowControlPrjApp:
// �� Ŭ������ ������ ���ؼ��� SDC_WindowControlPrj.cpp�� �����Ͻʽÿ�.
//

class CSDC_WindowControlPrjApp : public CWinApp
{
public:
	CSDC_WindowControlPrjApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CSDC_WindowControlPrjApp theApp;