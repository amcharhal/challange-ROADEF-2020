#include <iostream>
using namespace std;
#include "Instance.h"


Instance::Instance(const std::string & ar_fileName) :
	m_inputFile(ar_fileName)
{
	mp_jsonMap = new JsonMap(ar_fileName);
	mp_data = new Data(*mp_jsonMap);
}

Instance::~Instance()
{
	//delete mp_jsonMap;
	delete mp_data;
}

void Instance::solve()
{
	m_bestSolution = Solution(mp_data);
	m_bestSolution.setStartingTime(0, 1);
	m_bestSolution.setStartingTime(1, 1);
	m_bestSolution.setStartingTime(2, 2);

	m_bestSolution.setXit(0, 1, true);
	m_bestSolution.setXit(0, 2, true);
	m_bestSolution.setXit(0, 3, true);
	m_bestSolution.setXit(1, 1, true);
	m_bestSolution.setXit(2, 2, true);
}
