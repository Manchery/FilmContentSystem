#pragma once

#include <cstdio>
#include <cstdlib>
#include <algorithm>

class CharString
{
	int _len;
	wchar_t* _str;
public:
	CharString();
	CharString(wchar_t ch);
	CharString(const wchar_t *wstr);
	CharString(int len);
	CharString(const CharString& wstr);
	~CharString();
	int length() const { return _len; }
	wchar_t operator [](int x) const;
	CharString substring(int l, int r);
	int indexOf(CharString &b);
	CharString& assign(const CharString &b);
	CharString& operator=(const CharString &b);
	friend CharString concat(const CharString &a, const CharString &b);
	friend bool operator==(const CharString &a, const CharString &b);
};

CharString concat(const CharString &a, const CharString &b);
CharString operator + (const CharString &a, const CharString &b);
bool operator==(const CharString &a, const CharString &b);

