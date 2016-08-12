#ifndef BASSLINE_H
#define BASSLINE_H

#include <vector>
#include <random>
#include "GA.h"
#include "Progression.h"
#include "Note.h"

using namespace std;

class BassLine {
public:
	BassLine() = default;
	Note& operator[](int index);
	const Progression& getProgression();
	Scale& getScale();
	static BassLine generate(int length, string key, bool isMinor);
	int length();
private:
	static uniform_real_distribution<> rateRNG;
	static default_random_engine generator;
	
	BassLine(string key, bool isMinor, Progression prog, int length);
	int len;
	vector<Note> line;
	Progression prog;
	Scale scale;
};

#endif