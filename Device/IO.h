#pragma once

#include "AXL.h"
#include "AXD.h"

#define MAX_DIO_MODULE_NUM	4

typedef struct DIOINFO
{
	long	m_IBoardNo;
	long	m_IModulePos;
	DWORD	m_dwModuleID;
	CString m_strModuleName;

}typeDIOInfo;


class CIO
{
public:
	CIO(void);
	~CIO(void);

protected:
	typeDIOInfo m_DIOInfo[MAX_DIO_MODULE_NUM];

public:
	BOOL AXTDIOInit();
	BOOL AXTLibInit();
	BOOL AXTLibClose();

	void WriteDO(int nBit, BOOL bVal);
	BOOL ReadDO(int nBit);
	BOOL ReadDI(int nBit);
};