 ///
 /// @file    Cache.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2016-03-31 08:40:44
 ///
 
#ifndef __MY_CACHE_H_
#define __MY_CACHE_H_

#include <iostream>
#include <string>
#include <unordered_map>

using std::unordered_map;
using std::string;


class Cache
{
public:
	void readFromFile(const string & filename);

	void writeToFile(const string & filename);

	void update(const Cache & rhs);

	void addElement(const string & key, const string & value);

	void debug();

	string query(const string & word);
private:
	unordered_map<string, string> _hashMap;
};

#endif

