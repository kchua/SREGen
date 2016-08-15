#include "TwoPart.h"
#include "TwoPart_Segment.h"
#include "GA.h"


////////////////////////////
// TwoPart static members //
////////////////////////////

int TwoPart::currStart = 0;
int TwoPart::currEnd = 0;

BassLine TwoPart::bass = BassLine();

int TwoPart::length = 8;

string TwoPart::key = "c";

bool TwoPart::isMinor = false;

const PianoKey TwoPart::lowerBound = PianoKey("f", 0, 4);
const PianoKey TwoPart::upperBound = PianoKey("g", 0, 5);

uniform_int_distribution<> TwoPart::selectorRNG = uniform_int_distribution<>(0, 7);
uniform_int_distribution<> TwoPart::octaveRNG = uniform_int_distribution<>(3, 5);
default_random_engine TwoPart::generator =
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


/////////////////////
// TwoPart methods //
/////////////////////

TwoPart TwoPart::generateHarmony() {
	TwoPart harmony = TwoPart();
	vector<Note> melody;
	melody.resize(length);

	for (int i = 0; i < (int) ceil(length / 8.0); i++) {
		setCurrBounds(8 * i, min(length, 8 * (i + 1)));
		do {
			GA<Segment> segGen(300, 150, 2, 0.9, 0.5);
			Segment seg = segGen.runSimulation();
			for (int j = currStart; j < currEnd; j++) {
				melody[j] = seg.melody[j];
			}
		} while (checkCorrectness(melody, 0, currEnd) != 0);
	}

	harmony.soprano = melody;
	return harmony;
}

int TwoPart::checkCorrectness(vector<Note>& melody, int start, int end) {
	for (int i = start; i < end; i++) {
		PianoKey curr = bass.getScale().getPianoKey(melody[i]);
		if (curr > upperBound || curr < lowerBound) {                                                    // Has to stay in range
			return -1 * (end - i);
		} else if (melody[i].getScaleNum() == bass[i].getScaleNum() &&                   // No doubled 7th
			melody[i].getScaleNum() == 6) {
			return -1 * (end - i);
		} else if (i > start) {
			Note above = melody[i - 1];
			Note below = above;
			above.setOctave(above.getOctave() + 1);
			below.setOctave(below.getOctave() - 1);
			if (below <= melody[i] && melody[i] <= above) {
				int currInterval = melody[i].getIntervalBetween(bass[i]);
				int prevInterval = melody[i - 1].getIntervalBetween(bass[i - 1]);
				if (currInterval == prevInterval && (currInterval == 5 || currInterval == 1)) {          // No parallel fifths
					return -1 * (end - i);
				} else if (melody[i - 1].getScaleNum() == 6 &&                                   // 7 -> 1
					       (melody[i].getScaleNum() != 0 ||
						    melody[i - 1].getOctave() != melody[i].getOctave() - 1)) {
					return -1 * (end - i);
				} else if (melody[i - 1].getScaleNum() == 3 &&                                   // 3 -> 4
						   (melody[i].getScaleNum() != 2 ||
							melody[i - 1].getOctave() != melody[i].getOctave())) {
					return -1 * (length - i);
				} else if (i > start + 1 &&
					       (melody[i - 1].getIntervalBetween(melody[i - 2]) >= 4 ||
							(melody[i - 1] == melody[i - 2] &&
							 melody[i - 1].getOctave() != melody[i - 2].getOctave()))) {
					Note step = melody[i - 1];
					Note leap = step;
					if (melody[i - 1] <= melody[i - 2]) {
						step++;
						(leap++)++;
					} else {
						step--;
						(leap--)--;
					}
					if (!(melody[i].getScaleNum() == step.getScaleNum() || 
						  melody[i].getScaleNum() == leap.getScaleNum())) {             // Leaps greater than a fourth
						return -1 * (length - i);                            // are followed by a step
					}                                                                            // (or a third leap) in the opposite
				}																		         // direction
			} else {
				return -1 * (length - i);                                   // No leaps greater than an octave
			}
		}
	}
	return 0;
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
		file << scale[soprano[i]] << " ";
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


////////////////////////////
// TwoPart Static Methods //
////////////////////////////

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

void TwoPart::setCurrBounds(int start, int end) {
	currStart = start;
	currEnd = end;
	selectorRNG = uniform_int_distribution<>(start, end - 1);
}


///////////////////////////////
// GA method specializations //
///////////////////////////////


template<>
pair<TwoPart::Segment, TwoPart::Segment> GA<TwoPart::Segment>::crossover(TwoPart::Segment parent1, TwoPart::Segment parent2) {
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
	return pair<TwoPart::Segment, TwoPart::Segment>(parent1, parent2);
}


template<>
void GA<TwoPart::Segment>::mutate(TwoPart::Segment& child) {
	if (rateRNG(generator) <= mutationRate) {
		int index = TwoPart::selectorRNG(TwoPart::generator);
		if (rateRNG(generator) <= 0.25) {
			int prevOctave = child.melody[index].getOctave();
			child.melody[index] = TwoPart::bass.getProgression().at(index).getRandomNote();
			child.melody[index].setOctave(prevOctave);
		} else {
			child.melody[index].setOctave(TwoPart::octaveRNG(TwoPart::generator));
		}
	}
}

template<>
bool GA<TwoPart::Segment>::canTerminate() {
	return organisms[0].fitness == 0;
}

template<>
TwoPart::Segment& GA<TwoPart::Segment>::modifySolution(TwoPart::Segment& bestFit) {
	return bestFit;
}

int main() {
	TwoPart::setLength(11);
	TwoPart::setKey("c");
	TwoPart::setTonality(true);
	TwoPart::setProgressionStartingChord(Chord("i", Note(0), Note(2), Note(4)));
	TwoPart::setEndingCadence({ Chord("V", Note(4), Note(6, 1), Note(1)), Chord("i", Note(0), Note(2), Note(4)) });
	TwoPart::createBassLine();
	TwoPart result = TwoPart::generateHarmony();
	ofstream output;
	output.open("twopart.ly");
	result.outputToFile(output);
}
