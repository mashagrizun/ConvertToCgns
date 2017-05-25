#pragma once

using namespace std;

#if CGNS_VERSION < 3100
# define cgsize_t int
#else
# if CG_BUILD_SCOPE
#  error enumeration scoping needs to be off
# endif
#endif

class ConvertToCgns
{
	FileNamesAndParams *m_filesAndParams;

	double *m_coordinates;

public:
	ConvertToCgns(void);
	~ConvertToCgns(void);

	double *readCoordinates(
		string fileName
	  );
	
	float *readSolution(
		string fileName
	  );
	
	int *readHoles();

	void writeCgnsFiles();
};

