#include "PianoKey.h"

typedef unordered_map<string, int> keyMap;
const keyMap PianoKey::keysToIndices = keyMap{ {"c", 0},
											   {"d", 2},
											   {"e", 4},
											   {"f", 5},
											   {"g", 7},
											   {"a", 9},
											   {"b", 11} };

/* Creates a piano key, converting all provided information into the
   internal representation. */
PianoKey::PianoKey(string name, int accidental, int octave) {
	int key = keysToIndices.at(name) + accidental;
	if (key == -1) {
		key = 11;
		octave--;
	} else if (key == 12) {
		key = 0;
		octave++;
	}
	pairRepr = pair<int, int>(octave, key);
}

/* Returns true if two keys are found at the same location on the piano. */
bool PianoKey::operator==(const PianoKey & other) const {
	return this->pairRepr == other.pairRepr;
}

/* Returns true if two keys are found at different places on the piano. */
bool PianoKey::operator!=(const PianoKey & other) const {
	return !((*this) == other);
}

/* Returns true if THIS piano key is to the left of the other piano key. */
bool PianoKey::operator<(const PianoKey & other) const {
	return this->pairRepr.first < other.pairRepr.first ||
		  (this->pairRepr.first == other.pairRepr.first &&
		   this->pairRepr.second < other.pairRepr.second);
}

/* Returns true if THIS piano key is to the right of the other piano key. */
bool PianoKey::operator>(const PianoKey& other) const {
	return (!((*this) < other)) && (!((*this) == other));
}