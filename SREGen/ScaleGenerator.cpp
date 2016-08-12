#include <iostream>
#include <fstream>
#include <string>
#include "ScaleGenerator.h"
#include "Note.h"
#include "Scale.h"

/* Generates a scale as directed by command line arguments. */
void ScaleGenerator::generateScale(int argc, char* argv[]) {
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
	for (int i = 0; i < 6; i++) {	
		if (i == 4 && tonality.at(3) == 'n' && tonality.at(0) == 'm') {
			root = Note(5, 4, 4, 1);
		}
		if (i == 5 && tonality.at(3) == 'n' && tonality.at(0) == 'h') {
			root = Note(6, 4, 4, 1);
		}
		scaleFile << s[root++] << " ";
	}
	root = Note(0, 5, 4);
	scaleFile << s[root];
	scaleFile << "\n";
	scaleFile << "}";
}

/*
int main(int argc, char* argv[]) {
	SG::generateScale(argc, argv);
}
*/
