#ifndef __LOADER_H__
#define __LOADER_H__

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "SchemeOptions.h"

using namespace std;

class Loader {
public:
	void LoadFileIntoOptions(SchemeOptions* options);
private:
	vector<int> skipped;
	vector<char> LoadFaFile(string filePath);
	vector<vector<char>> LoadFastQReads(string filePath, int amountToRead);
	vector<int> LoadSolutions(string filePath, int amountToRead);
};

#endif
