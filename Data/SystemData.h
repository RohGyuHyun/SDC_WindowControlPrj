#pragma once

typedef struct LogInData
{
	CString UserName;
	CString ID;
	CString Password;

}typeLogInData;

class CSystemData
{
public:
	CSystemData(void);
	~CSystemData(void);

protected:
	//Login 관련
	typeLogInData m_sLogInUser;
	
	typeLogInData m_sCurrentUser;
	typeLogInData m_sOperator;
	typeLogInData m_sEngineer;
	typeLogInData m_sMaker;

	CString m_strWorkModelName;

	//OPTION 관련
	double m_dVelRatio;
	int m_nOpMode;
	BOOL m_bManualMuting;
	BOOL m_bManualSocket;
	BOOL m_bManualSkipFilmRemove;

	//모션 시퀀스 관련
	BOOL m_bSysPrepare;
	BOOL m_bUnitInit;

	//COM
	int m_nPortNoVisionPC;
	CString m_strIPAddressVisionPC;
public:
	//모션 시퀀스 관련
	void SetSysprepare(BOOL bVal = false) {m_bSysPrepare = bVal;}
	BOOL GetSysprepare() const {return m_bSysPrepare;}
	void SetUnitInit(BOOL bVal = false) {m_bUnitInit = bVal;}
	BOOL GetUnitInit() const{return m_bUnitInit;}

	//Get / Set Function
	typeLogInData GetCurUser() { return m_sCurrentUser;}
	typeLogInData GetOperator() { return m_sOperator;}
	typeLogInData GetEngineer() { return m_sEngineer;}
	typeLogInData GetMaker() { return m_sMaker;}

	CString GetWorkModelName() { return m_strWorkModelName; }
	void SetWorkModelName(CString strModelName) { m_strWorkModelName = strModelName; }

	//User Log
	void SetCurUserName(CString strName) { m_sCurrentUser.UserName = strName;}
	CString GetCurUserName() { return m_sCurrentUser.UserName;}

	void SetCurUserPW(CString strPW) { m_sCurrentUser.Password = strPW;}
	CString GetCurUserPW() { return m_sCurrentUser.Password;}

	void SetOperatorName(CString strName) { m_sOperator.UserName = strName;}
	CString GetOperatorName() { return m_sOperator.UserName;}
	void SetOperatorPW(CString strPW) { m_sOperator.Password = strPW;}
	CString GetOperatorPW() { return m_sOperator.Password;}
	
	void SetEngineerName(CString strName) { m_sEngineer.UserName = strName;}
	CString GetEngineerName() { return m_sEngineer.UserName;}
	void SetEngineerPW(CString strPW) { m_sEngineer.Password = strPW;}
	CString GetEngineerPW() { return m_sEngineer.Password;}
	
	void SetMakerName(CString strName) { m_sMaker.UserName = strName;}
	CString GetMakerName() { return m_sMaker.UserName;}
	void SetMakerPW(CString strPW) { m_sMaker.Password = strPW;}
	CString GetMakerPW() { return m_sMaker.Password;}
	

	//OPTION 관련
	void SetSystemVelRatio(double dvelRatio = 0.0) {m_dVelRatio = dvelRatio;}
	double GetSystemVelRatio() const{return m_dVelRatio;}

	void SetSystemOpMode(int nMode) { m_nOpMode = nMode; }
	int GetSystemOpMode() { return m_nOpMode; }

	void SetSystemManualMuting(BOOL bState) { m_bManualMuting = bState; }
	BOOL GetSystemManualMuting() { return m_bManualMuting; }

	void SetSystemManualSocket(BOOL bState) { m_bManualSocket = bState; }
	BOOL GetSystemManualSocket() { return m_bManualSocket; }

	void SetSystemManualSkipFilmRemove(BOOL bState) { m_bManualSkipFilmRemove = bState; }
	BOOL GetSystemManualSkipFilmRemove() { return m_bManualSkipFilmRemove; }

	//COM
	void SetPortNoVisionPC(int nVal = 7000) {m_nPortNoVisionPC = nVal;}
	int GetPortNoVisionPC() {return m_nPortNoVisionPC;}
	void SetIPAddressVisionPC(CString strVal = _T("192.168.10.150")) {m_strIPAddressVisionPC = strVal;}
	CString GetIPAddressVisionPC() {return m_strIPAddressVisionPC;}


public:
	BOOL LoadSystemData();
	BOOL SaveSystemData();

};
