#include "SREGen.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Note.h"
#include "Scale.h"
#include "Chord.h"

using namespace std;

void SREGen::generate(int argc, char * argv[]) {
	string mode = argv[1];
	if (mode == "scale") {
		generateScale(argc, argv);
	} else if (mode == "progression") {
		generateProgression(argc, argv);
	} else if (mode == "bass") {
		generateBassLine(argc, argv);
	} else if (mode == "twopartharm") {
		generateTwoPartHarmony(argc, argv);
	} else {
		cout << "Error. Mode argument invalid. Please try again." << endl;
		cout << "Exiting..." << endl;
	}
}

void SREGen::generateScale(int argc, char* argv[]) {
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
		cout << "Example usage (B-flat melodic minor): SREGen scale -s bes -t mminor";
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

void SREGen::generateProgression(int argc, char* argv[]) {
	int length = 0;
	int startChordDegree = 0;
	string endingCadence = "authentic";
	string tonality = "major";
	for (int i = 2; i < argc; i++) {
		if (string(argv[i]) == "-t") {
			tonality = argv[i + 1];
		} else if (string(argv[i]) == "-l") {
			length = atoi(argv[i + 1]);
		} else if (string(argv[i]) == "-scd") {
			startChordDegree = atoi(argv[i + 1]) - 1;
		} else if (string(argv[i]) == "-ec") {
			endingCadence = argv[i + 1];
		}
	}
	if (length == 0) {
		cout << "Missing required length argument. Please include the -l flag, as well as any other optional arguments." << endl;
		cout << "Example usage (Minor progression of length 10 starting with the tonic, and ending with a plagal cadence: SREGen progression -t minor -l 10 -scd 1 -ec plagal";
		exit(-1);
	}
	// TODO Make a chord of the chord degree (will probably make a helper function)
}