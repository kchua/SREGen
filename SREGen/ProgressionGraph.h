#ifndef PROGRESSIONGRAPH_H
#define PROGRESSIONGRAPH_H

#include <unordered_map>
#include <functional>
#include <vector>
#include <random>
#include <algorithm>
#include "Chord.h"

using namespace std;

class ProgressionGraph {
public:
	bool progressionBetween(Chord first, Chord second);
	Chord getRandomChord();
protected:
	ProgressionGraph(bool isMinor);
	void addChord(Chord chord);
	void makeProgressionBetween(Chord first, Chord second);
	Chord getTonic();
	void finalize();
private:
	struct chordHashFunction {
		size_t operator()(Chord chord) const;
	};
	Chord tonicChord;
	unordered_map<Chord, vector<Chord>, chordHashFunction> graph;
	vector<Chord> chords;
	uniform_int_distribution<> ChordRNG;
	default_random_engine generator;
};

#endif