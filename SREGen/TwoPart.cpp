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

void TwoPart::outputToFile(ofstream& file) {
	Scale scale = bass.getScale();
	file << "{\n";
	file << "\t<<\n";

	file << "\t\t\\new Staff {\n";
	file << "\t\t\t\\time 4/4\n";
	file << "\t\t\t\\clef treble\n";
	file << "\t\t\t\\key " << TwoPart::key << ((isMinor) ? " \\minor\n" : " \\major\n");
	file << "\t\t\t";
	for (int i = 0; i < length; i++) {
		file << scale[soprano[i]] << " ";
	}
	file << "\n";
	file << "\t\t}\n";


	file << "\t\t\\new Staff {\n";
	file << "\t\t\t\\clef bass\n";
	file << "\t\t\t\\key " << TwoPart::key << ((isMinor) ? " \\minor\n" : " \\major\n");
	file << "\t\t\t";
	for (int i = 0; i < length; i++) {
		file << scale[bass[i]] << " ";
	}
	file << "\n";
	file << "\t\t}\n";

	file << "\t>>\n";
	file << "}";
	file.close();
	bass.getProgression().outputRomanNumerals();
}

////////////////////////////
// TwoPart Static Methods //
////////////////////////////

TwoPart TwoPart::generateHarmony() {
	TwoPart harmony = TwoPart();
	vector<Note> melody;
	melody.resize(length);
	int returnsToStart = 0;
	int segLen = 6;

	for (int i = 0; i < (int) ceil(length / ((double) segLen)); i++) {
		setCurrBounds(segLen * i, min(length, segLen * (i + 1)));
		int attempts = 0;
		do {
			attempts++;
			if (attempts > 9) {
				if (i != 0) {
					cout << "Exceeded attempt limit. Regenerating previous segment." << endl;
					i -= 2;
					if (i == -1) {
						returnsToStart++;
					}
					if (returnsToStart > 2) {
						createBassLine();
						returnsToStart = 0;
					}
					break;
				}
			}
			cout << "Generating melody segment " << i + 1 << ", attempt " << attempts << "... ";
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
		if (!pianoKeyInBounds(curr)) {                                                    // Has to stay in range
			return -1 * (end - i);
		} else if (doubledSeventh(melody, i)) {
			return -1 * (end - i);
		} else if (i > start) {
			if (withinOctaveOfPrev(melody, i)) {
				if (hasSeventhLeap(melody, i)) {
					return -1 * (end - i);
				} else if (hasBadParallels(melody, i)) {
					return -1 * (end - i);
				} else if (!ifSeventhResToTonic(melody, i)) {
					return -1 * (end - i);
				} else if (!ifFourResToThree(melody, i)) {
					return -1 * (end - i);
				} else if (i > start + 1 &&
					       !ifLeapHasValidRes(melody, i)) {
					return -1 * (end - i);
				}
			} else {
				return -1 * (end - i);
			}
		}
	}
	return 0;
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

////////////////////////////
// Private static methods //
////////////////////////////

/* Changes which part of the melody is being looked at for segment generation. */
void TwoPart::setCurrBounds(int start, int end) {
	currStart = start;
	currEnd = end;
	selectorRNG = uniform_int_distribution<>(start, end - 1);
}

/* Returns true if a key is in the common range for a soprano voice. */
bool TwoPart::pianoKeyInBounds(PianoKey key) {
	return !(key > upperBound || key < lowerBound);
}

/* Returns true if there is a doubled seventh. */
bool TwoPart::doubledSeventh(vector<Note>& melody, int index) {
	return (melody[index].getScaleNum() == bass[index].getScaleNum() &&
			melody[index].getScaleNum() == 6 && 
			melody[index].getAccidental() == (isMinor) ? 1 : 0);
}

/* Returns true if a note at the given index is within an octave of the previous note. */
bool TwoPart::withinOctaveOfPrev(vector<Note>& melody, int index) {
	Note above = melody[index - 1];
	Note below = above;
	above.setOctave(above.getOctave() + 1);
	below.setOctave(below.getOctave() - 1);
	return below <= melody[index] && melody[index] <= above;
}

/* Returns true if there is a seventh leap between the note at the given index and
   the note before it. */
bool TwoPart::hasSeventhLeap(vector<Note>& melody, int index) {
	if (melody[index - 1].getIntervalBetween(melody[index]) == 7) {
		return true;
	}
	return false;
}

/* Returns true if there are parallel octaves or fifths between the chord voicing at
   the current index and the next. */
bool TwoPart::hasBadParallels(vector<Note>& melody, int index) {
	int currInterval = melody[index].getIntervalBetween(bass[index]);
	int prevInterval = melody[index - 1].getIntervalBetween(bass[index - 1]);
	return currInterval == prevInterval && (currInterval == 5 || currInterval == 1);
}

/* 
	Returns the truth value of the implication
									
      Note before index is a fourth ==> Note before index resolved down to the third.
*/
bool TwoPart::ifFourResToThree(vector<Note>& melody, int index) {
	if (melody[index - 1].getScaleNum() == 3) {
		if (melody[index].getScaleNum() != 2 ||
			melody[index - 1].getOctave() != melody[index].getOctave()) {
			return false;
		}
	}
	return true;
}

/* 
	Return the truth value of the implication

	  Note before index is the subtonic ==> Note before index resolved up to the tonic.
*/
bool TwoPart::ifSeventhResToTonic(vector<Note>& melody, int index) {
	if (melody[index - 1].getScaleNum() == 6 &&
		melody[index - 1].getAccidental() == (isMinor) ? 1 : 0) {
		if (melody[index].getScaleNum() != 0 ||
			melody[index - 1].getOctave() != melody[index].getOctave() - 1) {
			return false;
		}
	}
	return true;
}

/*
	Returns the truth value of the implication

	  Previous interval is a leap of at least a fourth
									    ==>
		                                 Resolved in a step in the opposite direction.
*/
bool TwoPart::ifLeapHasValidRes(vector<Note>& melody, int index) {
	if (melody[index - 1].getIntervalBetween(melody[index - 2]) >= 4 ||
		(melody[index - 1] == melody[index - 2] &&
			melody[index - 1].getOctave() != melody[index - 2].getOctave())) {
		Note step = melody[index - 1];
		if (melody[index - 1] <= melody[index - 2]) {
			step++;
		} else {
			step--;
		}
		if (melody[index].getScaleNum() != step.getScaleNum() ||
			melody[index].getOctave() != step.getOctave()) {
			return false;
		}
	}
	return true;
}

///////////////////////////////////////
// Segment GA method specializations //
///////////////////////////////////////

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

/*
int main() {
	TwoPart::setLength(12);
	TwoPart::setKey("bes");
	TwoPart::setTonality(false);
	TwoPart::setProgressionStartingChord(Chord("I", Note(0), Note(2), Note(4)));
	TwoPart::setEndingCadence({ Chord("V", Note(4), Note(6), Note(1)) });
	TwoPart::createBassLine();
	TwoPart result = TwoPart::generateHarmony();
	ofstream output;
	output.open("twopart.ly");
	result.outputToFile(output);
}
*/
