#ifndef __LOADER_H__
#define __LOADER_H__

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class Loader {
public:
	Loader();

	// methods
	vector<char> LoadRawTextFile(string filePath);
	vector<vector<char>> LoadFastQReads(string filePath, int amountToRead);
private:
};

#endif
