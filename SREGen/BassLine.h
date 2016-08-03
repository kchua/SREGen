#ifndef BASSLINE_H
#define BASSLINE_H

#include <vector>
#include "GA.h"
#include "Progression.h"
#include "Note.h"

using namespace std;

class BassLine {
public:
	Note& operator[](int index);
	Progression& getProgression();
	static BassLine generate(int length);
	int length();
private:
	BassLine();
	vector<Note> line;
	Progression p;
};

#endif