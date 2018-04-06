 ///
 /// @file    MyTask.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2016-03-28 15:34:24
 ///

#include "MyTask.h"
#include "MyDict.h"
#include "CacheManager.h"
#include "EditDistance.h"
#include "Thread.h"
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>

using std::cout;
using std::endl;



MyTask::MyTask(const string & queryWord, const wd::TcpConnectionPtr & conn)
: _queryWord(queryWord)//等待查询的单词
, _conn(conn)
{
}

void MyTask::execute()
{
	//1. 从cache里面进行查找
	Cache & cache = CacheManager::getCache(wd::str2int(wd::current_thread::threadName));
	string result = cache.query(_queryWord);
	if(result != string())//找到的result不等于空串
	{
		_conn->sendInLoop(result);
		cout << "> response client" << endl;
		return;//如果在Cache之中找到了，就不需要进行后面的查询了
	}

	queryIndexTable();//2. 执行查询,查询索引表

	response();//3. 给客户端返回结果
}


void MyTask::queryIndexTable()
{//查询索引
	auto indexTable = MyDict::createInstance()->get_index_table();
	string ch;

	for(int idx = 0; idx != _queryWord.size();)
	{
		size_t nBytes = nBytesCode(_queryWord[idx]);
		ch = _queryWord.substr(idx, nBytes);
		idx += nBytes;
		if(indexTable.count(ch))
		{
			cout << "indexTable has character " << ch << endl;
			statistic(indexTable[ch]);
		}
	}
}

void MyTask::response()
{//响应客户端的请求
	if(_resultQue.empty())
	{
		string result = "no answer!";
		_conn->sendInLoop(result);
	}
	else
	{
		MyResult result = _resultQue.top();
		_conn->sendInLoop(result._word);//这里只返回了一个候选词

		Cache & cache = CacheManager::getCache(wd::str2int(wd::current_thread::threadName));
		cache.addElement(_queryWord, result._word);//在缓存中添加新的查询结果
		cout << "> respone(): add Cache" << endl;
	}
	cout << "> reponse client" << endl;
}

void MyTask::statistic(set<int> & iset)
{//进行计算
	auto dict = MyDict::createInstance()->get_dict();
	auto iter = iset.begin();
	for(; iter != iset.end(); ++iter)
	{
		string rhsWord = dict[*iter].first;
		int idist = distance(rhsWord);//进行最小编辑距离的计算
		if(idist < 3)
		{
			MyResult res;
			res._word = rhsWord;
			res._iFreq = dict[*iter].second;
			res._iDist = idist;
			_resultQue.push(res);
		}
	}
}

int MyTask::distance(const string & rhsWord)
{//计算最小编辑距离
	return ::editDistance(_queryWord, rhsWord);
}
