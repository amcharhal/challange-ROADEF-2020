#include <iostream>
#include <fstream>
#include <stdlib.h>
#include<ctime>
#include<algorithm>
#include "SimulatedAnnealing.h"

using namespace std;

#include "Simulation.h"
#include "json.h"
#include "JsonMap.h"
#include "Instance.h"
#include "Tools.h"

//// colex
//#include <ilcplex/ilocplex.h>
//#include <ilcplex/ilocplex.h>
//ILOSTLBEGIN

// json
using json = nlohmann::json;

int main()
{
	//cout << "Instance initialization starts" << endl;
	//for (int i = 1; i <= 15; i++)
	//{
	//	string num;
	//	if (i <= 9)
	//	{
	//		num = "0";
	//	}
	//	num += to_string(i);
	//	string file = A_SET_FOLDER + "A_" + num + ".json";
	//	cout << file << endl;
	//	Instance inst(file);
	//}
	//cout << "Instance initialization finished" << endl;

	//cout << "Instance initialization starts" << endl;
	//Instance inst("A_01.json");
	
	//cout << "Instance initialization finished" << endl << endl;
	//inst.solve();
	/*cout << "Is the solution feasible ? " << inst.getBestSolution().isFeasible() << endl << endl;
	inst.getBestSolution().saveSolution("exemple1.txt");*/
	/*
	inst.getBestSolution().computeObjectives();
	cout << "mean risk over time: " << inst.getBestSolution().getRisk_t() << endl;
	cout << "Objective 1 (mean cost): " << inst.getBestSolution().getObj1() << endl;
	cout << "Quantile (Q" << inst.getData()->getQuantile() << ") over time: " << inst.getBestSolution().getQ_tau_t() << endl;
	cout << "Objective 2 (expected excess (Q" << inst.getData()->getQuantile() << ")): " << inst.getBestSolution().getObj2() << endl;
	cout << "Total objective (alpha*mean_risk + (1-alpha)*expected_excess): " << inst.getBestSolution().getObjective() << endl;
	*/
	//system("Pause");
	/*
	json result; 
	json expected; 
	vector<double> invect = { 1,2,3.000000 };
	vector<int> vect2 = { 1,2,3 };
	result["name"] = { 1,0,2.0 };
	result["date"] = invect;
	expected["name"] = { 1,0,2};
	expected["date"] = vect2;
	result["name2"];
	expected["name2"];
	if (expected == result)
		cout << "object are equal" << endl;
	else
	{
		cout << "not equal" << endl;
	}*/
	/*
	srand(static_cast<unsigned int>(time(static_cast<time_t>(NULL))));
	
	for (int i= 0; i < 10; i++) {
		//inst.solve();
		//cout << "Is the solution feasible ? " << inst.getBestSolution().isFeasible() << endl << endl;
		//inst.getBestSolution().saveSolution("exemple1.txt");
		vector<int> *initsolution = algorithm.initialSolution();

		inst.getBestSolution().setStartingTime(initsolution);
		inst.getBestSolution().setXit(initsolution);
		inst.getBestSolution().computeObjectives();
		//cout << "mean risk over time: " << inst.getBestSolution().getRisk_t() << endl;
		//cout << "Objective 1 (mean cost): " << inst.getBestSolution().getObj1() << endl;
		//cout << "Quantile (Q" << inst.getData()->getQuantile() << ") over time: " << inst.getBestSolution().getQ_tau_t() << endl;
		//cout << "Objective 2 (expected excess (Q" << inst.getData()->getQuantile() << ")): " << inst.getBestSolution().getObj2() << endl;
		cout << inst.getBestSolution().getObjective() << endl;
		}
	*/
	cout << "start read file" << endl;
	Instance inst("A_07.json");
	cout << "reading file is done" << endl;
	SimulatedAnnealing algorithm(&inst);

	srand(static_cast<unsigned int>(time(static_cast<time_t>(NULL))));
	algorithm.setInitialTemperature(100000);
	Solution mysol(inst.getData());
	/*for (int i = 0; i < 200; i++)
	{
		
		vector<int> *initsolution = algorithm.initialSolution();
		vector<int> *newSolution = algorithm.selectNeighbour(initsolution);
		for (auto e : *newSolution)
			cout << e << " ";
		cout << endl;
	}*/
	/*for(int i=0; i<200;i++)
	{ 
		
		int nbInervention = inst.getData()->getNbInterventions();
		int T = inst.getData()->getT();
		
		vector<int> *initsolution = algorithm.initialSolution();

		mysol.setStartingTime(initsolution);
		mysol.setXit(initsolution);
		mysol.computeObjectives();
		cout << mysol.getObjective() << " ";
		cout << endl;
			
	}*/
	cout << "start local search" << endl;
	algorithm.solveProblem();
	mysol.setStartingTime(algorithm.getBestSolution());
	mysol.saveSolution("A_10.txt"); 
	return 0;
}


/*srand(static_cast<unsigned int>(time(static_cast<time_t>(NULL))));
	Simulation senario_1(7, 10);
	Simulation senario_2(7, 10);
	vector<vector<int>> ysolution1 = senario_1.getDeltainstance();
	vector<vector<int>> ysolution2 = senario_2.getXsolution();
	*/
	/*
	bool check = true;
	for (int i = 100; i < 4000; i = i + 100)
	{
		Simulation senario_t(i, 365);
		if (!senario_1.checkConstraint("constraint_2") || !senario_1.checkConstraint("constraint_1") || !senario_1.checkConstraint("constraint_4"))
		{
			cout << "constraint are voilted " <<endl;
			check = false;
			break;
		}
		if (check)
			cout << "constraints are satisfied" << endl;
	}


	for (vector<int>::iterator it = ysolution1.at(4).begin(); it != ysolution1.at(4).end(); it++)
		cout << *it <<" "<< endl;
	int res = 0;
	int n = 4;
	int res = 0;
	for (int i = 1; i <= n; i++)
	{
		cout << "hello world" << endl;
		i = 0;
	}
	cout << "your results is " << res << endl;
	string filename("exemple1.json");
	JsonMap exemple1(filename);
	json *exemple1Map = new json;
	exemple1Map = exemple1.getJsonMap();
	cout << exemple1Map->at("Seasons") << endl;*/