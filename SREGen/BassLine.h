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
	static BassLine generate(int length);
private:
	BassLine();
	vector<Note> line;
};

#endif