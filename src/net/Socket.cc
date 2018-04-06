 ///
 /// @file    Socket.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2016-03-22 11:32:44
 ///
 
#include "Socket.h"
#include "InetAddress.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>



int createSocketFd()
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == fd)
	{
		perror("socket");
	}
	return fd;
}

namespace wd
{

Socket::Socket()
: _fd(createSocketFd())
{}

Socket::Socket(int fd)
: _fd(fd)
{}


void Socket::ready(InetAddress & addr)
{
	setReuseAddr(true);
	setReusePort(true);
	bind(addr);
	listen();
}

int Socket::accept()
{
	int peerfd = ::accept(_fd, NULL, NULL);
	if(-1 == peerfd)
	{
		perror("accept");
	}
	return peerfd;
}

void Socket::bind(InetAddress & addr)
{
	int ret = ::bind(_fd, 
		   (struct sockaddr*)(addr.getInetAddressPtr()),
		   sizeof(struct sockaddr));
	if(-1 == ret)
	{
		perror("bind");
		close(_fd);
		exit(EXIT_FAILURE);
	}
}

void Socket::listen()
{
	int ret = ::listen(_fd, 10);
	if(-1 == ret)
	{
		perror("listen");
		close(_fd);
		exit(EXIT_FAILURE);
	}
}

int Socket::fd()
{
	return _fd;
}

void Socket::setReuseAddr(bool flag)
{
	int on = flag ? 1 : 0;

	int ret = ::setsockopt(_fd,
						   SOL_SOCKET,
						   SO_REUSEADDR,
						   &on,
						   static_cast<socklen_t>(sizeof(on)));
	if(-1 == ret)
	{
		perror("setsockopt reuseaddr");
		close(_fd);
		exit(EXIT_FAILURE);
	}
}


void Socket::setReusePort(bool flag)
{
#ifdef SO_REUSEPORT
	int on = flag ? 1 : 0;
	int ret = ::setsockopt(_fd,
						   SOL_SOCKET,
						   SO_REUSEPORT,
						   &on,
						   static_cast<socklen_t>(sizeof(on)));
	if(-1 == ret)
	{
		perror("setsockopt reuseport");
		close(_fd);
		exit(EXIT_FAILURE);
	}
#else
	if(flag)
	{
		printf("SO_RESUEPORT is not supported!\n");
	}
#endif
}

void Socket::shutdownWrite()
{
	int ret = ::shutdown(_fd, SHUT_WR);
	if(-1 == ret)
	{
		perror("shutdown wr");
		exit(EXIT_FAILURE);
	}
}

InetAddress Socket::getLocalAddr(int fd)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(struct sockaddr_in);
	int ret = ::getsockname(fd, (struct sockaddr*)&addr, &len);
	if(-1 == ret)
	{
		perror("getsockname");
	}
	return InetAddress(addr);
}

InetAddress Socket::getPeerAddr(int fd)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(struct sockaddr_in);
	int ret = ::getpeername(fd, (struct sockaddr*)&addr, &len);
	if(-1 == ret)
	{
		perror("getpeername");
	}
	return InetAddress(addr);
}


}// end of namespace wd
