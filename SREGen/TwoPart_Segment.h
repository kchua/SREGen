#ifndef TWOPART_SEGMENT_H
#define TWOPART_SEGMENT_H

#include "TwoPart.h"

class TwoPart::Segment {
public:
	static Segment generateRandom();
	static void assignFitness(Segment& seg);
	bool operator<(const Segment& other) const;

	int fitness;

	vector<Note> melody;
};

#endif 
