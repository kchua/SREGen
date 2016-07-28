#include "Progression.h"
#include "GA.h"
#include "MajorProgression.h"
#include "MinorProgression.h"
#include <iostream>

////////////////////
// Static members //
////////////////////

int Progression::length = 10;
Chord Progression::startingChord = Chord("I", Note(0), Note(2), Note(4));
vector<Chord> Progression::endingCadence = { Chord("I", Note(0), Note(2), Note(4)) };
ProgressionGraph Progression::graph = MajorProgression();

/* Generates a random progression without regard for rules regarding chord
   resolution. */
Progression Progression::generateRandom() {
	Progression p = Progression();
	int size = length - 1 - endingCadence.size();
	for (int i = 1; i < size; i++) {
		p.chords.push_back(graph.getRandomChord());
	}
	return p;
}

/* Assigns a fitness score to a chord progression. */
void Progression::assignFitness(Progression& progression) {
	int score = 0;
	if (!graph.progressionBetween(startingChord, progression.chords[0])) {
		score--;
	}
	for (int i = 0; i < progression.chords.size() - 1; i++) {
		if (!graph.progressionBetween(progression.chords[i], progression.chords[i + 1])) {
			score--;
		}
	}
	if (!graph.progressionBetween(progression.chords[progression.chords.size() - 1],
		endingCadence[0])) {
		score--;
	}
	progression.fitness = score;
}


///////////////////////////
// Static Setter Methods //
///////////////////////////

/* Sets the length of the progression, including the starting chord and the
   ending cadence. */
void Progression::setProgresssionLength(int length) {
	Progression::length = length;
}

/* Sets the starting chord of the progression. */
void Progression::setStartingChord(Chord chord) {
	Progression::startingChord = chord;
}

/* Sets the ending cadence of the progression. */
void Progression::setEndingCadence(vector<Chord> cadence) {
	Progression::endingCadence = cadence;
}

/* Determines whether randomly generated progressions and fitness checking
   will be performed in the major or minor mode. */
void Progression::setMode(bool isMinor) {
	if (isMinor) {
		Progression::graph = MinorProgression();
	} else {
		Progression::graph = MajorProgression();
	}
}


/////////////////////
// Utility Methods //
/////////////////////

/* Returns true if this Progression is less fit than the other progression. */
bool Progression::operator<(const Progression& other) const {
	return fitness < other.fitness;
}

/* Output a progression as a string (NOT for lilypond files). */
string Progression::outputRomanNumerals() {
	string output = "";
	output += startingChord.getName();
	output += " -> ";
	for (int i = 0; i < chords.size(); i++) {
		output += chords[i].getName();
		output += " -> ";
	}
	for (int i = 0; i < endingCadence.size() - 1; i++) {
		output += endingCadence[i].getName();
		output += " -> ";
	}
	output.append(endingCadence[endingCadence.size() - 1].getName());
	return output;
}


///////////////////////////////
// GA method specializations //
///////////////////////////////

template<>
pair<Progression, Progression> GA<Progression>::crossover(Progression parent1, Progression parent2) {
	return pair<Progression, Progression>();
}

template<>
void GA<Progression>::mutate(Progression& child) {
	return;
}

template<>
bool GA<Progression>::canTerminate() {
	return false;
}

template<>
Progression& GA<Progression>::modifySolution(Progression& bestFit) {
	return bestFit;
}

int main() {
	Progression::setStartingChord(Chord("i", Note(0), Note(2), Note(4)));
	Progression::setEndingCadence({ Chord("i", Note(0), Note(2), Note(4)) });
	Progression::setMode(true);
	Progression::setProgresssionLength(10);
	cout << Progression::generateRandom().outputRomanNumerals() << endl;
	cout << Progression::generateRandom().outputRomanNumerals() << endl;
	cout << Progression::generateRandom().outputRomanNumerals() << endl;
	cout << Progression::generateRandom().outputRomanNumerals() << endl;
	cout << Progression::generateRandom().outputRomanNumerals() << endl;
	cout << Progression::generateRandom().outputRomanNumerals() << endl;
}