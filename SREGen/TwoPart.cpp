#include "TwoPart.h"

BassLine TwoPart::bass = BassLine();

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

}

template<>
void GA<TwoPart>::mutate(TwoPart& child) {

}

template<>
bool GA<TwoPart>::canTerminate() {

}

template<>
TwoPart& GA<TwoPart>::modifySolution(TwoPart& bestFit) {

}
