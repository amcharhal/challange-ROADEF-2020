#include "Intervention.h"

Intervention::Intervention(const std::string& ar_name,
	int a_tmax,
	const std::vector<int>& ar_delta,
	std::vector<std::vector<std::vector<double>>> &ap_workloads,
	std::vector<std::vector<std::vector<double>>> &ap_risks):
	m_name(ar_name),
	m_tmax(a_tmax),
	m_delta(ar_delta),
	mp_workloads (&ap_workloads),
	mp_risks(&ap_risks)
{
}

Intervention::~Intervention()
{
	// TODO
}
