 ///
 /// @file    EpollPoller.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2016-03-23 15:24:24
 ///
 

#include "EpollPoller.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <sys/eventfd.h>

#include <iostream>
using std::cout;
using std::endl;

const int kInitNumber = 2048;

int createEpollFd()
{
	int fd = ::epoll_create1(0);
	if(-1 == fd)
	{
		perror("epoll_create1");
	}
	return fd;
}

int createEventfd()
{
	int fd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if(-1 == fd)
	{
		perror("eventfd ");
	}
	return fd;
}


void addEpollFd(int efd, int fd)
{
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLIN;
	int ret = ::epoll_ctl(efd, 
						  EPOLL_CTL_ADD, 
						  fd,
						  &ev);
	if(-1 == ret)
	{
		perror("epoll_ctl add");
		exit(EXIT_FAILURE);
	}
}

void delEpollFd(int efd, int fd)
{
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLIN;
	int ret = ::epoll_ctl(efd, 
						  EPOLL_CTL_DEL, 
						  fd,
						  &ev);
	if(-1 == ret)
	{
		perror("epoll_ctl add");
		exit(EXIT_FAILURE);
	}
}


size_t recvPeek(int sockfd, char * buff, size_t count)
{
	int nread;
	do
	{
		nread = ::recv(sockfd, buff, count, MSG_PEEK);
	}while(-1 == nread && errno == EINTR);
	return nread;
}

//判断连接是否已经断开
bool isConnectionClose(int sockfd)
{
	char buff[1024];
	int nread = recvPeek(sockfd, buff, 1024);
	if(-1 == nread)
	{
		perror("recvPeek");
		exit(EXIT_FAILURE);
	}
	return (0 == nread);
}


namespace wd
{

EpollPoller::EpollPoller(int listenfd)
: _epollfd(createEpollFd())
, _eventfd(createEventfd())
, _listenfd(listenfd)
, _isLooping(false)
, _eventList(kInitNumber)
{
	addEpollFd(_epollfd, _listenfd);//监听listenfd
	addEpollFd(_epollfd, _eventfd);//监听eventfd
}

EpollPoller::~EpollPoller()
{
	::close(_epollfd);
}


void EpollPoller::loop()
{
	if(!_isLooping)
	{
		_isLooping = true;
		while(_isLooping)
		{
			waitEpollfd();
		}
	}
}


void EpollPoller::unloop()
{
	if(_isLooping)
	{
		_isLooping = false;
	}
}

void EpollPoller::runInLoop(const EpollPoller::Functor & cb)
{
	{
		MutexLockGuard guard(_mutex);
		_pendingFunctors.push_back(cb);
	}
	wakeup();
}

void EpollPoller::wakeup()//write通知
{
	uint64_t one = 1;
	ssize_t ret = ::write(_eventfd, &one, sizeof(one));
	if(ret != sizeof(one))
	{
		perror("EpollPoller::write error");
	}
}

void EpollPoller::handleRead()//read接受通知
{
	uint64_t one = 0;
	ssize_t ret = ::read(_eventfd, &one, sizeof(one));
	if(ret != sizeof(uint64_t))
	{
		perror("EpollPoller::handleRead() ret != 8");
	}
}

void EpollPoller::doPendingFunctors()
{
	cout << "> doPendingFunctors()" << endl;
	vector<Functor> functors;
	{
	MutexLockGuard guard(_mutex);
	_pendingFunctors.swap(functors);
	}

	for(auto & f : functors)
	{
		f();
	}
}

void EpollPoller::waitEpollfd()
{
	int nready;
	do
	{
		nready = epoll_wait(_epollfd, 
							&(*_eventList.begin()),
							_eventList.size(),
							5000);
	
	}while(nready == -1 && errno == EINTR);

	if(-1 == nready)
	{
		perror("epoll_wait");
		exit(EXIT_FAILURE);
	}
	else if(0 == nready)
	{
		printf("\n> epoll_wait timeout!\n");
	}
	else
	{
		//扩大容量
		if(nready == static_cast<int>(_eventList.size()))
		{
			_eventList.resize(nready * 2);
		}

		for(int idx = 0; idx != nready; ++idx)
		{
			if(_eventList[idx].data.fd == _listenfd)
			{//处理新连接
				if(_eventList[idx].events & EPOLLIN)
				{
					handleConnection();
				}
			}
			else if(_eventList[idx].data.fd == _eventfd)
			{//处理IO线程
				if(_eventList[idx].events & EPOLLIN)
				{
					handleRead();
					doPendingFunctors();
				}
			}
			else
			{
				if(_eventList[idx].events & EPOLLIN)
				{//处理旧连接
					handleMessage(_eventList[idx].data.fd);
				}
			}
		}
	}
}

void EpollPoller::handleConnection()
{
	int peerfd = ::accept(_listenfd, NULL, NULL);
	if(-1 == peerfd)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}

	//将新连接添加到epoll实例的监听列表中
	addEpollFd(_epollfd, peerfd);

	TcpConnectionPtr pConn(new TcpConnection(peerfd, this));
	pConn->setConnectionCallback(_onConnection);
	pConn->setMessageCallback(_onMessage);
	pConn->setCloseCallback(_onClose); 
	_connMap[peerfd] = pConn; 
	//给客户端发一些信息 onConnection();
	//pConn->send("welcome to server.");
	pConn->handleConnectionCallback();
}


void EpollPoller::handleMessage(int fd)
{
	bool isClosed = isConnectionClose(fd);
	map<int, TcpConnectionPtr>::iterator it = 
		_connMap.find(fd);
	assert(it != _connMap.end());		

	if(isClosed)
	{//发现连接断开的操作
		it->second->handleCloseCallback();
		delEpollFd(_epollfd, fd);
		_connMap.erase(it);
	}
	else
	{//如果是客户端发过来的数据
		it->second->handleMessageCallback();
	}
}

void EpollPoller::setConnectionCallback(EpollPollerCallback cb)
{
	_onConnection = cb;
}

void EpollPoller::setMessageCallback(EpollPollerCallback cb)
{
	_onMessage = cb;
}

void EpollPoller::setCloseCallback(EpollPollerCallback cb)
{
	_onClose = cb;
}
}// end of namespace wd
