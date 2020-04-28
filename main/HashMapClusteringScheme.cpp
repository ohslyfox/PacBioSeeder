#include "HashMapClusteringScheme.h"

HashMapClusteringScheme::HashMapClusteringScheme(vector<char> referenceGenome, int kmerLength) {
	this->KmerLength = kmerLength;
	this->GenomeMap = IndexGenome(referenceGenome);
}

vector<int> HashMapClusteringScheme::ExecuteScheme(vector<char> pacBioRead, int tolerance) {
	vector<int> res;

	// generate locations of kmer substrings
	unordered_set<string> keys = GenerateKmerLengthSubstrings(pacBioRead);

	vector<int> locationList;
	unordered_map<int, string> keyMap;
	for (string key : keys) {
		if (this->GenomeMap.find(key) != this->GenomeMap.end()) {
			auto locations = this->GenomeMap[key];
			for (int location : locations) {
				locationList.push_back(location);
				keyMap[location] = key;
			}
		}
	}

	//if (locationList.size() <= 0) return res;
	//sort(locationList.begin(), locationList.end());
	//
	//// group locations by distances based on tolerance
	////int maxGroupSize = pacBioRead.size() / this->KmerLength;
	//int currentGroup = locationList[0];
	////int currentGroupSize = 0;
	//int lastLocation = INT32_MIN/2;
	//tolerance = this->KmerLength * 3;
	//unordered_map<int, vector<int>> groups;
	//for (int location : locationList) {
	//	if (location - lastLocation < tolerance) {
	//		groups[currentGroup].push_back(location);
	//	}
	//	else {
	//		currentGroup = location;
	//		vector<int> newGroupList;
	//		newGroupList.push_back(location);
	//		groups[currentGroup] = newGroupList;
	//		//currentGroupSize++;
	//	}
	//	lastLocation = location;
	//}


	vector<Frame> densities;
	CalculateDensityConcentration(0, this->GenomeMap.size(), locationList, &densities);
	
	if (densities.size() > 0) {
		// find min frame size
		Frame min = densities[0];
		for (Frame density : densities) {
			if (density.frameSize < min.frameSize) {
				min = density;
			}
		}
		
		int currentLocation = min.left;
		while (keyMap.find(currentLocation) == keyMap.end()) {
			currentLocation++;
		}
		res.push_back(currentLocation);
	}




	// find groups with biggest clumps
	/*int currentMax = 0;
	for(auto group : groups) {
		if (group.second.size() > currentMax) {
			currentMax = group.second.size();
		}
	}

	for (auto group : groups) {
		if (group.second.size() == currentMax) {
			res.push_back(group.first);
		}
	}*/

	// score groups
	/*unordered_map<int, int> locationScores;
	int minScore = INT32_MAX;
	for (auto group : groups) {
		int currentScore = 0;
		auto values = group.second;

		for (int i = 1; i < values.size(); i++) {
			currentScore += values[i] - values[i - 1];
		}
		locationScores[group.first] = currentScore;
		if (currentScore < minScore) {
			minScore = currentScore;
		}
	}

	for (auto score : locationScores) {
		if (score.second == minScore) {
			res.push_back(score.first);
		}
	}*/

	//sort(res.begin(), res.end());
	return res;
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

	for (int i = 0; i < read.size() - this->KmerLength; i+= this->KmerLength * 2) {
		string currentString = "";
		for (int j = i; j < i + this->KmerLength; j++) {
			currentString += read.at(j);
		}

		res.insert(currentString);
	}

	return res;
}

void HashMapClusteringScheme::CalculateDensityConcentration(int left, int right, vector<int> locations, vector<Frame> *res) {
	if (right - left <= 1) {
		return;
	}

	// handle current
	// first find all locations within the frame size
	vector<int> validLocations;
	for (auto location : locations) {
		if (location <= right && location >= left) {
			validLocations.push_back(location);
		}
	}

	// calculte score of locations
	double score = 1;
	for (int i = 1; i < validLocations.size(); i++) {
		score += validLocations[i] - validLocations[i - 1];
	}

	// calculate the density of the region
	double density = validLocations.size() * (1 / score);

	if (density != 0 && density != 1) {
		Frame newCalculation;
		newCalculation.left = left;
		newCalculation.frameSize = right - left;
		res->push_back(newCalculation);
	}

	// recursively compute left side
	CalculateDensityConcentration(left, right / 2, locations, res);
	// recursively compute right side
	CalculateDensityConcentration(left + ((right - left) / 2), right, locations, res);
}