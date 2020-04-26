#pragma once

#include <string>
#include <vector>


/*
 * class Resource:
 */
class Resource
{
public:
	Resource(const std::string& ar_name,
		const std::vector<double>& ar_min,
		const std::vector<double>& ar_max);

	std::string getName() const { return m_name; }
	double getMin(int a_t) const { return m_min[a_t-1]; }	// period t stored at index t-1
	double getMax(int a_t) const { return m_max[a_t-1]; }
	std::vector<double> getMin() const { return m_min; };
	std::vector<double> getMax() const { return m_max; };
private:
	std::string m_name;
	std::vector<double> m_min;
	std::vector<double> m_max;
};

