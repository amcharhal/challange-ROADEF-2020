#include "JsonMap.h"
#include<string>
#include<fstream>
#include<iostream>


using namespace std;
JsonMap::JsonMap(const string &ar_inputFile) {
	ifstream openFile(ar_inputFile) ;
	try {
		if (!openFile.is_open())
			throw  string{ "File is not founded" };
		openFile >> m_jsonMap;
	}
	catch (string &name_error) {
		cerr << name_error << endl;
	}

}

json *JsonMap::getJson() {
	return &m_jsonMap;
}