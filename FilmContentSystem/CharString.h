#pragma once

#include "HashMap.hpp"
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <string>

/*
CharString
说明：	实现了wchar_t类型的字符串的基本功能
		动态分配内存，保证了 += 运算的均摊复杂度
接口：	字符串操作：取下标/赋值/substring/indexOf（子串查找）/ concat / >> / <<
		状态查询：length/capacity/empty/back
*/

class CharString
{
	int _len;
	int _capacity;		// 满足 len<=capacity
	wchar_t* _str;
public:

	CharString();
	CharString(int len);
	CharString(wchar_t ch);
	CharString(const wchar_t *wstr);
	CharString(const CharString& wstr);
	CharString(const std::wstring& wstr);
	~CharString();

	int length() const { return _len; }
	int capacity() const { return _capacity; }
	bool empty() const { return _len == 0; }
	wchar_t back() const { return _str[_len - 1]; }

	void clear() { _len = 0; }

	// 扩充容量，只有要求容量大于当前容量时，才重新分配内存
	void reserve(int cap);

	// 有范围检查的下标操作
	wchar_t operator [](int x) const;
	wchar_t& operator [](int x);
	// 取子串 [l,r) 
	CharString substring(int l, int r) const;

	// 赋值函数和赋值运算符
	CharString& assign(const CharString &b);
	CharString& operator=(const CharString &b);
	
	// concat操作，均摊复杂度 O(len(b))
	CharString& operator+=(const CharString &b);
	CharString& operator+=(wchar_t b);

	// 子串查找，返回第一次出现的位置，使用KMP算法
	int indexOf(const CharString &b) const;

	// 将字符串的值转化为double
	double toDouble() const;

	// 转化为wstring
	std::wstring toWString() const;

	friend std::wistream & operator >> (std::wistream & is, CharString &str);
	friend std::wostream & operator << (std::wostream & os, const CharString &str);
	friend CharString concat(const CharString &a, const CharString &b);
	friend bool operator==(const CharString &a, const CharString &b);
	friend bool operator<(const CharString &a, const CharString &b);
};

std::wistream & operator >> (std::wistream & is, CharString &str);
std::wostream & operator << (std::wostream & os, const CharString &str);
CharString concat(const CharString &a, const CharString &b);
CharString operator + (const CharString &a, const CharString &b);
bool operator==(const CharString &a, const CharString &b);
bool operator<(const CharString &a, const CharString &b);

// hash函数
extern hash_t charStringHash(const CharString& str);

template<>
class HashFunc<CharString> {
public:
	hash_t operator ()(const CharString& key) const { return charStringHash(key); }
};