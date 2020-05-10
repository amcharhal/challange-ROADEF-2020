#pragma once
#include "Instance.h"
#include<vector>
#include<memory>
#include "Solution.h"
#include<ctime>

class SimulatedAnnealing
{
public:

	SimulatedAnnealing(Instance *ar_Instance);
	std::vector<int> * initialSolution(); //generate a random solution
	void setInitialTemperature(double initT) { intialTemperature = initT; }; // set Temperature
	void setBigM(int M) { bigM = M; }; // SEt a big value as a penality for energyfunction
	std::vector<int> *selectNeighbour(std::vector<int> *initSolution); // select a neighbour
	std::vector<int> *getBestSolution() { return mp_bestSolution; };
	void solveProblem(); // this method solve the probleme and initialize mp_solution;
	bool isBetter(std::vector<int> *newSolution, std::vector<int> *currentSolution);
	bool probably_accepted(std::vector<int>* newSolution, std::vector<int>* currentSolution, double temp_iteration);

	
private:
	Instance *mp_Instance;
	Solution *mp_Solution;
	std::vector<int> *mp_bestSolution;
	double intialTemperature;
	int bigM;
};

/*
First we need set the initial temperature and create a random initial solution.
Then we begin looping until our stop condition is met. Usually either the system has sufficiently cooled, or a good-enough solution has been found.
From here we select a neighbour by making a small change to our current solution.
We then decide whether to move to that neighbour solution.
Finally, we decrease the temperature and continue looping

start with T init temperature and choose coolingRate as too smale double around (0.000)
and then at the end for each iteration decrease the temperature T *= 1-coolingRate;

select a neighbour solution by swaping two telements
if energy of the new solution is better then keep it otherwise accept with some probabilty acceptance creteria 
the probabiity well be defined like this Math.exp((energy - newEnergy) / temperature)> Math.random()
double r = (double)rand() / (double)RAND_MAX;
*/