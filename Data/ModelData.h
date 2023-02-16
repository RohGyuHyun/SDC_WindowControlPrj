#pragma once

typedef struct ModelData
{
	CString m_strCurrentModelName;
	CString m_strCurrentModelDirectory;


}typeModelData;

class CModelData
{
public:
	CModelData(void);
	~CModelData(void);

protected:
	typeModelData m_ModelData;

public:
	double m_dLP_LoadPosX;
	double m_dLP_InspPosX;
	double m_dLP_UnloadPosX;
	double m_dLP_SparePosX;

	double m_dIS_LoadPosY;
	double m_dIS_InspPosY;
	double m_dIS_UnloadPosY;
	double m_dIS_CamCenterPosY;

	void SetLP_LoadPosX(double dPos) { m_dLP_LoadPosX = dPos; }
	double GetLP_LoadPosX() { return m_dLP_LoadPosX; }
	
	void SetLP_InspPosX(double dPos) { m_dLP_InspPosX = dPos; }
	double GetLP_InspPosX() { return m_dLP_InspPosX; }

	void SetLP_UnloadPosX(double dPos) { m_dLP_UnloadPosX = dPos; }
	double GetLP_UnloadPosX() { return m_dLP_UnloadPosX; }

	void SetLP_SparePosX(double dPos) { m_dLP_SparePosX = dPos; }
	double GetLP_SparePosX() { return m_dLP_SparePosX; }

	void SetIS_LoadPosY(double dPos) { m_dIS_LoadPosY = dPos; }
	double GetIS_LoadPosY() { return m_dIS_LoadPosY; }
	
	void SetIS_InspPosY(double dPos) { m_dIS_InspPosY = dPos; }
	double GetIS_InspPosY() { return m_dIS_InspPosY; }

	void SetIS_UnloadPosY(double dPos) { m_dIS_UnloadPosY = dPos; }
	double GetIS_UnloadPosY() { return m_dIS_UnloadPosY; }

	void SetIS_CamCenterPosY(double dPos) { m_dIS_CamCenterPosY = dPos; }
	double GetIS_CamCenterPosY() { return m_dIS_CamCenterPosY; }

	BOOL LoadModelData(CString strModelName);
	BOOL SaveModelData(CString strModelName);

	//GET / SET Function

	CString GetWorkModelName() { return m_ModelData.m_strCurrentModelName; }
	void SetWorkModelName(CString strModelName) { }


};
