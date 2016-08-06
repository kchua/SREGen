#include "Scale.h"

/* Generate a scale whose root is SCALE and whose tonality
   is based on ISMINOR. */
Scale Scale::generateScale(string scale, bool isMinor) {
	if (!isMinor) {
		auto isFlatKeySig = [](string scale)->bool {
			return (scale == "f") || (scale.length() > 1 && scale.at(1) == 'e');
		};
		return generateScale(scale, majorPattern, isFlatKeySig(scale));
	} else {
		auto isFlatKeySig = [](string scale)->bool {
			return (scale == "f")
				|| (scale == "c")
				|| (scale == "g")
				|| (scale == "d")
				|| (scale.length() > 1 && scale.at(1) == 'e');
		};
		return generateScale(scale, minorPattern, isFlatKeySig(scale));
	}
}

/* Get the string name of a scale note in this particular scale. */
string Scale::operator[](Note note) {
	string noteName = Scale::getNoteName(note, *this);
	Scale::appendOctave(noteName, calculateNoteOctave(noteName, note, *this));
	noteName.append(to_string(note.getDuration()));
	return noteName;
}

/* Returns the specific piano key corresponding to a specific note to 
   for comparison uses. */
PianoKey Scale::getPianoKey(Note note) {
	string noteName = Scale::getNoteName(note, *this);
	return PianoKey(to_string(noteName.at(0)),
					note.getAccidental(),
					Scale::calculateNoteOctave(noteName, note, *this));
}

/////////////////////
// Private Methods //
/////////////////////

/* Scale constructor*/
Scale::Scale(string scale) : keys(7) {
	char curr = scale.at(0);
	keys[0] = string(1, curr);
	for (int i = 1; i < 7; i++) {
		curr++;
		if (curr == 'h') {
			curr = 'a';
		}
		keys[i] = string(1, curr);
	}
}

/* Generates a scale using a specific pattern in a specific keyboard. */
Scale Scale::generateScale(string scale, const vector<int>& pattern, bool isFlat) {
	Scale temp = Scale(scale);
	vector<string> keyboard = (isFlat) ? keyboardFlat : keyboardSharp;
	int kbIndex = keyIndex(scale, keyboard);
	int curr = 0;
	while (curr < pattern.size()) {
		correctScaleNote(temp, keyboard, curr, kbIndex);
		kbIndex = kbIndex + pattern[curr];
		if (kbIndex >= keyboard.size()) {
			kbIndex = kbIndex - keyboard.size();
		}
		curr++;
	}
	correctScaleNote(temp, keyboard, curr, kbIndex);
	return temp;
}

/* Finds where a key is on the keyboard. */
int Scale::keyIndex(string scale, const vector<string>& keyboard) {
	string base = string(1, scale.at(0));
	int i = 0;
	while (keyboard[i] != base) {
		i++;
	}
	while (scale.length() > 1) {
		if (scale.at(1) == 'i') {
			i = (i + 1) % keyboard.size();
		} else {
			i--;
			if (i < 0) {
				i = keyboard.size() - 1;
			}
		}
		scale = scale.substr(0, scale.size() - 2);
	}
	return i;
}

/* Returns a key on the keyboard modified to fit the a specific note on
   the scale. */
void Scale::correctScaleNote(Scale& s, const vector<string>& keyboard, int curr, int kbIndex) {
	int nameIndex = keyIndex(s.keys[curr], keyboard);
	if (keyboard == keyboardSharp) {
		if (nameIndex <= kbIndex) {
			for (int i = 0; i < kbIndex - nameIndex; i++) {
				s.keys[curr].append("is");
			}
		} else {
			for (int i = 0; i < keyboard.size() - (nameIndex + 1) + (kbIndex + 1); i++) {
				s.keys[curr].append("is");
			}
		}
	} else {
		if (nameIndex >= kbIndex) {
			for (int i = 0; i < nameIndex - kbIndex; i++) {
				s.keys[curr].append("es");
			}
		} else {
			for (int i = 0; i < keyboard.size() - (kbIndex + 1) + (nameIndex + 1); i++) {
				s.keys[curr].append("es");
			}
		}
	}
}

/* Returns the name of a scale note relative to a specific scale, taking
   accidentals into account. */
string Scale::getNoteName(Note note, Scale s) {
	string letter = s.keys[note.getScaleNum()];

	switch (note.getAccidental()) {
		case -1:
			if (letter.length() > 1 && letter.at(1) == 'i') {
				letter = letter.substr(0, letter.length() - 2);
			} else {
				letter.append("es");
			}
			break;

		case 1:
			if (letter.length() > 1 && letter.at(1) == 'e') {
				letter = letter.substr(0, letter.length() - 2);
			} else {
				letter.append("is");
			}
			break;
	}

	return letter;
}

/* Calculates in which octave a note should be placed. */
int Scale::calculateNoteOctave(string noteName, Note note, Scale s) {
	int offset = (s.keys[0].at(0) <= 'b') ? -1 : 0;

	int octaveChange = 0;
	if ((s.keys[0] < "c" && noteName >= "c")
		|| (noteName >= "c" && noteName < s.keys[0])
		|| (noteName.at(0) == 'a' && s.keys[0].at(0) == 'b')) {
		octaveChange = 1;
	}
	return note.getOctave() + offset + octaveChange;
}

/* Appends the octave after a note name in the Lilypond format. */
void Scale::appendOctave(string& noteName, int octave) {
	while (octave != 3) {
		if (octave > 3) {
			octave--;
			noteName.append("'");
		} else {
			octave++;
			noteName.append(",");
		}
	}
}

// The keyboard, with black keys viewed as sharps.
const vector<string> Scale::keyboardSharp = 
	vector<string>{ "c", "cis", "d", "dis", "e", "f", "fis", "g", "gis", "a", "ais", "b" };

// The keyboard, with black keys viewed as flats.
const vector<string> Scale::keyboardFlat =
	vector<string>{ "c", "des", "d", "ees", "e", "f", "ges", "g", "aes", "a", "bes", "b" };

// Major scale pattern
const vector<int> Scale::majorPattern =
	vector<int>{ 2, 2, 1, 2, 2, 2 };

// Natural minor scale pattern
const vector<int> Scale::minorPattern =
	vector<int>{ 2, 1, 2, 2, 1, 2 };