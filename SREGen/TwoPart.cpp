#include "TwoPart.h"
#include "TwoPart_Segment.h"
#include "GA.h"


////////////////////////////
// TwoPart static members //
////////////////////////////

BassLine TwoPart::bass = BassLine();

int TwoPart::length = 8;

string TwoPart::key = "c";

bool TwoPart::isMinor = false;


////////////////////////////
// Segment static members //
////////////////////////////

const PianoKey TwoPart::Segment::lowerBound = PianoKey("f", 0, 4);
const PianoKey TwoPart::Segment::upperBound = PianoKey("g", 0, 5);

uniform_int_distribution<> TwoPart::Segment::selectorRNG = uniform_int_distribution<>(0, 7);
uniform_int_distribution<> TwoPart::Segment::octaveRNG = uniform_int_distribution<>(3, 5);
default_random_engine TwoPart::Segment::generator =
	default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());


/////////////////////
// Segment methods //
/////////////////////

/* Generates a random melody for a bass line.
   WARNING: must call createBassLine method before calling this function! */
TwoPart::Segment TwoPart::Segment::generateRandom() {
	Segment harmony = Segment();
	for (int i = 0; i < length; i++) {
		harmony.melody.push_back(TwoPart::bass.getProgression().at(i).getRandomNote());
		harmony.melody[i].setOctave(octaveRNG(generator));
	}
	return harmony;
}

void TwoPart::Segment::assignFitness(TwoPart::Segment& seg) {
	seg.fitness = checkCorrectness(seg.melody, currStart, currEnd);
}

bool TwoPart::Segment::operator<(const Segment& other) const {
	return (*this).fitness < other.fitness;
}
















void TwoPart::assignFitness(TwoPart& harmony) {
	harmony.necessaryFitness = 0;
	harmony.optionalFitness = 0;
	for (int i = 0; i < length; i++) {
		PianoKey curr = harmony.bass.getScale().getPianoKey(harmony.melody[i]);
		if (curr > upperBound || curr < lowerBound) {                                                    // Has to stay in range
			harmony.necessaryFitness -= 1 * (length - i);
			return;
		} else if (harmony.melody[i].getScaleNum() == harmony.bass[i].getScaleNum() &&                   // No doubled 7th
			harmony.melody[i].getScaleNum() == 6) {
			harmony.necessaryFitness -= 1 * (length - i);
			return;
		} else if (i > 0) {
			Note above = harmony.melody[i - 1];
			Note below = above;
			above.setOctave(above.getOctave() + 1);
			below.setOctave(below.getOctave() - 1);
			if (below <= harmony.melody[i] && harmony.melody[i] <= above) {
				int currInterval = harmony.melody[i].getIntervalBetween(harmony.bass[i]);
				int prevInterval = harmony.melody[i - 1].getIntervalBetween(harmony.bass[i - 1]);
				if (currInterval == prevInterval && (currInterval == 5 || currInterval == 1)) {          // No parallel fifths
					harmony.necessaryFitness -= 1 * (length - i);
					return;
				} else if (harmony.melody[i - 1].getScaleNum() == 6 &&                                   // 7 -> 1
					       (harmony.melody[i].getScaleNum() != 0 ||
						    harmony.melody[i - 1].getOctave() != harmony.melody[i].getOctave() - 1)) {
					harmony.necessaryFitness -= 1 * (length - i);
					return;
				} else if (harmony.melody[i - 1].getScaleNum() == 3 &&                                   // 3 -> 4
						   (harmony.melody[i].getScaleNum() != 2 ||
							harmony.melody[i - 1].getOctave() != harmony.melody[i].getOctave())) {
					harmony.necessaryFitness -= 1 * (length - i);
					return;
				} else if (i > 1 &&
					       (harmony.melody[i - 1].getIntervalBetween(harmony.melody[i - 2]) >= 4 ||
							(harmony.melody[i - 1] == harmony.melody[i - 2] &&
							 harmony.melody[i - 1].getOctave() != harmony.melody[i - 2].getOctave()))) {
					Note step = harmony.melody[i - 1];
					Note leap = step;
					if (harmony.melody[i - 1] <= harmony.melody[i - 2]) {
						step++;
						(leap++)++;
					} else {
						step--;
						(leap--)--;
					}
					if (!(harmony.melody[i].getScaleNum() == step.getScaleNum() || 
						  harmony.melody[i].getScaleNum() == leap.getScaleNum())) {             // Leaps greater than a fourth
						harmony.necessaryFitness -= 1 * (length - i);                            // are followed by a step
						return;
					}                                                                            // (or a third leap) in the opposite
				}																		         // direction
			} else {
				harmony.necessaryFitness -= 1 * (length - i);                                   // No leaps greater than an octave
				return;
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

void TwoPart::outputToFile(ofstream& file) {
	Scale scale = bass.getScale();
	file << "{\n";
	file << "\t<<\n";

	file << "\t\t\\new Staff {\n";
	file << "\t\t\t\\time 4/4\n";
	file << "\t\t\t\\clef treble\n";
	file << "\t\t\t\\key " << TwoPart::key << ((isMinor) ? "\\minor\n" : " \\major\n");
	file << "\t\t\t";
	for (int i = 0; i < length; i++) {
		file << scale[melody[i]] << " ";
	}
	file << "\n";
	file << "\t\t}\n";


	file << "\t\t\\new Staff {\n";
	file << "\t\t\t\\clef bass\n";
	file << "\t\t\t\\key " << TwoPart::key << ((isMinor) ? "\\minor\n" : " \\major\n");
	file << "\t\t\t";
	for (int i = 0; i < length; i++) {
		file << scale[bass[i]] << " ";
	}
	file << "\n";
	file << "\t\t}\n";

	file << "\t>>";
	file << "}";
	file.close();
	bass.getProgression().outputRomanNumerals();
}

///////////////////////////////
// GA method specializations //
///////////////////////////////


template<>
pair<TwoPart, TwoPart> GA<TwoPart>::crossover(TwoPart parent1, TwoPart parent2) {
	if (rateRNG(generator) <= crossoverRate) {
		int index = TwoPart::selectorRNG(TwoPart::generator);
		for (int i = index; i < TwoPart::length; i++) {
			Note temp = parent1.melody[i];
			parent1.melody[i] = parent2.melody[i];
			parent2.melody[i] = temp;
		}
	}
	/*
	for (int i = 0; i < TwoPart::length; i++) {
		if (rateRNG(generator) <= crossoverRate) {
			Note temp = parent1.melody[i];
			parent1.melody[i] = parent2.melody[i];
			parent2.melody[i] = temp;
		}
	}
	*/
	return pair<TwoPart, TwoPart>(parent1, parent2);
}


template<>
void GA<TwoPart>::mutate(TwoPart& child) {
	if (rateRNG(generator) <= mutationRate) {
		int index = TwoPart::selectorRNG(TwoPart::generator);
		if (rateRNG(generator) <= 0.25) {
			int prevOctave = child.melody[index].getOctave();
			child.melody[index] = child.bass.getProgression().at(index).getRandomNote();
			child.melody[index].setOctave(prevOctave);
		} else {
			child.melody[index].setOctave(TwoPart::octaveRNG(TwoPart::generator));
		}
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

template<>
void GA<TwoPart::Segment>::mutate(TwoPart::Segment& child) {

}

int main() {
	TwoPart::setLength(8);
	TwoPart::setKey("bes");
	TwoPart::setTonality(false);
	TwoPart::setProgressionStartingChord(Chord("I", Note(0), Note(2), Note(4)));
	TwoPart::setEndingCadence({ Chord("V", Note(4), Note(6), Note(1)), Chord("I", Note(0), Note(2), Note(4)) });
	TwoPart::createBassLine();
	GA<TwoPart> test(300, 150, 2, 0.9, 0.5);
	TwoPart result = test.runSimulation();
	ofstream output;
	output.open("twopart.ly");
	result.outputToFile(output);
}
