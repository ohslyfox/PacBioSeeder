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
			options->ReferenceGenome = loader.LoadFaFile(options->FileName + ".fa");
			options->PacBioReads = loader.LoadFastQReads(options->FileName + ".fastq");
			options->Solutions = loader.LoadSolutions(options->FileName + ".maf");

			if (options->ReferenceGenome.size() < 1 ||
				options->PacBioReads.size() < 1) {

				throw invalid_argument("");
			}
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
		if (value == "execute") {
			res->RunType = SchemeOptions::RunType::ResultsOnly;
		}
		else if (value == "compare") {
			res->RunType = SchemeOptions::RunType::CompareToSolution;
		}
		else {
			throw invalid_argument("");
		}

		cout << "Enter scheme type: ";
		cin >> value;
		if (value == "max-group") {
			res->Scheme = SchemeOptions::Scheme::MaxGroup;
		}
		else if (value == "min-score") {
			res->Scheme = SchemeOptions::Scheme::MinScore;
		}
		else if (value == "min-frame") {
			res->Scheme = SchemeOptions::Scheme::MinFrameSize;
		}
		else if (value == "max-density") {
			res->Scheme = SchemeOptions::Scheme::MaxDensity;
		}
		else if (value == "min-frame-max-density") {
			res->Scheme = SchemeOptions::Scheme::MinFrameMaxDensity;
		}
		else {
			throw invalid_argument("");
		}

		cout << "Enter k-mer length: ";
		cin >> value;
		res->KmerLength = stoi(value);

		cout << "Enter file name: ";
		cin >> value;
		res->FileName = value;
		cout << endl;
	}
	catch (...) {}
	return res;
}

SchemeOptions* CommandLineArgumentHandler::GetOptionsFromArgs(int argc, char** argv) {
	SchemeOptions* res = new SchemeOptions();
	try {

		string key;
		string value;
		for (int i = 1; i < argc; i++) {
			if ((i - 1) % 2 == 0) {
				key = argv[i];
				if (key.find("--") == string::npos) {
					throw invalid_argument("");
				}
			}
			else {
				value = argv[i];
				if (key == "--run-type") {
					if (res->RunType == SchemeOptions::RunType::NoRunType) {
						if (value == "execute") {
							res->RunType = SchemeOptions::RunType::ResultsOnly;
						}
						else if (value == "compare") {
							res->RunType = SchemeOptions::RunType::CompareToSolution;
						}
						else {
							throw invalid_argument("");
						}
					}
					else {
						throw invalid_argument("");
					}
				}
				else if (key == "--scheme") {
					if (res->Scheme == SchemeOptions::Scheme::NoScheme) {
						if (value == "max-group") {
							res->Scheme = SchemeOptions::Scheme::MaxGroup;
						}
						else if (value == "min-score") {
							res->Scheme = SchemeOptions::Scheme::MinScore;
						}
						else if (value == "min-frame") {
							res->Scheme = SchemeOptions::Scheme::MinFrameSize;
						}
						else if (value == "max-density") {
							res->Scheme = SchemeOptions::Scheme::MaxDensity;
						}
						else if (value == "min-frame-max-density") {
							res->Scheme = SchemeOptions::Scheme::MinFrameMaxDensity;
						}
						else {
							throw invalid_argument("");
						}
					}
					else {
						throw invalid_argument("");
					}
				}
				else if (key == "--file-name") {
					if (res->FileName == "") {
						res->FileName = value;
					}
					else {
						throw invalid_argument("");
					}
				}
				else if (key == "--kmer-length") {
					res->KmerLength = stoi(value);
				}
			}
		}
	}
	catch (...) {}
	return res;
}