 ///
 /// @file    CacheManager.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2017-03-24 17:55:48
 ///
 
#include "CacheManager.h"
#include "MyConf.h"
#include <iostream>
using std::cout;
using std::endl;

//缓存的数量和线程个数一致
vector<Cache> CacheManager::_cacheList;

void CacheManager::initCache(size_t sz, const string & filename)
{//从磁盘文件中读取缓存信息
	Cache cache;
	cache.readFromFile(filename);

	for(size_t idx = 0; idx != sz; ++idx)
		_cacheList.push_back(cache);
}

Cache & CacheManager::getCache(size_t idx)
{//获取某个缓存
	return _cacheList[idx];
}

void CacheManager::periodicUpdateCaches()
{//定时更新所有的缓存
	cout << "> Starting update Caches!" << endl;
	auto it1 = _cacheList.begin();
	auto it2 = ++it1;
	for(; it2 != _cacheList.end(); ++it2)
	{
		it1->update(*it2);
	}

	it1 = _cacheList.begin();
	it2 = ++it1;
	for(; it2 != _cacheList.end(); ++it2)
	{
		it2->update(*it1);
	}
	//存储到本地
	it1->writeToFile(MyConf::getInstance()->getMap()["my_cache"]);
}

