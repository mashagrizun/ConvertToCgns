// ConvertToCgns.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ConvertToCgns.h"

ConvertToCgns::ConvertToCgns(void)
{
	m_filesAndParams = new FileNamesAndParams();
}

double *ConvertToCgns::readCoordinates(
	string fileName
  )
{
	const int iSize1 = (m_filesAndParams -> getGeometryParams()[3] + 1);
	const int jSize1 = (m_filesAndParams -> getGeometryParams()[4] + 1);
	const int kSize1 = (m_filesAndParams -> getGeometryParams()[5] + 1);

	const int size = (m_filesAndParams -> getGeometryParams()[0] + 1) *
					 (m_filesAndParams -> getGeometryParams()[1] + 1) *
					 (m_filesAndParams -> getGeometryParams()[2] + 1);

	int s    = 0;
	int step = pow(2.0, m_filesAndParams->getLevelDimMax() - m_filesAndParams->getLevelNumber());
	
	double *coordinates = new double[size];
	double ***buf;
	
    ifstream infile(fileName,ios::binary);

	buf = new double **[iSize1];
	for (int i = 0; i < iSize1; i++)
		buf[i] = new double *[jSize1];
    
	for (int i = 0; i < iSize1; i++)
		for (int j = 0; j < jSize1; j++)
			buf[i][j] = new double [kSize1];

    for (int k = 0; k < kSize1; k++)
		for (int i = 0; i < iSize1; i++)
			for (int j = 0; j < jSize1; j++)
				infile.read(reinterpret_cast<char*>(&buf[i][j][k]), sizeof(double));
    
	for (int k = 0; k < iSize1; k = k + step)
		for (int j = 0; j < jSize1; j = j + step)
            for (int i = 0; i < iSize1; i = i + step)
			{
				coordinates[s]=buf[i][j][k]; 
				cout<<s<<" "<<coordinates[s]<<endl; 
				s++;
			}
	
	return coordinates;
}
int *ConvertToCgns::readHoles()
{
	const int iSize = m_filesAndParams -> getGeometryParams()[0];
	const int jSize = m_filesAndParams -> getGeometryParams()[1];
	const int kSize = m_filesAndParams -> getGeometryParams()[2];

	int s=0; 

	int   *holes;
	int ***buf;
	
	ifstream infileH1(m_filesAndParams -> getH1Name());
    ifstream infileH2(m_filesAndParams -> getH2Name());
    ifstream infileHd(m_filesAndParams -> getHDName());
    ifstream infileHu(m_filesAndParams -> getHUName());

	buf = new int **[iSize];
	for (int i = 0; i < iSize; i++)
		buf[i] = new int *[jSize];
    
	for (int i = 0; i < iSize; i++)
		for (int j = 0; j < jSize; j++)
			buf[i][j] = new int [kSize];

    for (int k = 0; k < kSize; k++)
		for (int i = 0; i < iSize; i++)
			for (int j = 0; j < jSize; j++)
				buf[i][j][k]=0;

	holes = new int[iSize * jSize * kSize];

	for (int k = 0; k < kSize; k++)
		for (int i = 0; i < iSize; i++)
			infileH1.read(reinterpret_cast<char*>(&buf[i][0][k]), sizeof(char));

	for (int k = 0; k < kSize; k++)
		for (int i = 0; i < iSize; i++)
			infileH2.read(reinterpret_cast<char*>(&buf[i][jSize-1][k]), sizeof(char));

	for (int k = 0; k < kSize; k++)
		for (int j = 0; j < jSize; j++)
			infileHd.read(reinterpret_cast<char*>(&buf[0][j][k]), sizeof(char));

	for (int k = 0; k < kSize; k++)
		for (int j = 0; j < jSize; j++)
			infileHu.read(reinterpret_cast<char*>(&buf[iSize-1][j][k]), sizeof(char));
    
    for (int k = 0; k < kSize; k++)
		for (int j = 0; j < jSize; j++)
			for (int i = 0; i < iSize; i++)
			{
				holes[s]=buf[i][j][k]; \
	            s++;
			}

	return holes;
}

float *ConvertToCgns::readSolution(
	string fileName
  )
{
	const int iSize = m_filesAndParams -> getGeometryParams()[0];
	const int jSize = m_filesAndParams -> getGeometryParams()[1];
	const int kSize = m_filesAndParams -> getGeometryParams()[2];

	int s = 0;
	
	float *solution = new float[iSize * jSize * kSize];
	float ***buf;
	
    ifstream infile(fileName,ios::binary);

	buf = new float **[iSize];
	for (int i = 0; i < iSize; i++)
		buf[i] = new float *[jSize];
    
	for (int i = 0; i < iSize; i++)
		for (int j = 0; j < jSize; j++)
			buf[i][j] = new float [kSize];

    for (int k = 0; k < kSize; k++)
		for (int i = 0; i < iSize; i++)
			for (int j = 0; j < jSize; j++)
				infile.read(reinterpret_cast<char*>(&buf[i][j][k]), sizeof(float));
    
    for (int k = 0; k < kSize; k++)
		for (int j = 0; j < jSize; j++)
			for (int i = 0; i < iSize; i++)
			{
				solution[s]=buf[i][j][k]; 
				s++;
			}
	
	if (m_filesAndParams -> isDimensional())
	{
		double densityCritical = m_filesAndParams -> getDensityCritical();
		double aCritical       = m_filesAndParams -> getACritical();

		for (s = 0; s < iSize * jSize * kSize; s++)
		{
			if (fileName == m_filesAndParams -> getDensityName())
				solution[s] = densityCritical*solution[s];
			else
			if (fileName == m_filesAndParams -> getPressureName())
				solution[s] = densityCritical*aCritical*aCritical*solution[s];
			else
			if (fileName == m_filesAndParams -> getUName() ||
				fileName == m_filesAndParams -> getVName() ||
				fileName == m_filesAndParams -> getWName()
			   )
			{
				solution[s] = aCritical*solution[s];
			}
			else
			if (fileName == m_filesAndParams -> getQkName())
				solution[s] = aCritical*aCritical*solution[s];
			else
			if (fileName == m_filesAndParams -> getQpName())
				solution[s] = aCritical*solution[s];
		}
	}

	return solution;
}

void ConvertToCgns::writeCgnsFiles()
{
	const int size = 3;
	cgsize_t isize[size][size];
	
	int indexFile  = size,
		iCellDim   = size, 
		iPhysDim   = size, 
		indexBase  = 0, 
		indexFlow  = 0,
		indexZone  = 0,
		indexCoord = 0,
		indexField = 0;

	string baseName     = "Base", 
		   zoneName     = "Zone  1",
		   solutionName = "FlowSolution";

	const char *cgnsName = m_filesAndParams -> getCgnsFileName().c_str();
	if (cg_open(cgnsName, CG_MODE_WRITE, &indexFile)) 
		cg_error_exit();

	cg_base_write(indexFile, baseName.c_str(), iCellDim, iPhysDim, &indexBase);
	
	isize[0][0] = m_filesAndParams -> getGeometryParams()[0] + 1;
	isize[0][1] = m_filesAndParams -> getGeometryParams()[1] + 1;
	isize[0][2] = m_filesAndParams -> getGeometryParams()[2] + 1;

	isize[1][0] = isize[0][0] - 1;
	isize[1][1] = isize[0][1] - 1;
	isize[1][2] = isize[0][2] - 1;

	isize[2][0] = 0;
	isize[2][1] = 0;
	isize[2][2] = 0;

	cg_zone_write(indexFile, indexBase, zoneName.c_str(), *isize, Structured, &indexZone);

	cg_coord_write(
		indexFile, indexBase, indexZone, 
		RealDouble, "CoordinateX", 
		readCoordinates(m_filesAndParams -> getDkzName()), 
		&indexCoord
	  );

	cg_coord_write(
		indexFile, indexBase, indexZone, 
		RealDouble, "CoordinateY", 
		readCoordinates(m_filesAndParams -> getDkxName()), 
		&indexCoord
	  );

	cg_coord_write(
		indexFile, indexBase, indexZone, 
		RealDouble, "CoordinateZ", 
		readCoordinates(m_filesAndParams -> getDkyName()), 
		&indexCoord
	  );

	if (m_filesAndParams -> isHolesExists()) // если отверстия есть
	{
		cout << "Holes!" << endl;
			
		indexBase = 1;
		indexZone = 1;

		cg_sol_write(
			indexFile, indexBase, indexZone, 
			solutionName.c_str(), CellCenter,
			&indexFlow
		);

		cg_field_write(
			indexFile, indexBase, indexZone,
			indexFlow, Integer, "Holes", 
			readHoles(), 
			&indexField
		  );
	}

	if (m_filesAndParams -> isGridAndSolution())
	{
		if (!m_filesAndParams -> isHolesExists())
		{
			indexBase = 1;
			indexZone = 1;

			cg_sol_write(
				indexFile, indexBase, indexZone, 
				solutionName.c_str(), CellCenter,
				&indexFlow
			);
		}
		
		cg_field_write(indexFile, indexBase, indexZone, 
			indexFlow, RealDouble, 
			"Density", readSolution(m_filesAndParams -> getDensityName()), 
			&indexField
		  );

		cg_field_write(indexFile, indexBase, indexZone, 
			indexFlow, RealDouble, 
			"Pressure", readSolution(m_filesAndParams -> getPressureName()), 
			&indexField
		  );

		cg_field_write(indexFile, indexBase, indexZone, 
			indexFlow, RealDouble, 
			"VelocityX", readSolution(m_filesAndParams -> getWName()), 
			&indexField
		  );

		cg_field_write(indexFile, indexBase, indexZone, 
			indexFlow, RealDouble, 
			"VelocityY", readSolution(m_filesAndParams -> getUName()), 
			&indexField
		  );

		cg_field_write(indexFile, indexBase, indexZone, 
			indexFlow, RealDouble, 
			"VelocityZ", readSolution(m_filesAndParams -> getVName()), 
			&indexField
		  );

		cg_field_write(indexFile, indexBase, indexZone, 
			indexFlow, RealDouble, 
			"K", readSolution(m_filesAndParams -> getQkName()), 
			&indexField
		  );

		cg_field_write(indexFile, indexBase, indexZone, 
			indexFlow, RealDouble, 
			"Omega", readSolution(m_filesAndParams -> getQpName()), 
			&indexField
		  );
	}

}

ConvertToCgns::~ConvertToCgns(void)
{
}
