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
	Progression();
	void addChord(Chord chord);
	void makeProgressionBetween(Chord first, Chord second);
	void setFinalChord(Chord chord);
private:
	struct chordHashFunction {
		int operator()(Chord chord);
	};
	unordered_map<Chord, vector<Chord>, chordHashFunction> progressionGraph;
	vector<Chord> chords;
};

#endif