#include <iostream>
#include <fstream>
#include "BassLine.h"
#include "Scale.h"

Note& BassLine::operator[](int index) {
	return line[index];
}

BassLine BassLine::generate(int length) {
	Progression::setProgressionLength(length);
	GA<Progression> generator = GA<Progression>(300, 40, 40, 0.5, 0.05);
	BassLine b = BassLine();
	b.p = generator.runSimulation();
	for (int i = 0; i < length; i++) {
		b.line.push_back(b.p[i].getBottom());
		b.line[i].setOctave(3);
	}
	return b;
}

Progression& BassLine::getProgression() {
	return p;
}

int BassLine::length() {
	return line.size();
}

BassLine::BassLine() {

}

/*
int main() {
	Scale s = Scale::generateScale("e", false);
	ofstream output;
	output.open("bassline.ly");
	output << "{\n";
	output << "\t\\time 4/4\n";
	output << "\t\\clef bass\n";
	output << "\t";

	BassLine b = BassLine::generate(8);
	for (int i = 0; i < b.length(); i++) {
		output << s[b[i]] << " ";
	}
	output << "\n}";
	output.close();
	cout << b.getProgression().outputRomanNumerals();
}
*/