#include <iostream>
#include <fstream>
#include "ScaleGenerator.h"

typedef ScaleGenerator SG;

void SG::generateScale(int argc, char* argv[]) {
	string scale = "";
	string tonality = "";
	for (int i = 0; i < argc; i++) {
		if (string(argv[i]) == "-s") {
			scale = argv[i + 1];
		}
		if (string(argv[i]) == "-t") {
			tonality = argv[i + 1];
		}
	}
	if (scale == "" || tonality == "") {
		cout << "Invalid number of arguments. Please include the '-s' flag followed by the root of the scale, as well as the '-t' flag followed by the tonality of the scale (i.e. major, hminor, etc.)" << endl;
		cout << "Example usage (B-flat melodic minor): ScaleGenerator -s bes -t mminor";
		exit(-1);
	}

	ofstream scaleFile;
	scaleFile.open("scale.ly");
	scaleFile << "{\n";
	scaleFile << "\t\\time 4/4\n";
	scaleFile << "\t\\clef treble\n";
	scaleFile << "\t";

	Scale s = Scale::generateScale(scale, tonality.at(2) != 'j');

	Note root = Note(0, 4, 4);

	scaleFile << s[root++] << " ";
	for (int i = 0; i < 7; i++) {	
		scaleFile << s[root++] << " ";

	}
	scaleFile << "\n";
	scaleFile << "}";
}

int main(int argc, char* argv[]) {
	SG::generateScale(argc, argv);
}
