#pragma once

#include "HashMap.hpp"
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <string>

/*
CharString
˵����	ʵ����wchar_t���͵��ַ����Ļ�������
		��̬�����ڴ棬��֤�� += ����ľ�̯���Ӷ�
�ӿڣ�	�ַ���������ȡ�±�/��ֵ/substring/indexOf���Ӵ����ң�/ concat / >> / <<
		״̬��ѯ��length/capacity/empty/back
*/

class CharString
{
	int _len;
	int _capacity;		// ���� len<=capacity
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

	// ����������ֻ��Ҫ���������ڵ�ǰ����ʱ�������·����ڴ�
	void reserve(int cap);

	// �з�Χ�����±����
	wchar_t operator [](int x) const;
	wchar_t& operator [](int x);
	// ȡ�Ӵ� [l,r) 
	CharString substring(int l, int r) const;

	// ��ֵ�����͸�ֵ�����
	CharString& assign(const CharString &b);
	CharString& operator=(const CharString &b);
	
	// concat��������̯���Ӷ� O(len(b))
	CharString& operator+=(const CharString &b);
	CharString& operator+=(wchar_t b);

	// �Ӵ����ң����ص�һ�γ��ֵ�λ�ã�ʹ��KMP�㷨
	int indexOf(const CharString &b) const;

	// ���ַ�����ֵת��Ϊdouble
	double toDouble() const;

	// ת��Ϊwstring
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

// hash����
extern hash_t charStringHash(const CharString& str);

template<>
class HashFunc<CharString> {
public:
	hash_t operator ()(const CharString& key) const { return charStringHash(key); }
};