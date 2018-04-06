 ///
 /// @file    Buffer.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2016-03-18 15:09:17
 ///
 
#ifndef __MY_BUFFER_H_
#define __MY_BUFFER_H_


#include "MutexLock.h"
#include "Condition.h"

#include <queue>
#include <functional>

class Cache;
namespace wd
{

class Buffer
{
public:
	typedef std::function<void()> Task;
	Buffer(int size);
	~Buffer();

	void push(Task ptask);
	Task pop();

	bool full();
	bool empty();

private:
	int _size;
	std::queue<Task> _que;
	MutexLock _mutex;
	Condition _notFull;
	Condition _notEmpty;
};

}//end of namespace wd

#endif
