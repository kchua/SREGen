#ifndef SCALE_H
#define SCALE_H

#include <string>
#include <vector>
#include "Note.h"

using namespace std;

class Scale
{
public:
	static Scale generateScale(string scale, bool isMinor);
	string& operator[](Note note);
private:
	static const vector<string> keyboardSharp;
	static const vector<string> keyboardFlat;
	vector<string> keys;
};

const vector<string> Scale::keyboardSharp = 
	vector<string>{ "c", "cis", "d", "dis", "e", "f", "fis", "g", "gis", "a", "ais", "b" };

const vector<string> Scale::keyboardFlat =
	vector<string>{ "c", "des", "d", "ees", "e", "f", "ges", "g", "aes", "a", "bes", "b" };

#endif