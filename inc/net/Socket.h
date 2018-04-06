 ///
 /// @file    Socket.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2016-03-22 11:27:57
 ///
 
#ifndef _MY_SOCKET_H_
#define _MY_SOCKET_H_

#include "Noncopyable.h"

namespace wd
{
class InetAddress;

class Socket : Noncopyable
{
public:
	Socket();
	Socket(int);
	void ready(InetAddress & addr);
	int accept();
	int fd();

	void shutdownWrite();

	static InetAddress getLocalAddr(int fd);
	static InetAddress getPeerAddr(int fd);
private:
	void setReuseAddr(bool flag);
	void setReusePort(bool flag);
	void bind(InetAddress & addr);
	void listen();
private:
	int _fd;
};

}// end of namespace wd 

#endif
