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
#include "Analyzer.h"
#include "HashMapClusteringScheme.h"

int main()
{
	Loader loader;
	auto referenceGenome = loader.LoadRawTextFile(".\\testsim\\testsim.txt");
	auto pacBioReads = loader.LoadFastQReads(".\\testsim\\testsim_0001.fastq", 4);
	HashMapClusteringScheme* scheme = new HashMapClusteringScheme(referenceGenome, 9);

	for (int i = 0; i < 4; i++) {
		auto res = scheme->ExecuteScheme(pacBioReads[i], 4);
		for (auto location : res) {
			cout << location << ", ";
		}
		cout << endl << endl;
	}
}