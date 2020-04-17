#pragma once
#include<vector>
#include<list>
#include<string>
class Simulation

{
private:
	std::vector<std::vector<int>> ySolution;
	std::vector<std::vector<int>> xSolution;
	std::vector<std::vector<int>> deltaInstances;
public:
	Simulation(int numberOfInterventions, int T);
	std::vector<std::vector<int>> getYsolution() const;
	std::vector<std::vector<int>> getXsolution() const;
	std::vector<std::vector<int>> getDeltainstance() const;
	void setYsolution();
	void setXsolution();
	void setDeltainstance();
	bool checkConstraint(std::string name);
	int numberOfInterventions;
	int T;

};

