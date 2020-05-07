#ifndef __COMMAND_LINE_ARGUMENT_HANDLER__
#define __COMMAND_LINE_ARGUMENT_HANDLER__

#include "HashMapClusteringScheme.h"
#include "Loader.h"
#include "SchemeOptions.h"

using namespace std;

class CommandLineArgumentHandler {
public:
	void Execute(int argc, char** argv);
private:
	SchemeOptions* GetOptionsFromArgs(int argc, char** argv);
	SchemeOptions* GetOptionsFromCommandLine();
	enum SchemeOptions::RunType ParseValueForRunType(string value);
	enum SchemeOptions::Scheme ParseValueForScheme(string value);
};

#endif
