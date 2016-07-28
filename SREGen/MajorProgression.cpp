#include "MajorProgression.h"
#include <iostream>

MajorProgression::MajorProgression() : ProgressionGraph(false) {
	vector<Chord> standard = vector<Chord>();
	vector<string> names{ "ii", "iii", "IV", "V", "vi", "vii0" };
	Note root(1); Note third(3); Note fifth(5);

	standard.push_back(getTonic());
	for (int i = 0; i < 6; i++) {
		standard.push_back(Chord(names[i], root++, third++, fifth++));
		addChord(standard[i + 1]);
	}

	///////////////
	// Dominants //
	///////////////

	// Directed edges from the V chord
	makeProgressionBetween(standard[4], standard[0]);	// V -> I
	makeProgressionBetween(standard[4], standard[5]);   // V -> vi

	// Directed edge from the vii0 chord
	makeProgressionBetween(standard[6], standard[0]);   // vii0 -> I

	//////////////////
	// Subdominants //
	//////////////////

	// Directed edges from the IV chord
	makeProgressionBetween(standard[3], standard[0]);   // IV -> I
	makeProgressionBetween(standard[3], standard[4]);   // IV -> V
	makeProgressionBetween(standard[3], standard[6]);   // IV -> vii0
	makeProgressionBetween(standard[3], standard[1]);   // IV -> ii

	// Directed edges from the ii chord
	makeProgressionBetween(standard[1], standard[4]);   // ii -> V
	makeProgressionBetween(standard[1], standard[6]);   // ii -> vii0

	//////////////////
	// Other chords //
	//////////////////

	// Directed edges from the vi chord
	makeProgressionBetween(standard[5], standard[3]);	// vi -> IV
	makeProgressionBetween(standard[5], standard[1]);	// vi -> ii

	// Directed edges from the iii chord
	makeProgressionBetween(standard[2], standard[3]);	// iii -> IV
	makeProgressionBetween(standard[2], standard[5]);   // iii -> vi
}

/*
int main() {
	ProgressionGraph major = MajorProgression();

	Chord toAdd("ii", Note(1), Note(3), Note(5));
	Chord secondToAdd("V", Note(4), Note(6), Note(1));
	Chord tonic("I", Note(0), Note(2), Note(4));

	cout << "Performing basic tests" << endl;
	cout << "ii -> V: " << major.progressionBetween(toAdd, secondToAdd) << endl;
	cout << "V -> ii: " << major.progressionBetween(secondToAdd, toAdd) << endl << endl;

	cout << "ii -> V: " << major.progressionBetween(toAdd, secondToAdd) << endl;
	cout << "V -> ii: " << major.progressionBetween(secondToAdd, toAdd) << endl << endl << endl;

	
	cout << "Performing test on tonic chord" << endl;
	cout << "I -> ii: " << major.progressionBetween(tonic, toAdd) << endl;
	cout << "I -> V: " << major.progressionBetween(tonic, secondToAdd) << endl;
	cout << "I -> I: " << major.progressionBetween(tonic, tonic) << endl << endl << endl;


	cout << "Checking dominant" << endl;
	cout << "V -> I: " << major.progressionBetween(secondToAdd, tonic) << endl;
	cout << "ii -> I: " << major.progressionBetween(toAdd, tonic) << endl;
}
*/
