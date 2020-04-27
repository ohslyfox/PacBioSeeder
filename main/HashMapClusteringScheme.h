#ifndef __HASH_MAP_CLUSTERING_SCHEME__
#define __HASH_MAP_CLUSTERING_SCHEME__

#include <vector>
#include <iostream>
#include <algorithm>
#include<unordered_map>
#include<unordered_set>

using namespace std;

class HashMapClusteringScheme {
public:
	HashMapClusteringScheme(vector<char> referenceGenome, int kmerLength);

	vector<int> ExecuteScheme(vector<char> pacBioRead);
private:
	int KmerLength;
	unordered_map<string, vector<int>> GenomeMap;
	unordered_map<string, vector<int>> IndexGenome(vector<char> genomeToIndex);
	unordered_set<string> GenerateKmerLengthSubstrings(vector<char> read);
};

#endif
