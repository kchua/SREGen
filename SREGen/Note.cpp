#include "Note.h"

/////////////////
// Constructor //
/////////////////

/* Note class member initialization*/
Note::Note(int scaleNum, int octave, int duration, int accidental)
	: scaleNum(scaleNum), octave(octave), accidental(accidental), duration(duration) {}

/* Note constructor for when only the scaleNum (and possibly accidental) is relevant. */
Note::Note(int scaleNum, int accidental) 
	: Note(scaleNum, 4, 4, accidental) {}


///////////////
// Operators //
///////////////

/* Moves the note up the scale and returns it. */
Note& Note::operator++() {
	if (scaleNum < 6) {
		scaleNum++;
	} else {
		scaleNum = 0;
		octave++;
	}
	return *this;
}

/* Moves the note down the scale and returns it. */
Note& Note::operator--() {
	if (scaleNum > 0) {
		scaleNum--;
	} else {
		scaleNum = 6;
		octave--;
	}
	return *this;
}

/* Moves the note up the scale, and returns the note before it was moved up. */
Note Note::operator++(int) {
	Note temp(*this);
	++(*this);
	return temp;
}

/* Moves the note down the scale, and returns the note before it was moved down. */
Note Note::operator--(int) {
	Note temp(*this);
	--(*this);
	return temp;
}

/* Checks if one note is equal to another in terms of their scale numbers. */
bool Note::operator==(Note other) {
	return other.getScaleNum() == this->getScaleNum();
}


////////////////////////////
// Other public functions //
////////////////////////////

/* Raises the note up by an octave. */
void Note::octaveUp() {
	octave++;
}

/* Lowers the note by an octave. */
void Note::octaveDown() {
	octave--;
}

/* Sets the octave of the note. */
void Note::setOctave(int octave) {
	this->octave = octave;
}

/* Returns the current octave of the note. */
int Note::getOctave() {
	return octave;
}

/* Returns the scale number represented by the note. */
int Note::getScaleNum() {
	return scaleNum;
}

/* Returns the accidental of the note, if it exists. */
int Note::getAccidental() {
	return accidental;
}

/* Returns the duration of the note. */
int Note::getDuration() {
	return duration;
}