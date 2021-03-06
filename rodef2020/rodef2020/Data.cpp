#include <iostream>
using namespace std;
#include "Data.h"
#include<string>

Data::Data(JsonMap& ar_json)
{
	/* Create and initialize the |C| Resource objects dynamically and push_back them in the resources
	 * vector (no object copy is made) and add them to the resources map. */

	 // Create the Seasons object

	 /* Create and initialize the |I| Intervention objects dynamically and push_back them in the vector
	  * (no object copy is made) and add them to the interventions map. Use the resources map. */

	  /* Create and initialize the Exclusion objects dynamically and push_back them in the vector (no
	   * object copy is made). Use the interventions map. */

	   // initialize the non object attributes: m_T, m_scenariosNumber, m_quantile, m_alpha, and m_computationTime

	   //itertare over the keys of the map ar_json.getJson() [alpha, quantille , T, ...]

	   //initialize m_T
	m_T = ar_json.getJson()->at("T");
	//initialize Scenarios number
	if (ar_json.getJson()->contains("Scenarios_number"))
		m_scenariosNumber = ar_json.getJson()->at("Scenarios_number").get<std::vector<int>>();
	// 	initialize Resources (resourcesMap, m_resources)
	if (ar_json.getJson()->contains("Resources"))
	{
		int resourceID = 0;
		for (json::iterator it_resources = ar_json.getJson()->at("Resources").begin(); it_resources != ar_json.getJson()->at("Resources").end(); ++it_resources)
		{
			string ar_name = it_resources.key();
			vector<double> ar_min;
			vector<double> ar_max;
			m_resourcesMap[ar_name] = resourceID;
			if (it_resources.value().contains("min"))
				ar_min = it_resources.value()["min"].get<vector<double>>();
			if (it_resources.value().contains("max"))
				ar_max = it_resources.value()["max"].get<vector<double>>();
			Resource *resource = new Resource(ar_name, ar_min, ar_max);
			resourceID++;
			m_resources.push_back(resource);
		}
	}

	json::iterator itExc;	// exclusions have to be treated after the interventions

	// iterate over the jsonMap 
	for (json::iterator it = ar_json.getJson()->begin(); it != ar_json.getJson()->end(); ++it)
	{
		// alpha
		if (it.key() == "Alpha")
			m_alpha = it.value().get<double>();
		//Quantile
		else if (it.key() == "Quantile")
			m_quantile = it.value().get<double>();
		// computationTime
		else if (it.key() == "ComputationTime")
			m_computationTime = it.value().get<double>();
		//seasons 
		else if (it.key() == "Seasons")
		{
			std::vector<int> ar_winter;
			std::vector<int> ar_summer;
			std::vector<int> ar_is;
			std::vector<int> ar_full;

			if (it.value().contains("full"))
			{
				vector<string> vect = it.value()["full"].get<std::vector<string>>();
				for (string str : vect)
				{
					if (str[0] == '"')
					{
						str.erase(str.begin());
						str.erase(str.end());
					}
					ar_full.push_back(stoi(str));
				}
			}
			if (it.value().contains("is"))
			{
				vector<string> vect = it.value()["is"].get<std::vector<string>>();
				for (string str : vect)
				{
					if (str[0] == '"')
					{
						str.erase(str.begin());
						str.erase(str.end());
					}
					ar_is.push_back(stoi(str));
				}
			}
			if (it.value().contains("summer"))
			{
				vector<string> vect = it.value()["summer"].get<std::vector<string>>();
				for (string str : vect)
				{
					if (str[0] == '"')
					{
						str.erase(str.begin());
						str.erase(str.end());
					}
					ar_summer.push_back(stoi(str));
				}
			}
			if (it.value().contains("winter"))
			{
				vector<string> vect = it.value()["winter"].get<std::vector<string>>();
				for (string str : vect)
				{
					if (str[0] == '"')
					{
						str.erase(str.begin());
						str.erase(str.end());
					}
					ar_winter.push_back(stoi(str));
				}
			}
			m_seasons = Seasons(ar_winter, ar_summer, ar_is, ar_full);
			/*delete ar_full;
			delete ar_is;
			delete ar_summer;
			delete ar_winter;*/
		}

		// Exclusions 
		else if (it.key() == "Exclusions")
		{
			itExc = it;	// memrize the exclusions to treat them in the end, after the intervention maps are teated
		}

		// interventions InterventionsMap and m_interventions 
		else if (it.key() == "Interventions")
		{
			int intervention_Id = 0;
			for (json::iterator ip = it.value().begin(); ip != it.value().end(); ++ip)
			{
				string ar_name = ip.key();

				m_interventionsMap[ar_name] = intervention_Id;
				//vector<double> emptyArray = new (m_T, 0);
				//vector<vector<double>> time_periodArray(m_T, emptyArray);
				std::vector<std::vector<std::vector<double>>> *ap_workloads = new std::vector<std::vector<std::vector<double>>>(getNbResources());
				std::vector<std::vector<std::vector<double>>> *ap_risks = new std::vector<std::vector<std::vector<double>>>(m_T);
				int a_tmax;
				vector<int> ar_delta;
				for (json::iterator ip_t = ip.value().begin(); ip_t != ip.value().end(); ip_t++)
				{
					if (ip_t.key() == "tmax")
					{
						string val = ip_t.value();
						if (val[0] == '"')
						{
							val.erase(val.begin());
							val.erase(val.end());
						}
						a_tmax = stoi(val);
					}
					else if (ip_t.key() == "Delta")
						ar_delta = ip_t.value().get<vector<int>>();
					else if (ip_t.key() == "workload")
					{
						for (json::iterator ic = ip_t.value().begin(); ic != ip_t.value().end(); ++ic)
						{
							int realResourceID = m_resourcesMap[ic.key()];
							//vector<double> emptyArray(m_T, 0);
							vector<vector<double>> *time_periodArray = new vector<vector<double>>(m_T);
							for (json::iterator it_p = ic.value().begin(); it_p != ic.value().end(); ++it_p)
							{
								int time_period = stoi(it_p.key());
								vector<double> *workload_array = new vector<double>(m_T);
								for (json::iterator it_s = it_p.value().begin(); it_s != it_p.value().end(); ++it_s)
								{
									int start_time = stoi(it_s.key());
									double workload = it_s.value().get<double>();
									(*workload_array)[start_time - 1] = workload;
								}
								(*time_periodArray)[time_period - 1] = *workload_array;
								delete workload_array;
							}
							ap_workloads->at(realResourceID) = *time_periodArray;
							delete time_periodArray;
						}
					}
					else if (ip_t.key() == "risk")
					{
						for (json::iterator ip_tp = ip_t.value().begin(); ip_tp != ip_t.value().end(); ++ip_tp)
						{
							int time_period = stoi(ip_tp.key());
							int scenariosNumberAt_t = m_scenariosNumber[time_period - 1];
							//vector<double> emptyArray(scenariosNumberAt_t, 0);
							vector<vector<double>> *time_periodArray = new vector<vector<double>>(m_T);
							for (json::iterator ip_ts = ip_tp.value().begin(); ip_ts != ip_tp.value().end(); ++ip_ts)
							{
								vector<double> risk_array(scenariosNumberAt_t);
								int start_time = stoi(ip_ts.key());
								risk_array = ip_ts.value().get<vector<double>>();
								(*time_periodArray)[start_time - 1] = risk_array;
							}
							ap_risks->at(time_period - 1) = *time_periodArray;
							delete time_periodArray;
						}
					}
				}
				Intervention *ip_intervention = new Intervention(ar_name, a_tmax, ar_delta, *ap_workloads, *ap_risks);
				m_interventions.push_back(ip_intervention);
				intervention_Id++;
			}
		}
	}

	// Treat exclusions after all other data (particulary after the interventions)
	int exclusionId = 0;
	for (json::iterator ip = itExc.value().begin(); ip != itExc.value().end(); ++ip)
	{
		string name = ip.key();
		//int i1 = stoi(ip.value()[0].get<string>().substr(1));
		int i1 = m_interventionsMap[ip.value()[0].get<string>()];
		//int i2 = stoi(ip.value()[1].get<string>().substr(1));
		int i2 = m_interventionsMap[ip.value()[1].get<string>()];
		string s_season = ip.value()[2].get<string>();
		E_Season m_season;

		if (s_season == "full")
			m_season = FULL;
		else if (s_season == "winter")
			m_season = WINTER;
		else if (s_season == "is")
			m_season = IS;
		else if (s_season == "summer")
			m_season = SUMMER;

		Exclusion *ip_exclusion = new Exclusion(name, i1, i2, m_season);
		m_exclusions.push_back(ip_exclusion);
		exclusionId++;
	}
}


Data::~Data()
{
	for (unsigned int i = 0; i < m_resources.size(); i++)
	{
		delete m_resources[i];
	}

	for (unsigned int i = 0; i < m_interventions.size(); i++)
	{
		delete m_interventions[i];
	}

	for (unsigned int i = 0; i < m_exclusions.size(); i++)
	{
		delete m_exclusions[i];
	}
}