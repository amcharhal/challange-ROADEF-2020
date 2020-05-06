#pragma once

#include <string>
#include "JsonMap.h"
#include "Data.h"
#include "Solution.h"

/*
 * class Instance:
 */
class Instance
{
public:
	Instance(const std::string& ar_fileName);
	~Instance();

	std::string getInputFile() const { return m_inputFile; }
	JsonMap* getJsonMap() const { return mp_jsonMap; }
	Data* getData() const { return mp_data; }
	Solution& getBestSolution() { return m_bestSolution; }

	void solve();
private:
	std::string m_inputFile;
	JsonMap* mp_jsonMap;
	Data* mp_data;
	Solution m_bestSolution;
};
