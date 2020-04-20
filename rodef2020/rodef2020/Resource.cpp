#include <iostream>
using namespace std;
#include "Resource.h"


Resource::Resource(const std::string & ar_name,
	const std::vector<double>& ar_min,
	const std::vector<double>& ar_max):
	m_name(ar_name),
	m_min(ar_min),
	m_max(ar_max)
{
}