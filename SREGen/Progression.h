#ifndef PROGRESSION_H
#define PROGRESSION_H

#include <unordered_map>
#include <vector>
#include "Chord.h"

class Progression {
public:
	bool progressionBetween(Chord first, Chord second);
	Chord getRandomChord();
protected:
	Progression(bool isMinor);
	void addChord(Chord chord);
	void makeProgressionBetween(Chord first, Chord second);
private:
	struct chordHashFunction {
		int operator()(Chord chord);
	};
	Chord tonicChord;
	unordered_map<Chord, vector<Chord>, chordHashFunction> progressionGraph;
	vector<Chord> chords;
};

#endif