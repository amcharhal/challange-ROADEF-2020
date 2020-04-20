#pragma once

#include <vector>

/*
 * Alias (as an enum) to each possible season
 */
enum E_Season { WINTER, SUMMER, IS, FULL };


/*
 * class Seasons:
 */
class Seasons
{
public:
	Seasons() {}
	Seasons(const std::vector<int>& ar_winter,
	const std::vector<int>& ar_summer,
	const std::vector<int>& ar_is,
	const std::vector<int>& ar_full);

	std::vector<int>* getWinter() { return &m_winter; }
	std::vector<int>* getSummer() { return &m_summer; }
	std::vector<int>* getIs() { return &m_is; }
	std::vector<int>* getFull() { return &m_full; }

private:
	std::vector<int> m_winter;
	std::vector<int> m_summer;
	std::vector<int> m_is;
	std::vector<int> m_full;
};

