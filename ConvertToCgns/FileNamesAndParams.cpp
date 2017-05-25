#include "StdAfx.h"
#include "FileNamesAndParams.h"


FileNamesAndParams::FileNamesAndParams()
{
	const int size    = 6;
	int lineCount     = 0;
	int lineCountMax  = 8;

	string tString = "t";
	string slashes = "\\";

	double degree = 2.0;

	//string initName = ".\\temp\\initCgns.dat";
	string initName = "initCgns.dat";
	
	ifstream inFile(initName);
	while(lineCount <= lineCountMax)
	{
		switch (lineCount)
		{
			case 0:
				inFile >> m_path;          // путь
				break;
			case 1:
				inFile >> m_name;          // имя проточной части
				break;
			case 2:
				inFile >> m_grid;          // имя сетки
				break;
			case 3:
				inFile >> m_flow;          // имя течения
				break;
			case 4:
				inFile >> m_cascadeNumber; // номер венца
				break;
			case 5:
				inFile >> m_levelNumber;   // уровень сетки
				break;
			case 6:
				inFile >> m_isGridAndSolution;  // сетка - 0 - vs сетка+расчет - 1
				break;
			case 7:
				inFile >> m_isDimensional;  // undim - 0 - vs dim - 1
				break;
			case 8:
				inFile >> m_cgns;           // имя сохраняемого cgns файла + путь
		}
		lineCount++;
	}

	if (m_path == " ")
	{
		cout << "FDataBase is located inside of current directory\n";
		m_path = ".\\";
	}

	stringstream outLevelNumber, outCascadeNumber;
		
	string fullPath         = m_path + slashes + m_name + slashes + m_grid + slashes;
	outLevelNumber          << m_levelNumber;
	string addString        = outLevelNumber.str() + ".da";
	outCascadeNumber        << m_cascadeNumber;
	string cascadeNumberStr = (m_cascadeNumber == 1) ? tString : "" + outCascadeNumber.str();
	
	m_threeDflow = fullPath + m_flow + slashes + "3d_flow.dat";
    m_geometry   = fullPath + "3d_geom.dat";

	m_dkx = fullPath + "dkx.da" + cascadeNumberStr;
	m_dky = fullPath + "dky.da" + cascadeNumberStr;
	m_dkz = fullPath + "dkz.da" + cascadeNumberStr;
	
	m_holes = fullPath + "motv.dat";
	m_h1    = fullPath + "notvc1"  + addString + cascadeNumberStr;
	m_h2    = fullPath + "notvc2"  + addString + cascadeNumberStr;
	m_hD    = fullPath + "notvcdn" + addString + cascadeNumberStr;
	m_hU    = fullPath + "notvcup" + addString + cascadeNumberStr;

	lineCount = 0;
	ifstream inFileGeom(m_geometry);  // чтение файла  3d_geom
	
	m_levelDims = new int[6];
	string str;

	while (lineCount <= lineCountMax)
	{
		switch (lineCount)
		{
			case 2:
				inFileGeom >> m_levelDimMax;
				break;
			case 4:
				inFileGeom >> m_levelDims[lineCount - 1];
				break;
			case 5:
				inFileGeom >> m_levelDims[lineCount - 1];
				break;
			case 6:
				inFileGeom >> m_levelDims[lineCount - 1];
				break;
			default:
				inFileGeom >> str;
				break;
		}
		lineCount++;
	}

	// размерности сетки, в зависиомсти от выбранного уровня сетки
	m_levelDims[0] = m_levelDims[3] / pow(degree, m_levelDimMax - m_levelNumber);
	m_levelDims[1] = m_levelDims[4] / pow(degree, m_levelDimMax - m_levelNumber);
	m_levelDims[2] = m_levelDims[5] / pow(degree, m_levelDimMax - m_levelNumber);

	cout<<m_levelDims[0]<<endl<<m_levelDims[1]<<endl<<m_levelDims[2]<<endl;
	cout<<m_levelDims[3]<<endl<<m_levelDims[4]<<endl<<m_levelDims[5]<<endl;

	int line        = 0;
	lineCount       = 0;
	m_isHolesExists = false;


	ifstream holeFile(m_holes);
	
	while (lineCount <= lineCountMax)
	{
		
		if ((holeFile >> line) != 0)
		{
			m_isHolesExists = true;
			break;
		}
		lineCount ++;
	}

	if (m_isGridAndSolution)
	{
		string fullFlowPath = fullPath + m_flow + slashes;

		m_density  = fullFlowPath + "r" + addString + cascadeNumberStr;
		m_pressure = fullFlowPath + "p" + addString + cascadeNumberStr;

		m_velocityU = fullFlowPath + "u" + addString + cascadeNumberStr;
		m_velocityV = fullFlowPath + "v" + addString + cascadeNumberStr;
		m_velocityW = fullFlowPath + "w" + addString + cascadeNumberStr;
	
		m_qK = fullFlowPath + "qp" + addString + cascadeNumberStr;
		m_qP = fullFlowPath + "qk" + addString + cascadeNumberStr;;
	}

	if (m_isDimensional)       // если необходима запись размерных параметров
	{
		lineCount = 0;
		ifstream inFileFlow(m_threeDflow); // файл 3d_flow

		m_densityCritical = 0.0;
		m_aCritical = 0.0;

		lineCountMax = 25;

		while (lineCount <= lineCountMax)
		{
			switch(lineCount)
			{
				case(23):
					inFileFlow >> m_densityCritical;
					break;
				case(25):
					inFileFlow >> m_aCritical;
					break;
				default:
					inFileFlow >> str;
					break;
			}
			lineCount++;
		}		
	}
}


FileNamesAndParams::~FileNamesAndParams(void)
{
}
