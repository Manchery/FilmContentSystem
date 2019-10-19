#include "CharString.h"
#include <stdexcept>
#include <cstring>
#include <string>


CharString::CharString()
{
	_len = 0; _str = nullptr;
}

CharString::CharString(wchar_t ch)
{
	_len = 1;
	_str = new wchar_t[1]{ ch };
}

CharString::CharString(const wchar_t * wstr)
{
	_len = wcslen(wstr);
	if (_len == 0)
		_str = nullptr;
	else {
		_str = new wchar_t[_len];
		memcpy(_str, wstr, sizeof(wchar_t)*_len);
	}
}

CharString::CharString(int len)
{
	_len = len; 
	_str = len == 0 ? nullptr : new wchar_t[len] {0};
}

CharString::CharString(const CharString & wstr)
{
	_len = wstr._len;
	if (_len == 0)
		_str = nullptr;
	else {
		_str = new wchar_t[_len];
		memcpy(_str, wstr._str, sizeof(wchar_t)*_len);
	}
}

CharString::CharString(const std::wstring & wstr)
{
	_len = wstr.length();
	if (_len == 0)
		_str = nullptr;
	else {
		_str = new wchar_t[_len];
		for (int i = 0; i < _len; i++)
			_str[i] = wstr[i];
	}
}

CharString::~CharString()
{
	delete[] _str;
}

wchar_t CharString::operator[](int x) const
{
	if (x < 0 || x >= _len) throw std::out_of_range(""); //TODO
	return _str[x];
}

wchar_t & CharString::operator[](int x)
{
	if (x < 0 || x >= _len) throw std::out_of_range(""); //TODO
	return _str[x];
}

CharString CharString::substring(int l, int r) {
	if (l < 0 || r > _len)
		throw std::out_of_range(""); // TODO
	if (l >= r)
		return CharString(L"");
	CharString result(r - l);
	memcpy(result._str, _str + l, sizeof(wchar_t)*(r - l));
	return result;
}

int CharString::indexOf(CharString & b)
{
	int blen = b.length();
	int *next = new int[blen];
	next[0] = -1;
	int k = -1;
	for (int i = 1; i < blen; i++) {
		while (~k && b[k + 1] != b[i]) k = next[k];
		if (b[k + 1] == b[i]) k++;
		next[i] = k;
	}

	k = -1; int ans = -1;
	for (int i = 0; i < _len; i++) {
		while (~k && b[k + 1] != _str[i]) k = next[k];
		if (b[k + 1] == _str[i]) k++;
		if (k == blen - 1) {
			ans = i - k + 1; break;
		}
	}
	delete[] next;
	return ans;
}

CharString& CharString::assign(const CharString & b)
{
	_len = b.length();
	delete[] _str; _str = new wchar_t[_len];
	memcpy(_str, b._str, sizeof(wchar_t)*_len);
	return *this;
}

CharString & CharString::operator=(const CharString & b)
{
	return assign(b);
}

std::wistream & operator>>(std::wistream & is, CharString & str)
{
	std::wstring wstr;
	is >> wstr;
	str._len = wstr.length();
	delete[] str._str; str._str = new wchar_t[str._len];
	for (int i = 0; i < str._len; i++) str._str[i] = wstr[i];
	return is;
}

std::wostream & operator<<(std::wostream & os, CharString & str)
{
	for (int i = 0; i < str._len; i++) os << str._str[i];
	return os;
}

CharString concat(const CharString & a, const CharString & b)
{
	CharString result(a.length()+b.length());
	if (a.length())
		memcpy(result._str, a._str, sizeof(wchar_t)*a.length());
	if (b.length())
		memcpy(result._str + a.length(), b._str, sizeof(wchar_t)*b.length());
	return result;
}

CharString operator+(const CharString & a, const CharString & b)
{
	return concat(a, b);
}

bool operator==(const CharString & a, const CharString & b)
{
	if (a.length() != b.length()) return false;
	for (int i = 0; i < a.length(); i++)
		if (a[i] != b[i]) 
			return false;
	return true;
}

hash_t charStringHash(const CharString & str) {
	static hash_t seed = 50021;
	hash_t h = 0;
	for (int i = 0; i < str.length(); i++) {
		h = h * seed + (hash_t)str[i];
	}
	return h;
}
