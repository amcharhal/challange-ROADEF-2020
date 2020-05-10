#pragma once

#include <string>
#include <vector>
#include "Data.h"

/*
 * class Solution:
 */
class Solution
{
public:
	Solution() {}
	Solution(Data* ap_data);
	
	std::vector<std::vector<double>>& getRisk_st() { return m_risk_st; }
	std::vector<double>& getRisk_t() { return m_risk_t; }
	double getObj1() const { return m_obj1; }
	std::vector<double>& getQ_tau_t() { return m_Q_tau_t; }
	std::vector<double>& getExcess_tau_t() { return m_Excess_tau_t; }
	double getObj2() const { return m_obj2; }
	double getObjective() const { return m_objective; }
	void setStartingTime(int a_i, int a_t) { m_startingTimes[a_i] = a_t; }
	void setXit(int a_i, int a_t, bool a_val) { m_xit[a_i][a_t-1] = a_val; }

	void saveSolution(const std::string& ar_outputFile) const;	
	void computeObjectives();		
	bool isFeasible() const;

private:
	Data* mp_data;
	// decision variables
	std::vector<int> m_startingTimes;
	std::vector<std::vector<bool>> m_xit;

	// attributes and methods for objective computation
	std::vector<std::vector<double>> m_risk_st;		// cumulative planning risk at period t for scenario s (size = T x St)
	std::vector<double> m_risk_t;					// mean cumulative planning risk at period t (size = T)
	double m_obj1;									// Mean cost
	std::vector<double> m_Q_tau_t;					// quantile values (size = T)
	std::vector<double> m_Excess_tau_t;				// expected excess (size = T)
	double m_obj2;									// expected excess
	double m_objective;								// planning ranking

	// private methods
	void computeRisk_st();
	void computeRisk_t();
	void computeObj1();
	void computeQ_tau_t();
	void computeExcess_tau_t();
	void computeObj2();
};