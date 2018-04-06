 ///
 /// @file    MyTask.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2016-03-28 15:07:00
 ///
 
#ifndef __MY_TASK_H_
#define __MY_TASK_H_

#include "TcpConnection.h"

#include <string>
#include <vector>
#include <queue>
#include <set>

using std::string;
using std::priority_queue;
using std::vector;
using std::set;


struct MyResult
{
	string _word;
	int _iFreq;
	int _iDist;
};

struct MyCompare
{
	bool operator()(const MyResult & lhs, const MyResult & rhs)
	{
		if(lhs._iDist > rhs._iDist)
		{	return true;	}
		else if(lhs._iDist == rhs._iDist &&
				lhs._iFreq < rhs._iFreq)
		{	return true;	}
		else if(lhs._iDist == rhs._iDist &&
				lhs._iFreq == rhs._iFreq && 
				lhs._word > rhs._word)
		{	return true;	}
		else
			return false;
	}
};

class Cache;
class MyTask
{
public:
	MyTask(const string & queryWord, const wd::TcpConnectionPtr & conn); 
	void execute();
private:
	void queryIndexTable();
	void statistic(set<int> & iset);
	int distance(const string & rhs);
	void response();

private:
	string _queryWord;
	wd::TcpConnectionPtr _conn;
	priority_queue<MyResult, vector<MyResult>, MyCompare> _resultQue;
};

#endif
