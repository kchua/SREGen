#include "MinorProgression.h"

MinorProgression::MinorProgression() : ProgressionGraph(true) {
	vector<Note> notes{ Note(0), Note(1), Note(2), Note(3), Note(4), Note(5), Note(6, 1) };
	vector<Chord> standard = vector<Chord>();
	vector<string> names{ "ii0", "III+", "iv", "V", "VI", "vii0" };
	Note root(1); Note third(3); Note fifth(5);

	standard.push_back(getTonic());
	for (int i = 0; i < 6; i++) {
		standard.push_back(Chord(names[i], notes[(1 + i) % 7], notes[(3 + i) % 7], notes[(5 + i) % 7]));
		addChord(standard[i + 1]);
	}

	Chord altThird("III", Note(2), Note(4), Note(6));
	Chord altSeventh("VII", Note(6), Note(1), Note(3));

	addChord(altThird);
	addChord(altSeventh);

	///////////////
	// Dominants //
	///////////////

	// Directed edges from the V chord
	makeProgressionBetween(standard[4], standard[0]);	// V -> i
	makeProgressionBetween(standard[4], standard[5]);   // V -> VI

	// Directed edge from the vii0 chord
	makeProgressionBetween(standard[6], standard[0]);   // vii0 -> i

	//////////////////
	// Subdominants //
	//////////////////

	// Directed edges from the iv chord
	makeProgressionBetween(standard[3], standard[0]);   // iv -> i
	makeProgressionBetween(standard[3], standard[4]);   // iv -> V
	makeProgressionBetween(standard[3], standard[6]);   // iv -> vii0
	makeProgressionBetween(standard[3], standard[1]);   // iv -> ii0

	// Directed edges from the ii0 chord
	makeProgressionBetween(standard[1], standard[4]);   // ii -> V
	makeProgressionBetween(standard[1], standard[6]);   // ii -> vii0

	//////////////////
	// Other chords //
	//////////////////

	// Directed edges from the VI chord
	makeProgressionBetween(standard[5], standard[3]);	// VI -> iv
	makeProgressionBetween(standard[5], standard[1]);	// VI -> ii0

	// Directed edges from the VII chord
	makeProgressionBetween(altSeventh, altThird);	// VII -> III

	// Directed edges from the III chord
	makeProgressionBetween(altThird, standard[3]);	// III -> iv
	makeProgressionBetween(altThird, standard[5]);	// III -> VI
}
