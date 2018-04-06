 ///
 /// @file    Thread.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2016-03-18 11:06:13
 ///
 
#ifndef __MY_THREAD_H_
#define __MY_THREAD_H_

#include "Noncopyable.h"
#include <pthread.h>
#include <string>
#include <functional>
#include <sstream>

using std::string;



namespace wd
{


inline string int2str(int number)
{
	std::ostringstream oss;
	oss << number;
	return oss.str();
}

inline unsigned short str2unshort(const string & str)
{
	unsigned short port;
	std::istringstream iss(str);
	iss >> port;
	return port;
}

inline int str2int(const string & str)
{
	int number;
	std::istringstream iss(str);
	iss >> number;
	return number;
}



namespace current_thread
{
extern __thread const char * threadName;
}//end of namespace current_thread


class Thread : Noncopyable
{
public:
	typedef std::function<void()> ThreadCallback;
	Thread(ThreadCallback cb, const string & name = string());
	~Thread();

	void start();
	void join();
	pthread_t getThreadId()const
	{	return _pthId;	}

	static void * threadFunc(void * arg);

private:
	pthread_t _pthId;
	bool _isRunning;
	ThreadCallback _cb;
	string _name;
};

}//end of namespace wd

#endif
