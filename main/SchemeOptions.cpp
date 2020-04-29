#include "SchemeOptions.h"

SchemeOptions::SchemeOptions() {
	this->RunType = SchemeOptions::RunType::NoRunType;
	this->Scheme = SchemeOptions::Scheme::NoScheme;
	this->FileName = "";
	this->KmerLength = 9;
	this->ReadsToTest = 0;
}