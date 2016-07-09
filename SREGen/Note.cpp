#include "Note.h"

/* Note class member initialization*/
Note::Note(int scaleNum, int octave, int duration, int accidental)
	: scaleNum(scaleNum), octave(octave), accidental(accidental), duration(duration) {}

/* Moves the note up the scale. */
Note Note::operator++(int){
	Note temp(*this);
	if (scaleNum < 6) {
		scaleNum++;
	} else {
		scaleNum = 0;
		octave++;
	}
	return temp;
}

/* Moves the note down the scale. */
Note Note::operator--(int) {
	Note temp(*this);
	if (scaleNum > 0) {
		scaleNum--;
	} else {
		scaleNum = 6;
		octave--;
	}
	return temp;
}

int Note::getScaleNum() {
	return scaleNum;
}

int Note::getOctave() {
	return octave;
}

int Note::getAccidental() {
	return accidental;
}

int Note::getDuration() {
	return duration;
}