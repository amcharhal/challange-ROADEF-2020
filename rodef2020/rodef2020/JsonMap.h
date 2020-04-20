#pragma once
#include "json.h"


using json = nlohmann::json;
class JsonMap
{
private:
	json m_jsonMap;

public:
	JsonMap(const std::string &ar_inputFile);
	json *getJson();

	
};

