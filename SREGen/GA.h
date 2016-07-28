#ifndef GA_H
#define GA_H

#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

template<typename Organism>
// Must have implemented:
//		1. static Organism generateRandom();				- generates a random instance of an organism
//      2. static void assignFitness(Organism& organism);	- assigns a fitness score to an individual
//                                                            and keeps an internal record
//      3. bool operator>(Organism other) const;			- returns whether *this is less fit 
//													          than the other organism.
class GA {
public:
	/* Starts a GA experiment by initializing POPCOUNT individuals and assigning a fitness score
	   to each one. */
	GA(int popCount, int childCount, int tournamentSize, double crossoverRate, double mutationRate)
		: popCount(popCount), childCount(childCount), tournamentSize(tournamentSize),
		crossoverRate(crossoverRate), mutationRate(mutationRate) 
	{
		organisms.resize(popCount);

		#pragma omp parallel for
		for (int i = 0; i < popCount; i++) {
			organisms[i] = Organism::generateRandom();
			organisms[i].assignFitness();
		}

		sort(organisms.rbegin(), organisms.rend());
	};
	
	/* Starts the GA experiment and returns the best fit individual that satisfies termination
	   requirements. */
	Organism runSimulation() {
		while (!canTerminate()) {
			vector<Organism> children;
			children.resize(childCount);

			#pragma omp parallel for
			for (int i = 0; i < childCount / 2; i++) {
				pair<Organism, Organism> offspring
					= crossOver(holdTournamentSelection(), holdTournamentSelection());
				mutate(offspring.first);
				mutate(offspring.second);

				offspring.first.assignFitness();
				offspring.second.assignFitness();

				children[2 * i] = offspring.first;
				children[2 * i + 1] = offspring.second;
			}
			
			sort(children.rbegin(), children.rend());
			addOffspring();
		}
		modifySolution(organisms[0]);
		return organisms[0];
	};
private:
	int popCount;
	int childCount;
	int tournamentSize;
	double crossoverRate;
	double mutationRate;
	vector<Organism> organisms;

	Organism holdTournamentSelection();
	void addOffspring();
	
	// All functions below must be specified for each kind of organism.
	pair<Organism, Organism> crossOver(Organism parent1, Organism parent2);
	void mutate(Organism& child);
	bool canTerminate();
	Organism modifySolution(Organism& bestFit);
};

#endif