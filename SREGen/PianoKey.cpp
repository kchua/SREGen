#include "PianoKey.h"

typedef unordered_map<string, int> keyMap;
const keyMap PianoKey::keysToIndices = keyMap{ {"c", 0},
											   {"d", 2},
											   {"e", 4},
											   {"f", 5},
											   {"g", 7},
											   {"a", 9},
											   {"b", 11} };

PianoKey::PianoKey(string name, int accidental, int octave) {
	int key = keysToIndices.at(name) + accidental;
	if (key == -1) {
		key = 12;
		octave--;
	} else if (key == 12) {
		key = 0;
		octave++;
	}
	pairRepr = pair<int, int>(octave, key);
}

bool PianoKey::operator==(const PianoKey & other) const {
	return this->pairRepr == other.pairRepr;
}

bool PianoKey::operator!=(const PianoKey & other) const {
	return !((*this) == other);
}

bool PianoKey::operator<(const PianoKey & other) const {
	return this->pairRepr.first < other.pairRepr.first ||
		  (this->pairRepr.first == other.pairRepr.first &&
		   this->pairRepr.second < other.pairRepr.second);
}

bool PianoKey::operator>(const PianoKey& other) const {
	return (!((*this) < other)) && (!((*this) == other));
}