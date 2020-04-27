#include "HashMapClusteringScheme.h"

HashMapClusteringScheme::HashMapClusteringScheme(vector<char> referenceGenome, int kmerLength) {
	this->KmerLength = kmerLength;
	this->GenomeMap = IndexGenome(referenceGenome);
}

vector<int> HashMapClusteringScheme::ExecuteScheme(vector<char> pacBioRead) {
	// generate locations of kmer substrings
	unordered_set<string> keys = GenerateKmerLengthSubstrings(pacBioRead);

	vector<int> locationList;
	for (string key : keys) {
		if (this->GenomeMap.find(key) != this->GenomeMap.end()) {
			auto locations = this->GenomeMap[key];
			for (int location : locations) {
				locationList.push_back(location);
			}
		}
	}

	sort(locationList.begin(), locationList.end());

	// group locations by distances based on tolerance
	int tolerance = 6;
	int currentGroup = locationList[0];
	int lastLocation = INT32_MIN/2;
	unordered_map<int, vector<int>> groups;
	for (int location : locationList) {
		if (location - lastLocation < tolerance) {
			groups[currentGroup].push_back(location);
		}
		else {
			currentGroup = location;
			vector<int> newGroupList;
			newGroupList.push_back(location);
			groups[currentGroup] = newGroupList;
		}
		lastLocation = location;
	}

	// find groups with biggest clumps
	vector<int> maxList;
	int currentMax = 0;
	for(auto group : groups) {
		if (group.second.size() > currentMax) {
			currentMax = group.second.size();
		}
	}

	for (auto group : groups) {
		if (group.second.size() == currentMax) {
			maxList.push_back(group.first);
		}
	}

	return maxList;
}

unordered_map<string, vector<int>> HashMapClusteringScheme::IndexGenome(vector<char> genomeToIndex) {
	unordered_map<string, vector<int>> res;

	for (unsigned int i = 0; i < genomeToIndex.size() - this->KmerLength; i++) {
		string currentString = "";
		for (int j = i; j < i + this->KmerLength; j++) {
			currentString += genomeToIndex.at(j);
		}

		if (res.find(currentString) == res.end()) {
			// hash map does not contain string
			vector<int> indexList;
			indexList.push_back(i);
			res[currentString] = indexList;
		}
		else {
			res[currentString].push_back(i);
		}
	}

	return res;
}

unordered_set<string> HashMapClusteringScheme::GenerateKmerLengthSubstrings(vector<char> read) {
	unordered_set<string> res;

	for (int i = 0; i < read.size() - this->KmerLength; i += this->KmerLength) {
		string currentString = "";
		for (int j = i; j < i + this->KmerLength; j++) {
			currentString += read.at(j);
		}

		res.insert(currentString);
	}

	return res;
}