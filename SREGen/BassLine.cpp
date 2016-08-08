#include <iostream>
#include <fstream>
#include "BassLine.h"
#include "Scale.h"

Note& BassLine::operator[](int index) {
	return line[index];
}

BassLine BassLine::generate(int length, string key, bool isMinor) {
	Progression::setProgressionLength(length);
	GA<Progression> generator = GA<Progression>(300, 40, 40, 0.5, 0.05);
	BassLine b = BassLine(key, isMinor, generator.runSimulation(), length);
	return b;
}

Progression BassLine::getProgression() {
	return prog;
}

int BassLine::length() {
	return line.size();
}

BassLine::BassLine(string key, bool isMinor, Progression prog, int length) 
	: scale(Scale::generateScale(key, isMinor)), prog(prog), len(length) {
	string tonic = key.substr(0, 1);
	int accidental = 0;
	if (key.length() != 1) {
		if (key.at(1) == 'i') {
			accidental = 1;
		} else {
			accidental = -1;
		}
	}

	PianoKey upperScaleLimit = PianoKey("g", -1);
	PianoKey scaleTonic = PianoKey(tonic, accidental);

	int baseOctave = (scaleTonic < upperScaleLimit) ? 3 : 2;
	PianoKey upperBaseLimit = PianoKey("g", 1);

	for (int i = 0; i < length; i++) {
		line.push_back((*this).prog[i].getBottom());
		if (scale.getPianoKey(line[i]) < upperBaseLimit) {
			line[i].setOctave(baseOctave);
		} else {
			line[i].setOctave(baseOctave - 1);
		}
	}
}

int main() {
	Scale s = Scale::generateScale("f", false);
	ofstream output;
	output.open("bassline.ly");
	output << "{\n";
	output << "\t\\time 4/4\n";
	output << "\t\\clef bass\n";
	output << "\t\\key f \\major\n";
	output << "\t";

	BassLine b = BassLine::generate(8, "f", false);
	for (int i = 0; i < b.length(); i++) {
		output << s[b[i]] << " ";
	}
	output << "\n}";
	output.close();
	cout << b.getProgression().outputRomanNumerals();
}