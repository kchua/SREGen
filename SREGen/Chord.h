#ifndef CHORD_H
#define CHORD_H

#include <string>
#include <deque>
#include <vector>
#include <random>
#include "Note.h"
#include "Scale.h"

using namespace std;

class Chord {
public:
	Chord(string name, Note root, Note third, Note fifth);
	Chord(string name, Note root, Note third, Note fifth, Note seventh);

	bool operator==(const Chord& other) const;

	void invert();
	bool contains(Note& note);
	Note getBottom();
	Note getRandomNote();
	string getName();
	string getNameWithoutInv();
	string outputChord(Scale& scale, int octave);
	int getRootDeg();

private:
	static uniform_int_distribution<> triadRNG;
	static uniform_int_distribution<> seventhRNG;
	static default_random_engine generator;

	string name;
	bool isSeventh;
	deque<Note> chord;
	int inversion;

	void setOctave(int octave);

	static const vector<string> triadInversions;
	static const vector<string> seventhInversions;
};

#endif