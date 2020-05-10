#include "SimulatedAnnealing.h"
#include <stdlib.h>
#include "Solution.h"
#include <math.h>
#include<iostream>

using namespace std;

#define MAX_E 900000000000;

SimulatedAnnealing::SimulatedAnnealing(Instance *ar_Instance):
	mp_Instance(ar_Instance)
{
	mp_Solution = new Solution(ar_Instance->getData());
}

std::vector<int>* SimulatedAnnealing::initialSolution()
{
	int nbInervention = mp_Instance->getData()->getNbInterventions();
	int T = mp_Instance->getData()->getT();
	vector<int> *initsolution = new vector<int>(nbInervention, 0);
	
	for (int interention = 0; interention < nbInervention; interention++)
	{
		int randedSolution = rand() % T+1;
		if (randedSolution == 0)
			randedSolution++;
		initsolution->at(interention) = randedSolution;
	}
	return initsolution;
}

std::vector<int>* SimulatedAnnealing::selectNeighbour(std::vector<int>* initSolution)
{
	
	int nbInervention = mp_Instance->getData()->getNbInterventions();
	int randInervention1 = rand() % nbInervention;
	int randInervention2 = rand() % nbInervention;
	if (randInervention1 == nbInervention)
		randInervention1--;
	if (randInervention2 == nbInervention)
		randInervention2--;
	/*if (randInervention2 == nbInervention)
		randInervention2--;*/
	vector<int> *newSolution = new vector<int>(*initSolution);
	int T = mp_Instance->getData()->getT();
	int randedSolution = rand() % T+1;
	if (randedSolution == 0)
		randedSolution++;
	int randedSolution2 = rand() % T + 1;
	if (randedSolution2 == 0)
		randedSolution2++;
	newSolution->at(randInervention1) = randedSolution;
	newSolution->at(randInervention2) = randedSolution2;
	return newSolution;
}

void SimulatedAnnealing::solveProblem()
{
	// intialization
	
	vector<int>*initSolution = initialSolution();
	double temp_iteration = intialTemperature;
	vector<int>* currentSolution(initSolution);
	mp_bestSolution = initSolution;
	mp_Solution->setStartingTime(mp_bestSolution);
	mp_Solution->setXit(mp_bestSolution);
	mp_Solution->computeObjectives();
	double bestEnergy = mp_Solution->getObjective();
	if (!mp_Solution->isFeasible())
		bestEnergy += MAX_E;
	// start itteration
	string keep;
	while (!mp_Solution->isFeasible() || temp_iteration>1) {
		vector<int>*newSolution(selectNeighbour(currentSolution));
		if (isBetter(newSolution, currentSolution))
		{
			currentSolution = newSolution;
			mp_bestSolution = currentSolution;
			
			//cout << "keep it by evaluation" << endl;

		}
		else
		{
			if (probably_accepted(newSolution, currentSolution, temp_iteration))
			{
				currentSolution = newSolution;
				//mp_bestSolution = currentSolution;
				
				//cout << "keep it by probability" << endl;
			}
			//cout << "no keep it " << endl;
		}
		temp_iteration *= 1 - 0.003;
		mp_Solution->setStartingTime(mp_bestSolution);
		mp_Solution->setXit(mp_bestSolution);
		mp_Solution->computeObjectives();

		bestEnergy = mp_Solution->getObjective();
		if (!mp_Solution->isFeasible())
			bestEnergy += MAX_E;

		//for(auto e : *newSolution)
			//cout << e <<" " ;
		cout << " the objective value " << bestEnergy << " "<<keep<<endl;
	}
	//for(auto e : *currentSolution)
		//	cout << e <<" " ;
	cout << " the objective value " << mp_Solution->getObjective()  << " " << mp_Solution->isFeasible() << endl;
}

bool SimulatedAnnealing::isBetter(std::vector<int>* newSolution, std::vector<int>* currentSolution)
{
	mp_Solution->setStartingTime(currentSolution);
	mp_Solution->setXit(currentSolution);
	mp_Solution->computeObjectives();

	double currentEnergy = mp_Solution->getObjective();
	if (!mp_Solution->isFeasible())
		currentEnergy += MAX_E;
	mp_Solution->setStartingTime(newSolution);
	mp_Solution->setXit(newSolution);
	mp_Solution->computeObjectives();
	
	double newEnergy = mp_Solution->getObjective();
	if (!mp_Solution->isFeasible())
		newEnergy += MAX_E;

	if (newEnergy  < currentEnergy)
		return true;
	else
		return false;
}

bool SimulatedAnnealing::probably_accepted(std::vector<int>* newSolution, std::vector<int>* currentSolution, double temp_iteration)
{
	
	double normalrandom = (double)rand() / (double)RAND_MAX; // random (0,1)

	mp_Solution->setStartingTime(currentSolution);
	mp_Solution->setXit(currentSolution);
	mp_Solution->computeObjectives();

	double currentEnergy = mp_Solution->getObjective();
	if (!mp_Solution->isFeasible())
		currentEnergy += MAX_E;
	mp_Solution->setStartingTime(newSolution);
	mp_Solution->setXit(newSolution);
	mp_Solution->computeObjectives();

	double newEnergy = mp_Solution->getObjective();
	if (!mp_Solution->isFeasible())
		newEnergy += MAX_E;
	if (exp((currentEnergy - newEnergy) / temp_iteration) > 0.1*normalrandom)
		return true;
	else
		return false;
}
