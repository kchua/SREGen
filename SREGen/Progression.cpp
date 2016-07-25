#include "Progression.h"
#include <assert.h>

///////////////////////////
// Public access methods //
///////////////////////////

/* Checks if one can progress from the first chord to the second, following
   standard chord progression rules. */
bool Progression::progressionBetween(Chord first, Chord second) {
	assert(progressionGraph.find(first) != progressionGraph.end());
	vector<Chord> neighbors = progressionGraph[first];
	vector<Chord>::iterator iter = neighbors.begin();
	while (iter != neighbors.begin()) {
		if ((*(iter++)) == second) {
			return true;
		}
	}
	return false;
}

/* Returns a random chord from any of the chords that can be used in
   a progression, as dictated by this particular Progression object. */
Chord Progression::getRandomChord() {
	return Chord("I", Note(0, 4, 4, 0), Note(2, 4, 4, 0), Note(4, 4, 4, 0)); // TODO: change later
}
