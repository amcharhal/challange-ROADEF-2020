#pragma once

#include <vector>
#include <map>
#include "JsonMap.h"
#include "Resource.h"
#include "Seasons.h"
#include "Intervention.h"
#include "Exclusion.h"

/*
 * class Data:
 * A Data object contains all the requiered data of an instance. One Data object must be created per instance.
 * The Data object is created by reading the Json object of the instance and building step by step all data items:
 *	1. vector<Resource> (for all c in C):
 *		1.1. min		(vector<double> of size T)	} lower bounds on resource availability for each period t: l_t^c
 *		1.2. max		(vector<double> of size T)	} upper bounds on resource availability for each period t: l_t^c
 *
 *	2. Seasons (1 object)
 *		2.1. winter		(vector<int> of variable size)	}
 *		2.2. summer		(vector<int> of variable size)	} The sum of sizes of the 4 vectors equals T,
 *		2.3. is			(vector<int> of variable size)	} each period t in H occurs exactly onces
 *		2.4. full		(vector<int> of variable size)	}
 *
 *	3. vector<Intervention> (for all i in I):
 *		3.1. tmax		(int)	} period before which the intervention has to be scheduled
 *		3.1. delta		(vector<int> of size T)	} duration of the intervention depending on its starting date t: delta_i,t
 *		3.1. workloads	(double***)	} workload due to intervention i for resource c at time t when started at time st: workloads[c][t][st]
 *		3.2. risks		(double***)	} risk in scenario s over the in-process interventions at t when started at time st: risks[c][t][st]
 *
 *	4. vector<Exclusion> (for all (i1, i2, Season) in Exc)
 *		4.1. i1			(int)			} ID if intervention 1
 *		4.2. i2			(int)			} ID if intervention 2
 *		4.1. season		(ENUM_SEASON)	} season on which i1 and i2 shall not be concurrently ongoing on any of the season's period
 *
 *	5. m_T (int)	// number of time steps/periods
 *
 *	6. scenarios number (vector<int> of size T)	} number of available and considered scenarios for each timestep
 *
 *	7. quantile (double)	} quantile used to compute the expected excess indicator of the objective function
 *
 *	8. alpha (double)	} alpha parameter used in the objective fonction
 *
 *	9. computation time (double)	} allowed time in minutes for computation
 */
class Data
{
public:
	Data(JsonMap& ar_json);
	~Data();

	Resource* getResource(int a_resourceId) const { return m_resources[a_resourceId]; }
	int getNbResources() const { return m_resources.size(); }
	Seasons* getSeasons() { return &m_seasons; }
	Intervention* getIntervention(int a_interventionId) const { return m_interventions[a_interventionId]; }
	int getNbInterventions() const { return m_interventions.size(); }
	Exclusion* getExclusion(int a_exclusionId) const { return m_exclusions[a_exclusionId]; }
	int getNbExclusions() const { return m_exclusions.size(); }
	int getT() const { return m_T; }
	int getScenariosNumber(int a_t) const { return m_scenariosNumber[a_t-1]; }
	double getQuantile() const { return m_quantile; }
	double getAlpha() const { return m_alpha; }
	double getComputationTime() const { return m_computationTime; }
	double getWorkload(int a_i, int a_c, int a_t, int a_st) { return m_interventions[a_i]->getWorkload(a_c, a_t, a_st); }
	double getrisk(int a_i, int a_s, int a_t, int a_st) { return m_interventions[a_i]->getRisk(a_s, a_t, a_st); }
	std::map<std::string, int> getResourcesMap() { return m_resourcesMap; };
	std::map<std::string, int> getInterventionsMap() { return m_interventionsMap; };
private:
	std::vector<Resource*> m_resources;
	std::map<std::string, int> m_resourcesMap;	// (resource name, resource ID)
	Seasons m_seasons;
	std::vector<Intervention*> m_interventions;
	std::map<std::string, int> m_interventionsMap;	// (intervention name, intervention ID)
	std::vector<Exclusion*> m_exclusions;
	int m_T;
	std::vector<int> m_scenariosNumber;
	double m_quantile;
	double m_alpha;
	double m_computationTime;
	int m_numberOfResources;
};
