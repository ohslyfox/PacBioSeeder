#include "CommandLineArgumentHandler.h"

void CommandLineArgumentHandler::Execute(int argc, char** argv) {
	HashMapClusteringScheme* scheme = NULL;
	SchemeOptions* options;
	Loader loader;

	try {

		if (argc > 1) {
			options = GetOptionsFromArgs(argc, argv);
		}
		else {
			options = GetOptionsFromCommandLine();
		}

		// load data
		if (options->FileName == "" || 
			options->RunType == SchemeOptions::RunType::NoRunType || 
			options->Scheme == SchemeOptions::Scheme::NoScheme || 
			options->KmerLength < 1) {

			throw invalid_argument("");
		}
		else {
			loader.LoadFileIntoOptions(options);
		}

		// run scheme
		scheme = new HashMapClusteringScheme(options);
		scheme->ExecuteScheme();
		delete scheme;
	}
	catch (...) {
		exit(1);
	}
}

SchemeOptions* CommandLineArgumentHandler::GetOptionsFromCommandLine() {
	SchemeOptions* res = new SchemeOptions();
	string value = "";

	try {
		cout << "Enter run type: ";
		cin >> value;
		res->RunType = ParseValueForRunType(value);

		cout << "Enter scheme type: ";
		cin >> value;
		res->Scheme = ParseValueForScheme(value);

		cout << "Enter k-mer length: ";
		cin >> value;
		res->KmerLength = stoi(value);

		cout << "Enter number of reads to test (0 for all): ";
		cin >> value;
		res->ReadsToTest = stoi(value);

		cout << "Enter file name: ";
		cin >> value;
		res->FileName = value;

		cout << "Enter output file name (\"console\" for console output): ";
		cin >> value;
		res->OutputFileName = value;
		cout << endl;
	}
	catch (...) { exit(1); }
	return res;
}

SchemeOptions* CommandLineArgumentHandler::GetOptionsFromArgs(int argc, char** argv) {
	SchemeOptions* res = new SchemeOptions();
	try {
		string key;
		string value;
		vector<string> keysFound;
		for (int i = 1; i < argc; i++) {
			if ((i - 1) % 2 == 0) {
				key = argv[i];
				if (key.find("--") == string::npos) {
					throw invalid_argument("");
				}
				if (find(keysFound.begin(), keysFound.end(), key) == keysFound.end()) {
					keysFound.push_back(key);
				}
				else {
					throw invalid_argument("");
				}
			}
			else {
				value = argv[i];
				if (key == "--run-type") {
					res->RunType = ParseValueForRunType(value);
				}
				else if (key == "--scheme") {
					res->Scheme = ParseValueForScheme(value);
				}
				else if (key == "--file-name") {
					res->FileName = value;
				}
				else if (key == "--kmer-length") {
					res->KmerLength = stoi(value);
				}
				else if (key == "--reads") {
					res->ReadsToTest = stoi(value);
				}
				else if (key == "--output") {
					res->OutputFileName = value;
				}
			}
		}
	}
	catch (...) { exit(1); }
	return res;
}

enum SchemeOptions::RunType CommandLineArgumentHandler::ParseValueForRunType(string value) {
	enum SchemeOptions::RunType res = SchemeOptions::RunType::NoRunType;
	try {
		if (value == "execute") {
			res = SchemeOptions::RunType::ResultsOnly;
		}
		else if (value == "compare") {
			res = SchemeOptions::RunType::CompareToSolution;
		}
		else {
			throw invalid_argument("");
		}
	}
	catch (...) { exit(1); }

	return res;
}

enum SchemeOptions::Scheme CommandLineArgumentHandler::ParseValueForScheme(string value) {
	enum SchemeOptions::Scheme res = SchemeOptions::Scheme::NoScheme;
	try {
		if (value == "max-group") {
			res = SchemeOptions::Scheme::MaxGroup;
		}
		else if (value == "min-score") {
			res = SchemeOptions::Scheme::MinScore;
		}
		else if (value == "min-frame") {
			res = SchemeOptions::Scheme::MinFrameSize;
		}
		else if (value == "max-density") {
			res = SchemeOptions::Scheme::MaxDensity;
		}
		else if (value == "min-frame-max-density") {
			res = SchemeOptions::Scheme::MinFrameMaxDensity;
		}
		else {
			throw invalid_argument("");
		}
	}
	catch (...) { exit(1); }

	return res;
}