 ///
 /// @file    Cache.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2016-03-31 08:52:49
 ///
 
#include "Cache.h"
#include <iostream>
#include <fstream>

using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

void Cache::addElement(const string & key, const string & value)
{//往缓存中添加数据
	_hashMap[key] = value;	
}

string Cache::query(const string & word)
{//cache中查询
	auto iter = _hashMap.find(word);
	if(iter != _hashMap.end())
	{
		return iter->second;
	}
	else
	{
		return string();
	}
}

void Cache::readFromFile(const string & filename)
{//从文件中读取缓存信息
	ifstream ifs(filename.c_str());
	if(!ifs.good())
	{
		cout << "open cache file error!" << endl;
		return;
	}

	string key, value;
	while(ifs >> key >> value)
	{
		_hashMap.insert(std::make_pair(key, value));
	}
}

void Cache::writeToFile(const string & filename)
{//将缓存信息写入到文件中
	ofstream ofs(filename.c_str());
	if(!ofs.good())
	{
		cout << "ofstream:write cache file error" << endl;
		return ;
	}

	auto iter = _hashMap.begin();
	for(; iter != _hashMap.end(); ++iter)
	{
		ofs << iter->first << "\t" << iter->second << endl;
	}
	ofs.close();
}

void Cache::update(const Cache & rhs)
{//更新缓存信息
	auto iter = rhs._hashMap.begin();
	for(; iter != rhs._hashMap.end(); ++iter)
	{
		_hashMap.insert(*iter);
	}
}

void Cache::debug()
{
	cout << "Cache begin:" << endl;
	for(auto & elem : _hashMap)
	{
		cout << elem.first << "-->" << elem.second << endl;
	}
	cout << "Cache end: " << endl;
}
