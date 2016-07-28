#ifndef PROGRESSION_H
#define PROGRESSION_H

#include "Chord.h"

class Progression {
public:
	static Progression generateRandom();
	static void assignFitness(Progression& progression);
	
	static void setProgresssionLength(int length);
	static void setStartingChord(Chord chord);
	static void setEndingCadence(vector<Chord> chord);

	bool operator<(Progression& other) const;

	string outputRomanNumerals();
	// TODO: declare a subscript operator for later on.

	vector<Chord> chords;
private:
	static int length;
	static Chord startingChord;
	static vector<Chord> endingCadence;

	int fitness;
};

#endif