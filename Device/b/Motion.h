// Motion.h: interface for the CMotion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOTION_H__FA61C432_FD7B_4B4E_B2D9_AA5343D9283D__INCLUDED_)
#define AFX_MOTION_H__FA61C432_FD7B_4B4E_B2D9_AA5343D9283D__INCLUDED_
/*
#include "Define.h"
#include "Axis.h"	// Added by ClassView
#include "Axis_def.h" //130214 JSPark
#include "AxisConfig.h"
*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define NO_OF_MMCBD		 1
#define MAX_NODE_NUM     4	

class CMotion  
{
public:
	CMotion();
	virtual ~CMotion();
	/*
	void DestroyMotion();
	void CreateMotion();
	BOOL SetSyncControl(int master, int slave, long gain);
	BOOL GetSyncControl(int master);
	BOOL RetSyncControl(int master, int slave);

	void ResetHoming(int iAxis); //131013 JSPark
	void MoveStopAll();
	void MoveEstopAll();
	void ResetHomingAll();
	BOOL IsHoming();
	void BrakeHoming();
	void ClearHomingErr();
	BOOL AmpDisableAll();
	BOOL AmpEnableAll();
	void SetHomingOrder();
	bool CheckOrigin(int nAxis=MAX_AXIS_NUM);
	bool CheckAmp(); //JMHWANG 130826
	void SetAxisOrder();
	void SetAxis();
	void SafeOperation(void);

	//>> ¹°·ù Axis
	CAxis *m_paxAlignX;
	CAxis *m_paxAlignY;
	CAxis *m_paxAlignZ;
	CAxis *m_paxAlignT;
	CAxis *m_paxShuttleX;
	CAxis *m_paxShuttleZ;
	CAxis *m_paxUnloadX;
	CAxis *m_paxUnloadZ;
	CAxis *m_paxLoadCV;



	CAxisConfig* m_pAxisConfig;

	bool MMC_INITIALZED;
	bool Close();
	bool Initialize(CAxisConfig* axisConfig);
	bool LoadAxisConfig();
	bool SaveAxisConfig();
	void ReadAxisConfig(int axis_no);
	bool SetAxisConfig(int axis_no);
#ifdef _USE_NMC
	bool m_bMNCInitFlage;
#endif
	//For MMC Or ML2
	CAxis *m_pAxis[MAX_AXIS_NUM];
#ifdef _USE_DNET
	UINT8 portID;
	int mAxisNumCount;
#endif
	//////////////////////////////////////////////////////////////////////////

	*/
};

extern CMotion g_Motion;


#endif // !defined(AFX_MOTION_H__FA61C432_FD7B_4B4E_B2D9_AA5343D9283D__INCLUDED_)
