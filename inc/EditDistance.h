 ///
 /// @file    EditDistance.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2016-04-01 10:51:43
 ///
 
#ifndef __MY_EDIT_DISTANCE_H_
#define __MY_EDIT_DISTANCE_H_
#include <iostream>
#include <string>

size_t nBytesCode(const char ch);

std::size_t length(const std::string &str);

int tripleMin(const int &a, const int &b, const int &c);

int editDistance(const std::string & lhs, const std::string &rhs);

#endif
