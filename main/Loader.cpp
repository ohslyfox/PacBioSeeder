#include "Loader.h"

Loader::Loader() {
	// do nothing
}

vector<char> Loader::LoadRawTextFile(string filePath) {
	vector<char> res;

	try {
		ifstream file;
		file.open(filePath, ifstream::in);

		if (file.is_open()) {
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
			string line;
			while (reads < amountToRead && getline(file, line)) {
				if (count % 4 == 1) {
					vector<char> read;
					for(char c : line) {
						read.push_back(c);
					}
					res.push_back(read);
					reads++;
				}
				count++;
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
