#include "Intervention.h"

Intervention::Intervention(const std::string& ar_name,
	int a_tmax,
	const std::vector<int>& ar_delta,
	std::vector<std::vector<std::vector<double>>> &ap_workloads,
	std::vector<std::vector<std::vector<double>>> &ap_risks) :
	m_name(ar_name),
	m_tmax(a_tmax),
	m_delta(ar_delta),
	mp_workloads(&ap_workloads),
	mp_risks(&ap_risks)
{
}
double Intervention::getRisk(int a_t, int a_st, int a_s) {
	if (mp_risks->at(a_t - 1).size() == 0)
		return 0;
	else {
		if (mp_risks->at(a_t - 1).at(a_st - 1).size() == 0)
			return 0;
		else
			return mp_risks->at(a_t - 1).at(a_st - 1).at(a_s);
	}
}

double Intervention::getWorkload(int a_c, int a_t, int a_st) {


	if (mp_workloads->at(a_c).size() == 0)
		return 0;
	else {
		if (mp_workloads->at(a_c).at(a_t - 1).size() == 0)
			return 0;
		else
			return mp_workloads->at(a_c).at(a_t - 1).at(a_st - 1);
	}

}
Intervention::~Intervention()
{
	delete mp_risks;
	delete mp_workloads;
}
