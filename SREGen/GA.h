#ifndef GA_H
#define GA_H

#include <vector>
#include <utility>
#include <algorithm>
#include <random>
#include <iostream>

using namespace std;

template<typename Organism>
// Must have implemented:
//		1. static Organism generateRandom();				- generates a random instance of an organism
//      2. static void assignFitness(Organism& organism);	- assigns a fitness score to an individual
//                                                            and keeps an internal record
//      3. bool operator<(Organism other) const;			- returns whether *this is less fit 
//													          than the other organism.
class GA {
public:
	/* Starts a GA experiment by initializing POPCOUNT individuals and assigning a fitness score
	   to each one. */
	GA(int popCount, int childCount, int tournamentSize, double crossoverRate, double mutationRate)
		: popCount(popCount), childCount(childCount), tournamentSize(tournamentSize),
		  crossoverRate(crossoverRate), mutationRate(mutationRate), 
		  selectorRNG(0, popCount - 1), rateRNG(0, 1)
	{
		default_random_engine generator =
			default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

		organisms.resize(popCount);

		#pragma omp parallel for
		for (int i = 0; i < popCount; i++) {
			organisms[i] = Organism::generateRandom();
			Organism::assignFitness(organisms[i]);
		}

		sort(organisms.rbegin(), organisms.rend());
	};
	
	/* Runs the GA experiment and returns the best fit individual that satisfies termination
	   requirements. */
	Organism runSimulation() {
		int gen = 0;
		double mRate = mutationRate;
		while (!canTerminate()) {
			cout << gen++ << " ";
			vector<Organism> children;
			children.resize(childCount);

			#pragma omp parallel for
			for (int i = 0; i < childCount / 2; i++) {
				pair<Organism, Organism> offspring
					= crossover(holdTournamentSelection(), holdTournamentSelection());
				mutate(offspring.first);
				mutate(offspring.second);

				Organism::assignFitness(offspring.first);
				Organism::assignFitness(offspring.second);

				children[2 * i] = offspring.first;
				children[2 * i + 1] = offspring.second;
			}
			
			sort(children.rbegin(), children.rend());
			addOffspring(children);

			if (organisms[0].necessaryFitness == -1) {
				Organism::assignFitness(organisms[0]);
			}

			if (!(organisms[popCount / 2] < organisms[1])) {
				cout << "| ";
				#pragma omp parallel for
				for (int i = 1; i < popCount; i++) {
					organisms[i] = Organism::generateRandom();
					Organism::assignFitness(organisms[i]);
				}

				sort(organisms.rbegin(), organisms.rend());
			}
		}
		modifySolution(organisms[0]);
		cout << "Experiment terminated after " << gen << " generations(s)." << endl;
		return organisms[0];
	};

private:
	int popCount;
	int childCount;
	int tournamentSize;
	double crossoverRate;
	double mutationRate;
	uniform_int_distribution<> selectorRNG;
	uniform_real_distribution<> rateRNG;
	default_random_engine generator;
	vector<Organism> organisms;

	/* Holds tournament selection by choosing TOURNAMENTSIZE individuals and returning the
	   individual with the highest fitness score. */
	Organism holdTournamentSelection() {
		Organism bestIndividual = organisms[selectorRNG(generator)];
		for (int i = 1; i < tournamentSize; i++) {
			Organism testIndividual = organisms[selectorRNG(generator)];
			if (bestIndividual < testIndividual) {
				bestIndividual = testIndividual;
			}
		}
		return bestIndividual;
	};

	/* Combines a sorted list of offspring with a sorted list of the overall population, creating
	   a new sorted list of the overall population, discarding the least fit CHILDCOUNT
	   invidivuals. */
	void addOffspring(vector<Organism>& children) {
		vector<Organism> newPopulation = vector<Organism>();
		newPopulation.resize(popCount);

		int popCounter = 0; int childCounter = 0;
		for (int i = 0; i < popCount; i++) {
			if (childCounter < childCount) {
				if (children[childCounter] < organisms[popCounter]) {
					newPopulation[popCounter + childCounter] = organisms[popCounter];
					popCounter++;
				} else {
					newPopulation[popCounter + childCounter] = children[childCounter];
					childCounter++;
				}
			} else {
				newPopulation[popCounter + childCounter] = organisms[popCounter];
				popCounter++;
			}
		}
		organisms = newPopulation;
	};
	
	// All functions below must be specified for each kind of organism.
	pair<Organism, Organism> crossover(Organism parent1, Organism parent2);
	void mutate(Organism& child);
	bool canTerminate();
	Organism& modifySolution(Organism& bestFit);
};

#endif