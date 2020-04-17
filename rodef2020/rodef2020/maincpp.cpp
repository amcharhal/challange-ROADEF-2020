#include <iostream>
#include "json.h"
#include <fstream>
#include <stdlib.h>
#include "Simulation.h"
#include<ctime>


using namespace std;
using json = nlohmann::json;
int main() {
	srand(static_cast<unsigned int>(time(static_cast<time_t>(NULL))));
	Simulation senario_1(7, 10);
	Simulation senario_2(7, 10);
	vector<vector<int>> ysolution1 = senario_1.getXsolution();
	vector<vector<int>> ysolution2 = senario_2.getXsolution();

	bool check = true;
	for (int i = 100; i < 4000; i = i + 100)
	{
		Simulation senario_t(i, 365);
		if (!senario_1.checkConstraint("constraint_3") || !senario_1.checkConstraint("constraint_1") || !senario_1.checkConstraint("constraint_4"))
		{
			cout << "constraint are voilted " << endl;
			check = false;
			break;
		}
		if (check)
			cout << "constraints are satisfie" << endl;
	}


	return 0;
}