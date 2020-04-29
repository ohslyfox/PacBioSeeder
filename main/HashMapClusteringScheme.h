#ifndef __HASH_MAP_CLUSTERING_SCHEME__
#define __HASH_MAP_CLUSTERING_SCHEME__

#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include "SchemeOptions.h"

using namespace std;

class HashMapClusteringScheme {
public:
	HashMapClusteringScheme(SchemeOptions* options);
	~HashMapClusteringScheme();

	void ExecuteScheme();
private:
	struct Frame {
		int left;
		int frameSize;
	};
	SchemeOptions* Options;
	unordered_map<string, vector<int>> GenomeMap;
	unordered_map<string, vector<int>> IndexGenome(vector<char> genomeToIndex);
	unordered_set<string> GenerateKmerLengthSubstrings(vector<char> read, int multiplier);
	void CalculateDensityConcentration(int left, int right, vector<int> locations, vector<Frame>* res);

	vector<int> MaxGroupScheme(vector<char> pacBioRead);
	vector<int> MinScoreScheme(vector<char> pacBioRead);
	vector<int> MinFrameSizeScheme(vector<char> pacBioRead);
};

#endif
