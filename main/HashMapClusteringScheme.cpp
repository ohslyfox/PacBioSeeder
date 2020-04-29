#include "HashMapClusteringScheme.h"

HashMapClusteringScheme::HashMapClusteringScheme(SchemeOptions* options) {
	this->Options = options;
	this->GenomeMap = IndexGenome(options->ReferenceGenome);
}

HashMapClusteringScheme::~HashMapClusteringScheme() {
	delete this->Options;
}

void HashMapClusteringScheme::ExecuteScheme() {
	vector<vector<int>> foundLocations;

	for (int i = 0; i < this->Options->PacBioReads.size(); i++) {
		switch (this->Options->Scheme) {
		case SchemeOptions::Scheme::MaxGroup:
			foundLocations.push_back(MaxGroupScheme(this->Options->PacBioReads[i]));
			break;
		case SchemeOptions::Scheme::MinScore:
			foundLocations.push_back(MinScoreScheme(this->Options->PacBioReads[i]));
			break;
		case SchemeOptions::Scheme::MinFrameSize:
			foundLocations.push_back(MinFrameSizeScheme(this->Options->PacBioReads[i]));
			break;
		default:
			exit(1);
		}
	}

	if (foundLocations.size() > 0) {
		for (int i = 0; i < this->Options->PacBioReads.size(); i++) {
			switch (this->Options->RunType) {
			case SchemeOptions::RunType::ResultsOnly:
				for (int location : foundLocations[i]) {
					cout << "PacBio read (" << i+1 << ")" << " derived location " << location << "." << endl;
				}
				break;
			case SchemeOptions::RunType::CompareToSolution:
				for (int location : foundLocations[i]) {
					cout << "PacBio read (" << i+1 << ")" << " derived location " << location << "." << endl;
					cout << "Actual location " << this->Options->Solutions[i] << endl;
					cout << "Delta = " << abs(location - this->Options->Solutions[i]) << endl << endl;
				}
				break;
			default:
				exit(1);
			}
		}
	}
}

vector<int> HashMapClusteringScheme::MinFrameSizeScheme(vector<char> pacBioRead) {
	vector<int> res;

	// generate locations of kmer substrings
	unordered_set<string> keys = GenerateKmerLengthSubstrings(pacBioRead, 2);

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

	vector<Frame> frames;
	CalculateDensityConcentration(0, this->Options->ReferenceGenome.size(), locationList, &frames);

	if (frames.size() > 0) {
		// find min frame size
		int min = INT32_MAX;
		for (Frame frame : frames) {
			if (frame.frameSize < min) {
				min = frame.frameSize;
			}
		}

		for (Frame frame : frames) {
			if (frame.frameSize == min) {
				int currentLocation = frame.left;
				while (keyMap.find(currentLocation) == keyMap.end()) {
					currentLocation++;
				}
				res.push_back(currentLocation);
			}
		}
	}

	return res;
}

vector<int> HashMapClusteringScheme::MaxGroupScheme(vector<char> pacBioRead) {
	vector<int> res;

	// generate locations of kmer substrings
	unordered_set<string> keys = GenerateKmerLengthSubstrings(pacBioRead, 1);

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
	int tolerance = 4;
	int currentGroup = locationList[0];
	int lastLocation = INT32_MIN / 2;
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
	int currentMax = 0;
	for (auto group : groups) {
		if (group.second.size() > currentMax) {
			currentMax = group.second.size();
		}
	}

	for (auto group : groups) {
		if (group.second.size() == currentMax) {
			res.push_back(group.first);
		}
	}

	return res;
}

vector<int> HashMapClusteringScheme::MinScoreScheme(vector<char> pacBioRead) {
	vector<int> res;

	// generate locations of kmer substrings
	unordered_set<string> keys = GenerateKmerLengthSubstrings(pacBioRead, 1);

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
	int tolerance = 4;
	int currentGroup = locationList[0];
	int lastLocation = INT32_MIN / 2;
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

	// score groups
	unordered_map<int, int> locationScores;
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
	}

	return res;
}

unordered_map<string, vector<int>> HashMapClusteringScheme::IndexGenome(vector<char> genomeToIndex) {
	unordered_map<string, vector<int>> res;

	for (unsigned int i = 0; i < genomeToIndex.size() - this->Options->KmerLength; i++) {
		string currentString = "";
		for (int j = i; j < i + this->Options->KmerLength; j++) {
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

unordered_set<string> HashMapClusteringScheme::GenerateKmerLengthSubstrings(vector<char> read, int multiplier) {
	unordered_set<string> res;

	for (int i = 0; i < read.size() - this->Options->KmerLength; i+= this->Options->KmerLength * multiplier) {
		string currentString = "";
		for (int j = i; j < i + this->Options->KmerLength; j++) {
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