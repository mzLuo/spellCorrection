 ///
 /// @file    TcpConnection.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2016-03-23 11:20:09
 ///
 
#include "TcpConnection.h"
#include "EpollPoller.h"
#include <string.h>
#include <iostream>
#include <sstream>

namespace wd
{

TcpConnection::TcpConnection(int fd, EpollPoller * ioLoop)
: _sock(fd)
, _sockIO(fd)
, _localAddr(Socket::getLocalAddr(fd))
, _peerAddr(Socket::getPeerAddr(fd))
, _isShutdownWrite(false)
, _ioLoop(ioLoop)
{
}


TcpConnection::~TcpConnection()
{
	if(!_isShutdownWrite)
	{
		shutdown();
	}
}

std::string TcpConnection::receive()
{
	char buff[1024];
	memset(buff, 0, sizeof(buff));
	_sockIO.readline(buff, 1024);
	return std::string(buff);
}

void TcpConnection::send(const std::string & msg)
{
	_sockIO.writen(msg.c_str(), msg.size());
}
	
void TcpConnection::sendInLoop(const std::string & msg)
{
	_ioLoop->runInLoop(std::bind(&TcpConnection::send, this, msg));
}

void TcpConnection::shutdown()
{
	if(!_isShutdownWrite) { _isShutdownWrite = true;
		_sock.shutdownWrite();
	}
}

std::string TcpConnection::toString() const
{
	std::ostringstream oss;
	oss << _localAddr.ip() << ":" << _localAddr.port()
			  << "--->"
			  << _peerAddr.ip() <<  ":" << _peerAddr.port()
			  << " ";
	return oss.str();
}

int TcpConnection::fd()
{
	return _sock.fd();
}


void TcpConnection::setConnectionCallback(TcpConnectionCallback cb)
{
	_onConnectionCb = cb;
}

void TcpConnection::setMessageCallback(TcpConnectionCallback cb)
{
	_onMessageCb = cb;
}

void TcpConnection::setCloseCallback(TcpConnectionCallback cb)
{
	_onCloseCb = cb;
}

void TcpConnection::handleConnectionCallback()
{
	if(_onConnectionCb)
		_onConnectionCb(shared_from_this());
}

void TcpConnection::handleMessageCallback()
{
	if(_onMessageCb)
		_onMessageCb(shared_from_this());
}

void TcpConnection::handleCloseCallback()
{
	if(_onCloseCb)
		_onCloseCb(shared_from_this());
}

}// end of namespace wd
