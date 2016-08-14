#ifndef TWOPART_SEGMENT_H
#define TWOPART_SEGMENT_H

#include "TwoPart.h"

class TwoPart::Segment {
public:
	static TwoPart generateRandom();
	static void assignFitness(TwoPart& harmony);
	bool operator<(const TwoPart& other) const;

	int necessaryFitness;
	int optionalFitness;

	vector<Note> melody;
	static uniform_int_distribution<> selectorRNG;
	static uniform_int_distribution<> octaveRNG;
	static default_random_engine generator;
private:
 	static const PianoKey lowerBound;
 	static const PianoKey upperBound;
};

#endif 
