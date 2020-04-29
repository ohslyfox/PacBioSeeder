#ifndef __SCHEME_OPTIONS__
#define __SCHEME_OPTIONS__

#include <string>
#include <vector>

using namespace std;

class SchemeOptions {
public:
	SchemeOptions();

	enum class RunType {
		NoRunType,
		ResultsOnly,
		CompareToSolution
	};
	
	enum class Scheme {
		NoScheme,
		MaxGroup,
		MinScore,
		MinFrameSize,
		MaxDensity,
		MinFrameMaxDensity
	};

	RunType RunType;
	Scheme Scheme;
	string FileName;
	int KmerLength;
	vector<char> ReferenceGenome;
	vector<vector<char>> PacBioReads;
	vector<int> Solutions;
};

#endif
