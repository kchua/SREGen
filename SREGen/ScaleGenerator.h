#ifndef SCALEGENERATOR_H
#define SCALEGENERATOR_H

#include "Note.h"
#include "Scale.h"
#include <vector>
#include <string>

using namespace std;

class ScaleGenerator
{
public:
	void generateScale(int argc, char* argv[]);
private:
	static enum class Tonality { MAJOR, NMINOR, HMINOR, MMINOR };
	static enum class Staff { BASS, TREBLE, TENOR, ALTO };
	static void generateScale(string scale, Tonality tone, string filename);
};

#endif