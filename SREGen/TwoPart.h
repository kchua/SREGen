#ifndef TWOPART_H
#define TWOPART_H

#include <random>
#include <fstream>
#include "Chord.h"
#include "GA.h"
#include "BassLine.h"

class TwoPart {
	friend class GA<TwoPart>;
public:
	static TwoPart generateRandom();
	static void assignFitness(TwoPart& harmony);

	static void setBassLine(int length, bool isMinor);
	static void setProgressionStartingChord(Chord start);
	static void setEndingCadence(vector<Chord> cadence);

	bool operator<(const TwoPart& other) const;
	void outputToFile(ofstream& file);

	int necessaryFitness;
	int optionalFitness;

private:
	BassLine bass;
	static uniform_int_distribution<> selectorRNG;
	static default_random_engine generator;
};

#endif