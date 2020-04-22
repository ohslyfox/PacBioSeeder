#ifndef __LOADER_H__
#define __LOADER_H__

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class Loader {
public:
	static vector<char> LoadFile(string fileName);
private:
};

#endif
