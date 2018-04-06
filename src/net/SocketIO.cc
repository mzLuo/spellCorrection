 ///
 /// @file    SocketIO.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2016-03-23 10:01:14
 ///
 
#include "SocketIO.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>



namespace wd
{

SocketIO::SocketIO(int fd)
: _fd(fd)
{}

ssize_t SocketIO::readn(char * buff, size_t count)
{
	size_t nleft = count;
	char * pbuf = buff;
	ssize_t ret = 0;
	while(nleft > 0)
	{
		ret = read(_fd, pbuf, nleft);
		if(-1 == ret)
		{
			if(errno == EINTR)
			{	continue;	}
			perror("read");
			exit(EXIT_FAILURE);
			
		}
		else if(0 == ret)//读到文件末尾
		{
			break;
		}
		pbuf += ret;
		nleft -= ret;
	}
	return (count - nleft);
}

ssize_t SocketIO::writen(const char * buff, size_t count)
{
	size_t nleft = count;
	const char * pbuf = buff;
	ssize_t ret = 0;
	while(nleft > 0)
	{
		ret = write(_fd, pbuf, nleft);
		if(-1 == ret)
		{
			if(errno == EINTR)
				continue;
			perror("write");
			exit(EXIT_FAILURE);
		}	
		pbuf += ret;
		nleft -= ret;
	}
	return (count - nleft);
}

ssize_t SocketIO::readPeek(char * buff, size_t count)
{
	ssize_t nread;
	do
	{
		//并不移走内核缓冲区的数据
		nread = ::recv(_fd, buff, count, MSG_PEEK);
	}while(nread == -1 && errno == EINTR);
	return nread;
}


ssize_t SocketIO::readline(char * buff, size_t max)
{
	size_t nleft = max - 1;
	//如果是max个字节数据里面真的没有
	// '\n', 就取max-1个字节的数据，
    // 最后一个字节存放 '\0'
	char * pbuf = buff;
	size_t total = 0;
	while(nleft > 0)
	{
		//先去内核缓冲区预取数据
		ssize_t  nread = readPeek(pbuf, nleft); 
		if(nread <= 0)
			return nread;

		//检查nread个字节的数据里面有没有'\n'
		for(int idx = 0; idx != nread; ++idx)
		{
			if(pbuf[idx] == '\n')
			{ // 找到'\n'，直接从内核缓冲区中移走数据,并返回
				int nsize = idx + 1;
				if(readn(pbuf, nsize) != nsize)
					return EXIT_FAILURE;
				nleft -= nsize;
				total += nsize;
				pbuf += nsize;
				*pbuf = '\0';//给已经读取的字符串赋予\0,代表该字符串结束
				return total;
			}
		}    
		
		//没有找到，就移走内核缓冲区里面的nread个字节的数据
		if(readn(pbuf, nread) != nread)
			return EXIT_FAILURE;

		nleft -= nread;
		pbuf += nread;
		total += nread;
	}
	//最终也没有找到'\n'
	*pbuf = '\0';//最大的max个字节，留一个给\0
	return max - 1;
}



}// end of namespace wd
