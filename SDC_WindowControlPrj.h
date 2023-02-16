
// SDC_WindowControlPrj.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.


// CSDC_WindowControlPrjApp:
// 이 클래스의 구현에 대해서는 SDC_WindowControlPrj.cpp을 참조하십시오.
//

class CSDC_WindowControlPrjApp : public CWinApp
{
public:
	CSDC_WindowControlPrjApp();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

// 구현입니다.

	DECLARE_MESSAGE_MAP()
};

extern CSDC_WindowControlPrjApp theApp;