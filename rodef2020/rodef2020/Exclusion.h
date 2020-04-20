#pragma once

#include "Seasons.h"

/*
 * class Exclusion:
 */
class Exclusion
{
public:
	Exclusion(int a_i1, int a_i2, E_Season a_season);

	int getI1() const { return m_i1; }
	int getI2() const { return m_i2; }
	E_Season getSeason() const { return m_season; }

private:
	int m_i1;			// ID of intervention 1 between 0 and |I|-1
	int m_i2;			// ID of intervention 2 between 0 and |I|-1
	E_Season m_season;	// Season on which i1 and i2 shall not be concurrently ongoing
};

