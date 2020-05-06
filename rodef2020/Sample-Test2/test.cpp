#include "pch.h"
#include "../rodef2020/Instance.h"
#include "../rodef2020/Data.h"
#include <string>
#include<vector>
#include<fstream>
#include "../rodef2020/json.h"
#include "../rodef2020/Simulation.h"
#include<iostream>


/*Data* CreateData()
{
	std::string fileName = "exemple1.json";
	Instance myinstance = Instance(fileName);
	Data *data = myinstance.getData();
	return data;
}*/

class InstanceExemple1Test : public testing::Test
{
public:
	InstanceExemple1Test()
	{
		std::string fileName = "exemple1.json";
		Instance myinstance(fileName);
		Data *data = myinstance.getData();
		_dataExemple1 = new Data(*data);

	}

	Data* _dataExemple1;
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
using namespace std;

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
	string fileName = "exemple1.json";
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
	EXPECT_EQ(2, exclusion->getI1());
	EXPECT_EQ(3, exclusion->getI2());
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
		EXPECT_EQ(ex_deltaI1[i], intervention_1->getDelta_it(i + 1));

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

	for (int a_c = 0; a_c < dataExemple1->getT(); a_c++)
	{
		for (int a_t = 0; a_t < dataExemple1->getT(); a_t++)
		{
			for (int a_s = 0; a_s < dataExemple1->getT(); a_s++)
				EXPECT_EQ(ex_risk_1.at(a_c).at(a_t).at(a_s), intervention_1->getRisk(a_c, a_t + 1, a_s + 1));
		}
	}

	// ******************************************* test for intervention 2
	Intervention *intervention_2 = dataExemple1->getIntervention(1);
	EXPECT_EQ("I2", intervention_2->getName());
	EXPECT_EQ(3, intervention_2->getTMax());
	vector<int> ex_deltaI2 = { 1, 1, 1 };
	for (int i = 0; i < ex_deltaI2.size(); i++)
		EXPECT_EQ(ex_deltaI2[i], intervention_2->getDelta_it(i + 1));
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
				EXPECT_EQ(ex_risk_2.at(a_c).at(a_t).at(a_s), intervention_2->getRisk(a_c, a_t + 1, a_s + 1));
		}
	}

	// *******************************************test for intervention 3
	Intervention *intervention_3 = dataExemple1->getIntervention(2);
	EXPECT_EQ("I3", intervention_3->getName());
	EXPECT_EQ(2, intervention_3->getTMax());
	vector<int> ex_deltaI3 = { 1, 1, 2 };
	for (int i = 0; i < ex_deltaI3.size(); i++)
		EXPECT_EQ(ex_deltaI3[i], intervention_3->getDelta_it(i + 1));

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
				EXPECT_EQ(ex_risk_3.at(a_c).at(a_t).at(a_s), intervention_3->getRisk(a_c, a_t + 1, a_s + 1));
		}
	}
}*/

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}