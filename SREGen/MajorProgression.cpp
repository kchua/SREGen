#include "MajorProgression.h"
#include <iostream>

MajorProgression::MajorProgression() : Progression(false) {
	/* TODO: REPLACE WITH COMPLETE CHART
	Chord toAdd("ii", Note(1), Note(3), Note(5));
	addChord(toAdd);
	Chord secondToAdd("V", Note(4), Note(6), Note(1));
	addChord(secondToAdd);

	makeProgressionBetween(toAdd, secondToAdd);
	makeProgressionBetween(secondToAdd, getTonic());
	*/
}

/*
int main() {
	Progression major = MajorProgression();

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
