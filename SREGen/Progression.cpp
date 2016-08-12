#include <iostream>
#include "Progression.h"

////////////////////
// Static members //
////////////////////

// default length
int Progression::length = 16;

// default starting chord is the tonic chord
Chord Progression::startingChord = Chord("I", Note(0), Note(2), Note(4));

// default ending chord is the tonic chord
vector<Chord> Progression::endingCadence = { Chord("V", Note(4), Note(6), Note(1)), 
											 Chord("I", Note(0), Note(2), Note(4)) };

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
void Progression::assignFitness(Progression& prog) {
	prog.optionalFitness = 0;
	prog.necessaryFitness = 0;
	bool hasPlagal = false;
	bool hasDiminishedResolution = false;
	bool hasConsecutiveTonics = false;

	for (int i = 0; i < prog.chords.size() + 1; i++) {
		if (!graph.progressionBetween(prog[i], prog[i + 1])) {
			prog.necessaryFitness--;
		} else if (prog[i].getBottom().getScaleNum() == 3        // Only one IV/iv -> I/i
			&& prog[i + 1].getBottom().getScaleNum() == 0) {
			if (hasPlagal) {
				prog.optionalFitness--;
			} else {
				hasPlagal = true;
			}
		} else if (prog[i].getBottom().getScaleNum() == 6        // Only one vii0 -> I/i
			&& prog[i + 1].getBottom().getScaleNum() == 0) {
			if (hasDiminishedResolution) {
				prog.optionalFitness--;
			} else {
				hasDiminishedResolution = true;
			}
		} else if (prog[i].getBottom().getScaleNum() == 0        // Only one I/i -> I/i
			&& prog[i + 1].getBottom().getScaleNum() == 0) {
			if (hasConsecutiveTonics) {
				prog.optionalFitness--;
			} else {
				hasConsecutiveTonics = true;
			}
		} else if (i > 1 && prog[i] == prog[i - 1]               // No chord repetition more than
			             && prog[i - 1] == prog[i - 2]) {        // three times in a row.
			prog.optionalFitness--;				
		} else if (i > 2 && prog[i - 3].getRootDeg() == 4        // No V -> I/i -> V -> I/i
						 && prog[i - 2].getRootDeg() == 0 
						 && prog[i - 1].getRootDeg() == 4 
						 && prog[i].getRootDeg() == 0) {
			prog.necessaryFitness -= 2;
		}
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

/* Chord getter method which returns a CONST reference. */
const Chord& Progression::at(int index) const {
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
string Progression::outputRomanNumerals() const {
	string output = "";
	output += startingChord.getName();
	output += " -> ";
	for (int i = 0; i < chords.size(); i++) {
		output += chords.at(i).getName();
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

/* Takes two progressions and performs crossover of chords between them. */
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

/* Mutates a random chord within the progression from time to time. */
template<>
void GA<Progression>::mutate(Progression& child) {
	if (rateRNG(generator) <= mutationRate) {
		child.chords[Progression::selectorRNG(Progression::generator)] =
			child.graph.getRandomChord();
	}
}

/* Checks termination conditions. */
template<>
bool GA<Progression>::canTerminate() {
	return organisms[0].necessaryFitness == 0;
}

/* Modifies the final solution by making common inversions to the chords. */
template<>
Progression& GA<Progression>::modifySolution(Progression& bestFit) {
	for (int i = 0; i < bestFit.length - 2; i++) {
		if (bestFit[i] == bestFit[i + 1] && bestFit[i].getRootDeg() == 0) {
			bestFit[i + 1].invert();
		} else if (bestFit[i] == bestFit[i + 2] && bestFit[i].getBottom().getScaleNum() == 0
			&& bestFit[i + 1].getRootDeg() == 3) {
			bestFit[i + 1].invert();
			bestFit[i + 1].invert();
			i += 2;
		} else if (bestFit[i] == bestFit[i + 2] && bestFit[i + 1].getRootDeg() == 4 
					&& i == bestFit.length - 3) {
			while (bestFit[i].getBottom().getScaleNum() != 0) {
				bestFit[i].invert();
			}
			bestFit[i].invert();
			bestFit[i].invert();
		} else if (bestFit[i].getRootDeg() == 6) {
			bestFit[i].invert();
		} else if (bestFit[i].getRootDeg() == 3 && bestFit[i + 1].getRootDeg() == 1) {
			if (rateRNG(generator) < 1) {
				bestFit[i + 1].invert();
			}
		} else if (bestFit[i].getRootDeg() == 4) {
			if (rateRNG(generator) < 0.5) {
				bestFit[i].invert();
				if (bestFit[i + 1].getRootDeg() == 5) {
					bestFit[i + 1].invert();
				}
				i++;
			}
		}
	}
	return bestFit;
}

/*
int main() {
	Progression::setProgressionLength(8);
	for (int i = 0; i < 20; i++) {
		GA<Progression> test(300, 40, 40, 0.5, 0.05);
		Progression p = test.runSimulation();
		cout << p.outputRomanNumerals() << endl;
	}
}
*/