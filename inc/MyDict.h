#ifndef _MYDICT_H
#define _MYDICT_H

#include <string>
#include <vector>
#include <set>
#include <utility>
#include <map>

using std::vector;
using std::map;
using std::set;
using std::pair;
using std::string;

class MyDict
{
public:
	//static MyDict* createInstance(const char *dictpath, const char * dictpath2);
	static MyDict* createInstance();

	void init(const char * dictpath, const char * cnDictPath);
	void show_dict();
	void show_index_table();

	vector<pair<string, int> > & get_dict();
	map<string, set<int> > & get_index_table();
private:
	//MyDict(const char * dictpath, const char * dictpath2);
	MyDict();
	void read_from(const char * dictpath);
	void record_to_index(int idx);

private:
	static MyDict *pInstance;
	vector<pair<string, int> > dict_;
	map<string, set<int> > index_table_;
};

#endif//end _MYDICT_H
