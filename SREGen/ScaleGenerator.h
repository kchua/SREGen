#ifndef SCALEGENERATOR_H
#define SCALEGENERATOR_H

#include "Note.h"
#include "Scale.h"
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class ScaleGenerator
{
public:
	static void generateScale(int argc, char* argv[]);
};

#endif