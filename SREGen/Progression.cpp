#include <iostream>
#include "Progression.h"

////////////////////
// Static members //
////////////////////

// default length
int Progression::length = 16;

// default starting chord is the tonic chord
Chord Progression::startingChord = Chord("i", Note(0), Note(2), Note(4));

// default ending chord is the tonic chord
vector<Chord> Progression::endingCadence = { Chord("V", Note(4), Note(6, 1), Note(1)), 
											 Chord("i", Note(0), Note(2), Note(4)) };

// default mode is major
ProgressionGraph Progression::graph = MajorProgression();

// initialize random number generator
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
	progression.optionalFitness = 0;
	progression.necessaryFitness = 0;
	bool hasPlagal = false;
	bool hasDiminishedResolution = false;
	bool hasConsecutiveTonics = false;
	Chord beforeBefore = graph.getRandomChord();
	Chord before = graph.getRandomChord();
	Chord after = graph.getRandomChord();

	for (int i = 0; i < progression.chords.size() + 1; i++) {
		if (!graph.progressionBetween(progression[i], progression[i + 1])) {
			progression.necessaryFitness--;
		}
		if (progression[i].getBottom().getScaleNum() == 3               // Only one IV/iv -> I/i
			&& progression[i + 1].getBottom().getScaleNum() == 0) {
			if (hasPlagal) {
				progression.optionalFitness--;
			} else {
				hasPlagal = true;
			}
		} else if (progression[i].getBottom().getScaleNum() == 6        // Only one vii0 -> I/i
			&& progression[i + 1].getBottom().getScaleNum() == 0) {
			if (hasDiminishedResolution) {
				progression.optionalFitness--;
			} else {
				hasDiminishedResolution = true;
			}
		} else if (progression[i].getBottom().getScaleNum() == 0        // Only one I/i -> I/i
			&& progression[i + 1].getBottom().getScaleNum() == 0) {
			if (hasConsecutiveTonics) {
				progression.optionalFitness--;
			} else {
				hasConsecutiveTonics = true;
			}
		} else if (i > 1 && before == after && after == progression[i]) {
			progression.optionalFitness--;
		} else if (i > 2 && beforeBefore.getBottom().getScaleNum() == 4 && // No V -> I/i -> V -> I/i
			before.getBottom().getScaleNum() == 0 &&
			after.getBottom().getScaleNum() == 4 &&
			progression[i].getBottom().getScaleNum() == 0) {
			progression.necessaryFitness -= 2;
		}

		beforeBefore = before;
		before = after;
		after = progression[i];
	}
}

Chord& Progression::operator[](int index) {
	if (index == 0) {
		return startingChord;
	} else if (index < length - endingCadence.size()) {
		return chords[index - 1];
	} else {
		return endingCadence[index - 1 - chords.size()];
	}
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
	return necessaryFitness + optionalFitness < other.necessaryFitness + other.optionalFitness;
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
	return organisms[0].necessaryFitness + (organisms[0].optionalFitness / 2) == 0;
}

template<>
Progression& GA<Progression>::modifySolution(Progression& bestFit) {
	return bestFit;
}


int main() {
	Progression::setProgressionLength(16);
	Progression::setMode(true);
	for (int i = 0; i < 10; i++) {
		GA<Progression> test(500, 40, 30, 0.5, 0.3);
		Progression p = test.runSimulation();
		cout << p.outputRomanNumerals() << endl << endl;
	}
}