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
	auto pacBioReads = loader.LoadFastQReads(".\\testsim\\testsim_0001.fastq", 1);
	HashMapClusteringScheme* scheme = new HashMapClusteringScheme(referenceGenome, 5);
	auto res = scheme->ExecuteScheme(pacBioReads[0]);

	for (auto location : res) {
		cout << location << ", ";
	}
	cout << endl;
}