/*
	* PacBio Seeder
	* Authors:
	* Patrick Finger, Robert Oller, Benjamin Ladick
	* Brief:
	* This program locates potential seeds (or indecies)
	* of PacBio indel errors.
*/
#include <iostream>
#include <stdlib.h>
#include "Loader.h"
#include "HashMapClusteringScheme.h"

int main()
{
	Loader loader;
	auto referenceGenome = loader.LoadRawTextFile(".\\testsim\\testsim.txt");
	auto pacBioReads = loader.LoadFastQReads(".\\testsim\\testsim_0001.fastq", 4);
	auto solutionIndexes = loader.LoadSolutions(".\\testsim\\testsim_0001.maf", 4);
	HashMapClusteringScheme* scheme = new HashMapClusteringScheme(referenceGenome, 9);

	vector<int> indexes;
	for (int i = 0; i < 4; i++) {
		auto res = scheme->ExecuteScheme(pacBioReads[i], 4);
		for (int j = 0; j < 4; j++) {
			cout << "Scheme result: " << res[0] << endl;
			cout << "Solution: " << solutionIndexes[j] << endl;
			cout << "Difference: " << solutionIndexes[j] - res[0] << endl << endl;
		}
		cout << endl << endl << endl;
	}
}
