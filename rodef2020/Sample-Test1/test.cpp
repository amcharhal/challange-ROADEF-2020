#include "pch.h"
#include "../rodef2020/Instance.h"
#include "../rodef2020/Data.h"
#include <string>
#include<vector>
#include<fstream>
#include "../rodef2020/json.h"
#include "../rodef2020/Simulation.h"
#include<iostream>

using namespace std;


Data* CreateData()
{
	std::string fileName = "example1.json";
	Instance myinstance = Instance(fileName);
	Data *data = myinstance.getData();
	return data;
};

vector<string>  convertVector(const vector<int> &arr) {
	vector<string> res;
	for (int el : arr)
		res.push_back(to_string(el));
	return res;

};

class InstanceExemple1Test : public testing::Test
{
public:
	InstanceExemple1Test()
	{
		std::string fileName = "example1.json";
		Instance myinstance(fileName);
		Data *data = myinstance.getData();
		_dataExemple1 = new Data(*data);

	}

	Data* _dataExemple1;
};


class SerializeJson {
public:
	SerializeJson(Data &data) {
		std::vector<string> keys{ "Resources","Seasons","Interventions",  "Exclusions", "T",  "Scenarios_number", "Quantile", "Alpha", "ComputationTime" };
		for (vector<string>::iterator it = keys.begin(); it != keys.end(); it++)
			serializedJson[*it];

		// T
		serializedJson["T"] = data.getT();
		// Alpha
		serializedJson["Alpha"] = data.getAlpha();
		//Quantile
		serializedJson["Quantile"] = data.getQuantile();
		// m_computationTime
		serializedJson["ComputationTime"] = data.getComputationTime();
		// Excusions
		//map<int, string> exclusionMap = data.getExclusionMap();
		serializedJson["Exclusions"] = json({});
		for (int exclusionId = 0; exclusionId < data.getNbExclusions(); exclusionId++)
		{
			string a_I1 = data.getIntervention(data.getExclusion(exclusionId)->getI1())->getName();
			string a_I2 = data.getIntervention(data.getExclusion(exclusionId)->getI2())->getName();
			E_Season season = data.getExclusion(exclusionId)->getSeason();
			string s_season;
			if (season == FULL)
				s_season = "full";
			else if (season == WINTER)
				s_season = "winter";
			else if (season == IS)
				s_season = "is";
			else if (season = SUMMER)
				s_season = "summer";
			;
			serializedJson["Exclusions"][data.getExclusion(exclusionId)->getName()] = { a_I1, a_I2, s_season };
		}
		//season
		serializedJson["Seasons"]["winter"] = convertVector(*data.getSeasons()->getWinter());
		serializedJson["Seasons"]["is"] = convertVector(*data.getSeasons()->getIs());
		serializedJson["Seasons"]["summer"] = convertVector(*data.getSeasons()->getSummer());
		if (!data.getSeasons()->getFull()->empty())
			serializedJson["Seasons"]["full"] = convertVector (*data.getSeasons()->getFull());
		//Scenarios_number
		vector<int> Scenarios_number(data.getT());
		for (int i = 0; i < data.getT(); i++)
			Scenarios_number[i] = data.getScenariosNumber(i + 1);
		serializedJson["Scenarios_number"] = Scenarios_number;
		//resources 
		//map<int, string> resourcesMapId = data.getResourcesMapId();
		for (int resourcesId = 0; resourcesId < data.getNbResources(); resourcesId++)
		{
			serializedJson["Resources"][data.getResource(resourcesId)->getName()]["min"] = data.getResource(resourcesId)->getMin();
			serializedJson["Resources"][data.getResource(resourcesId)->getName()]["max"] = data.getResource(resourcesId)->getMax();
		}
		//interventons
		for (int interventionId = 0; interventionId < data.getNbInterventions(); interventionId++)
		{
			string interventionName = data.getIntervention(interventionId)->getName();
			serializedJson["Interventions"][interventionName]["tmax"] = to_string(data.getIntervention(interventionId)->getTMax());
			vector<int> delta(data.getT());
			for (int i = 0; i < data.getT(); i++)
				delta[i] = data.getIntervention(interventionId)->getDelta(i + 1);
			serializedJson["Interventions"][interventionName]["Delta"] = delta;
			for (int resource = 0; resource < data.getNbResources(); resource++)
			{
				for (int time_period = 0; time_period < data.getT(); time_period++)
				{
					for (int start_time = 0; start_time < data.getT(); start_time++)
					{
						if (data.getIntervention(interventionId)->getWorkload(resource, time_period + 1, start_time + 1) > 0)
							serializedJson["Interventions"][interventionName]["workload"][data.getResource(resource)->getName()][to_string(time_period + 1)][to_string(start_time + 1)] =
							data.getIntervention(interventionId)->getWorkload(resource, time_period + 1, start_time + 1);
					}
				}
			}
			for (int time_period = 0; time_period < data.getT(); time_period++)
			{
				for (int start_time = 0; start_time < data.getT(); start_time++)
				{
					vector<double> risk(data.getScenariosNumber(time_period + 1), 0);
					for (int scenario = 0; scenario < data.getScenariosNumber(time_period + 1); scenario++)
					{
						risk[scenario] = data.getIntervention(interventionId)->getRisk(time_period + 1, start_time + 1, scenario);
					}
					bool isEmpty = true;
					for (double el : risk)
					{
						if (el > 0)
							isEmpty = false;
						break;
					}
					if (!isEmpty)
					{
						serializedJson["Interventions"][interventionName]["risk"][to_string(time_period + 1)][to_string(start_time + 1)] =
							risk;

					}

				}
			}

		};
	};
	json *getSerializedJson() { return &serializedJson; };

private:
	json serializedJson;
};

/*
Test Plan

Test all attributs in Data.h
**********these test are separated
->m_alpha
->m_quantile
->m_numberOfresources
->m_T
->m_computationTtime
->nbOfExclusions
->nbbOfInterventions
->scenariosNumber

// dataparsing test contains tests for resources , iterventions , seasons, Exclusions

resources -> (Map(name, ID), min , max
interventions ->  Map(name, ID), delta, getWorkload, getRisks)
Exclusions -> I1, I2, Season
Season ->FULL, Summer, Winter, Is enum




*/


TEST_F(InstanceExemple1Test, Alpha) {
	double alpha = _dataExemple1->getAlpha();
	EXPECT_EQ(alpha, 0.5);
}

TEST_F(InstanceExemple1Test, quantile) {
	double m_quantile = _dataExemple1->getQuantile();
	EXPECT_EQ(m_quantile, 0.5);
}
TEST_F(InstanceExemple1Test, numberofResources) {
	int m_numberofResources = _dataExemple1->getNbResources();
	EXPECT_EQ(m_numberofResources, 1);
}
TEST_F(InstanceExemple1Test, T) {
	int T = _dataExemple1->getT();
	EXPECT_EQ(T, 3);
}
TEST_F(InstanceExemple1Test, computationTtime) {
	double computationTtime = _dataExemple1->getComputationTime();
	EXPECT_EQ(computationTtime, NULL);
}
TEST_F(InstanceExemple1Test, nbOfExclusions) {
	int nbOfExclusions = _dataExemple1->getNbExclusions();
	EXPECT_EQ(nbOfExclusions, 1);
}
TEST_F(InstanceExemple1Test, nbbOfInterventions) {
	int nbbOfInterventions = _dataExemple1->getNbInterventions();
	EXPECT_EQ(nbbOfInterventions, 3);
}
TEST_F(InstanceExemple1Test, scenariosNumber) {
	vector<int> ex_scenariosNumber = { 3,3,3 };
	for (int i = 0; i < _dataExemple1->getT(); i++)
		EXPECT_EQ(ex_scenariosNumber[i], _dataExemple1->getScenariosNumber(i + 1));
}
/*
TEST(TestExemple1, dataParsing) {
	string fileName = "example1.json";
	//create data under dataExemple1 name
	Instance myinstance(fileName);
	Data *dataExemple1 = myinstance.getData();
	//test resources 
	vector<string> resourceNames;
	resourceNames.push_back("c1");
	Resource* resource = dataExemple1->getResource(0);
	vector<double> ex_min = { 10, 0, 6 };
	vector<double> ex_max = { 49, 23, 15 };
	map<string, int> ex_reourcesMap;
	ex_reourcesMap["c1"] = 0;
	EXPECT_EQ(resource->getName(), resourceNames[0]);
	EXPECT_EQ(resource->getMin(), ex_min);
	EXPECT_EQ(resource->getMax(), ex_max);
	EXPECT_EQ(ex_reourcesMap, dataExemple1->getResourcesMap());
	//test Seasons 
	vector<int> ex_Full{ 1, 2, 3 };
	Seasons *season = dataExemple1->getSeasons();
	EXPECT_EQ(ex_Full, *(season->getFull()));
	EXPECT_EQ(0, season->getWinter()->size());
	EXPECT_EQ(0, season->getSummer()->size());
	EXPECT_EQ(0, season->getIs()->size());

	//test exclusion
	Exclusion* exclusion = dataExemple1->getExclusion(0);
	enum E_Season { WINTER, SUMMER, IS, FULL };
	EXPECT_EQ(1, exclusion->getI1());
	EXPECT_EQ(2, exclusion->getI2());
	EXPECT_EQ(E_Season::FULL, exclusion->getSeason());

	//test getInterventionsMap  
	map<string, int> ex_interventionsMap;
	ex_interventionsMap["I1"] = 0;
	ex_interventionsMap["I2"] = 1;
	ex_interventionsMap["I3"] = 2;
	EXPECT_EQ(ex_interventionsMap, dataExemple1->getInterventionsMap());

	// **************************************************Intervention1
	Intervention *intervention_1 = dataExemple1->getIntervention(0);
	EXPECT_EQ("I1", intervention_1->getName());
	EXPECT_EQ(1, intervention_1->getTMax());
	vector<int> ex_deltaI1 = { 3, 3, 2 };
	for (int i = 0; i < ex_deltaI1.size(); i++)
		EXPECT_EQ(ex_deltaI1[i], intervention_1->getDelta(i + 1));

	//wtest workload for intervention 1
	vector<vector<vector<double>>> ex_workload_1 = { { {31,0,0}, {0,0,0}, {8,0,0} } };
	for (int a_c = 0; a_c < dataExemple1->getNbResources(); a_c++)
	{
		for (int a_t = 0; a_t < dataExemple1->getT(); a_t++)
		{
			for (int a_s = 0; a_s < dataExemple1->getT(); a_s++)
				EXPECT_EQ(ex_workload_1.at(a_c).at(a_t).at(a_s), intervention_1->getWorkload(a_c, a_t + 1, a_s + 1));
		}
	}

	//test risk for intervention 1
	vector<vector<vector<double>>> ex_risk_1 = { { {7,4,8}, {0,0,0}, {0,0,0} },
												 { {1,10,10}, {0,0,0}, {0,0,0} },
												 { {1,4,4}, {0,0,0}, {0,0,0} } };

	vector<vector<vector<double>>> *vec = intervention_1->getRisks();


	for (int a_c = 0; a_c < dataExemple1->getT(); a_c++)
	{
		for (int a_t = 0; a_t < dataExemple1->getT(); a_t++)
		{
			for (int a_s = 0; a_s < dataExemple1->getScenariosNumber(a_c + 1); a_s++)
				EXPECT_EQ(ex_risk_1.at(a_c).at(a_t).at(a_s), intervention_1->getRisk(a_c + 1, a_t + 1, a_s));
		}
	}

	// ******************************************* test for intervention 2
	Intervention *intervention_2 = dataExemple1->getIntervention(1);
	EXPECT_EQ("I2", intervention_2->getName());
	EXPECT_EQ(3, intervention_2->getTMax());
	vector<int> ex_deltaI2 = { 1, 1, 1 };
	for (int i = 0; i < ex_deltaI2.size(); i++)
		EXPECT_EQ(ex_deltaI2[i], intervention_2->getDelta(i + 1));
	//wtest workload for intervention 2
	vector<vector<vector<double>>> ex_workload_2 = { { {14,0,0}, {0,14,0}, {0,0,14} } };
	for (int a_c = 0; a_c < dataExemple1->getNbResources(); a_c++)
	{
		for (int a_t = 0; a_t < dataExemple1->getT(); a_t++)
		{
			for (int a_s = 0; a_s < dataExemple1->getT(); a_s++)
				EXPECT_EQ(ex_workload_2.at(a_c).at(a_t).at(a_s), intervention_2->getWorkload(a_c, a_t + 1, a_s + 1));
		}
	}

	//test risk for intervention 2
	vector<vector<vector<double>>> ex_risk_2 = { { {5,4,5}, {0,0,0}, {0,0,0} },
												 { {0,0,0}, {5,4,5}, {0,0,0} },
												 { {0,0,0}, {0,0,0}, {5,4,5} } };

	for (int a_c = 0; a_c < dataExemple1->getT(); a_c++)
	{
		for (int a_t = 0; a_t < dataExemple1->getT(); a_t++)
		{
			for (int a_s = 0; a_s < dataExemple1->getT(); a_s++)
				EXPECT_EQ(ex_risk_2.at(a_c).at(a_t).at(a_s), intervention_2->getRisk(a_c + 1, a_t + 1, a_s));
		}
	}

	// *******************************************test for intervention 3
	Intervention *intervention_3 = dataExemple1->getIntervention(2);
	EXPECT_EQ("I3", intervention_3->getName());
	EXPECT_EQ(2, intervention_3->getTMax());
	vector<int> ex_deltaI3 = { 1, 1, 2 };
	for (int i = 0; i < ex_deltaI3.size(); i++)
		EXPECT_EQ(ex_deltaI3[i], intervention_3->getDelta(i + 1));

	//test workload for intervention 3
	vector<vector<vector<double>>> ex_workload_3 = { { {5,0,0}, {0,5,0}, {0,0,0} } };
	for (int a_c = 0; a_c < dataExemple1->getNbResources(); a_c++)
	{
		for (int a_t = 0; a_t < dataExemple1->getT(); a_t++)
		{
			for (int a_s = 0; a_s < dataExemple1->getT(); a_s++)
				EXPECT_EQ(ex_workload_3.at(a_c).at(a_t).at(a_s), intervention_3->getWorkload(a_c, a_t + 1, a_s + 1));
		}
	}

	//test risk for intervention 3
	vector<vector<vector<double>>> ex_risk_3 = { { {4,8,2}, {0,0,0}, {0,0,0} },
												 { {0,0,0}, {3,8,1}, {0,0,0} },
												 { {0,0,0}, {0,0,0}, {0,0,0} } };
	for (int a_c = 0; a_c < dataExemple1->getT(); a_c++)
	{
		for (int a_t = 0; a_t < dataExemple1->getT(); a_t++)
		{
			for (int a_s = 0; a_s < dataExemple1->getT(); a_s++)
				EXPECT_EQ(ex_risk_3.at(a_c).at(a_t).at(a_s), intervention_3->getRisk(a_c + 1, a_t + 1, a_s));
		}
	}
};

*/

TEST(TestA1, A1) {
	std::string file = "A_01.json";
	Instance myinstance(file);
	json *expectedjson = myinstance.getJsonMap()->getJson();
	Data *data = myinstance.getData();
	//serialized file
	SerializeJson serializeobject(*data);
	json *serializedJson = serializeobject.getSerializedJson();
	if (json::diff(*expectedjson, *serializedJson).size() == 0 && json::diff(*serializedJson, *expectedjson).size() == 0)
		ASSERT_EQ(0, 0);
	else {
		cout << json::diff(*expectedjson, *serializedJson) << endl;
		ASSERT_EQ(1, 0);
	}

}

TEST(TestA_02, A_02) {
	std::string file = "A_02.json";
	Instance myinstance(file);
	json *expectedjson = myinstance.getJsonMap()->getJson();
	Data *data = myinstance.getData();
	//serialized file
	SerializeJson serializeobject(*data);
	json *serializedJson = serializeobject.getSerializedJson();
	if (json::diff(*expectedjson, *serializedJson).size() == 0 && json::diff(*serializedJson, *expectedjson).size() == 0)
		ASSERT_EQ(0, 0);
	else {
		cout << json::diff(*expectedjson, *serializedJson) << endl;
		ASSERT_EQ(1, 0);
	}

}
TEST(TestA_03, A_03) {
	std::string file = "A_03.json";
	Instance myinstance(file);
	json *expectedjson = myinstance.getJsonMap()->getJson();
	Data *data = myinstance.getData();
	//serialized file
	SerializeJson serializeobject(*data);
	json *serializedJson = serializeobject.getSerializedJson();
	if (json::diff(*expectedjson, *serializedJson).size() == 0 && json::diff(*serializedJson, *expectedjson).size() == 0)
		ASSERT_EQ(0, 0);
	else {
		cout << json::diff(*expectedjson, *serializedJson) << endl;
		ASSERT_EQ(1, 0);
	}

}
TEST(TestA_05, A_05) {
	std::string file = "A_05.json";
	Instance myinstance(file);
	json *expectedjson = myinstance.getJsonMap()->getJson();
	Data *data = myinstance.getData();
	//serialized file
	SerializeJson serializeobject(*data);
	json *serializedJson = serializeobject.getSerializedJson();
	if (json::diff(*expectedjson, *serializedJson).size() == 0 && json::diff(*serializedJson, *expectedjson).size() == 0)
		EXPECT_EQ(0, 0);
	else {
		cout << json::diff(*expectedjson, *serializedJson) << endl;
		EXPECT_EQ(1, 0);
	}


}

TEST(TestA_06, A_06) {
	std::string file = "A_06.json";
	Instance myinstance(file);
	json *expectedjson = myinstance.getJsonMap()->getJson();
	Data *data = myinstance.getData();
	//serialized file 
	SerializeJson serializeobject(*data);
	json *serializedJson = serializeobject.getSerializedJson();
	if (json::diff(*expectedjson, *serializedJson).size() == 0 && json::diff(*serializedJson, *expectedjson).size() == 0)
		EXPECT_EQ(0, 0);
	else {
		cout << json::diff(*expectedjson, *serializedJson) << endl;
		EXPECT_EQ(1, 0);
	}

}

TEST(TestA_07, A_07) {
	std::string file = "A_07.json";
	Instance myinstance(file);
	json *expectedjson = myinstance.getJsonMap()->getJson();
	Data *data = myinstance.getData();
	//serialized file 
	SerializeJson serializeobject(*data);
	json *serializedJson = serializeobject.getSerializedJson();
	if (json::diff(*expectedjson, *serializedJson).size() == 0 && json::diff(*serializedJson, *expectedjson).size() == 0)
		EXPECT_EQ(0, 0);
	else {
		cout << json::diff(*expectedjson, *serializedJson) << endl;
		EXPECT_EQ(1, 0);
	}

}
TEST(TestA_08, A_08) {
	std::string file = "A_08.json";
	Instance myinstance(file);
	json *expectedjson = myinstance.getJsonMap()->getJson();
	Data *data = myinstance.getData();
	//serialized file 
	SerializeJson serializeobject(*data);
	json *serializedJson = serializeobject.getSerializedJson();
	if (json::diff(*expectedjson, *serializedJson).size() == 0 && json::diff(*serializedJson, *expectedjson).size() == 0)
		EXPECT_EQ(0, 0);
	else {
		cout << json::diff(*expectedjson, *serializedJson) << endl;
		EXPECT_EQ(1, 0);
	}

}
TEST(TestA_09, A_09) {
	std::string file = "A_09.json";
	Instance myinstance(file);
	json *expectedjson = myinstance.getJsonMap()->getJson();
	Data *data = myinstance.getData();
	//serialized file 
	SerializeJson serializeobject(*data);
	json *serializedJson = serializeobject.getSerializedJson();
	if (json::diff(*expectedjson, *serializedJson).size() == 0 && json::diff(*serializedJson, *expectedjson).size() == 0)
		EXPECT_EQ(0, 0);
	else {
		cout << json::diff(*expectedjson, *serializedJson) << endl;
		EXPECT_EQ(1, 0);
	}

}
TEST(TestA_10, A_10) {
	std::string file = "A_10.json";
	Instance myinstance(file);
	json *expectedjson = myinstance.getJsonMap()->getJson();
	Data *data = myinstance.getData();
	//serialized file 
	SerializeJson serializeobject(*data);
	json *serializedJson = serializeobject.getSerializedJson();
	if (json::diff(*expectedjson, *serializedJson).size() == 0 && json::diff(*serializedJson, *expectedjson).size() == 0)
		EXPECT_EQ(0, 0);
	else {
		cout << json::diff(*expectedjson, *serializedJson) << endl;
		EXPECT_EQ(1, 0);
	}
}
TEST(TestA_11, A_11) {
	std::string file = "A_11.json";
	Instance myinstance(file);
	json *expectedjson = myinstance.getJsonMap()->getJson();
	Data *data = myinstance.getData();
	//serialized file 
	SerializeJson serializeobject(*data);
	json *serializedJson = serializeobject.getSerializedJson();
	if (json::diff(*expectedjson, *serializedJson).size() == 0 && json::diff(*serializedJson, *expectedjson).size() == 0)
		EXPECT_EQ(0, 0);
	else {
		cout << json::diff(*expectedjson, *serializedJson) << endl;
		EXPECT_EQ(1, 0);
	}
}

TEST(TestA_12, A_12) {
	std::string file = "A_12.json";
	Instance myinstance(file);
	json *expectedjson = myinstance.getJsonMap()->getJson();
	Data *data = myinstance.getData();
	//serialized file 
	SerializeJson serializeobject(*data);
	json *serializedJson = serializeobject.getSerializedJson();
	if (json::diff(*expectedjson, *serializedJson).size() == 0 && json::diff(*serializedJson, *expectedjson).size() == 0)
		EXPECT_EQ(0, 0);
	else {
		cout << json::diff(*expectedjson, *serializedJson) << endl;
		EXPECT_EQ(1, 0);
	}
}
TEST(TestA_13, A_13) {
	std::string file = "A_13.json";
	Instance myinstance(file);
	json *expectedjson = myinstance.getJsonMap()->getJson();
	Data *data = myinstance.getData();
	//serialized file 
	SerializeJson serializeobject(*data);
	json *serializedJson = serializeobject.getSerializedJson();
	if (json::diff(*expectedjson, *serializedJson).size() == 0 && json::diff(*serializedJson, *expectedjson).size() == 0)
		EXPECT_EQ(0, 0);
	else {
		cout << json::diff(*expectedjson, *serializedJson) << endl;
		EXPECT_EQ(1, 0);
	}
}
TEST(TestA_14, A_14) {
	std::string file = "A_14.json";
	Instance myinstance(file);
	json *expectedjson = myinstance.getJsonMap()->getJson();
	Data *data = myinstance.getData();
	//serialized file 
	SerializeJson serializeobject(*data);
	json *serializedJson = serializeobject.getSerializedJson();
	if (json::diff(*expectedjson, *serializedJson).size() == 0 && json::diff(*serializedJson, *expectedjson).size() == 0)
		EXPECT_EQ(0, 0);
	else {
		cout << json::diff(*expectedjson, *serializedJson) << endl;
		EXPECT_EQ(1, 0);
	}
}

TEST(TestA_15, A_15) {
	std::string file = "A_15.json";
	Instance myinstance(file);
	json *expectedjson = myinstance.getJsonMap()->getJson();
	Data *data = myinstance.getData();
	//serialized file 
	SerializeJson serializeobject(*data);
	json *serializedJson = serializeobject.getSerializedJson();
	if (*expectedjson == *serializedJson)
		EXPECT_EQ(0, 0);
	else {
		cout << json::diff(*expectedjson, *serializedJson) << endl;
		EXPECT_EQ(1, 0);
	}
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}