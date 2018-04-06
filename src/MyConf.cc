
#include "MyConf.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>

using std::ifstream;
using std::cout;
using std::endl;
using std::istringstream;


MyConf * MyConf::pInstance_ = getInstance();

MyConf * MyConf::getInstance()
{
	if(NULL == pInstance_)
	{	pInstance_ = new MyConf;	}
	return pInstance_;
}

bool MyConf::init(const string & filename)
{
	ifstream ifs(filename.c_str());
	if(!ifs)
	{
		cout << "file read error" << endl;
		return false;
	}
	string line;
	while(getline(ifs, line))
	{
		istringstream iss(line);
		string key, value;
		iss >> key >> value;
		map_.insert(make_pair(key, value));
	}
	ifs.close();
	return true;
}

void MyConf::show()
{
	map<string, string>::const_iterator mit = map_.begin();
	for(; mit != map_.end(); ++mit)
	{
		cout << mit->first << " --> " << mit->second << endl;
	}
}
