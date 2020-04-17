#include "Simulation.h"
#include <stdlib.h>
using namespace std;

Simulation::Simulation(int numberofI, int T) {
	numberOfInterventions = numberofI;
	this->T = T;
	setYsolution();
	setDeltainstance();
	setXsolution();
	
}

std::vector<std::vector<int>> Simulation::getDeltainstance() const {
	return deltaInstances;
}

std::vector<std::vector<int>> Simulation::getXsolution() const {
	return xSolution;
}

std::vector<std::vector<int>> Simulation::getYsolution() const {
	return ySolution;
}

void Simulation::setYsolution() {
	for(int i=0; i<numberOfInterventions ;i++)
	{
		vector<int> *lignetime = new vector<int>(T,0);
		int randedSolution = rand() % T;
		lignetime->at(randedSolution) = 1;
		ySolution.push_back(*lignetime);
		delete lignetime;
	}
	
}

void Simulation::setDeltainstance() {

	for (int i = 0; i < numberOfInterventions; i++)
	{
		vector<int>  *tempDelta = new vector<int>(T);
		for (int t = 0; t < T; t++)
		{
			int delta_it;
			int y_it= ySolution.at(i).at(t);
			if (y_it == 1)
				delta_it = rand() % (T-t)+1;
			else
				delta_it = rand() % 18+1;

			tempDelta->at(t) = delta_it;
		}
		deltaInstances.push_back(*tempDelta);
		delete tempDelta;
	}

}

void Simulation::setXsolution() {

	for (int i = 0; i < numberOfInterventions; i++)
	{	
		int y_it;
		int wantedInex;
		vector<int> tempX(T,0);
		for (int t = 0; t < T; t++)
		{
			y_it = ySolution.at(i).at(t);
			if (y_it == 1)
				wantedInex = t;
		}
		int delta_it = deltaInstances.at(i).at(wantedInex);
		for (int k = 0; k < delta_it; k++)
		{
			tempX[k + wantedInex] = 1;
		}
		xSolution.push_back(tempX);
	}
}

bool Simulation::checkConstraint(string name)
{
	if (name == "constraint_3")
	{
		
		for (auto intervention : ySolution)
		{
			int sum = 0;
			for (int value : intervention)
				sum += value;
			if (sum != 1)
				return false;
		}
		
	}
	else if (name == "constraint_4")
	{
		for (int i = 0; i < numberOfInterventions; i++)
		{
			int sumy_it = 0;
			for (int t = 0; t < T; t++)
			{
				sumy_it += ySolution.at(i).at(t)*(t + deltaInstances.at(i).at(t));
			}
			if (sumy_it > T + 1)
				return false;
		}
	}
	else if ("constraint_1")
	{
		for (int i = 0; i < numberOfInterventions; i++)
		{
			int leftSum = 0;
			for (int t = 0; t < T; t++)
			{
				leftSum = ySolution.at(i).at(t)*deltaInstances.at(i).at(t);
				int rightSum = 0;
				if (deltaInstances.at(i).at(t) + t > T)
				{
					for (int t_p = t; t_p < T; t_p++)
						rightSum += xSolution.at(i).at(t_p);
				}
				else {
					for (int t_p = 0; t_p < deltaInstances.at(i).at(t); t_p++)
						rightSum += xSolution.at(i).at(t_p + t);
				}
				if (leftSum > rightSum)
				{
					return false;
					break;
				}
			}
		}
	}
	return true;
}