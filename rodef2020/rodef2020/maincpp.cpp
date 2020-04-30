#include <iostream>
#include <fstream>
#include <stdlib.h>
#include<ctime>
#include<algorithm>

#include "Simulation.h"
#include "json.h"
#include "JsonMap.h"
#include "Instance.h"

using namespace std;
using json = nlohmann::json;
int main() {
	srand(static_cast<unsigned int>(time(static_cast<time_t>(NULL))));
	Simulation senario_1(500, 356);
	//Simulation senario_2(7, 10);
	//vector<vector<int>> ysolution1 = senario_1.getDeltainstance();
	//vector<vector<int>> ysolution2 = senario_2.getXsolution();
	
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

	/*Instance inst("exemple1.json");
	Data *mydata= inst.getData();
	cout << mydata->getNbResources()<<endl;
	cout << "hello world" << endl;
	//inst.solve();
	*/
	
	return 0;
}