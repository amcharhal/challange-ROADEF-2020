#include <iostream>
using namespace std;
#include "Instance.h"


Instance::Instance(const std::string & ar_fileName):
	m_inputFile(ar_fileName)
{
	cout << "Instance constructor" << endl;

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
	cout << "Solve method" << endl;
	m_solution = Solution();
}
