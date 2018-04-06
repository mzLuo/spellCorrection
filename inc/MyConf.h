#ifndef _CONF_H
#define _CONF_H

#include <string>
#include <map>

using std::string;
using std::map;

///
///  配置文件类
///
class MyConf
{
public:
	static MyConf * getInstance();

	map<string, string> &getMap(){	return map_;	}

	bool init(const string & filename);

	void show();

private:
	MyConf(){}

	~MyConf(){}

private:
	map<string, string> map_;

	static MyConf * pInstance_;
};

#endif
