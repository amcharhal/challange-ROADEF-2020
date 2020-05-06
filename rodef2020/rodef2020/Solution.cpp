#include <iostream>
#include <fstream>
#include <algorithm>
#include <math.h>
using namespace std;
#include "Solution.h"
#include "Exclusion.h"
#include "Tools.h"


Solution::Solution(Data* ap_data):
	mp_data(ap_data)
{
	// initially, interventions are not scheduled
	m_startingTimes.resize(mp_data->getNbInterventions());

	m_xit.resize(mp_data->getNbInterventions());
	for (int i = 0; i < mp_data->getNbInterventions(); i++)
	{
		m_xit[i].resize(mp_data->getT());
	}
}

void Solution::saveSolution(const std::string& ar_outputFile) const
{
	string path = OUTPUT_FOLDER + ar_outputFile;
	ofstream stream(path.c_str());
	if (stream)
	{
		for (int i = 0; i < m_startingTimes.size(); i++)
		{
			stream << mp_data->getInterventionName(i) << SPACE << m_startingTimes[i] << endl;
		}
	}
	else
	{
		cerr << "Error while opening the file " << path << endl;
	}
}


void Solution::computeObjectives()
{
	computeRisk_st();
	computeRisk_t();
	computeObj1();
	computeQ_tau_t();
	computeExcess_tau_t();
	computeObj2();
	m_objective = mp_data->getAlpha() * m_obj1 + (1 - mp_data->getAlpha()) * m_obj2;
}

bool Solution::isFeasible() const
{
	int T = mp_data->getT();

	// 1.2. Interventions are scheduled once
	for (int i = 0; i < mp_data->getNbInterventions(); i++)
	{
		if (0 == m_startingTimes[i])
		{
			return false;
		}
	}
	cout << "1.2 ok" << endl;

	// 1.3. No work left
	for (int i = 0; i < mp_data->getNbInterventions(); i++)
	{
		if (m_startingTimes[i] > mp_data->getIntervention(i)->getTMax())
		{
			return false;
		}
	}
	cout << "1.3 ok" << endl;

	// 1.1. Non-preemptive scheduling
	for (int i = 0; i < mp_data->getNbInterventions(); i++)
	{
		for (int t = 1; t <= T; t++)
		{
			if (m_startingTimes[i] <= t && t <= m_startingTimes[i] + mp_data->getIntervention(i)->getDelta(m_startingTimes[i]) - 1)
			{
				if (m_xit[i][t-1] == false)
				{
					return false;
				}
			}
			else
			{
				if (m_xit[i][t-1] == true)
				{
					return false;
				}
			}
		}
	}
	cout << "1.1 ok" << endl;
	

	// 2. Resource constraints
	for (int c = 0; c < mp_data->getNbResources(); c++)
	{
		for (int t = 1; t <= T; t++)
		{
			double rct = 0.0;
			for (int i = 0; i < mp_data->getNbInterventions(); i++)
			{
				rct += m_xit[i][t-1] * mp_data->getIntervention(i)->getWorkload(c, t, m_startingTimes[i]);
			}

			cout << "resource (" << c << "," << t << ") : " << mp_data->getResource(c)->getMin(t) << " <= " <<
				rct << " <= " << mp_data->getResource(c)->getMax(t) << "?" << endl;
			if (rct < mp_data->getResource(c)->getMin(t) || rct > mp_data->getResource(c)->getMax(t))
			{
				return false;
			}
		}
	}
	cout << "2. ok" << endl;


	// 3. Disjunctive constraints
	for (int e = 0; e < mp_data->getNbExclusions(); e++)
	{
		Exclusion* p_exc = mp_data->getExclusion(e);
		int i1 = p_exc->getI1();
		int i2 = p_exc->getI2();
		E_Season season = p_exc->getSeason();

		cout << "(" << i1 << "," << i2 << "," << season << ")" << endl;

		switch (season)
		{
		case WINTER:
			for (int t: *mp_data->getSeasons()->getWinter())
			{
				if (m_xit[i1][t] && m_xit[i2][t])
				{
					return false;
				}
			}
			break;
		case SUMMER:
			for (int t : *mp_data->getSeasons()->getSummer())
			{
				if (m_xit[i1][t] && m_xit[i2][t])
				{
					return false;
				}
			}
			break;
		case IS:
			for (int t : *mp_data->getSeasons()->getIs())
			{
				if (m_xit[i1][t] && m_xit[i2][t])
				{
					return false;
				}
			}
			break;
		case FULL:
			for (int t : *mp_data->getSeasons()->getFull())
			{
				if (m_xit[i1][t-1] && m_xit[i2][t-1])
				{
					return false;
				}
			}
			break;
		}
	}
	cout << "3 ok" << endl;

	// all constraints checked
	return true;
}


void Solution::computeRisk_st()
{
	int T = mp_data->getT();
	m_risk_st.resize(T);
	for (int t = 1; t <= T; t++)
	{
		int St = mp_data->getScenariosNumber(t);
		for (int s = 0; s < St; s++)
		{
			double risk_st = 0.0;
			for (int i = 0; i < mp_data->getNbInterventions(); i++)
			{
				risk_st += m_xit[i][t - 1] * mp_data->getIntervention(i)->getRisk(t, m_startingTimes[i], s);
			}
			m_risk_st[t - 1].push_back(risk_st);
		}
	}
}

void Solution::computeRisk_t()
{
	int T = mp_data->getT();
	for (int t = 1; t <= T; t++)
	{
		double risk_t = 0.0;
		int St = mp_data->getScenariosNumber(t);
		for (int s = 0; s < St; s++)
		{
			risk_t += m_risk_st[t-1][s];
		}
		risk_t /= St;
		m_risk_t.push_back(risk_t);
	}
}


void Solution::computeObj1()
{
	double meanCost = 0.0;
	int T = mp_data->getT();
	for (int t = 1; t <= T; t++)
	{
		meanCost += m_risk_t[t - 1];
	}
	m_obj1 = meanCost / T;
}

void Solution::computeQ_tau_t()
{
	//m_Q_tau_t = {12, 11, 4};

	int T = mp_data->getT();
	for (int t = 1; t <= T; t++)
	{
		// E is the set containing the values of m_risk_st[t - 1] sorted in ascending order
		vector<double> E = m_risk_st[t - 1];
		sort(E.begin(), E.end());

		// X is the smallest subset of E of size >= quantile * |E| -> X contains the smallest values of E
		double Xsize = mp_data->getQuantile() * E.size();
		if (floor(Xsize) != Xsize)
		{
			// quantile * |E| is not integer, so we Xsize is equal to the integer above quantile * |E|
			Xsize = floor(Xsize) + 1;
		}
		// q must be larger to any x in X, so we choose q = the largest x in X, i.e., the last value in X
		m_Q_tau_t.push_back(E[Xsize-1]);
	}
}

void Solution::computeExcess_tau_t()
{
	for (int t = 1; t <= mp_data->getT(); t++)
	{
		m_Excess_tau_t.push_back(max(0.0, m_Q_tau_t[t-1] - m_risk_t[t-1]));
	}
}

void Solution::computeObj2()
{
	double expectedExcess = 0.0;
	int T = mp_data->getT();
	for (int t = 1; t <= T; t++)
	{
		expectedExcess += m_Excess_tau_t[t - 1];
	}

	m_obj2 = expectedExcess / T;
}