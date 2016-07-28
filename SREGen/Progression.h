#ifndef PROGRESSION_H
#define PROGRESSION_H

#include "Chord.h"
#include "ProgressionGraph.h"

class Progression {
public:
	static Progression generateRandom();
	static void assignFitness(Progression& progression);
	
	static void setProgresssionLength(int length);
	static void setStartingChord(Chord chord);
	static void setEndingCadence(vector<Chord> cadence);
	static void setMode(bool isMinor);

	bool operator<(const Progression& other) const;
	string outputRomanNumerals();
	Chord& operator[](const int index);

private:
	static int length;
	static Chord startingChord;
	static vector<Chord> endingCadence;
	static ProgressionGraph graph;

	vector<Chord> chords;

	int fitness;
};

#endif