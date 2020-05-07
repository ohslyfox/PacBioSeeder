#include "SchemeOptions.h"

SchemeOptions::SchemeOptions() {
	this->RunType = SchemeOptions::RunType::NoRunType;
	this->Scheme = SchemeOptions::Scheme::NoScheme;
	this->FileName = "";
	this->OutputFileName = "console";
	this->KmerLength = 9;
	this->ReadsToTest = 0;
}