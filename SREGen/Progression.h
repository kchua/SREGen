#ifndef PROGRESSION_H
#define PROGRESSION_H

#include <unordered_map>
#include <functional>
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
	Chord getTonic();
private:
	struct chordHashFunction {
		size_t operator()(Chord chord) const;
	};
	Chord tonicChord;
	unordered_map<Chord, vector<Chord>, chordHashFunction> progressionGraph;
	vector<Chord> chords;
};

#endif