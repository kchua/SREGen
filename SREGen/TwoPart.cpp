#include "TwoPart.h"

BassLine TwoPart::bass = BassLine();

int TwoPart::length = 8;

string TwoPart::key = "c";

bool TwoPart::isMinor = false;

uniform_int_distribution<> TwoPart::selectorRNG = uniform_int_distribution<>(0, 7);
uniform_int_distribution<> TwoPart::octaveRNG = uniform_int_distribution<>(4, 5);
default_random_engine TwoPart::generator =
	default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

/* Generates a random melody for a bass line.
   WARNING: must call createBassLine method before calling this function! */
TwoPart TwoPart::generateRandom() {
	TwoPart harmony = TwoPart();
	for (int i = 0; i < length; i++) {
		harmony.melody.push_back(harmony.bass.getProgression().at(i).getRandomNote());
		harmony.melody[i].setOctave(octaveRNG(generator));
	}
	return harmony;
}

void TwoPart::assignFitness(TwoPart& harmony) {
	for (int i = 0; i < length; i++) {
		if (harmony.melody[i].getScaleNum() == 6 &&
			harmony.bass[i].getScaleNum() == 6) {
			harmony.necessaryFitness -= 10;
		}
		if (i > 0) {
			if (harmony.melody[i - 1].getScaleNum() == 6 &&
				harmony.melody[i].getScaleNum() != 0) {
				harmony.necessaryFitness -= 10;
			} else if (harmony.melody[i - 1].getScaleNum() == 4 &&
				       harmony.melody[i].getScaleNum() != 3) {
				harmony.necessaryFitness -= 10;
			}
			int prevInterval = harmony.melody[i - 1].getIntervalBetween(harmony.bass[i - 1]);
			int currInterval = harmony.melody[i].getIntervalBetween(harmony.bass[i]);
			if (currInterval == prevInterval) {
				if (currInterval == 5 || currInterval == 1) {
					harmony.necessaryFitness -= 10;
				}
			}
			if (i > 1) {
				if ((harmony.melody[i - 1].getIntervalBetween(harmony.melody[i - 2]) >= 4) ||
					(harmony.melody[i - 1] == harmony.melody[i - 2] && 
						harmony.melody[i - 1].getOctave() != harmony.melody[i - 2].getOctave())) {
					Note prev = harmony.melody[i - 1];
					if (harmony.melody[i - 2] <= harmony.melody[i - 1]) {
						if (!((--prev) == harmony.melody[i])) {
							harmony.optionalFitness--;
						}
					} else {
						if (!((++prev) == harmony.melody[i])) {
							harmony.optionalFitness--;
						}
					}
				}
			}
		}
	}
}

void TwoPart::setProgressionStartingChord(Chord chord) {
	Progression::setStartingChord(chord);
}

void TwoPart::setEndingCadence(vector<Chord> cadence) {
	Progression::setEndingCadence(cadence);
}

void TwoPart::setLength(int len) {
	length = len;
}

void TwoPart::setKey(string key) {
	TwoPart::key = key;
}

void TwoPart::setTonality(bool isMinor) {
	TwoPart::isMinor = isMinor;
}

/* Generates the bassline upon which the melody would be based. 
   Warning: must be called before starting the genetic algorithm! */
void TwoPart::createBassLine() {
	bass = BassLine::generate(length, key, isMinor);
}

bool TwoPart::operator<(const TwoPart& other) const {
	return this->necessaryFitness + this->optionalFitness < other.necessaryFitness + other.optionalFitness;
}


///////////////////////////////
// GA method specializations //
///////////////////////////////


template<>
pair<TwoPart, TwoPart> GA<TwoPart>::crossover(TwoPart parent1, TwoPart parent2) {
	for (int i = 0; i < TwoPart::length; i++) {
		if (rateRNG(generator) <= crossoverRate) {
			Note temp = parent1.melody[i];
			parent1.melody[i] = parent2.melody[i];
			parent2.melody[i] = temp;
		}
	}
	return pair<TwoPart, TwoPart>(parent1, parent2);
}


template<>
void GA<TwoPart>::mutate(TwoPart& child) {
	if (rateRNG(generator) <= mutationRate) {
		int index = TwoPart::selectorRNG(TwoPart::generator);
		child.melody[index] = child.bass.getProgression().at(index).getRandomNote();
	}
}

template<>
bool GA<TwoPart>::canTerminate() {
	return organisms[0].necessaryFitness == 0;
}

template<>
TwoPart& GA<TwoPart>::modifySolution(TwoPart& bestFit) {
	return bestFit;
}
