#ifndef CHORD_H
#define CHORD_H

#include <string>
#include <deque>
#include <vector>
#include "Note.h"
#include "Scale.h"

using namespace std;

class Chord {
public:
	Chord(string name, Note root, Note third, Note fifth);
	Chord(string name, Note root, Note third, Note fifth, Note seventh);

	void invert();
	bool contains(Note note);
	Note getBottom();
	string getName();
	string outputChord(Scale scale, int octave);

private:
	string name;
	bool isSeventh;
	deque<Note> chord;
	int inversion;

	void setOctave(int octave);

	static const vector<string> triadInversions;
	static const vector<string> seventhInversions;
};

#endif