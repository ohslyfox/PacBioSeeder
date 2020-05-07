#include "Loader.h"

void Loader::LoadFileIntoOptions(SchemeOptions* options) {
	try {
		int amountToRead = options->ReadsToTest <= 0 ? INT32_MAX : options->ReadsToTest;
		options->ReferenceGenome = LoadFaFile(options->FileName + ".fa");
		options->PacBioReads = LoadFastQReads(options->FileName + ".fastq", amountToRead);
		if (options->RunType == SchemeOptions::RunType::CompareToSolution) {
			options->Solutions = LoadSolutions(options->FileName + ".maf", amountToRead);
		}

		if (options->ReferenceGenome.size() < 1 ||
			options->PacBioReads.size() < 1) {

			throw invalid_argument("");
		}
	}
	catch (...) { exit(1); }
}

vector<char> Loader::LoadFaFile(string filePath) {
	vector<char> res;

	try {
		ifstream file;
		file.open(filePath, ifstream::in);

		if (file.is_open()) {
			// ignore first line
			string line;
			getline(file, line);

			res.push_back(file.get());
			while (file.good()) {
				res.push_back(file.get());
			}

			file.close();
		}
		else {
			throw invalid_argument("");
		}
		
	}
	catch (...) {
		cout << "Error reading file " << filePath;
	}

	return res;
}

vector<vector<char>> Loader::LoadFastQReads(string filePath, int amountToRead) {
	vector<vector<char>> res;

	try {
		ifstream file;
		file.open(filePath, ifstream::in);

		if (file.is_open()) {
			int count = 0;
			int reads = 0;
			int totalRead = 0;
			string line;
			while (reads < amountToRead && getline(file, line)) {
				if (count % 4 == 1) {
					vector<char> read;
					bool add = true;
					for(char c : line) {
						if (c == 'A' || c == 'a' ||
							c == 'C' || c == 'c' ||
							c == 'G' || c == 'g' ||
							c == 'T' || c == 't') {
							read.push_back(c);
						}
						else {
							add = false;
							break;
						}
					}
					if (add) {
						res.push_back(read);
						reads++;
					}
					else {
						skipped.push_back(totalRead);
					}
					totalRead++;
				}
				count++;
			}

			file.close();
		}
		else {
			//throw invalid_argument("");
		}
	}
	catch (...) {
		cout << "Error reading file " << filePath;
	}

	return res;
}

vector<int> Loader::LoadSolutions(string filePath, int amountToRead) {
	vector<int> res;
	try {
		ifstream file;
		file.open(filePath, ifstream::in);

		if (file.is_open()) {
			int totalCount = 0;
			int count = 0;
			string str;
			while (count < amountToRead && getline(file, str)) {
				if (str.substr(0,5).compare("s ref") == 0) {
					if (find(skipped.begin(), skipped.end(), totalCount) == skipped.end()) {
						res.push_back(stoi(str.substr(6, 4)));
						count++;
					}
					totalCount++;
				}
			}

			file.close();
		}
		else {
			//throw invalid_argument("");
		}
	}
	catch (...) {
		cout << "Error reading file " << filePath;
	}

	return res;
}
