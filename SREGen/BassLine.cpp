#include <iostream>
#include <fstream>
#include <chrono>
#include "BassLine.h"
#include "Scale.h"

/* Random number generation stuff. */
uniform_real_distribution<> BassLine::rateRNG = uniform_real_distribution<>(0, 1);
default_random_engine BassLine::generator =
	default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

/* Return a reference to the note at a given index. */
Note& BassLine::operator[](int index) {
	return line[index];
}

/* Generates a BassLine of a given length in a specific key of specific tonality. */
BassLine BassLine::generate(int length, string key, bool isMinor) {
	Progression::setProgressionLength(length);
	Progression::setMode(isMinor);
	GA<Progression> generator = GA<Progression>(300, 40, 40, 0.5, 0.05);
	BassLine b = BassLine(key, isMinor, generator.runSimulation(), length);
	return b;
}

/* Returns the underlying progression of a bass line. */
const Progression& BassLine::getProgression() {
	return prog;
}

Scale& BassLine::getScale() {
	return scale;
}

/* Returns the length of the bass line. */
int BassLine::length() {
	return line.size();
}

/* Creates a bass line of specified length based on a given progression. */
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

	PianoKey upperScaleLimit = PianoKey("f", 0);
	PianoKey scaleTonic = PianoKey(tonic, accidental);

	int baseOctave = (scaleTonic < upperScaleLimit) ? 3 : 2;

	PianoKey upperBassLimit = PianoKey("g", 1, 3);
	PianoKey lowerBassLimit = PianoKey("e", -1, 2);

	for (int i = 0; i < length; i++) {
		Note above = (*this).prog[i].getBottom();
		Note below = (*this).prog[i].getBottom();
		above.setOctave(baseOctave);
		below.setOctave(baseOctave - 1);

		if (scale.getPianoKey(above) < upperBassLimit) {
			if (scale.getPianoKey(below) > lowerBassLimit) {
				if (i > 0) {
					int aboveInterval = above.getIntervalBetween(line[i - 1]);
					int belowInterval = below.getIntervalBetween(line[i - 1]);
					if (abs(aboveInterval - belowInterval) == 1) {
						line.push_back((rateRNG(generator) < 0.5) ? above : below);
					} else if (aboveInterval - belowInterval == 0) {
						above.setOctave(line[i - 1].getOctave());
						line.push_back(above);
					} else if (aboveInterval < belowInterval) {
						line.push_back(above);
					} else {
						line.push_back(below);
					}
				} else {
					line.push_back((rateRNG(generator) < 0.5) ? above : below);
				}
			} else {
				line.push_back(above);
			}
		} else {
			line.push_back(below);
		}
	}
}

/*
int main() {
	Scale s = Scale::generateScale("e", false);
	ofstream output;
	output.open("bassline.ly");
	output << "{\n";
	output << "\t\\time 4/4\n";
	output << "\t\\clef bass\n";
	output << "\t\\key e \\major\n";
	output << "\t";

	BassLine b = BassLine::generate(8, "e", false);
	for (int i = 0; i < b.length(); i++) {
		output << s[b[i]] << " ";
	}
	output << "\n}";
	output.close();
	cout << b.getProgression().outputRomanNumerals();
}
*/