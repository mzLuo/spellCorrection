 ///
 /// @file    SpellcorrectServer.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2016-03-25 21:29:32
 ///
 
#include "SpellcorrectServer.h"
#include "MyDict.h"
#include "MyTask.h"
#include "CacheManager.h"

#include <iostream>
#include <sstream>
#include <functional>

using std::cout;
using std::endl;


namespace wd
{

SpellcorrectServer::SpellcorrectServer()
: _tcpServer(MyConf::getInstance()->getMap()["my_ip"].c_str(), 
			 wd::str2unshort(MyConf::getInstance()->getMap()["my_port"]))
, _threadpool(wd::str2int(MyConf::getInstance()->getMap()["my_thread_num"]),
			  wd::str2int(MyConf::getInstance()->getMap()["my_buff_size"]))
, _timer(std::bind(&CacheManager::periodicUpdateCaches),
		wd::str2int(MyConf::getInstance()->getMap()["my_init_time"]), 
		wd::str2int(MyConf::getInstance()->getMap()["my_update_time"]))
{
	MyDict * pMyDict = MyDict::createInstance();
	pMyDict->init(MyConf::getInstance()->getMap()["my_dict"].c_str(),
				  MyConf::getInstance()->getMap()["my_cn_dict"].c_str());
}


void SpellcorrectServer::start()
{
	//初始化线程池Cache
	_threadpool.start();
	_timer.start();

	using namespace std::placeholders;
	_tcpServer.setConnectionCallback(
			std::bind(&SpellcorrectServer::onConnection, this, _1));
	_tcpServer.setMessageCallback(
			std::bind(&SpellcorrectServer::onMessage, this, _1));
	_tcpServer.setCloseCallback(
			std::bind(&SpellcorrectServer::onClose, this, _1));

	_tcpServer.start();
}

void SpellcorrectServer::onConnection(TcpConnectionPtr conn)
{
	cout << "\n> " << conn->toString() << "has connected!" << endl;
	//conn->send("> welcome to SpellCorrect Server!");
}

void SpellcorrectServer::onMessage(TcpConnectionPtr conn)
{
	string msg = conn->receive();
	cout << msg << endl;
	
	string data;
	int pos = msg.find('\n');
	if(pos >= 0)
		data = msg.substr(0, pos);
	else
		data = msg;
	cout << "\n> " << data << endl;
	MyTask task(data, conn);
	_threadpool.addTask(std::bind(&MyTask::execute, task));
}

void SpellcorrectServer::onClose(TcpConnectionPtr conn)
{
	cout << "\n> " << conn->toString() << " has closed!" << endl;
}

}// end of namespace wd
