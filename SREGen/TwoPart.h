#ifndef TWOPART_H
#define TWOPART_H

#include <random>
#include <fstream>
#include <string>
#include "Chord.h"
#include "GA.h"
#include "BassLine.h"

class TwoPart {
	friend class GA<TwoPart>;
public:
	static TwoPart generateRandom();
	static void assignFitness(TwoPart& harmony);

	static void setProgressionStartingChord(Chord chord);
	static void setEndingCadence(vector<Chord> cadence);
	static void setLength(int len);
	static void setKey(string key);
	static void setTonality(bool isMinor);
	static void createBassLine();

	bool operator<(const TwoPart& other) const;
	void outputToFile(ofstream& file);

	int necessaryFitness;
	int optionalFitness;
private:
	TwoPart() = default;
	vector<Note> melody;
	static BassLine bass;
	static int length;
	static string key;
	static bool isMinor;
	static uniform_int_distribution<> selectorRNG;
	static uniform_int_distribution<> octaveRNG;
	static default_random_engine generator;
};

#endif