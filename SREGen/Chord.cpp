#include "Chord.h"

//////////////////
// Constructors //
//////////////////

/* Constructs a triad in root position. */
Chord::Chord(string name, Note root, Note third, Note fifth) 
	: name(name), isSeventh(false), chord{ root, third, fifth }, inversion(0) {}

/* Constructs a seventh chord in root position. */
Chord::Chord(string name, Note root, Note third, Note fifth, Note seventh)
	: name(name), isSeventh(true), chord{ root, third, fifth, seventh }, inversion(0) {}

/////////////////////
// Other Functions //
/////////////////////

/* Inverts the given chord. */
void Chord::invert() {
	Note temp = getBottom();
	temp.octaveUp();
	chord.pop_front();
	chord.push_back(temp);
	if (inversion == ((isSeventh) ? 4 : 3)) {
		deque<Note>::iterator iter = chord.begin();
		while (iter != chord.end()) {
			(iter++)->octaveDown();
		}
		inversion = 0;
	}
}

/* Checks if the chord contains the argument note. */
bool Chord::contains(Note note) {
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

/* Returns the name of the chord (Roman Numeral), with the inversion appended. */
string Chord::getName() {
	return name.append(((isSeventh) ? seventhInversions : triadInversions)[inversion]);
}

string Chord::outputChord(Scale scale, int octave) {
	setOctave(octave);
	// TODO: Extract notes and output in Lilypond format.
}

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