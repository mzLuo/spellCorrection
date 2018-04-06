
#include "EditDistance.h"
#include <string>

using std::cout;
using std::endl;
using std::string;
  
/// 获取一个字节高位开头为1的个数
size_t nBytesCode(const char ch)
{
	if(ch & (1 << 7))
	{
		int nBytes = 1;
		for(int idx = 0; idx != 6; ++idx)
		{
			if(ch & (1 << (6 - idx)))
			{
				++nBytes;	
			}
			else
				break;
		}
		return nBytes;
	}
	return 1;
}  
  
  
#if 0
//该算法复杂了，不够简洁
size_t nBytesCode(const char ch)
{
	size_t nBytes = 0;
	if(ch &(1 << 7))
	{//对中文进行处理-utf8编码
		if((ch & 0xF0) == 0xC0 || (ch & 0xF0) == 0xD0)   //  1111 0000
		{												 // &11xx xxxx
			nBytes += 2;								 //  1100 0000	
		}												 //  1101 0000
		else if((ch & 0xF0) == 0xE0)
		{
			nBytes += 3;
		}
		else if((ch & 0xFF) == 0xF0 ||
				(ch & 0xFF) == 0xF1 ||
				(ch & 0xFF) == 0xF2 ||
				(ch & 0xFF) == 0xF3 ||
				(ch & 0xFF) == 0xF4 ||
				(ch & 0xFF) == 0xF5 ||
				(ch & 0xFF) == 0xF6 ||
				(ch & 0xFF) == 0xF7 )
		{
			nBytes += 4;
		}
		else if((ch & 0xFF) == 0xF8 ||
				(ch & 0xFF) == 0xF9 ||
				(ch & 0xFF) == 0xFA ||
				(ch & 0xFF) == 0xFB) 
		{
			nBytes += 5;
		}
		else if((ch & 0xFF) == 0xFC)
		{
			nBytes += 6;
		}
	}
	else
	{//1字节编码或英文
		nBytes += 1;	
	}
	return nBytes;
}
#endif

std::size_t length(const std::string &str)
{
	std::size_t ilen = 0;
	for(std::size_t idx = 0; idx != str.size(); ++idx)
	{
		int nBytes = nBytesCode(str[idx]);
		idx += (nBytes - 1);
		++ilen;
	}
	return ilen;
}

int triple_min(const int &a, const int &b, const int &c)
{//三个数之中,哪个最小
	return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

int editDistance(const std::string & lhs, const std::string &rhs)
{//计算最小编辑距离-包括处理中英文
	size_t lhs_len = length(lhs);
	size_t rhs_len = length(rhs);
	int editDist[lhs_len + 1][rhs_len + 1];
	for(size_t idx = 0; idx <= lhs_len; ++idx)
	{
		editDist[idx][0] = idx;
	}

	for(size_t idx = 0; idx <= rhs_len; ++idx)
	{
		editDist[0][idx] = idx;
	}
	
	std::string sublhs, subrhs;
	for(std::size_t dist_i = 1, lhs_idx = 0; dist_i <= lhs_len; ++dist_i, ++lhs_idx)
	{
		size_t nBytes = nBytesCode(lhs[lhs_idx]);
		sublhs = lhs.substr(lhs_idx, nBytes);
		lhs_idx += (nBytes - 1);

		for(std::size_t dist_j = 1, rhs_idx = 0; dist_j <= rhs_len; ++dist_j, ++rhs_idx)
		{
			nBytes = nBytesCode(rhs[rhs_idx]);
			subrhs = rhs.substr(rhs_idx, nBytes);
			rhs_idx += (nBytes - 1);
			if(sublhs == subrhs)
			{
				editDist[dist_i][dist_j] = editDist[dist_i - 1][dist_j - 1];
			}
			else
			{
				editDist[dist_i][dist_j] = triple_min(
					editDist[dist_i][dist_j - 1] + 1,
					editDist[dist_i - 1][dist_j] + 1,
					editDist[dist_i - 1][dist_j - 1] + 1);
			}
		}
	}
	return editDist[lhs_len][rhs_len];
}
