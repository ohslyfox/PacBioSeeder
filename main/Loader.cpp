#include "Loader.h"

vector<char> Loader::LoadFile(string fileName) {
	vector<char> res;

	try {
		if (fileName.find(".txt") == string::npos) {
			fileName += ".txt";
		}
		ifstream file;
		file.open(fileName, ifstream::in);

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
		cout << "Error reading file " << fileName;
	}

	return res;
}
