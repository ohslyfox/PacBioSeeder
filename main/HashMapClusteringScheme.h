#ifndef __HASH_MAP_CLUSTERING_SCHEME__
#define __HASH_MAP_CLUSTERING_SCHEME__

#include <vector>
#include <iostream>
#include <algorithm>
#include <tuple>
#include<unordered_map>
#include<unordered_set>

using namespace std;

class HashMapClusteringScheme {
public:
	HashMapClusteringScheme(vector<char> referenceGenome, int kmerLength);

	vector<int> ExecuteScheme(vector<char> pacBioRead, int tolerance);
private:
	struct Frame {
		int left;
		int frameSize;
	};
	int KmerLength;
	unordered_map<string, vector<int>> GenomeMap;
	unordered_map<string, vector<int>> IndexGenome(vector<char> genomeToIndex);
	unordered_set<string> GenerateKmerLengthSubstrings(vector<char> read);
	void CalculateDensityConcentration(int left, int right, vector<int> locations, vector<Frame>* res);
};

#endif
