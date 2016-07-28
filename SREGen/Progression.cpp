#include "Progression.h"
#include "GA.h"

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
Progression GA<Progression>::modifySolution(Progression& bestFit) {

}