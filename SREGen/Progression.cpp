#include <iostream>
#include "Progression.h"

////////////////////
// Static members //
////////////////////

// default length
int Progression::length = 10;

// default starting chord is the tonic chord
Chord Progression::startingChord = Chord("I", Note(0), Note(2), Note(4));

// default ending chord is the tonic chord
vector<Chord> Progression::endingCadence = { Chord("I", Note(0), Note(2), Note(4)) };

// default mode is major
ProgressionGraph Progression::graph = MajorProgression();

// initialize random number generators
uniform_int_distribution<> Progression::selectorRNG = uniform_int_distribution<>(0, 7);
default_random_engine Progression::generator =
	default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

/* Generates a random progression without regard for rules regarding chord
   resolution. */
Progression Progression::generateRandom() {
	Progression p = Progression();
	int size = length - 1 - endingCadence.size();
	for (int i = 0; i < size; i++) {
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
void Progression::setProgressionLength(int length) {
	Progression::selectorRNG = uniform_int_distribution<>(0, length - 2 - endingCadence.size());
	Progression::length = length;
}

/* Sets the starting chord of the progression. */
void Progression::setStartingChord(Chord chord) {
	Progression::startingChord = chord;
}

/* Sets the ending cadence of the progression. */
void Progression::setEndingCadence(vector<Chord> cadence) {
	Progression::selectorRNG = uniform_int_distribution<>(0, length - 2 - cadence.size());
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
	for (int i = 0; i < parent1.chords.size(); i++) {
		if (rateRNG(generator) <= crossoverRate) {
			Chord temp = parent1.chords[i];
			parent1.chords[i] = parent2.chords[i];
			parent2.chords[i] = temp;
		}
	}
	return pair<Progression, Progression>(parent1, parent2);
}

template<>
void GA<Progression>::mutate(Progression& child) {
	if (rateRNG(generator) <= mutationRate) {
		child.chords[Progression::selectorRNG(Progression::generator)] =
			child.graph.getRandomChord();
	}
}

template<>
bool GA<Progression>::canTerminate() {
	return organisms[0].fitness == 0;
}

template<>
Progression& GA<Progression>::modifySolution(Progression& bestFit) {
	return bestFit;
}

int main() {
	Progression::setProgressionLength(8);
	GA<Progression> test(1000, 50, 50, 0.5, 0.05);
	Progression p = test.runSimulation();
	cout << p.outputRomanNumerals();
}