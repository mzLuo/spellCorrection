 ///
 /// @file    SpellcorrectServer.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2016-03-25 21:19:44
 ///
 
#ifndef __MY_SPELLCORRECTSERVER_H_
#define __MY_SPELLCORRECTSERVER_H_

#include "TcpServer.h"
#include "Threadpool.h"
#include "TimerThread.h"

#include <string>

using std::string;

namespace wd
{

class SpellcorrectServer
{
public:
	SpellcorrectServer();

	void start();

	void onConnection(TcpConnectionPtr conn);
	void onMessage(TcpConnectionPtr conn);
	void onClose(TcpConnectionPtr conn);

private:
	TcpServer _tcpServer;//IO线程
	Threadpool _threadpool;//计算线程
	TimerThread _timer;
};


}// end of namespace wd

#endif
