#ifndef SCALE_H
#define SCALE_H

#include <string>
#include <vector>
#include "Note.h"

using namespace std;

class Scale
{
public:
	static Scale generateMajorScale(string scale);
	static Scale generateMinorScale(string scale);
	string operator[](Note note);
private:
	static const vector<string> keyboardSharp;
	static const vector<string> keyboardFlat;

	static const vector<int> majorPattern;
	static const vector<int> minorPattern;

	vector<string> keys;

	Scale(string scale);
	static int keyIndex(string scale, const vector<string>& keyboard);
	static string scaleNote(Scale s, const vector<string>& keyboard, int curr, int kbIndex);
};




#endif