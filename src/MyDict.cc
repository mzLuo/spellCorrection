#include "MyDict.h"
#include <iostream>
#include <fstream>
#include <sstream>

MyDict *MyDict::pInstance = NULL;

MyDict* MyDict::createInstance()
{
	if(NULL == pInstance)
	{
		pInstance = new MyDict;
	}
	return pInstance;
}

std::vector<std::pair<std::string, int> > & MyDict::get_dict()
{//获取词典
	return dict_;
}

std::map<std::string, std::set<int> > &MyDict::get_index_table()
{//获取索引表
	return index_table_;
}

void MyDict::show_dict()
{
	auto iter = dict_.begin();
	for(; iter != dict_.end(); ++iter)
	{
		std::cout << iter->first << "-->"
				  << iter->second << std::endl;
	}
}

void MyDict::show_index_table()
{
	auto iter = index_table_.begin();
	for(; iter != index_table_.end(); ++iter)
	{
		std::cout << iter->first << "-->";
		auto sit = iter->second.begin();
		for(; sit != iter->second.end(); ++sit)
		{
			std::cout << *sit << " ";
		}
		std::cout << std::endl;
	}
}

void MyDict::read_from(const char * dictpath)
{
	std::ifstream in(dictpath);//把路径放入in里	
	if(!in)
	//路径不对
	{
		std::cout << __DATE__ << " " << __TIME__
				  << __FILE__ << " " << __LINE__
	              << ": dict read error" << std::endl;
		exit(-1);
	}
	//路径对了
	std::string line;
	while(getline(in, line))
	//读词典
	{
		std::stringstream ss(line);
		std::string key;
		int value;
		ss >> key >> value;
		dict_.push_back(make_pair(key, value));
	}
	in.close();
}
//private:
MyDict::MyDict()
{}


void MyDict::init(const char *dictPath, const char * cnDictPath)
{//通过中文和英文词典文件路径初始化词典
	read_from(dictPath);
	read_from(cnDictPath);
	//创建索引表
	for(size_t idx = 0; idx != dict_.size(); ++idx)
	{
		record_to_index(idx);
	}
}

void MyDict::record_to_index(int idx)
{//建索引
	std::string key;
	std::string word = dict_[idx].first;//先获取一个词
//以下的做法有点麻烦了，建议参考Edit...那个文档
	for(std::size_t iidx = 0; iidx != word.size(); ++iidx)
	{
		char ch = word[iidx];
		if(ch & (1 << 7))
		{//存储utf-8编码的中文字符
			if((ch & 0xF0) == 0xC0 || (ch & 0xF0) == 0xD0)
			{
				key = word.substr(iidx, 2);
				++iidx;
			}
			else if((ch & 0xF0) == 0xE0)
			{
				key = word.substr(iidx, 3);
				iidx += 2;
			}
			else if((ch & 0xFF) == 0xF0 || 
					(ch & 0xFF) == 0xF1 || 
					(ch & 0xFF) == 0xF2 || 
					(ch & 0xFF) == 0xF3 || 
					(ch & 0xFF) == 0xF4 || 
					(ch & 0xFF) == 0xF5 || 
					(ch & 0xFF) == 0xF6 || 
					(ch & 0xFF) == 0xF7)
			{
				key = word.substr(iidx, 4);
				iidx += 3;
			}
			else if((ch & 0xFF) == 0xF8 ||
					(ch & 0xFF) == 0xF9 || 
					(ch & 0xFF) == 0xFA || 
					(ch & 0xFF) == 0xFB) 
			{
				key = word.substr(iidx, 5);
				iidx += 4;
			}
			else if((ch & 0xFF) == 0xFC)
			{
				key = word.substr(iidx, 6);
				iidx += 5;
			}
		}
		else
		{
			key = word.substr(iidx, 1);//截取字符串
		}
		index_table_[key].insert(idx);//重点,获取字符，建索引
	}
}
