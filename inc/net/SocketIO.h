 ///
 /// @file    SocketIO.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2016-03-23 09:54:51
 ///
#ifndef __MY_SOCKETIO_H_
#define __MY_SOCKETIO_H_

#include "Noncopyable.h"
#include <unistd.h>

namespace wd
{
class SocketIO : Noncopyable
{
public:
	SocketIO(int fd);

	ssize_t readn(char * buff, size_t count);
	ssize_t writen(const char * buff, size_t count);
	ssize_t readline(char * buff, size_t max);

private:
	ssize_t readPeek(char * buff, size_t count);

private:
	int _fd;
};

}// end of namespace wd

#endif
