#ifndef PROGRESSION_H
#define PROGRESSION_H

#include <random>
#include <chrono>
#include "Chord.h"
#include "ProgressionGraph.h"
#include "MajorProgression.h"
#include "MinorProgression.h"
#include "GA.h"

class Progression {
	friend class GA<Progression>;
public:
	Progression() = default;
	static Progression generateRandom();
	static void assignFitness(Progression& progression);
	
	static void setProgressionLength(int length);
	static void setStartingChord(Chord chord);
	static void setEndingCadence(vector<Chord> cadence);
	static void setMode(bool isMinor);

	bool operator<(const Progression& other) const;
	string outputRomanNumerals() const;
	Chord& operator[](const int index);
	const Chord& at(const int index) const;

	int necessaryFitness;
	int optionalFitness;

private:
	static int length;
	static Chord startingChord;
	static vector<Chord> endingCadence;
	static ProgressionGraph graph;
	static uniform_int_distribution<> selectorRNG;
	static default_random_engine generator;

	vector<Chord> chords;
};

#endif