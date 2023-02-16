// AxisConfig.cpp: implementation of the CAxisConfig class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AxisConfig.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAxisConfig::CAxisConfig()
{
	CString strFilePath = _T("");
	strFilePath.Format(_T("%s"),DATA_AXIS_CONFIG_FILE);

	CProfileDataIF::SetFilePath(strFilePath);
}

CAxisConfig::~CAxisConfig()
{

}
