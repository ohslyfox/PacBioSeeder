/*
	* PacBio Seeder
	* Authors:
	* Patrick Finger, Robert Oller, Benjamin Ladick
	* Brief:
	* This program locates potential seeds (or indecies)
	* of PacBio indel errors.
*/
#include <iostream>
#include "Loader.h"

int main()
{
	vector<char> test = Loader::LoadFile("test");

	for (auto c : test) {
		cout << c;
	}
	cout << endl;
}