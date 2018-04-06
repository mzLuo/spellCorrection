 ///
 /// @file    CacheManager.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2017-03-24 17:47:47
 ///
 

#ifndef __WD_CACHEMANAGER_H__
#define __WD_CACHEMANAGER_H__
 
#include "Cache.h"
#include <iostream>
#include <string>
#include <vector>

using std::vector;
using std::string;

class CacheManager
{
public:
	static void initCache(size_t sz, const string & filename);
	static Cache & getCache(size_t idx);
	static void periodicUpdateCaches();
private:
	static vector<Cache> _cacheList;
};


#endif
