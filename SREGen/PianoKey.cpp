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
	} else if (key = 12) {
		key = 0;
		octave++;
	}
	pairRepr = pair<int, int>(octave, key);
}