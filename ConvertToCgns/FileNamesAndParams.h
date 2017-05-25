#pragma once

using namespace std;

class FileNamesAndParams
{
	string m_path;
	string m_cgns;
	string m_name;
	string m_grid;
	string m_flow;
	
	string m_geometry;
	string m_dkx, m_dky, m_dkz;
	string m_threeDflow;

	string m_holes, m_h1, m_h2, m_hD, m_hU;

	string m_density, m_pressure;
	string m_velocityU, m_velocityV, m_velocityW;
	string m_qK, m_qP;

	int  m_cascadeNumber;
	int  m_levelNumber;
	int *m_levelDims;
	int  m_levelDimMax;

	double m_densityCritical;
	double m_aCritical;

	bool m_isGridAndSolution;
	bool m_isDimensional;
	bool m_isHolesExists;

public:

	FileNamesAndParams(void);
	~FileNamesAndParams(void);

	int *getGeometryParams(){ return m_levelDims; }
	int getLevelDimMax()    { return m_levelDimMax; }
	int getLevelNumber()    { return m_levelNumber; }

	double getDensityCritical(){ return m_densityCritical; }
	double getACritical()      { return m_aCritical; }

	string getH1Name(){ return m_h1; }
	string getH2Name(){ return m_h2; }
	string getHDName(){ return m_hD; }
	string getHUName(){ return m_hU; }

	string getCgnsFileName(){ return m_cgns; }
	string getDkxName()     { return m_dkx; }
	string getDkyName()     { return m_dky; }
	string getDkzName()     { return m_dkz; }
	
	string getDensityName() { return m_density; }
	string getPressureName(){ return m_pressure; }
	string getUName()       { return m_velocityU; }
	string getVName()       { return m_velocityV; }
	string getWName()       { return m_velocityW; }

	string getQkName(){ return m_qK; }
	string getQpName(){ return m_qP; }

	bool isHolesExists()    { return m_isHolesExists; }
	bool isGridAndSolution(){ return m_isGridAndSolution; }
	bool isDimensional()    { return m_isDimensional; }
};

