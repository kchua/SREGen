#ifndef TWOPART_H
#define TWOPART_H

#include <random>
#include <fstream>
#include <string>
#include "Chord.h"
#include "BassLine.h"
#include "PianoKey.h"

class TwoPart {
private:
	class Segment;
	friend class GA<Segment>;
public:
	TwoPart() = default;
	static TwoPart generateHarmony();

	static int checkCorrectness(vector<Note>& melody, int start, int end);

	static void setProgressionStartingChord(Chord chord);
	static void setEndingCadence(vector<Chord> cadence);
	static void setLength(int len);
	static void setKey(string key);
	static void setTonality(bool isMinor);
	static void createBassLine();

	void outputToFile(ofstream& file);

private:
	vector<Note> soprano;

	static void setCurrBounds(int start, int end);

	static BassLine bass;
	static int length;
	static string key;
	static bool isMinor;
 	static const PianoKey lowerBound;
 	static const PianoKey upperBound;

	static int currStart;
	static int currEnd;

	static bool pianoKeyInBounds(PianoKey key);
	static bool doubledSeventh(vector<Note>& melody, int index);
	static bool withinOctaveOfPrev(vector<Note>& melody, int index);
	static bool hasSeventhLeap(vector<Note>& melody, int index);
	static bool hasBadParallels(vector<Note>& melody, int index);
	static bool ifFourResToThree(vector<Note>& melody, int index);
	static bool ifSeventhResToTonic(vector<Note>& melody, int index);
	static bool ifLeapHasValidRes(vector<Note>& melody, int index);

	static uniform_int_distribution<> selectorRNG;
	static uniform_int_distribution<> octaveRNG;
	static default_random_engine generator;
};

#endif