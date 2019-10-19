#pragma once

#include "HashMap.hpp"
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <iostream>

class CharString
{
	int _len;
	int _capacity;
	wchar_t* _str;
public:

	CharString();
	CharString(wchar_t ch);
	CharString(const wchar_t *wstr);
	CharString(int len);
	CharString(const CharString& wstr);
	CharString(const std::wstring& wstr);
	~CharString();

	int length() const { return _len; }
	int capacity() const { return _capacity; }
	void reserve(int cap);
	void clear() { _len = 0; }
	bool empty() const { return _len == 0; }

	wchar_t operator [](int x) const;
	wchar_t& operator [](int x);
	CharString substring(int l, int r) const;
	int indexOf(CharString &b) const;
	CharString& assign(const CharString &b);
	CharString& operator=(const CharString &b);
	CharString& operator+=(const CharString &b);
	CharString& operator+=(wchar_t b);

	friend std::wistream & operator >> (std::wistream & is, CharString &str);
	friend std::wostream & operator << (std::wostream & os, const CharString &str);
	friend CharString concat(const CharString &a, const CharString &b);
	friend bool operator==(const CharString &a, const CharString &b);
};
std::wistream & operator >> (std::wistream & is, CharString &str);
std::wostream & operator << (std::wostream & os, const CharString &str);
CharString concat(const CharString &a, const CharString &b);
CharString operator + (const CharString &a, const CharString &b);
bool operator==(const CharString &a, const CharString &b);

hash_t charStringHash(const CharString& str);

