#include "BassLine.h"

Note& BassLine::operator[](int index) {
	return line[index];
}

BassLine BassLine::generate(int length) {
	Progression::setProgressionLength(length);
	GA<Progression> generator = GA<Progression>(300, 40, 40, 0.5, 0.05);
	Progression p = generator.runSimulation();
	BassLine b = BassLine();
	for (int i = 0; i < length; i++) {
		b.line[i] = p[i].getBottom();
	}
	return b;
}

BassLine::BassLine() {

}