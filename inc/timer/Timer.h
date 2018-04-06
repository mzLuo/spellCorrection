 ///
 /// @file    timer.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2016-03-21 10:44:57
 ///

#ifndef __MY_TIME_H_
#define __MY_TIME_H_
#include "Noncopyable.h"
#include <iostream>
#include <functional>


class Timer : wd::Noncopyable
{
public:
	typedef std::function<void()> TimerCallback;
	Timer(TimerCallback cb, int init_sec, int interval_sec);
	~Timer();

	void start();
	void stop();

private:
	int _fd;
	TimerCallback _cb;
	bool _isStarted;
};

#endif 
