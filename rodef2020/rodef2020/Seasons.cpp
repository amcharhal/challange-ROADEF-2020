#include "Seasons.h"


Seasons::Seasons(const std::vector<int>& ar_winter,
	const std::vector<int>& ar_summer,
	const std::vector<int>& ar_is,
	const std::vector<int>& ar_full):
	m_winter(ar_winter),
	m_summer(ar_summer),
	m_is(ar_is),
	m_full(ar_full)
{
}
