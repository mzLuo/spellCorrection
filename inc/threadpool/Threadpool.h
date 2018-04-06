 ///
 /// @file    Threadpool.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2016-03-19 10:18:25
 ///
 
#ifndef _MY_THREADPOOL_H_
#define _MY_THREADPOOL_H_
#include "Buffer.h"
#include "MyConf.h"
#include <vector>
#include <functional>

using std::vector;

namespace wd 
{
class Thread;
class Threadpool
{
public:
	typedef std::function<void()> Task;
	Threadpool(int threadNum, int bufSize);
	~Threadpool();

	void start();
	void stop();

	void addTask(Task task);

	void threadFunc();

private:
	Task getTask();
	
private:
	int _threadNum;
	vector<Thread *> _vecThreads;
	int _bufSize;
	Buffer _buf;
	bool _isExit;
};

}//end of namespace wd

#endif
