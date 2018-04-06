 ///
 /// @file    Condition.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2016-03-18 15:02:04
 ///
#ifndef __MY_CONDITION_H_
#define __MY_CONDITION_H_

#include "Noncopyable.h"
#include <pthread.h>


namespace wd
{

class MutexLock;

class Condition : Noncopyable
{
public:
	Condition(MutexLock & mutex);
	~Condition();

	void wait();
	void notify();
	void notifyAll();
private:
	pthread_cond_t _cond;
	MutexLock & _mutex;
};

}//end of namespace wd


#endif
