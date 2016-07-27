#ifndef GA_H
#define GA_H

#include <vector>
#include <utility>

using namespace std;

template<typename Organism>
class GA {
public:
	GA(); // Maybe include parameters?
	Organism runSimulation();
private:
	int popCount;
	int childCount;
	int tournamentSize;
	double crossoverRate;
	double mutationRate;
	vector<Organism> organisms;

	Organism holdTournamentSelection();
	
	int fitnessFunction();
	pair<Organism, Organism> crossOver(Organism parent1, Organism parent2);
	void mutate(Organism& child);
	bool canTerminate();
	Organism modifySolution(Organism bestFit);
};

#endif