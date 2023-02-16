#pragma once

#include "Axis.h"

class CMotion
{
public:
	CMotion(void);
	~CMotion(void);

protected:
	

public:
	CAxis *m_pAxis[MAX_AXIS_NUM];

	BOOL Initialize();
	void SetAxis();
	BOOL SetAxisConfig(int axis_no);
	BOOL LoadAxisConfig();
	BOOL SaveAxisConfig();
	BOOL AmpEnableAll();
	BOOL AmpDisableAll();

	BOOL SetOriginMethod(int axis_no) { return m_pAxis[axis_no]->SetOriginMethod(); }
	BOOL StartOrigin(int axis_no) { return m_pAxis[axis_no]->StartOrigin(); }
	DWORD ReadOriginResult(int axis_no) { return m_pAxis[axis_no]->ReadOriginResult(); }
	BOOL SetPosition(int axis_no, double dPos) { return m_pAxis[axis_no]->SetPosition(dPos); }
	void SetOriginReturn(int axis_no, BOOL bState) { m_pAxis[axis_no]->SetOriginReturn(bState); }
	
};
