#ifndef SCALE_H
#define SCALE_H

#include <string>
#include <vector>
#include "Note.h"
#include "PianoKey.h"

using namespace std;

class Scale {
public:
	Scale() = default;
	static Scale generateScale(string scale, bool isMinor);
	string operator[](Note note);
	PianoKey getPianoKey(Note note);
private:
	Scale(string scale);
	static Scale generateScale(string scale, const vector<int>& pattern, bool isFlat);
	static int keyIndex(string scale, const vector<string>& keyboard);
	static void correctScaleNote(Scale& s, const vector<string>& keyboard, int curr, int kbIndex);
	
	static string getNoteName(Note note, Scale s);
	static int calculateNoteOctave(string noteName, Note note, Scale s);
	static void appendOctave(string& noteName, int octave);

	static const vector<string> keyboardSharp;
	static const vector<string> keyboardFlat;

	static const vector<int> majorPattern;
	static const vector<int> minorPattern;

	vector<string> keys;

};

#endif