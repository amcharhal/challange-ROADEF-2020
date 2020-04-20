#pragma once
#include<vector>
#include<list>
#include<string>
#include<tuple>
class Simulation

{
private:
	std::vector<std::vector<int>> ySolution;
	std::vector<std::vector<int>> xSolution;
	std::vector<std::vector<int>> deltaInstances;
	std::vector<std::tuple<int , int , int >> exclusionSet;
public:
	Simulation(int numberOfInterventions, int T);
	std::vector<std::vector<int>> getYsolution() const;
	std::vector<std::vector<int>> getXsolution() const;
	std::vector<std::vector<int>> getDeltainstance() const;
	std::vector<std::tuple<int, int, int >> getExclusionSet() const;
	void setYsolution();
	void setXsolution();
	void setDeltainstance();
	void setExclusionSet();
	bool checkConstraint(std::string name);
	int numberOfInterventions;
	int T;

};

