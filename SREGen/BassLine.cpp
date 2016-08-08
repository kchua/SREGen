#include <iostream>
#include <fstream>
#include "BassLine.h"
#include "Scale.h"

uniform_real_distribution<> BassLine::rateRNG = uniform_real_distribution<>(0, 1);
default_random_engine BassLine::generator =
	default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

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

	PianoKey upperBassLimit = PianoKey("g", 1, 3);
	PianoKey lowerBassLimit = PianoKey("e", -1, 2);

	for (int i = 0; i < length; i++) {
		line.push_back((*this).prog[i].getBottom());
		line[i].setOctave(baseOctave);
		if (scale.getPianoKey(line[i]) < upperBassLimit) {
			line[i].setOctave(baseOctave - 1);
			if (scale.getPianoKey(line[i]) > lowerBassLimit) {
				if (rateRNG(generator) < 0.5) {
					line[i].setOctave(baseOctave);
				}
			} else {
				line[i].setOctave(baseOctave);
			}
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