#include "Chord.h"
#include <iostream>
#include <chrono>

uniform_int_distribution<> Chord::triadRNG = uniform_int_distribution<>(0, 2);
uniform_int_distribution<> Chord::seventhRNG = uniform_int_distribution<>(0, 3);
default_random_engine Chord::generator =
	default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

//////////////////
// Constructors //
//////////////////

/* Constructs a triad in root position. */
Chord::Chord(string name, Note root, Note third, Note fifth) 
	: name(name), isSeventh(false), chord{ root, third, fifth }, inversion(0) {}

/* Constructs a seventh chord in root position. */
Chord::Chord(string name, Note root, Note third, Note fifth, Note seventh)
	: name(name), isSeventh(true), chord{ root, third, fifth, seventh }, inversion(0) {}


///////////////
// Operators //
///////////////

/* Checks if two chords are the same, regardless of inversion or octave. */
bool Chord::operator==(const Chord& other) const {
	return other.name == this->name;
}


/////////////////////
// Other Functions //
/////////////////////

/* Inverts the given chord. */
void Chord::invert() {
	Note temp = getBottom();
	temp.octaveUp();
	chord.pop_front();
	chord.push_back(temp);
	inversion++;
	if (inversion == ((isSeventh) ? 4 : 3)) {
		deque<Note>::iterator iter = chord.begin();
		while (iter != chord.end()) {
			(iter++)->octaveDown();
		}
		inversion = 0;
	}
}

/* Checks if the chord contains the argument note. */
bool Chord::contains(Note& note) {
	deque<Note>::iterator iter = chord.begin();
	while (iter != chord.end()) {
		if ((iter++)->getScaleNum() == note.getScaleNum()) {
			return true;
		}
	}
	return false;
}

/* Returns the note at the bottom of the chord. */
Note Chord::getBottom() {
	return chord.front();
}

/* Returns a random note from the chord. */
Note Chord::getRandomNote() const {
	uniform_int_distribution<> RNG = (isSeventh) ? seventhRNG : triadRNG;
	return chord[RNG(generator)];
}

/* Returns the name of the chord (Roman Numeral), with the inversion appended. */
string Chord::getName() const {
	string temp = name;
	return temp.append(((isSeventh) ? seventhInversions : triadInversions)[inversion]);
}

/* Returns the name of the chord without appending the inversion of the chord. */
string Chord::getNameWithoutInv() const {
	return name;
}

/* Outputs a chord as string that can be used in a .ly file. */
string Chord::outputChord(Scale& scale, int octave) {
	setOctave(octave);
	int durLength = to_string(getBottom().getDuration()).size();
	string output = "<";
	deque<Note>::iterator iter = chord.begin();
	while (iter != chord.end()) {
		string note = scale[*(iter++)];
		output += note.substr(0, note.size() - durLength);
		output += " ";
	}
	output = output.substr(0, output.size() - 1);
	output += ">";
	output += to_string(getBottom().getDuration());
	return output;
}

/* Returns the scale degree of the root of the chord. */
int Chord::getRootDeg() {
	int i = inversion;
	while (inversion != 0) {
		invert();
	}
	int temp = getBottom().getScaleNum();
	while (inversion != i) {
		invert();
	}
	return temp;
}

/* Sets the octave of a chord. */
void Chord::setOctave(int octave) {
	deque<Note>::iterator iter = chord.begin();
	int base = iter->getOctave();
	while (iter != chord.end()) {
		(iter)->setOctave(iter->getOctave() - base + octave);
		iter++;
	}
}

const vector<string> Chord::triadInversions{ "", "6", "6/4" };
const vector<string> Chord::seventhInversions{ "7", "6/5", "4/3", "4/2" };

/*  // Basic Tests
int main() {
	Note root(0, 4, 4, 0);
	Note third(2, 4, 4, 0);
	Note fifth(4, 4, 4, 0);
	Scale C = Scale::generateScale("c", false);
	Chord CMaj("I", root, third, fifth);
	cout << "Root position: " << CMaj.getName() << endl;
	cout << CMaj.outputChord(C, 4) << endl << endl;
	CMaj.invert();
	cout << "First inversion: " << CMaj.getName() << endl;
	cout << CMaj.outputChord(C, 4) << endl << endl;
	CMaj.invert();
	cout << "Second inversion: " << CMaj.getName() << endl;
	cout << CMaj.outputChord(C, 4) << endl << endl;
	CMaj.invert();
	cout << "Root position: " << CMaj.getName() << endl;
	cout << CMaj.outputChord(C, 4) << endl << endl;
}
*/