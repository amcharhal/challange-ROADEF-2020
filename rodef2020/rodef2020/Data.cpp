#include <iostream>
using namespace std; 
#include "Data.h"
#include<string>

Data::Data(JsonMap& ar_json)
{
	cout << "Data constructor" << endl;

	/* Create and initialize the |C| Resource objects dynamically and push_back them in the resources
	 * vector (no object copy is made) and add them to the resources map. */

	// Create the Seasons object

	/* Create and initialize the |I| Intervention objects dynamically and push_back them in the vector
	 * (no object copy is made) and add them to the interventions map. Use the resources map. */

	/* Create and initialize the Exclusion objects dynamically and push_back them in the vector (no
	 * object copy is made). Use the interventions map. */

	// initialize the non object attributes: m_T, m_scenariosNumber, m_quantile, m_alpha, and m_computationTime
	for (json::iterator it = ar_json.getJson()->begin(); it != ar_json.getJson()->end(); ++it)
	{
		if (it.key() == "Alpha")
			m_alpha = it.value().get<double>();
		else if (it.key() == "Quantile")
			m_quantile = it.value().get<double>();
		else if (it.key() == "Scenarios_number")
			m_scenariosNumber = it.value().get<std::vector<int>>();
		else if (it.key() == "m_computationTime")
			m_computationTime = it.value().get<double>();
		else if (it.key() == "T")
			m_T = it.value().get<int>();
		else if (it.key() == "Seasons")
		{
			std::vector<int> *ar_winter = new std::vector<int>;
			std::vector<int> *ar_summer = new std::vector<int>;
			std::vector<int> *ar_is = new std::vector<int>;
			std::vector<int> *ar_full = new std::vector<int>;

			if (it.value().contains("full"))
				*ar_full = it.value()["full"].get<std::vector<int>>();
			else if (it.value().contains("is"))
				*ar_is = it.value()["is"].get<std::vector<int>>();
			else if (it.value().contains("summer"))
				*ar_summer = it.value()["summer"].get<std::vector<int>>();
			else if (it.value().contains("winter"))
				*ar_winter = it.value()["winter"].get<std::vector<int>>();
			m_seasons = Seasons(*ar_winter, *ar_summer, *ar_is, *ar_full);
			delete ar_full;
			delete ar_is;
			delete ar_summer;
			delete ar_winter;
		}
		else if (it.key() == "Exclusions")
		{
			for (json::iterator ip = it.value().begin(); ip != it.value().end(); ++ip)
			{
				int m_i1 = stoi(ip.value()[0].get<string>().substr(1));
				int m_i2 = stoi(ip.value()[1].get<string>().substr(1));
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

				Exclusion *ip_exclusion = new Exclusion(m_i1, m_i2, m_season);
				m_exclusions.push_back(ip_exclusion);
			}
		}
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

