#include "ProgressionGraph.h"
#include <chrono>
#include <assert.h>

///////////////////////////
// Public access methods //
///////////////////////////

/* Checks if one can progress from the first chord to the second, following
   standard chord progression rules. */
bool ProgressionGraph::progressionBetween(Chord first, Chord second) {
	assert(graph.find(first) != graph.end());
	vector<Chord> neighbors = graph[first];
	vector<Chord>::iterator iter = neighbors.begin();
	while (iter != neighbors.end()) {
		if ((*(iter++)) == second) {
			return true;
		}
	}
	return false;
}

/* Returns a random chord from any of the chords that can be used in
   a progression, as dictated by this particular Progression object. */
Chord ProgressionGraph::getRandomChord() {
	return chords[ChordRNG(generator)];
}


/////////////////////////
// Protected functions //
/////////////////////////

/*
	These are helper functions to be used by objects extending the Progression
	class only in their constructors, as these functions establish which
	chords can move on to which other chords.
*/

/* Creates an empty Progression object by setting the appropriate tonic chord,
   which would be linked to every other chord added, as well as itself. */
ProgressionGraph::ProgressionGraph(bool isMinor)
	: tonicChord(((isMinor) ? "i" : "I"), Note(0), Note(2), Note(4)) {
	addChord(tonicChord);
}

/* Adds the chord to the progression if it does not exist yet, and establishes
   a link from the tonic chord to the new chord. */
void ProgressionGraph::addChord(Chord chord) {
	if (graph.find(chord) == graph.end()) {
		graph[chord] = vector<Chord>();
		chords.push_back(chord);
		makeProgressionBetween(tonicChord, chord);
	}
}

/* Creates a directed link from the first chord to the second 
   chord. */
void ProgressionGraph::makeProgressionBetween(Chord first, Chord second) {
	assert(graph.find(second) != graph.end());
	if (!progressionBetween(first, second)) {
		graph.at(first).push_back(second);
	}
}

/* Returns a copy of the tonic chord. */
Chord ProgressionGraph::getTonic() {
   return tonicChord;
}

/* Finalizes a chord progression graph and sets up the random integer generator.
   Note: any chords added after calling this function is inaccessible by the
   getRandomChord method. */
void ProgressionGraph::finalize() {
	ChordRNG = uniform_int_distribution<>(0, chords.size() - 1);

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	generator = default_random_engine(seed);
}


/* Calculates a hash code for Chords based on their name. */
size_t ProgressionGraph::chordHashFunction::operator()(Chord chord) const {
	return std::hash<string>()(chord.getNameWithoutInv());
}
