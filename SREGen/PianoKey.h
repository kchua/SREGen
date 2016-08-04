#ifndef PIANOKEY_H
#define PIANOKEY_H

#include <string>
#include <utility>
#include <unordered_map>

using namespace std;

class PianoKey {
public:
	PianoKey(string name, int accidental, int octave = 4);
	bool operator==(const PianoKey& other) const;
	bool operator!=(const PianoKey& other) const;
	bool operator<(const PianoKey& other) const;
	bool operator>(const PianoKey& other) const;
private:
	pair<int, int> pairRepr;
	static const unordered_map<string, int> keysToIndices;
};

#endif